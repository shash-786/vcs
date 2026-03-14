#pragma once
#include "GitObject.h"

namespace Object 
{
    class Blob: public Object::GitObject 
    {
        std::string m_hash ;
        std::vector<uint8_t> m_data ;
        size_t m_size ;
        const Object::type m_type = Object::type::OBJECT_BLOB ;

        public:
        Blob (std::string _h, std::vector<uint8_t>&& content, const size_t _s) : m_hash(std::move(_h)), m_data(std::move(content)), m_size(_s)  {}

        std::span<const uint8_t> getContent() const override ;
        std::string getHash() const override ;
        std::string getType() const override ;
        size_t getSize() const override ;
    } ;
} 