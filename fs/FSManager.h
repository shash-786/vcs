#pragma once
#include "FSIFace.h"

namespace FS
{
    class FSManager : public FSIFace 
    {
        public:
        // bool readFile(const std::filesystem::path &inputfile) ;
        Result<std::vector<uint8_t>, errorcode> readFile(const std::filesystem::path &inputfile) override ;
        int writeFile(const std::filesystem::path &inputfile, const std::string& fname, std::span<const uint8_t> data) override;
        int removeFile(const std::filesystem::path &inputFile) override;
        int removeDirectory(const std::filesystem::path &dirpath) override;
        bool createDirectory(const std::filesystem::path &root, const std::string& dname) override;
        bool isPresent(const std::filesystem::path &inputfile) override;
        Result<std::filesystem::file_time_type, errorcode> lastModified(const std::filesystem::path &inputfile) override;
        Result<uintmax_t, errorcode> getFilesize(const std::filesystem::path &inputFile) override; 
    };
}