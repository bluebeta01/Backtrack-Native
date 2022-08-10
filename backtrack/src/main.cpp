#include <archive.h>
#include <vector>
#include <string>
#include <iostream>
#include <filesystem>
#include "configuration.h"


int main()
{
	Configuration::loadConfigFromFile("D:/exampleconfig.txt");

	/*std::vector<std::string> files;
	for (const std::filesystem::directory_entry& dir_entry : std::filesystem::recursive_directory_iterator("C:/Espressif/"))
	{
		if (dir_entry.is_directory())
			continue;
		try
		{
			files.push_back(dir_entry.path().string());
		}
		catch(std::exception e) {}
	}

	printf("Uncompressed...\n");
	Archive::createArchive("C:/misc/btatest", "espressif_uncompressed", 0, files, BtaEncryption::NONE, BtaCompression::NONE);
	printf("Compressed...\n");
	Archive::createArchive("C:/misc/btatest", "espressif_zlib", 0, files, BtaEncryption::NONE, BtaCompression::ZLIB);

	Archive a = {};
	a.open("C:/misc/btatest/espressif_uncompressed.bta");
	for (const FileDescriptor& desc : a.fileDescriptors)
	{
		std::cout << "[uncompressed]: " << desc.filePath << "\n";
		a.extractFile(desc, "C:/misc/btatest/uncompressed_out");
	}
	a.close();

	a = {};
	a.open("C:/misc/btatest/espressif_zlib.bta");
	for (const FileDescriptor& desc : a.fileDescriptors)
	{
		std::cout << "[zlib]: " << desc.filePath << "\n";
		a.extractFile(desc, "C:/misc/btatest/zlib_out");
	}
	a.close();*/
}