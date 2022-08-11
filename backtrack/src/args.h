#pragma once
#include <string>
#include <vector>

struct Args
{
	static std::string configFilePath;
	static std::vector<std::string> btanames;
	static bool parseArguments(int argc, char* argv[]);
	static void printUsage();
};