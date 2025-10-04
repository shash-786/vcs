#include <gtest/gtest.h>
#include "FSManager.h"
#include <fstream>

const std::filesystem::path TEMP_PATH = std::filesystem::temp_directory_path() / "FS_TEST" ;
const std::string SAMPLE = "Hello Git! This sample string for FS Operations" ;

class FSManagerTestFixture : public ::testing::Test {
    protected:

    void SetUp() override {
        try {
            if (std::filesystem::create_directory(TEMP_PATH)) {
                std::cout << "Directory '" << TEMP_PATH << "' created successfully." << std::endl;
            } else {
                std::cout << "Directory '" << TEMP_PATH << "' already exists or could not be created." << std::endl;
            }
        } catch (const std::filesystem::filesystem_error& e) {
            throw std::runtime_error("Filesystem error") ;
        }

        std::filesystem::path filepath = TEMP_PATH / "sample.txt" ;
        std::ofstream fstream(filepath) ;
        if (fstream.is_open()) {
            fstream << SAMPLE ;
            fstream.close() ;
        } else {
            throw std::runtime_error("FileStream error") ;
        }
    }

    void TearDown() override {
        std::filesystem::remove_all(TEMP_PATH) ;
    }
};

TEST_F(FSManagerTestFixture, ReadTest) {
    FS::FSManager m ;
    auto result = m.readFile(TEMP_PATH/"sample.txt") ;
    ASSERT_TRUE(result.success()) ;
    std::vector<uint8_t> bytes = result.value() ;
    EXPECT_EQ(SAMPLE, std::string(bytes.begin(), bytes.end()) ) ;
}

TEST_F(FSManagerTestFixture, FileStatTest) {
    FS::FSManager m ;
    auto result = m.getFilesize(TEMP_PATH/"sample.txt") ;
    ASSERT_TRUE(result.success()) ;
    auto ans = m.lastModified(TEMP_PATH/"sample.txt") ;
    ASSERT_TRUE(ans.success()) ;
}

TEST_F(FSManagerTestFixture, WriteRemoveTest) {
    FS::FSManager m ;
    const std::string FILENAME = "test.txt" ;
    std::vector<uint8_t> bytes(SAMPLE.begin(), SAMPLE.end()) ;

    int result = m.writeFile(TEMP_PATH, FILENAME, bytes) ;
    EXPECT_EQ(result, 1) << "Write File has failed";
    
    result = m.removeFile(TEMP_PATH/FILENAME) ;
    EXPECT_EQ(result, 1) << "Remove File Failure";
}

TEST_F(FSManagerTestFixture, DirectoryTest) {
    FS::FSManager m ;
    std::string DIR = "SAMPLE_DIR" ;
    bool result = m.createDirectory(TEMP_PATH, DIR) ;
    ASSERT_TRUE(result) ;
    result = m.isPresent(TEMP_PATH/DIR) ;
    EXPECT_TRUE(result) ;
    result = m.removeDirectory(TEMP_PATH/DIR) ;
    ASSERT_TRUE(result) ;
}


int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv) ;
    return RUN_ALL_TESTS() ;
}