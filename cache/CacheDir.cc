#include "CacheDir.h"
#include <fstream>
#include <netinet/in.h>
#include <cstring>
#include <sys/stat.h>
#include <iostream>

cache_entry_ptr CacheDir::make_cache_entry_unique(size_t size)
{
    void* ptr = calloc (1, size) ;
    if (!ptr) {
        throw std::bad_alloc();
    }
    cache_entry* p_ce = static_cast<cache_entry*> (ptr) ;
    return cache_entry_ptr(p_ce, std::free);
}

size_t CacheDir::find_position_in_sorted_index(const std::string &name)
{
    return size_t();
}

void CacheDir::init_index()
{
    cache_header h ;
    h.hdr_signature = htonl (CACHE_SIGNATURE) ;
    h.hdr_entries = htonl (0) ;
    h.hdr_version = htonl (2) ;
    size_t header_size = sizeof (h) ;
    
    auto pdata = reinterpret_cast<uint8_t*>(&h) ;
    std::vector<uint8_t> data ;
    std::memcpy (data.data(), pdata, header_size) ;
    if (auto res = manager.writeFile(repo.getIndexPath(), "index", data) ; !res ) {
        fprintf(stderr, "Could not initialize the index file") ;
        exit (1) ;
    }
}

bool CacheDir::verify_index(const std::vector<uint8_t>& data)  
{
    cache_header header ;
    memcpy(&header, data.data(), sizeof (cache_header)) ;

    if (ntohl(header.hdr_signature) != CACHE_SIGNATURE) {
        fprintf(stderr, "header did not match!") ;
        return false ;
    }

    auto version = ntohl(header.hdr_version) ;
    if (version < 2 || version > 4) {  
        fprintf(stderr, "version did not match!") ;
        return false ;
    }

    entries = ntohl(header.hdr_entries) ;
    return true ;
}

void CacheDir::read_index()
{
    const auto& index_path = repo.getIndexPath() ;
    if (!manager.isPresent(index_path)) {
        init_index() ;
        return ;
    }

    auto res = manager.readFile(index_path) ;
    if (res.err_present()) {
        fprintf(stderr, "Cannot open existing index file!") ;
        exit(1) ;
    }

    auto data = std::move(res.value()) ;
    if (!verify_index(data)) {
        fprintf(stderr, "Corrupt Index Header!") ;
        exit(1) ;
    }
    
    auto offset = sizeof (cache_header) ;
    for (int i = 0; i < entries; i++) {
        struct cache_entry* ce = reinterpret_cast<cache_entry*>(data.data() + offset) ; 
        offset = offset + ce_size (ce) ;
        active_cache.push_back (ce) ;
    }
    std::cout << entries << " successfully loaded from the cache " << std::endl ;
}

void CacheDir::write_index() 
{
}

bool CacheDir::insert_entry (const std::string& name) {
    read_index () ;
    struct stat st ;
    if (lstat(name.c_str(), &st) != 0) {
        fprintf (stderr, "Cannot stat %s lstat returned error message :%s", name.c_str(), strerror(errno)) ; 
        exit (1) ;
    }

    size_t nameLen = name.size(), ce_size = cache_entry_size (nameLen)  ;
    std::unique_ptr<cache_entry, void(*)(void*)> ce = make_cache_entry_unique (ce_size) ;
    std::memcpy (ce->name, name.c_str(), nameLen) ;
    
    ce->ce_ctime.sec = htonl (st.st_ctim.tv_sec) ;
    ce->ce_ctime.nsec = htonl (st.st_ctim.tv_nsec) ;
    ce->ce_mtime.sec = htonl (st.st_mtim.tv_sec) ;
    ce->ce_mtime.nsec = htonl (st.st_mtim.tv_nsec) ;
    ce->ce_gid = htonl (st.st_gid) ;
    ce->ce_dev = htonl (st.st_dev) ;
    ce->ce_ino = htonl (st.st_ino) ;
    ce->ce_size = htonl (st.st_size) ;
    ce->ce_mode = htonl (st.st_mode) ;
    // ce->flag handle here

    size_t pos = find_position_in_sorted_index (name) ;
    // pos handling
    std::cout << "Inserted Cache Entry of " << name << " at position " << pos << std::endl ;
    active_cache.insert (active_cache.begin() + pos, ce) ;
}