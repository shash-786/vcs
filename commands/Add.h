#pragma once
#include "Command.h"
#include "fs/FSManager.h"

namespace Command 
{
    class Add : public Command 
    {
    public:
        int execute (const std::vector<std::string>& args) override ;
        std::string pathspec(const std::vector<std::string>& args) ;
        Add(FS::FSManager& manager_, Repo::Repository repo_, std::string prefix_) : manager(manager_), repo(repo_), prefix(prefix_) {} 

    private:
        std::string prefix ;
        FS::FSManager& manager ;
        Repo::Repository& repo ;
    };
};