#pragma once
#include <string>

#define CACHE_SIGNATURE 0x44495243	/* "DIRC" */
#define CE_NAMEMASK  (0x0fff)
#define CE_VALID     (0x8000)
#define ce_namelen(ce) (CE_NAMEMASK & ntohs((ce)->ce_flags))
#define cache_entry_size(len) ((offsetof(struct cache_entry,name) + (len) + 8) & ~7)
#define ce_size(ce) cache_entry_size(ce_namelen(ce))

struct cache_header {
	uint32_t hdr_signature ;
	uint32_t hdr_version ;
	uint32_t hdr_entries ;
} ;

struct cache_time {
	uint32_t sec ;
	uint32_t nsec ;
} ;

struct cache_entry {
	struct cache_time ce_ctime ;
	struct cache_time ce_mtime ;
	uint32_t ce_dev ;
	uint32_t ce_ino ;
	uint32_t ce_mode ;
	uint32_t ce_uid ;
	uint32_t ce_gid ;
	uint32_t ce_size ;
	uint8_t sha1[20] ;
	uint16_t ce_flags ;
	// We had initially using std::string for this
	char name[0]; 
} ;

using cache_entry_ptr = std::unique_ptr<cache_entry, void(*)(void*)> ;
