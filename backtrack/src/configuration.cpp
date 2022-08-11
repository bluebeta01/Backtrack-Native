#include <fstream>
#include <sstream>
#include <algorithm>
#include <locale>
#include "configuration.h"

std::vector<ArchiveConfig> Configuration::archiveConfigs;

void Configuration::loadConfigFromFile(const std::string& filePath)
{
	std::fstream fileStream;
	fileStream.open(filePath);
	if (fileStream.fail())
		return;
	std::stringstream buffer;
	buffer << fileStream.rdbuf();
	fileStream.close();

	std::string line;
	while (!buffer.eof())
	{
		std::getline(buffer, line);
		if (std::find_if(line.begin(), line.end(), [](char c) {return !std::isspace<char>(c, std::locale::classic()); }) == line.end())
			continue;
		if (line.rfind("bta ", 0) == 0)
		{
			std::string btadir = line.substr(4);
			auto it = std::find_if(btadir.begin(), btadir.end(), [](char c) {return !std::isspace<char>(c, std::locale::classic()); });
			btadir.erase(btadir.begin(), it);
			Configuration::archiveConfigs.push_back({});
			ArchiveConfig& currentArchiveConfig = Configuration::archiveConfigs.back();
			currentArchiveConfig.archivePath = btadir;
		}

		if (line.rfind("origin ", 0) == 0)
		{
			std::string origin = line.substr(7);
			auto it = std::find_if(origin.begin(), origin.end(), [](char c) {return !std::isspace<char>(c, std::locale::classic()); });
			origin.erase(origin.begin(), it);
			ArchiveConfig& currentArchiveConfig = Configuration::archiveConfigs.back();
			FileOrigin fileOrigin;
			fileOrigin.origin = origin;
			currentArchiveConfig.origins.push_back(fileOrigin);
		}

		if (line.rfind("archive ", 0) == 0)
		{
			std::string include = line.substr(8);
			auto it = std::find_if(include.begin(), include.end(), [](char c) {return !std::isspace<char>(c, std::locale::classic()); });
			include.erase(include.begin(), it);
			ArchiveConfig& currentArchiveConfig = Configuration::archiveConfigs.back();
			FileOrigin& fileOrigin = currentArchiveConfig.origins.back();
			fileOrigin.includes.push_back(include);
		}

		if (line.rfind("exclude ", 0) == 0)
		{
			std::string exclude = line.substr(8);
			auto it = std::find_if(exclude.begin(), exclude.end(), [](char c) {return !std::isspace<char>(c, std::locale::classic()); });
			exclude.erase(exclude.begin(), it);
			ArchiveConfig& currentArchiveConfig = Configuration::archiveConfigs.back();
			FileOrigin& fileOrigin = currentArchiveConfig.origins.back();
			fileOrigin.excludes.push_back(exclude);
		}

		if (line.rfind("compression ", 0) == 0)
		{
			std::string compression = line.substr(12);
			auto it = std::find_if(compression.begin(), compression.end(), [](char c) {return !std::isspace<char>(c, std::locale::classic()); });
			compression.erase(compression.begin(), it);
			ArchiveConfig& currentArchiveConfig = Configuration::archiveConfigs.back();
			if (compression == "zlib")
				currentArchiveConfig.compressionType = BtaCompression::ZLIB;
		}

		if (line.rfind("name ", 0) == 0)
		{
			std::string btaname = line.substr(5);
			auto it = std::find_if(btaname.begin(), btaname.end(), [](char c) {return !std::isspace<char>(c, std::locale::classic()); });
			btaname.erase(btaname.begin(), it);
			ArchiveConfig& currentArchiveConfig = Configuration::archiveConfigs.back();
			currentArchiveConfig.name = btaname;
		}
	}
}
