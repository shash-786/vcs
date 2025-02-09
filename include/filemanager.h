#pragma once
#include "fileIFace.h"

class filemanager : public fileIFace {
public:
  // File Functions
  virtual bool readFile(const std::filesystem::path &inputfile) override;

  virtual bool writeFile(const std::filesystem::path &inputfile,
                         std::vector<uint8_t> data) override;
  virtual bool removeFile(const std::filesystem::path &inputFile) override;

  // Directory Functions
  virtual bool removeDirectory(const std::filesystem::path &path) override;
  virtual bool createDirectory(const std::filesystem::path &path) override;

  // Check Functions
  virtual bool isPresent(const std::filesystem::path &inputfile) override;

  virtual std::filesystem::file_time_type
  lastModified(const std::filesystem::path &inputfile) override;

  // Utility Functions
  virtual uintmax_t
  getFilesize(const std::filesystem::path &inputFile) override;
};
