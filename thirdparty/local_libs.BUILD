load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "sdl",
    srcs = ['sdl2/2.28.5/lib/libSDL2.dylib'],
    hdrs = glob(["sdl2/2.28.5/include/SDL2/*.h"]), 
    defines = ['SDL_DISABLE_IMMINTRIN_H'],
    includes = ["sdl2/2.28.5/include/SDL2"],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "sdl_image",
    srcs = ['sdl2_image/2.8.0/lib/libSDL2_image.dylib'],
    hdrs = glob(["sdl2_image/2.8.0/include/SDL2/*.h"]), 
    includes = ["sdl2_image/2.8.0/include/SDL2/"],
    visibility = ["//visibility:public"],
)