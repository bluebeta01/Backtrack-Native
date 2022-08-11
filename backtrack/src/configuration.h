#pragma once
#include <vector>
#include <string>
#include "btaheader.h"

struct FileOrigin
{
	std::string origin;
	std::vector<std::string> includes;
	std::vector<std::string> excludes;
};

struct ArchiveConfig
{
	std::string archivePath;
	std::string name;
	std::vector<FileOrigin> origins;
	BtaCompression compressionType = BtaCompression::NONE;
	BtaEncryption encryptionType = BtaEncryption::NONE;
};

struct Configuration
{
	static std::vector<ArchiveConfig> archiveConfigs;
	static void loadConfigFromFile(const std::string& filePath);
};