#pragma once

#include "common/result.h"
#include "common/errors.h"
#include "core/repo/Repository.h"
#include <optional>

namespace Command 
{
    class Command 
    {
        public:
        virtual int execute (const std::vector<std::string>& args) = 0;
    } ;
}