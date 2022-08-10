#pragma once
#include <vector>
#include <stdint.h>

class CompressionProvider
{
public:
	virtual std::vector<unsigned char> compressData(unsigned char* data, int dataSize, bool endOfFile)
	{
		std::vector<unsigned char> outData;
		outData.resize(dataSize);
		memcpy_s(outData.data(), outData.size(), data, dataSize);
		return outData;
	};
	virtual std::vector<unsigned char> decompressData(unsigned char* data, int dataSize, bool endOfFile)
	{
		std::vector<unsigned char> outData;
		outData.resize(dataSize);
		memcpy_s(outData.data(), outData.size(), data, dataSize);
		return outData;
	};
	virtual void initializeCompression() {}
	virtual void initializeDecompression() {}
};