file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
list(FILTER SOURCES EXCLUDE REGEX ".*\\.test\\.cpp$")

add_library(chess_core
        STATIC
            ${SOURCES})

enable_testing()
add_subdirectory(tests)