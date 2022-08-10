#include <fstream>
#include <chrono>
#include <filesystem>
#include <memory>
#include "archive.h"
#include "fileheader.h"
#include "zlibcompression.h"

void Archive::open(const std::string& archivePath)
{
	archiveStream.open(archivePath, std::ios_base::in | std::ios_base::binary);
	if (archiveStream.fail())
		return;

	archiveStream.read(reinterpret_cast<char*>(&btaHeader), sizeof(btaHeader));
	if (btaHeader.magicNumber != BTA_MAGIC)
	{
		archiveStream.close();
		return;
	}

	switch (static_cast<BtaCompression>(btaHeader.compressionType))
	{
	case BtaCompression::NONE:
	{
		compressionProvider = std::make_shared<CompressionProvider>();
		break;
	}
	case BtaCompression::ZLIB:
	{
		compressionProvider = std::make_shared<ZlibCompressionProvider>();
		break;
	}
	}

	std::vector<uint64_t> fileHeaderOffsets;
	fileHeaderOffsets.resize(btaHeader.fileCount);
	archiveStream.read(reinterpret_cast<char*>(fileHeaderOffsets.data()), fileHeaderOffsets.size() * sizeof(uint64_t));

	for (uint64_t offset : fileHeaderOffsets)
	{
		//TODO: implement the file header decryption
		archiveStream.seekg(offset, std::ios_base::beg);

		FileHeader header;
		archiveStream.read(reinterpret_cast<char*>(&header), sizeof(header));

		FileDescriptor descriptor;
		descriptor.header = header;

		uint32_t filePathLength;
		archiveStream.read(reinterpret_cast<char*>(&filePathLength), sizeof(filePathLength));

		descriptor.filePath.resize(filePathLength);
		archiveStream.read(descriptor.filePath.data(), filePathLength);

		descriptor.blobOffset = archiveStream.tellg();

		fileDescriptors.push_back(descriptor);
	}
}

void Archive::close()
{
	archiveStream.close();
}

void Archive::extractFile(const FileDescriptor& fileDescriptor, const std::string& outputDirectory)
{
	//std::string outputPath = outputDirectory + "/" + std::filesystem::path(fileDescriptor.filePath).filename().string();
	std::string outputPath = outputDirectory + "/" + fileDescriptor.filePath.substr(3);
	std::filesystem::create_directories(std::filesystem::path(outputPath).parent_path());

	std::fstream outputStream;
	outputStream.open(outputPath, std::ios_base::out | std::ios_base::binary);
	if (outputStream.fail())
		return;

	archiveStream.seekg(fileDescriptor.blobOffset);
	std::vector<unsigned char> readBuffer(16384);

	uint64_t bytesLeftToRead = fileDescriptor.header.compressedSize;

	compressionProvider->initializeDecompression();

	while (bytesLeftToRead > 0)
	{
		int readByteCount = std::min(bytesLeftToRead, readBuffer.size());
		bytesLeftToRead -= readByteCount;

		archiveStream.read((char*)readBuffer.data(), readByteCount);
		std::vector<unsigned char> decompressedData = compressionProvider->decompressData(readBuffer.data(), readByteCount, bytesLeftToRead == 0);
		outputStream.write((char*)decompressedData.data(), decompressedData.size());
	}

	outputStream.flush();
	outputStream.close();
}

void Archive::createArchive(const std::string& archiveDirectory, const std::string& archiveName, uint32_t identifier,
	const std::vector<std::string>& files, BtaEncryption encryption, BtaCompression compression)
{
	std::string archivePath = archiveDirectory + "/" + archiveName + ".bta";
	std::fstream archiveFileStream;
	archiveFileStream.open(archivePath, std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
	if (archiveFileStream.fail())
		return;

	std::shared_ptr<CompressionProvider> compressionProvider;
	switch (compression)
	{
	case BtaCompression::NONE:
	{
		compressionProvider = std::make_shared<CompressionProvider>();
		break;
	}
	case BtaCompression::ZLIB:
	{
		compressionProvider = std::make_shared<ZlibCompressionProvider>();
		break;
	}
	}

	BtaHeader btaHeader;
	btaHeader.btaVersion = 1;
	btaHeader.identifier = identifier;
	btaHeader.archiveTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	btaHeader.encryptionType = static_cast<uint32_t>(encryption);
	btaHeader.compressionType = static_cast<uint32_t>(compression);
	btaHeader.fileCount = files.size();
	archiveFileStream.write(reinterpret_cast<char*>(&btaHeader), sizeof(btaHeader));

	std::vector<uint64_t> fileHeaderOffsets;
	fileHeaderOffsets.resize(files.size());
	std::streampos fileTableOrigin = archiveFileStream.tellg();
	archiveFileStream.write(reinterpret_cast<char*>(fileHeaderOffsets.data()), fileHeaderOffsets.size() * sizeof(uint64_t));

	const int READ_BLOCK_SIZE = 16384;
	std::vector<unsigned char> readBuffer;
	readBuffer.resize(READ_BLOCK_SIZE);

	for (int i = 0; i < files.size(); i++)
	{
		fileHeaderOffsets[i] = 0;
		std::fstream fileStream;
		fileStream.open(files[i], std::ios_base::in | std::ios_base::binary);
		if (fileStream.fail())
			continue;

		std::streampos fileHeaderOrigin = archiveFileStream.tellg();
		fileHeaderOffsets[i] = fileHeaderOrigin;

		FileHeader fileHeader;
		fileHeader.modifyTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::filesystem::last_write_time(files[i]).time_since_epoch()).count();


		int fileHeaderEncryptedSize = sizeof(FileHeader) + sizeof(uint32_t) + sizeof(char)*files[i].length();


		archiveFileStream.seekg(fileHeaderEncryptedSize, std::ios_base::cur);
		
		compressionProvider->initializeCompression();

		while (!fileStream.eof())
		{
			fileStream.read((char*)readBuffer.data(), READ_BLOCK_SIZE);

			int read = fileStream.gcount();

			//TODO:Compress and encrypt readBuffer and add those values to the fileheader
			std::vector<unsigned char> compressedData = compressionProvider->compressData(readBuffer.data(), fileStream.gcount(), fileStream.eof());

			fileHeader.size += fileStream.gcount();
			//fileHeader.compressedEncryptedSize += fileStream.gcount();
			fileHeader.compressedSize += compressedData.size();

			archiveFileStream.write((char*)compressedData.data(), compressedData.size());
		}

		fileStream.close();

		std::streampos nextFilePos = archiveFileStream.tellg();


		//Encrypt the file header and write it where it goes
		archiveFileStream.seekg(fileHeaderOrigin);
		archiveFileStream.write(reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader));
		uint32_t fileNameLength = files[i].length();
		archiveFileStream.write(reinterpret_cast<char*>(&fileNameLength), sizeof(fileNameLength));
		archiveFileStream.write(files[i].c_str(), fileNameLength);


		archiveFileStream.seekg(nextFilePos);
	}

	archiveFileStream.seekg(fileTableOrigin);
	archiveFileStream.write(reinterpret_cast<char*>(fileHeaderOffsets.data()), fileHeaderOffsets.size() * sizeof(uint64_t));

	archiveFileStream.seekg(0, std::ios_base::beg);
	archiveFileStream.write(reinterpret_cast<const char*>(&BTA_MAGIC), sizeof(BTA_MAGIC));

	archiveFileStream.close();
}
