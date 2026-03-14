#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <span>

namespace Object
{
    enum class type {
        OBJECT_BLOB = 0,
        OBJECT_TREE ,
        OBJECT_COMMIT 
    } ;

    static const std::unordered_map<type, std::string> enum_to_str = {
        {type::OBJECT_BLOB, "blob" }, 
        {type::OBJECT_COMMIT, "commit" }, 
        {type::OBJECT_TREE, "tree" }, 
    } ;

    class GitObject
    {
    protected:
        GitObject() = default;

    public:
        ~GitObject() = default;
        virtual std::span<const uint8_t> getContent() const = 0 ;
        virtual std::string getHash() const = 0 ;
        virtual std::string getType() const = 0 ;
        virtual size_t getSize() const = 0 ;
    };
}