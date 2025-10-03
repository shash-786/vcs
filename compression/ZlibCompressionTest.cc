#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>
#include "ZlibCompression.h"


const std::filesystem::path TEMP_PATH = std::filesystem::temp_directory_path() / "ZLIB_TEST";
const std::string SAMPLE = "Hello Git! This is a long message to ensure that zlib has enough data to compress effectively. We must verify that the entire stream, headers, and footer checksums are intact" ;


class ZlibCompressionTestFixture: public ::testing::Test 
{
    protected:
    void SetUp() override {
        try {
            if (std::filesystem::create_directory(TEMP_PATH)) {
                std::cout << "Directory '" << TEMP_PATH << "' created successfully." << std::endl;
            } else {
                std::cout << "Directory '" << TEMP_PATH << "' already exists or could not be created." << std::endl;
            }
        } catch (const std::filesystem::filesystem_error& e) {
            std::cerr << "Filesystem error: " << e.what() << std::endl;
        }
        
        std::filesystem::path filepath = TEMP_PATH / "sample.txt" ;
        std::ofstream fstream(filepath) ;
        if (fstream.is_open()) {
            fstream << SAMPLE ;
            fstream.close() ;
        } else {
            std::cerr << "Error: Unable to open file 'example.txt'." << std::endl;
        }
    }

    void TearDown() override {
        std::filesystem::remove_all(TEMP_PATH) ;
    }
};

TEST_F(ZlibCompressionTestFixture, WrongPathTest) 
{
    Compression::ZlibUtils compressor ;
    auto result = compressor.compress(TEMP_PATH/"dummy.txt") ;
    ASSERT_TRUE(result.err_present()) << "Expected compress() to fail, but it returned success" ;
    EXPECT_EQ(result.error(), errorcode::FailToOpen) ;
}

TEST_F(ZlibCompressionTestFixture, RoundTripIntegrity) 
{
    Compression::ZlibUtils compressor;
    auto compression_result = compressor.compress(TEMP_PATH/"sample.txt");
    ASSERT_TRUE(compression_result.success()) << "Compression failed unexpectedly.";
    
    std::vector<uint8_t> compressed_bytes = compression_result.value();
    ASSERT_FALSE(compressed_bytes.empty()) << "Compression produced no data.";

    std::filesystem::path compressed_filepath = TEMP_PATH/"sample.zlib";
    std::ofstream compressed_file(compressed_filepath, std::ios::binary);
    ASSERT_TRUE(compressed_file.is_open()) << "Failed to open temporary file for writing compressed data.";

    compressed_file.write(reinterpret_cast<const char*>(compressed_bytes.data()), compressed_bytes.size());
    compressed_file.close();

    auto decompression_result = compressor.decompress(compressed_filepath);
    ASSERT_TRUE(decompression_result.success()) << "Decompression failed unexpectedly.";

    std::vector<uint8_t> decompressed_bytes = decompression_result.value();
    std::string decompressed_content(decompressed_bytes.begin(), decompressed_bytes.end());

    // std::cout << "comparing" << std::endl ;
    // std::cout << SAMPLE.substr(10) << std::endl ;
    // std::cout << decompressed_content.substr(10) << std::endl ;

    EXPECT_EQ(SAMPLE, decompressed_content);
}

TEST_F(ZlibCompressionTestFixture, NonZlibDecompress) 
{
    Compression::ZlibUtils decompressor;
    auto result = decompressor.decompress(TEMP_PATH/"sample.txt") ;
    ASSERT_TRUE(result.err_present()) << "Expected decompress() to fail, but it returned success" ;
    EXPECT_EQ(result.error(), errorcode::InvalidArgument) ;
}

int main(int argc, char *argv[]) 
{
    ::testing::InitGoogleTest(&argc, argv) ;
    return RUN_ALL_TESTS();
}
