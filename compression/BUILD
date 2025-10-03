load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

cc_library (
    name = "compression_interface",

    srcs = [
        "CompressionIFace.h"
    ],

    deps = [
        "//common:common"
    ], 

    visibility = ["//visibility:private"],
)

cc_library (
    name = "zlib",

    hdrs = [
        "ZlibCompression.h"
    ],

    srcs = [
        "ZlibCompression.cc"
    ],

    deps = [
        ":compression_interface",
        "@zlib//:zlib"
    ], 

    visibility = ["//visibility:public"],
)

cc_test (
    name = "zlib_test",
    srcs = ["ZlibCompressionTest.cc"],
    deps = [
        ":zlib",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ]
)