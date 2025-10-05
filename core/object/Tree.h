#pragma once
#include "GitObject.h"

namespace Object 
{
    class Tree: public Object::GitObject {

        public:
        std::string getContent() override ;
        std::string getHash() override ;
        std::string getType() override ;
    };
}