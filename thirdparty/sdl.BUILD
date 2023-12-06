load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "sdl",
    hdrs = glob(["sdl2/2.28.5/include/*.h"]), 
    includes = ["sdl2/2.28.5/include/"],
    # linkopts = ["-L/usr/local/lib", "-lsdl2"],
    visibility = ["//visibility:public"],
)
