package(default_visibility = ["//visibility:public"])

cc_library(
    name = "git_object",
    hdrs = [
        "GitObject.h",
    ],
    deps = [
        "//common:errors",
        "//hash:sha1_hash",
    ],
)

cc_library(
    name = "blob",
    srcs = [
        "Blob.cc",
    ],
    hdrs = [
        "Blob.h",
    ],
    deps = [
        ":git_object",
    ],
)

cc_library(
    name = "tree",
    srcs = [
        "Tree.cc",
    ],
    hdrs = [
        "Tree.h",
    ],
    deps = [
        ":git_object",
    ],
)

cc_library(
    name = "commit",
    srcs = [
        "Commit.cc",
    ],
    hdrs = [
        "Commit.h",
    ],
    deps = [
        ":git_object",
    ],
)