#include "Blob.h"

std::span<const uint8_t> Object::Blob::getContent() const
{
    return m_data ;
}

std::string Object::Blob::getHash() const
{
    return m_hash ;
}

std::string Object::Blob::getType() const
{
    return Object::enum_to_str.at(m_type) ;
}

size_t Object::Blob::getSize() const
{
    return m_size ;
}