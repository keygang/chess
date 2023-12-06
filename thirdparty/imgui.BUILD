load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "imgui",
    srcs = glob(["*.cpp", "backends/*_sdlrenderer2.cpp", "backends/*_sdl2.cpp", "backends/*_opengl*.cpp"]),
    hdrs = glob(["*.h", "backends/*_sdlrenderer2.h", "backends/*_sdl2.h", "backends/*_opengl*.h"]),
    includes = [".", "backends/"],
    visibility = ["//visibility:public"],
    deps = [
        '@local_libs//:sdl',
    ]
)
