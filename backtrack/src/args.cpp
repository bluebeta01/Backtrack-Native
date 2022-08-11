#include <iostream>
#include <string>
#include "args.h"

std::string Args::configFilePath;
std::vector<std::string> Args::btanames = {"all"};

bool Args::parseArguments(int argc, char* argv[])
{
	if (argc <= 1)
		return false;

	for (int i = 0; i < argc; i++)
	{
		std::string arg = argv[i];
		if (arg == "-config" && i < argc - 1)
			Args::configFilePath = argv[i + 1];
		if (arg == "-bta" && i < argc - 1)
			Args::btanames.push_back(argv[i + 1]);
	}

	if (Args::configFilePath == "")
		return false;

	return true;
}

void Args::printUsage()
{
	//TODO: print usage
	std::cout << "Bad usage\n";
}
