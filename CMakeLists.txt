cmake_minimum_required(VERSION 3.24)
project(chess)

set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror")

include(${PROJECT_SOURCE_DIR}/thirdparty/fmt.cmake)
include(${PROJECT_SOURCE_DIR}/thirdparty/imgui.cmake)
include(${PROJECT_SOURCE_DIR}/thirdparty/sdl.cmake)
include(${PROJECT_SOURCE_DIR}/thirdparty/sdl_image.cmake)

include_directories(src)
add_subdirectory(src)
