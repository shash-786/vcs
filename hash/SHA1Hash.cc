#include "SHA1Hash.h"
#include <vector>
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <fstream>
#include <sstream>
#include <iostream>

#define CHUNK 0x200000
#define SHA1_DIGEST_LENGTH 20

Result<std::string, errorcode> Hash::SHA1Hash::compute_hash(std::span<const uint8_t> bytes)
{
    EVP_MD_CTX *ctx = EVP_MD_CTX_create() ;
    const EVP_MD *sha_obj = EVP_sha1() ;
    EVP_DigestInit_ex(ctx, sha_obj, nullptr) ;

    if (EVP_DigestUpdate(ctx, bytes.data(), bytes.size()) != 1) {
        EVP_MD_CTX_free(ctx) ;
        fprintf(stderr, "error in updating sha1 context - content") ;
        return errorcode::UpdateError ;
    }

    unsigned char hash[SHA1_DIGEST_LENGTH] ;
    if (EVP_DigestFinal_ex(ctx, hash, nullptr) != 1) {
        EVP_MD_CTX_free(ctx) ;
        fprintf(stderr, "error in finalising hash") ;
        return errorcode::InternalError ;
    }

    EVP_MD_CTX_free(ctx) ;
    EVP_cleanup() ;  
    return hash_to_hex(hash) ;
}

Result<std::string, errorcode> Hash::SHA1Hash::compute_hash(std::string header, std::span<const uint8_t> bytes)
{
    EVP_MD_CTX *ctx = EVP_MD_CTX_create() ;
    const EVP_MD *sha_obj = EVP_sha1() ;
    EVP_DigestInit_ex(ctx, sha_obj, nullptr) ;

    std::vector<uint8_t> headerBytes(header.begin(), header.end()) ;

    if (EVP_DigestUpdate(ctx, headerBytes.data(), headerBytes.size()) != 1) {
        EVP_MD_CTX_free(ctx) ;
        fprintf(stderr, "error in updating sha1 context - header") ;
        return errorcode::UpdateError ;
    }

    if (EVP_DigestUpdate(ctx, bytes.data(), bytes.size()) != 1) {
        EVP_MD_CTX_free(ctx) ;
        fprintf(stderr, "error in updating sha1 context - header") ;
        return errorcode::UpdateError ;
    }

    unsigned char hash[SHA1_DIGEST_LENGTH] ;
    if (EVP_DigestFinal_ex(ctx, hash, nullptr) != 1) {
        EVP_MD_CTX_free(ctx) ;
        fprintf(stderr, "error in finalising hash") ;
        return errorcode::InternalError ;
    }

    EVP_MD_CTX_free(ctx) ;
    EVP_cleanup() ;  
    return hash_to_hex(hash) ;
}

Result<std::string, errorcode> Hash::SHA1Hash::compute_hash(std::string header, const std::filesystem::path filePath)
{
    EVP_MD_CTX *ctx = EVP_MD_CTX_create() ;
    const EVP_MD *sha_obj = EVP_sha1() ;
    EVP_DigestInit_ex(ctx, sha_obj, nullptr) ;

    std::vector<uint8_t> headerBytes(header.begin(), header.end()) ;

    if (EVP_DigestUpdate(ctx, headerBytes.data(), headerBytes.size()) != 1) {
        EVP_MD_CTX_free(ctx) ;
        fprintf(stderr, "error in updating sha1 context - header") ;
        return errorcode::UpdateError ;
    }

    std::ifstream file(filePath, std::ios::binary) ;
    if (!file.is_open()) {
        EVP_MD_CTX_free(ctx) ;
        fprintf(stderr, "error in opening file") ;
        return errorcode::FailToOpen ;
    }

    char *buffer = new char[CHUNK] ;
    while (file.read(buffer, CHUNK) || file.gcount() > 0) {
        if (EVP_DigestUpdate(ctx, buffer, file.gcount()) != 1) {
            EVP_MD_CTX_free(ctx) ;
            delete[] buffer ;
            file.close() ;
            fprintf(stderr, "error in updating the digest context - istream") ;
            return errorcode::UpdateError ;
        }
    }
    
    if (file.fail() && !file.eof()) {
        file.close() ;
        delete[] buffer ;
        std::cerr << "Stream Read Error" << std::endl ;
        return errorcode::ReadError;
    }

    file.close() ;
    delete[] buffer ;

    unsigned char hash[SHA1_DIGEST_LENGTH];
    if (EVP_DigestFinal_ex(ctx, hash, nullptr) != 1) {
        EVP_MD_CTX_free(ctx) ;
        fprintf(stderr, "error in finalising hash") ;
        return errorcode::InternalError ;
    }

    EVP_MD_CTX_free(ctx);
    EVP_cleanup();  
    return hash_to_hex(hash) ;
}

std::string Hash::SHA1Hash::hash_to_hex(std::span<const uint8_t> hash)
{
    std::stringstream ss ;
    ss << std::hex << std::setfill('0') ;
    for (int i = 0; i < SHA1_DIGEST_LENGTH; i++) {
        ss << std::setw(2) << static_cast<int>(hash[i]) ;
    }
    // std::cout << "-------> SIZE OF THE HASH STRING : " << ss.str().size() << std::endl ;
    return ss.str() ;
}
