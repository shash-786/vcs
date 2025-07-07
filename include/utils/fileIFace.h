#pragma once
#include <filesystem>

class fileIFace {
public:
  // File Functions
  virtual bool readFile(const std::filesystem::path &inputfile) = 0;

  virtual bool writeFile(const std::filesystem::path &inputfile,
                         std::vector<uint8_t> data) = 0;
  virtual bool removeFile(const std::filesystem::path &inputFile) = 0;

  // Directory Functions
  virtual bool removeDirectory(const std::filesystem::path &inputFile) = 0;
  virtual bool createDirectory(const std::filesystem::path &inputFile) = 0;

  // Check Functions
  virtual bool isPresent(const std::filesystem::path &inputfile) = 0;

  virtual std::filesystem::file_time_type
  lastModified(const std::filesystem::path &inputfile) = 0;

  // Utility Functions
  virtual uintmax_t getFilesize(const std::filesystem::path &inputFile) = 0;
};
