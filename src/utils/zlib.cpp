#include "../../include/utils/zlib.h"
#include <cassert>
#include <cstdio>

Result<std::vector<uint8_t>, errorcode>
ZlibUtils::compress(const std::filesystem::path &inputFile) {
  std::ifstream file(inputFile, std::ios::binary);
  std::vector<uint8_t> compressed;

  if (!file.is_open()) {
    fprintf(stderr, "error in opening file");
    return errorcode::FailToOpen;
  }

  int flush, ret;
  z_stream strm;
  strm.zalloc = Z_NULL;
  strm.zfree = Z_NULL;
  strm.opaque = Z_NULL;

  ret = deflateInit(&strm, Z_DEFAULT_COMPRESSION);
  if (ret != Z_OK) {
    fprintf(stderr, "deflateInit error with ret %d\n", ret);
    file.close();
    return errorcode::IntitializationError;
  }

  std::vector<uint8_t> in(CHUNK);
  std::vector<uint8_t> out(CHUNK);

  do {
    /* strm.avail_in: Number of bytes available in the input buffer;
     * strm.next_in: Pointer to the next byte to be read from the input buffer;
     * strm.avail_out: Number of bytes available in the output buffer;
     * strm.next_out: Pointer to the next free byte in the output buffer.
     */

    file.read(reinterpret_cast<char *>(in.data()), CHUNK);
    strm.avail_in = file.gcount();
    if (file.fail() && !file.eof()) { // Check for read errors (excluding EOF)
      (void)deflateEnd(&strm);
      file.close(); // Close the file
      fprintf(stderr, "Z_ERRNO: %d\n", Z_ERRNO);
      return errorcode::ReadError;
    }
    flush = file.eof() ? Z_FINISH : Z_NO_FLUSH;
    strm.next_in = in.data();

    do {
      strm.avail_out = CHUNK;
      strm.next_out = out.data();
      ret = deflate(&strm, flush);
      assert(ret != Z_STREAM_ERROR);
      int have = CHUNK - strm.avail_out;

      if (have > 0) {
        compressed.insert(compressed.end(), out.begin(), out.begin() + have);
      }

    } while (strm.avail_out == 0);
    assert(strm.avail_in == 0);

  } while (flush != Z_FINISH);
  assert(ret == Z_STREAM_END);

  (void)deflateEnd(&strm);
  return std::move(compressed);
}

Result<std::vector<uint8_t>, errorcode>
ZlibUtils::decompress(const std::filesystem::path &inputFile) {

  std::vector<uint8_t> raw_content;
  std::ifstream file(inputFile, std::ios::binary);

  if (!file.is_open()) {
    fprintf(stderr, "error in opening file\n");
    return errorcode::FailToOpen;
  }

  if (!isZlib(file)) {
    fprintf(stderr, "usage ZlibUtils::decompress: not a zlib file!\n");
    return errorcode::PermissionDenied;
  }

  int ret;
  unsigned have;
  z_stream strm;
  strm.zalloc = Z_NULL;
  strm.zfree = Z_NULL;
  strm.opaque = Z_NULL;

  std::vector<uint8_t> in(CHUNK);
  std::vector<uint8_t> out(CHUNK);

  strm.avail_in = 0;
  strm.next_in = Z_NULL;

  ret = inflateInit(&strm);
  if (ret != Z_OK) {
    fprintf(stderr, "inflateInit error with ret %d\n", ret);
    file.close();
    return errorcode::IntitializationError;
  }

  do {

    file.read(reinterpret_cast<char *>(in.data()), CHUNK);
    strm.avail_in = file.gcount();
    if (file.fail() && !file.eof()) { // Check for read errors (excluding EOF)
      (void)inflateEnd(&strm);
      file.close(); // Close the file
      fprintf(stderr, "Z_ERRNO: %d\n", Z_ERRNO);
      return errorcode::ReadError;
    }

    if (strm.avail_in == 0)
      break;
    strm.next_in = in.data();

    do {
      strm.avail_out = CHUNK;
      strm.next_out = out.data();
      ret = inflate(&strm, Z_NO_FLUSH);
      assert(ret != Z_STREAM_ERROR);

      switch (ret) {
      case Z_NEED_DICT:
        ret = Z_DATA_ERROR; /* and fall through */
      case Z_DATA_ERROR:
      case Z_MEM_ERROR:
        fprintf(stderr, "inflate error with ret %d\n", ret);
        file.close();
        (void)inflateEnd(&strm);
        return errorcode::IntitializationError;
      }

      have = CHUNK - strm.avail_out;

      if (have > 0) {
        raw_content.insert(raw_content.end(), out.begin(), out.begin() + have);
      }

    } while (strm.avail_out == 0);
  } while (ret != Z_STREAM_END);

  (void)inflateEnd(&strm);
  return raw_content;
}

bool ZlibUtils::isZlib(std::ifstream &file) {
  uint16_t header;
  file.read(reinterpret_cast<char *>(&header), sizeof(header));
  if (file.gcount() < sizeof(header)) {
    fprintf(stderr, "not enough bytes for a zlib file\n");
    return false;
  }
  return (header == 0x789C || header == 0x78DA || header == 0x7801 ||
          header == 0x785E || header == 0x7885);
}
