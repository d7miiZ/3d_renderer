cmake_minimum_required(VERSION 3.16)
project(3d_renderer)

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

add_compile_options(-Wall -std=c99)

add_subdirectory(deps/SDL EXCLUDE_FROM_ALL)

file(GLOB SRC_FILES "src/*.c")

add_executable(3d_renderer ${SRC_FILES})

target_link_libraries(3d_renderer PRIVATE SDL3::SDL3)