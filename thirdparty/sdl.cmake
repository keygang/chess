include(FetchContent)

FetchContent_Declare(
        sdl
        GIT_REPOSITORY git@github.com:libsdl-org/SDL.git
        GIT_TAG release-2.26.5
)

FetchContent_MakeAvailable(sdl)