#pragma once
#include "common/result.h"
#include "common/errors.h"
#include <filesystem>
#include <span>

namespace Hash {

    class HashIFace {
        public:
        virtual Result< std::string, errorcode > compute_hash(std::span<const uint8_t> data) = 0 ;
        virtual Result< std::string, errorcode > compute_hash(std::string header, std::span<const uint8_t> data) = 0 ;
        virtual Result< std::string, errorcode > compute_hash(std::string header, const std::filesystem::path filePath) = 0 ;
    }; 

}