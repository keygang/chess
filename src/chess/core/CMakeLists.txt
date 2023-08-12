option(CHESS_CORE_TESTING "Enabling chess core testing" ON)
option(CODECOVERAGE "Enabling code coverage" OFF)

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
list(FILTER SOURCES EXCLUDE REGEX ".*\\.test\\.cpp$")

add_library(chess_core
        STATIC
            ${SOURCES})

if(CHESS_CORE_TESTING)
    enable_testing()
    add_subdirectory(tests)

    if(CODECOVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        # Add required flags (GCC & LLVM/Clang)
        add_library(coverage_config INTERFACE)
        target_compile_options(coverage_config INTERFACE
                -O0        # no optimization
                -g         # generate debug info
                --coverage # sets all required flags
                )

        target_link_options(coverage_config INTERFACE --coverage)
    endif()
endif()