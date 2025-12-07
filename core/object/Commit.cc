#include "Commit.h"

std::span<const uint8_t> Object::Commit::getContent() const
{
    return m_data ;
}

std::string Object::Commit::getHash() const
{
    return m_hash ;
}

std::string Object::Commit::getType() const
{
    return Object::enum_to_str.at(m_type) ;
}

size_t Object::Commit::getSize() const
{
    return m_size ;
}