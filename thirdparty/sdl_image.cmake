include(FetchContent)

FetchContent_Declare(
    SDL_image
    GIT_REPOSITORY git@github.com:libsdl-org/SDL_image.git
    GIT_TAG        release-2.6.3
)

set(SDL2IMAGE_INSTALL OFF)
set(BUILD_SHARED_LIBS FALSE)

FetchContent_MakeAvailable(SDL_image)
