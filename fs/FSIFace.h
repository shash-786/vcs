#pragma once 
#include <filesystem>
#include <vector>
#include <span>
#include "common/result.h"
#include "common/errors.h"

namespace FS 
{
    class FSIFace 
    {
    public:
        virtual Result<std::vector<uint8_t>, errorcode> readFile(const std::filesystem::path &inputfile) = 0;
        virtual bool writeFile(const std::filesystem::path &path, const std::string& fname, std::span<const uint8_t> data) = 0;
        virtual bool removeFile(const std::filesystem::path &inputFile) = 0;
        virtual bool removeDirectory(const std::filesystem::path &dirpath) = 0;
        virtual bool createDirectory(const std::filesystem::path &root, const std::string& dname) = 0;
        virtual bool createDirectories(const std::filesystem::path &dirs) = 0;
        virtual bool isPresent(const std::filesystem::path &inputfile) = 0;
        virtual Result<std::filesystem::file_time_type, errorcode> lastModified(const std::filesystem::path &inputfile) = 0;
        virtual Result<uintmax_t, errorcode> getFilesize(const std::filesystem::path &inputFile) = 0; 
    }; 
}