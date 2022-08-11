#pragma once
#include "configuration.h"

struct FileCollector
{
	static std::vector<std::string> collectFiles(const ArchiveConfig& archiveConfig);
};