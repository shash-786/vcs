#include "../../include/utils/filemanager.h"
#include <cstdint>
#include <filesystem>
#include <vector>

bool filemanager::readFile(const std::filesystem::path &inputFile) {
  return true;
}

bool filemanager::writeFile(const std::filesystem::path &inputFile,
                            std::vector<uint8_t> data) {
  return true;
}

bool filemanager::removeFile(const std::filesystem::path &inputFile) {
  return true;
}

bool filemanager::removeDirectory(const std::filesystem::path &path) {
  return true;
}

bool filemanager::createDirectory(const std::filesystem::path &path) {
  return true;
}

bool filemanager::isPresent(const std::filesystem::path &inputFile) {
  return true;
}

std::filesystem::file_time_type
lastModified(const std::filesystem::path &inputFile) {
  std::filesystem::file_time_type t;
  return t;
}

uintmax_t getFilesize(const std::filesystem::path &inputFile) {
  uintmax_t size = 0;
  return size;
}
