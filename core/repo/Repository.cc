#include "Repository.h"

[[nodiscard]]Repo::Repository& Repo::Repository::getInstance(){
    static Repo::Repository r ;
    return r ;
}

void Repo::Repository::setRoot(std::filesystem::path _root)
{
    if (!std::filesystem::is_directory(_root) || !std::filesystem::is_empty(_root)) 
        throw std::runtime_error("Error in setting path for Repo Object") ;
    root = _root ;
    gitPath = root / ".git" ;
    indexPath = root / ".git" / "index" ;
}

const std::filesystem::path Repo::Repository::getRoot() const
{
    return root ;
}

const std::filesystem::path Repo::Repository::getGitPath() const 
{
    return gitPath ;
}

const std::filesystem::path Repo::Repository::getIndexPath() const
{
    return indexPath ;
}
