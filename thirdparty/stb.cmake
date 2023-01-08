include(FetchContent)

FetchContent_Declare(
    stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG        8b5f1f37b5b75829fc72d38e7b5d4bcbf8a26d55
)

FetchContent_MakeAvailable(stb)

file(GLOB SOURCES ${stb_SOURCE_DIR}/*.h ${stb_SOURCE_DIR}/*.c)
add_library(stb STATIC ${SOURCES})
set_target_properties(stb PROPERTIES LINKER_LANGUAGE C)
include_directories(${stb_SOURCE_DIR})

message("stb is added")