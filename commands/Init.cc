#include "Init.h"
#include <iostream>

std::optional<errorcode> Command::Init::execute(const std::vector<std::string> &args)
{
    if (manager.isPresent(repo.getGitPath())) {
        std::cerr << ".git directory already exists\n";
        return errorcode::AlreadyExists;
    }

    manager.createDirectory(repo.getRoot(), ".git") ;
    manager.createDirectory(repo.getGitPath(), "objects") ;
    manager.createDirectories(repo.getGitPath() / "refs" / "heads");    

    std::string branch = "ref: refs/heads/main";
    std::span<uint8_t> bytes(reinterpret_cast<uint8_t*>(branch.data()), branch.size());

    bool ret = manager.writeFile(repo.getGitPath(), "HEAD", bytes) ;
    if (!ret) {
        std::cerr << ".git/heads cannot be created" ;
        return errorcode::IntitializationError ;
    }

    bool ret = manager.writeFile(repo.getGitPath(), "index", std::vector<uint8_t>()) ;
    if (!ret) {
        std::cerr << ".git/index cannot be created" ;
        return errorcode::IntitializationError ;
    }
    return {};
}