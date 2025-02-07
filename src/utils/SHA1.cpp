#include "../../include/types.h"
#include <cstdio>
#include <iomanip>
#include <istream>
#include <openssl/evp.h>
#include <sstream>

#define SHA_DIGEST_LENGTH 40

std::string hash_to_hex(unsigned char hash[]) {
  std::stringstream ss;
  ss << std::hex << std::setfill('0');
  for (int i = 0; i < SHA_DIGEST_LENGTH; i++) {
    ss << std::setw(2) << static_cast<int>(hash[i]);
  }
  return ss.str();
}

std::string compute_sha1(const char *metadata, int m_len, const char *content,
                         int c_len) {
  EVP_MD_CTX *ctx = EVP_MD_CTX_create();
  if (!ctx) {
    fprintf(stderr, "error in creating envolope context");
    return "";
  }

  const EVP_MD *sha_obj = EVP_sha1();
  if (!sha_obj) {
    EVP_MD_CTX_free(ctx);
    fprintf(stderr, "error in creating sha1 context");
    return "";
  }

  EVP_DigestInit_ex(ctx, sha_obj, nullptr);

  if (EVP_DigestUpdate(ctx, metadata, m_len) != 1) {
    EVP_MD_CTX_free(ctx);
    fprintf(stderr, "error in updating sha1 context - metadata");
    return "";
  }

  if (EVP_DigestUpdate(ctx, content, c_len) != 1) {
    EVP_MD_CTX_free(ctx);
    fprintf(stderr, "error in updating sha1 context - content");
    return "";
  }

  unsigned char hash[SHA_DIGEST_LENGTH];
  if (EVP_DigestFinal_ex(ctx, hash, nullptr) != 1) {
    EVP_MD_CTX_free(ctx);
    fprintf(stderr, "error in finalising hash");
    return "";
  }

  EVP_MD_CTX_free(ctx);
  return hash_to_hex(hash);
}

std::string compute_sha1(const char *metadata, int m_len, std::ifstream &i) {
  EVP_MD_CTX *ctx = EVP_MD_CTX_create();
  if (!ctx) {
    fprintf(stderr, "error in creating envolope context");
    return "";
  }

  const EVP_MD *sha_obj = EVP_sha1();
  if (!sha_obj) {
    EVP_MD_CTX_free(ctx);
    fprintf(stderr, "error in creating sha1 context");
    return "";
  }
  EVP_DigestInit_ex(ctx, sha_obj, nullptr);

  if (EVP_DigestUpdate(ctx, metadata, m_len) != 1) {
    EVP_MD_CTX_free(ctx);
    fprintf(stderr, "error in updating the digest context - metadata");
    return "";
  }

  char *buffer = new char[4096];
  while (i.read(buffer, 4096) || i.gcount() > 0) {
    if (EVP_DigestUpdate(ctx, buffer, i.gcount()) != 1) {
      EVP_MD_CTX_free(ctx);
      delete[] buffer;
      fprintf(stderr, "error in updating the digest context - istream");
      return "";
    }
  }
  delete[] buffer;

  unsigned char hash[SHA_DIGEST_LENGTH];
  if (EVP_DigestFinal_ex(ctx, hash, nullptr) != 1) {
    EVP_MD_CTX_free(ctx);
    fprintf(stderr, "error in finalising hash");
    return "";
  }

  EVP_MD_CTX_free(ctx);
  return hash_to_hex(hash);
}
