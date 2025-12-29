#pragma once
#include <vector>
#include "Cache.h"
#include "fs/FSManager.h"
#include "core/repo/Repository.h"

class CacheDir {
    public:
    void init_index () ;
    void read_index () ;
    void write_index () ;
    bool insert_entry (const std::string& path) ;
    CacheDir(FS::FSManager& manager_, Repo::Repository& repo_) : manager(manager_), repo(repo_) {} ;

    private:
    bool verify_index (const std::vector<uint8_t>& data) ;
    FS::FSManager& manager ;
    Repo::Repository& repo ;
    std::vector<cache_entry> active_cache ;
    int entries=0; 
} ;