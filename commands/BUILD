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