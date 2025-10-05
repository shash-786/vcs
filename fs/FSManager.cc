#include "FSManager.h"
#include <fstream>
#include <iostream>

#define CHUNK 0x200000

Result<std::vector<uint8_t>, errorcode> FS::FSManager::readFile(const std::filesystem::path &path)
{
    std::vector<uint8_t> bytes ;
    std::ifstream fstrm(path, std::ios::binary) ;
    if (!fstrm.is_open()) {
        std::cerr << "Unable to open the stream" << std::endl ;
        return errorcode::FailToOpen;
    }

    std::vector<uint8_t> buffer(CHUNK);

    if (fstrm.good())  {
        while (fstrm.read(reinterpret_cast<char*>(buffer.data()), buffer.size()) || fstrm.gcount() > 0) {
            bytes.insert(bytes.end(), buffer.begin(), buffer.begin() + fstrm.gcount());
        }
    }

    if (fstrm.fail() && !fstrm.eof()) {
        fstrm.close() ;
        std::cerr << "Stream Read Error" << std::endl ;
        return errorcode::ReadError;
    }
    return bytes ;
}

bool FS::FSManager::writeFile(const std::filesystem::path &path, const std::string &fname, std::span<const uint8_t> bytes)
{
    std::filesystem::path complete_path = path / fname ;
    std::ofstream fstrm(complete_path, std::ios::binary) ;

    if (!fstrm.is_open()) {
        std::cerr << "Unable to open the stream" << std::endl ;
        return false ;
    }

    fstrm.write(reinterpret_cast<const char*>(bytes.data()), bytes.size()) ;

    if (!fstrm) {
        std::cerr << "Failed to write all data to file: " << complete_path << std::endl;
        return false ;
    }
    fstrm.close() ;
    return true ;
}

bool FS::FSManager::removeFile(const std::filesystem::path &inputFile)
{
    if (!isPresent(inputFile)) {
        std::cerr << "Does not exist -->" << inputFile << std::endl;
        return false;
    }

    if (std::filesystem::is_directory(inputFile)) {
        std::cerr<< "It is a directory." << std::endl ;
        return false ;
    }

    return std::filesystem::remove(inputFile) ;
}
  
bool FS::FSManager::removeDirectory(const std::filesystem::path &dirpath)
{
    if (!isPresent(dirpath)) {
        return false ;
    }

    if (std::filesystem::is_regular_file(dirpath)) {
        std::cerr<< "It is a regular file." << std::endl ;
        return false ;
    }
    return std::filesystem::remove_all(dirpath) ;
}

bool FS::FSManager::createDirectory(const std::filesystem::path &root, const std::string &dname) 
{
    std::filesystem::path complete_path = root / dname;
    return std::filesystem::create_directory(complete_path);
}

bool FS::FSManager::createDirectories(const std::filesystem::path &dirs)
{
    return std::filesystem::create_directories(dirs) ; 
}

bool FS::FSManager::isPresent(const std::filesystem::path &inputfile)
{
    return std::filesystem::exists(inputfile) ;
}

Result<std::filesystem::file_time_type, errorcode> FS::FSManager::lastModified(const std::filesystem::path &inputFile)
{
    if (!isPresent(inputFile)) {
        return errorcode::NotFound ;
    }

    return std::filesystem::last_write_time(inputFile) ;
}

Result<uintmax_t, errorcode> FS::FSManager::getFilesize(const std::filesystem::path &inputFile)
{
    if (!isPresent(inputFile)) {
        return errorcode::NotFound ;
    }

    return std::filesystem::file_size(inputFile) ;
}
