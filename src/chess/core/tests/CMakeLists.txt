include(${PROJECT_SOURCE_DIR}/thirdparty/gtest.cmake)
include(GoogleTest)

file(GLOB_RECURSE TESTS_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/unit/*.test.cpp")

add_executable(chess_tests ${TESTS_SRCS})

gtest_add_tests(TARGET chess_tests SOURCES ${TESTS_SRCS} TEST_LIST tests)
set_tests_properties(${tests} PROPERTIES TIMEOUT 1)

target_link_libraries(chess_tests PUBLIC
        chess_core
        gtest_main)
