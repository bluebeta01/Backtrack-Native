#pragma once
#include <string>

struct Args
{
	static std::string configFilePath;
	static bool parseArguments(int argc, char* argv[]);
	static void printUsage();
};