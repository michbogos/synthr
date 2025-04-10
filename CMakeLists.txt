cmake_minimum_required(VERSION 3.11)

project(synthr)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAG "${CMAKE_C_FLAGS} -std=c11 -g -Wall -Wextra")

include(ExternalProject)
include(FetchContent)



FetchContent_Declare(
    soundio
    URL https://github.com/andrewrk/libsoundio/archive/refs/tags/2.0.1-7.tar.gz
)
FetchContent_Populate(soundio)

FetchContent_Declare(
    rtmidi
    URL https://github.com/thestk/rtmidi/archive/refs/tags/6.0.0.tar.gz
)
FetchContent_Populate(rtmidi)

add_subdirectory(${soundio_SOURCE_DIR})
add_subdirectory(${rtmidi_SOURCE_DIR})



file(GLOB C_FILES "src/*.c")
message(${C_FILES})
file(GLOB INCLUDE_FILES "include/*.h")

list(FILTER C_FILES EXCLUDE REGEX "main\\.c")

add_library(synthr STATIC ${C_FILES})
target_link_libraries(synthr m)
target_include_directories(synthr PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_sources(synthr PUBLIC ${C_FILES})
set_target_properties(synthr PROPERTIES LINKER_LANGUAGE C)

list(APPEND C_FILES "./src/main.c")

add_executable(program ${C_FILES})
target_include_directories(program PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_include_directories(program PUBLIC ${soundio_SOURCE_DIR} ${rtmidi_SOURCE_DIR})
target_sources(program PUBLIC ${C_FILES})
target_link_libraries(program m)
target_link_libraries(program libsoundio_static)
target_link_libraries(program libsoundio_shared)
target_link_libraries(program rtmidi)
set_target_properties(program PROPERTIES LINKER_LANGUAGE C)
# target_link_libraries(synthr synthr)