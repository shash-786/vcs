#pragma once
#include "HashIFace.h"

namespace Hash {

    class SHA1Hash : public HashIFace {
        public:
        Result< std::string, errorcode > compute_hash(std::span<const uint8_t> data) override ;
        Result< std::string, errorcode > compute_hash(std::string header, std::span<const uint8_t> data) override ;
        Result< std::string, errorcode > compute_hash(std::string header, const std::filesystem::path filePath) override ;

        private:
        std::string hash_to_hex(const std::span<const uint8_t> hash) ;
    };
}
