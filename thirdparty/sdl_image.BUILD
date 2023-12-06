load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "sdl_image",
    hdrs = glob(["include/*.h"]), 
    includes = ["include/"],
    linkopts = ["-L/usr/local/lib", "-lsdl2_image"],
    visibility = ["//visibility:public"],
)
