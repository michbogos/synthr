cmake_minimum_required(VERSION 3.11)

project(synthr)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAG "${CMAKE_CXX_FLAGS} -std=c89 -O2")

include(ExternalProject)
include(FetchContent)

FetchContent_Declare(
    soundio
    URL https://github.com/andrewrk/libsoundio/archive/refs/tags/2.0.1-7.tar.gz
)
FetchContent_Populate(soundio)

add_subdirectory(${soundio_SOURCE_DIR})

file(GLOB C_FILES "${CMAKE_SOURCE_DIR}/src/*.c")
file(GLOB INCLUDE_FILES "${CMAKE_SOURCE_DIR}/include/*.h")

list(REMOVE_ITEM C_FILES "${CMAKE_SOURCE_DIR}/src/main.c")

add_library(synthr ${C_FILES})
target_include_directories(synthr PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_sources(synthr PUBLIC ${C_FILES})
set_target_properties(synthr PROPERTIES LINKER_LANGUAGE C)

list(APPEND C_FILES "${CMAKE_SOURCE_DIR}/src/main.c")

add_executable(program ${C_FILES})
target_include_directories(program PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_include_directories(program PUBLIC ${soundio_SOURCE_DIR})
target_sources(program PUBLIC ${C_FILES})
target_link_libraries(program m)
target_link_libraries(program libsoundio_static)
target_link_libraries(program libsoundio_shared)
set_target_properties(program PROPERTIES LINKER_LANGUAGE C)
# target_link_libraries(synthr libsynthr)