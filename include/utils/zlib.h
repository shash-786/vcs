#pragma once

#include "../../common/errors.h"
#include "../../common/result.h"
#include <cstdint>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <memory>
#include <vector>
#include <zlib.h>

#define CHUNK 16384

class ZlibUtils {
public:
  Result<std::vector<uint8_t>, errorcode>
  compress(const std::filesystem::path &inputFile);

  Result<std::vector<uint8_t>, errorcode>
  decompress(const std::filesystem::path &inputFile);

  bool isZlib(std::ifstream &file);
};
