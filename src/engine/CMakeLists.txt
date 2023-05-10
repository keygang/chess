file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_library(engine
        STATIC
        ${SOURCES})

find_package(OpenGL)

target_link_libraries(engine SYSTEM imgui_glfw SDL2-static SDL2_image OpenGL::GL fmt)