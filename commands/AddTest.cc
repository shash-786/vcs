#include <gtest/gtest.h>
#include "Add.h"
#include "core/repo/Repository.h"
#include "fs/FSManager.h"

class AddTest : public ::testing::Test {
protected:
    FS::FSManager manager;
    Repo::Repository& repo = Repo::Repository::getInstance();
    Command::Add add_cmd{manager, repo, ""};
};

TEST_F(AddTest, FindCommonRoot_Simple) {
    std::vector<std::string> args = {"src/main.cc", "src/utils.cc"};
    std::string root = add_cmd.find_common_root(args);
    EXPECT_EQ(root, "src/");
}

TEST_F(AddTest, FindCommonRoot_Deep) {
    std::vector<std::string> args = {"src/api/v1/main.cc", "src/api/v1/utils.cc"};
    std::string root = add_cmd.find_common_root(args);
    EXPECT_EQ(root, "src/api/v1/");
}

TEST_F(AddTest, FindCommonRoot_NoCommon) {
    std::vector<std::string> args = {"src/main.cc", "include/utils.h"};
    std::string root = add_cmd.find_common_root(args);
    EXPECT_EQ(root, "");
}

TEST_F(AddTest, FindCommonRoot_RootFile) {
    std::vector<std::string> args = {"main.cc", "utils.cc"};
    std::string root = add_cmd.find_common_root(args);
    EXPECT_EQ(root, "");
}

TEST_F(AddTest, Match_Exact) {
    std::vector<bool> seen(1, false);
    bool res = add_cmd.match("main.cc", "main.cc", seen, 0);
    EXPECT_TRUE(res);
    EXPECT_TRUE(seen[0]);
}

TEST_F(AddTest, Match_Pattern) {
    std::vector<bool> seen(1, false);
    bool res = add_cmd.match("*.cc", "main.cc", seen, 0);
    EXPECT_TRUE(res);
    EXPECT_TRUE(seen[0]);
}

TEST_F(AddTest, Match_Fail) {
    std::vector<bool> seen(1, false);
    bool res = add_cmd.match("*.h", "main.cc", seen, 0);
    EXPECT_FALSE(res);
    EXPECT_FALSE(seen[0]);
}

TEST_F(AddTest, Resolve_Simple) {
    Command::Add cmd(manager, repo, "src/");
    std::string path = "main.cc";
    std::string resolved = cmd._resolve(path);
    EXPECT_EQ(resolved, "src/main.cc");
}

TEST_F(AddTest, Resolve_Parent) {
    Command::Add cmd(manager, repo, "src/api/");
    std::string path = "../main.cc";
    std::string resolved = cmd._resolve(path);
    EXPECT_EQ(resolved, "src/main.cc");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
