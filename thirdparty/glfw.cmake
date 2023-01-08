include(FetchContent)

FetchContent_Declare(
        glfw
        GIT_REPOSITORY "https://github.com/glfw/glfw"
        GIT_TAG        3.3.8
)

FetchContent_MakeAvailable(glfw)

message("glfw is added")