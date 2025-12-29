#include "CacheDir.h"
#include <fstream>
#include <netinet/in.h>
#include <cstring>

void CacheDir::init_index()
{
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

    const auto& data = res.value() ;
    if (!verify_index(data)) {
        fprintf(stderr, "Corrupt Index Header!") ;
        exit(1) ;
    }
    
    auto offset = sizeof (cache_header) ;
    for (int i = 0; i < entries; i++) {
        struct cache_entry* ce = reinterpret_cast<> ; 
        offset = offset + ce_size (ce) ;
        active_cache.push_back (*ce) ;
    }
}