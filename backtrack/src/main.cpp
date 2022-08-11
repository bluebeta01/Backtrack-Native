#include "args.h"
#include "configuration.h"


int main(int argc, char* argv[])
{
	if (!Args::parseArguments(argc, argv))
	{
		Args::printUsage();
		return -1;
	}
	Configuration::loadConfigFromFile(Args::configFilePath);

	for (const std::string& btaname : Args::btanames)
	{
		auto archiveConfig = std::find_if(Configuration::archiveConfigs.begin(), Configuration::archiveConfigs.end(), [btaname](const ArchiveConfig& config) {
			return config.name == btaname;
		});
		if (archiveConfig == Configuration::archiveConfigs.end())
			continue;

		
	}
}