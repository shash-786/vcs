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

    CacheDir(FS::FSManager& manager_, Repo::Repository& repo_) : manager(manager_), repo(repo_) {} ;

    private:
    FS::FSManager& manager ;
    Repo::Repository& repo ;
    
    std::vector<cache_entry_ptr>active_cache ;
    int entries=0; 

    bool verify_index (const std::vector<uint8_t>& data) ;
    bool insert_entry (const std::string& name) ;
    size_t find_position_in_sorted_index (const std::string& name) ;
    cache_entry_ptr make_cache_entry_unique (size_t size) ;
} ;