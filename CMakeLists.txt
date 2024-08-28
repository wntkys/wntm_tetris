cmake_minimum_required(VERSION 3.2)

set (OUTPUT_SUBFOLDER ${CMAKE_CURRENT_BINARY_DIR}/bin)
set (RESOURCE_SUBFOLDER ${OUTPUT_SUBFOLDER}/resources)


project(wntm_tetris
    LANGUAGES C)

set(SDL_STATIC ON)    # If you want to use static SDL library
set(SDL_SHARED OFF)    # If you want to use static SDL library
add_subdirectory(deps/SDL-main)
link_libraries(SDL3::SDL3)
include_directories(${SDL3_SOURCE_DIR}/include)



FILE(GLOB_RECURSE MyCSources src/*.c)

include_directories(deps/gl3w/include)
include_directories(deps/cglm-master/include)


add_executable(wntm_tetris ${MyCSources} deps/gl3w/src/gl3w.c)
set_target_properties(wntm_tetris PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_SUBFOLDER})

FILE(GLOB RESOURCES_SOURCE res/*)



file(COPY res/ DESTINATION ${RESOURCE_SUBFOLDER})