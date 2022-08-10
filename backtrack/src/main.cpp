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


}