#pragma once
#include "common/errors.h"
#include "common/result.h"
#include <vector>
#include <filesystem>

namespace Compression 
{
    class CompressionIFace 
    {
        public:
            virtual Result<std::vector<uint8_t>, errorcode> compress(const std::filesystem::path &inputFile) = 0;
            virtual Result<std::vector<uint8_t>, errorcode> decompress(const std::filesystem::path &inputFile) = 0;
            virtual ~CompressionIFace() = default ;
    };
}