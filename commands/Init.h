#pragma once 
#include "Command.h"
#include "fs/FSManager.h"

namespace Command 
{
    class Init : public Command::Command 
    {
    public:
        int execute (const std::vector<std::string>& args) override ;
        Init(FS::FSManager& manager_, Repo::Repository repo_) : manager(manager_), repo(repo_) {}
        
    private:
        FS::FSManager& manager ;
        Repo::Repository& repo ;
    } ;
}