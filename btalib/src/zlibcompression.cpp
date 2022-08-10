#pragma once
#include <zlib.h>
#include <assert.h>
#include "zlibcompression.h"

std::vector<unsigned char> ZlibCompressionProvider::compressData(unsigned char* data, int dataSize, bool endOfFile)
{
    if (dataSize == 0)
        return {};

    int zlibReturn = 0;
    std::vector<unsigned char> deflatedData;

    std::vector<unsigned char> bufferData(16384);

    
    m_zlibStream.avail_in = dataSize;
    m_zlibStream.next_in = data;

    do
    {
        m_zlibStream.avail_out = 16384;
        m_zlibStream.next_out = bufferData.data();
        zlibReturn = deflate(&m_zlibStream, endOfFile ? Z_FINISH : Z_NO_FLUSH);
        assert(zlibReturn >= 0);
        int bytesDeflated = 16384 - m_zlibStream.avail_out;
        for (int i = 0; i < bytesDeflated; i++)
            deflatedData.push_back(bufferData[i]);
    } while (m_zlibStream.avail_out == 0);

    if(endOfFile)
        deflateEnd(&m_zlibStream);

    return deflatedData;
}

std::vector<unsigned char> ZlibCompressionProvider::decompressData(unsigned char* data, int dataSize, bool endOfFile)
{
    int zlibReturn = 0;

    std::vector<unsigned char> inflatedData;
    
    std::vector<unsigned char> out(16384);

    m_zlibStream.avail_in = dataSize;
    m_zlibStream.next_in = data;
    do
    {
        m_zlibStream.avail_out = out.size();
        m_zlibStream.next_out = out.data();
        zlibReturn = inflate(&m_zlibStream, Z_NO_FLUSH);
        assert(zlibReturn >= 0 && zlibReturn != Z_NEED_DICT);

        int inflatedDataSize = out.size() - m_zlibStream.avail_out;

        for (int i = 0; i < inflatedDataSize; i++)
            inflatedData.push_back(out[i]);
    } while (m_zlibStream.avail_in != 0);
    
    if(endOfFile)
        inflateEnd(&m_zlibStream);
    
    return inflatedData;
}

void ZlibCompressionProvider::initializeCompression()
{
    m_zlibStream = {};
    m_zlibStream.zalloc = Z_NULL;
    m_zlibStream.zfree = Z_NULL;
    m_zlibStream.opaque = Z_NULL;
    assert(deflateInit(&m_zlibStream, Z_BEST_SPEED) == Z_OK);
}

void ZlibCompressionProvider::initializeDecompression()
{
    m_zlibStream = {};
    m_zlibStream.zalloc = Z_NULL;
    m_zlibStream.zfree = Z_NULL;
    m_zlibStream.opaque = Z_NULL;
    assert(inflateInit(&m_zlibStream) == Z_OK);
}
