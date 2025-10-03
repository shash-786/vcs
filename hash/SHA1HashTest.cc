#include <gtest/gtest.h>
#include "SHA1Hash.h"
#include <fstream>
#include <openssl/rand.h>

const std::filesystem::path TEMP_PATH = std::filesystem::temp_directory_path() / "ZLIB_TEST";
const std::string SAMPLE = "Hello Git! Calculate the SHA1 hash of this message" ;
#define SIZE 0x200000

class SHA1HashTestFixture: public ::testing::Test 
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
            throw std::runtime_error("Filesystem error") ;
        }
        
        std::filesystem::path filepath = TEMP_PATH / "random.bin" ;
        std::ofstream fstream(filepath, std::ios::binary) ;
        std::vector<uint8_t> random_bytes(SIZE) ;

        if (RAND_bytes(reinterpret_cast<unsigned char*>(random_bytes.data()), SIZE) != 1) {
            throw std::runtime_error("error generating random bytes") ;
        }

        if (fstream.is_open()) {
            fstream.write(reinterpret_cast<const char*>(random_bytes.data()), SIZE) ;
            fstream.close() ;
        } else {
            std::cerr << "Error: Unable to open file 'random.bin'." << std::endl;
        }
        
        filepath = TEMP_PATH / "hello.txt" ;
        std::ofstream fstream2(filepath, std::ios::binary) ;

        if (fstream2.is_open()) {
            fstream2 << SAMPLE ;
            fstream2.close() ;
        } else {
            std::cerr << "Error: Unable to open file 'hello.txt'." << std::endl;
        }
    }

    void TearDown() override {
        std::filesystem::remove_all(TEMP_PATH) ;
    }
};

TEST(SHA1HashTestSuite, HashComparison) {
    Hash::SHA1Hash hasher ;
    
    std::vector<uint8_t> bytes(SAMPLE.begin(), SAMPLE.end()) ;

    auto result = hasher.compute_hash(bytes) ;

    ASSERT_TRUE(result.success()) ;
    std::string hashStr = result.value() ;
    EXPECT_EQ(hashStr, "262e891e94649570d3ff52dfa87980f3d2dc049c") ;
}

TEST_F(SHA1HashTestFixture, FileHashCalculation) {
    Hash::SHA1Hash hasher ;
    std::string header = "blob " + std::to_string(SIZE) + '\0';
    std::cout << header << std::endl ;

    auto result = hasher.compute_hash(header, TEMP_PATH/"random.bin") ;
    ASSERT_TRUE(result.success()) ;
}

TEST_F(SHA1HashTestFixture, FileHashComparisonWithGitHeader) {
    Hash::SHA1Hash hasher;
    std::filesystem::path filepath = TEMP_PATH / "hello.txt";
    std::string header = "blob " + std::to_string(SAMPLE.length()) + '\0';
    auto result = hasher.compute_hash(header, filepath);

    ASSERT_TRUE(result.success());
    // $ echo -n "Hello Git! Calculate the SHA1 hash of this message" | git hash-object --stdin
    EXPECT_EQ(result.value(), "51107c4f1544465e05500263bc3b9632bc8e4ffb");
}

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv) ;
    return RUN_ALL_TESTS();
}