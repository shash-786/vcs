#pragma once
#include "Command.h"
#include "fs/FSManager.h"
#include "commands/WorkTree.h"

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

        std::string _resolve(std::string& path) ;
        std::string find_common_root(std::span<const std::string> args) ;

        void resolve_paths(std::vector<std::string>& args) ;
        void get_and_filter_work_tree_files(worktree& w, const std::vector<std::string>& args) ;
        void filter_work_tree_files(worktree& w, std::span<const std::string> args) ;
        void read_work_tree(worktree& w, const std::string& base) ;
        void _read_work_tree_recursive(worktree& w, const std::string& base) ;

        int push_excludes_from(worktree& w, const std::filesystem::path& wd) ;
    };
};