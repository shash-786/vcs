#include "Tree.h"

std::span<const uint8_t> Object::Tree::getContent() const
{
    return m_data ;
}

std::string Object::Tree::getHash() const
{
    return m_hash ;
}

std::string Object::Tree::getType() const
{
    return Object::enum_to_str.at(m_type) ;
}

size_t Object::Tree::getSize() const
{
    return m_size ;
}