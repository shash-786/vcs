#pragma once
#include <string>

#define CACHE_SIGNATURE 0x44495243	/* "DIRC" */
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
	std::string name ; 
} ;