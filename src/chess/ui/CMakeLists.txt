find_package(OpenGL 3 REQUIRED)

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_executable(chess_ui ${SOURCES})
target_compile_definitions(chess_ui PRIVATE -DASSETS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/assets")
target_link_libraries(chess_ui PUBLIC chess_core imgui_glfw glfw OpenGL::GL fmt stb)