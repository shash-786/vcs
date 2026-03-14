load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

cc_library (
    name = "command_interface",
    srcs = [ "Command.h" ],
    deps = [
        "//common:common" ,
        "//core/repo:repo"
    ], 
    visibility = ["//visibility:private"],
)

cc_library (
    name = "init",
    hdrs = [ "Init.h"], 
    srcs = [ "Init.cc"],
    deps = [
        ":command_interface" ,
        "//fs:fsmanager" ,
    ], 
    visibility = ["//visibility:public"],
)

cc_library (
    name = "worktree",
    hdrs = ["WorkTree.h"],
    visibility = ["//visibility:private"],
)

cc_library (
    name = "add",
    hdrs = ["Add.h"],
    srcs = ["Add.cc"],
    deps = [
        ":command_interface",
        "//fs:fsmanager",
        ":worktree"
    ],
    visibility = ["//visibility:public"],
)

cc_test(
    name = "add_test",
    srcs = ["AddTest.cc"],
    deps = [
        ":add",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
        "//core/repo:repo",
        "//fs:fsmanager"
    ],
)
