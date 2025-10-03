#pragma once

#include "CompressionIFace.h"

namespace Compression {

  class ZlibUtils : public CompressionIFace 
  {
  public:
      Result<std::vector<uint8_t>, errorcode> compress(const std::filesystem::path &inputFile) override ;
      Result<std::vector<uint8_t>, errorcode> decompress(const std::filesystem::path &inputFile) override ;
      ~ZlibUtils() = default ;
  };

}