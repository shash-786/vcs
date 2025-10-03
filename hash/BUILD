load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

cc_library (
    name = "hash_interface",

    srcs = [
        "HashIFace.h"
    ],

    deps = [
        "//common:common"
    ], 

    visibility = ["//visibility:private"],
)

cc_library (
    name = "sha1",

    hdrs = [
        "SHA1Hash.h"
    ],

    srcs = [
        "SHA1Hash.cc"
    ],

    deps = [
        ":hash_interface",
        "@openssl//:ssl", 
    ], 

    visibility = ["//visibility:public"],
)

cc_test (
    name = "sha1_test",
    srcs = ["SHA1HashTest.cc"],
    deps = [
        ":sha1",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ]
)
