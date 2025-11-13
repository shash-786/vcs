#include "Init.h"
#include <iostream>

int Command::Init::execute(std::vector<std::string> &args)
{
    if (manager.isPresent(repo.getGitPath())) {
        std::cerr << ".git directory already exists\n";
        return 1;
    }

    repo.setRoot(std::filesystem::current_path()) ;
    manager.createDirectory(repo.getRoot(), ".git") ;
    manager.createDirectory(repo.getGitPath(), "objects") ;
    manager.createDirectories(repo.getGitPath() / "refs" / "heads");    

    std::string branch = "ref: refs/heads/main";
    std::span<uint8_t> bytes(reinterpret_cast<uint8_t*>(branch.data()), branch.size());

    bool ret = manager.writeFile(repo.getGitPath(), "HEAD", bytes) ;
    if (!ret) {
        std::cerr << ".git/heads cannot be created" ;
        return 1 ;
    }

    return 0 ;
}