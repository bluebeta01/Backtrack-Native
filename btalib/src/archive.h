#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <memory>
#include "btaheader.h"
#include "fileheader.h"
#include "compressionprovider.h"

class Archive
{
public:
	BtaHeader btaHeader;
	std::vector<FileDescriptor> fileDescriptors;

	void open(const std::string& archivePath);
	void close();
	void extractFile(const FileDescriptor& fileDescriptor, const std::string& outputDirectory);

	static void createArchive(const std::string& archiveDirectory, const std::string& archiveName, uint32_t identifier,
		const std::vector<std::string>& files, BtaEncryption encryption, BtaCompression compression);

private:
	std::fstream archiveStream;
	std::shared_ptr<CompressionProvider> compressionProvider;
};