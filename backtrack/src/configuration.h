#pragma once
#include <vector>
#include <string>

struct FileOrigin
{
	std::string origin;
	std::vector<std::string> includes;
	std::vector<std::string> excludes;
};

struct ArchiveConfig
{
	std::string archivePath;
	std::vector<FileOrigin> origins;
};

struct Configuration
{
	static std::vector<ArchiveConfig> archiveConfigs;
	static void loadConfigFromFile(const std::string& filePath);
};