cmake_minimum_required(VERSION 3.16)
project(3d_renderer C)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

add_subdirectory(deps/SDL EXCLUDE_FROM_ALL)

include_directories(include)

file(GLOB SRC_FILES "src/*.c")

add_executable(3d_renderer ${SRC_FILES})

target_compile_options(3d_renderer PRIVATE -Wall -std=c99)

set_target_properties(3d_renderer PROPERTIES LINKER_LANGUAGE C)

target_link_libraries(3d_renderer PRIVATE SDL3::SDL3)