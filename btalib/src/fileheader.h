#pragma once
#include <stdint.h>
#include <string>

struct FileHeader
{
	uint32_t creationTime = 0;
	uint32_t modifyTime = 0;
	uint64_t compressedEncryptedSize = 0;
	uint64_t compressedSize = 0;
	uint64_t size = 0;
};

struct FileDescriptor
{
	FileHeader header = {};
	std::string filePath;
	uint64_t blobOffset = 0;
};