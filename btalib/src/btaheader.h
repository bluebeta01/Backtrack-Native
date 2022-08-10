#pragma once
#include <stdint.h>

enum class BtaEncryption
{
	NONE = 0,
	AES256 = 1
};

enum class BtaCompression
{
	NONE = 0,
	ZLIB = 1
};

const uint32_t BTA_MAGIC = 0x42544146;

struct BtaHeader
{
	uint32_t magicNumber = 0;
	uint32_t btaVersion = 0;
	uint32_t identifier = 0;
	uint32_t archiveTime = 0;
	uint32_t encryptionType = 0;
	uint32_t compressionType = 0;
	uint32_t fileCount = 0;
};