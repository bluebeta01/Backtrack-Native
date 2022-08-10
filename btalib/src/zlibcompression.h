#pragma once
#include "compressionprovider.h"
#include <zlib.h>

class ZlibCompressionProvider : public CompressionProvider
{
public:
	std::vector<unsigned char> compressData(unsigned char* data, int dataSize, bool endOfFile) override;
	std::vector<unsigned char> decompressData(unsigned char* data, int dataSize, bool endOfFile) override;
	virtual void initializeCompression() override;
	virtual void initializeDecompression() override;

private:
	z_stream m_zlibStream;
};