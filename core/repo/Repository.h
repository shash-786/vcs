#pragma once

#include <filesystem>
#include "fs/FSManager.h"

namespace Repo
{
    class Repository
    {
    private:
        std::filesystem::path root ;
        std::filesystem::path gitPath ;
        std::filesystem::path indexPath;
        Repository() ;
    public:
        static Repository& getInstance() ;
        void setRoot(std::filesystem::path _root) ;
        const std::filesystem::path getRoot() const ;
        const std::filesystem::path getGitPath() const ;
        const std::filesystem::path getIndexPath() const ;
    };
}