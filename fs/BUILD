
load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

cc_library (
    name = "fs_interface",

    srcs = [
        "FSIFace.h"
    ],

    deps = [
        "//common:common"
    ], 

    visibility = ["//visibility:private"],
)

cc_library (
    name = "fsmanager",

    hdrs = [
        "FSManager.h"
    ],

    srcs = [
        "FSManager.cc"
    ],

    deps = [
        ":fs_interface",
    ], 

    visibility = ["//visibility:public"],
)

cc_test (
    name = "fs_test",
    srcs = ["FSManagerTest.cc"],
    deps = [
        ":fsmanager",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ]
)