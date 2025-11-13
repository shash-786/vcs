#pragma once
#include "Command.h"
#include "fs/FSManager.h"

namespace Command 
{
    class Add : public Command 
    {
    public:
        int execute (std::vector<std::string>& args) override ;
        Add(FS::FSManager& manager_, Repo::Repository repo_, std::string prefix_) : manager(manager_), repo(repo_), prefix(prefix_) {} 

    private:
        std::string prefix ;
        FS::FSManager& manager ;
        Repo::Repository& repo ;

        void resolve_paths(std::vector<std::string>& args) ;
        std::string _resolve(std::string& path) ;
    };
};