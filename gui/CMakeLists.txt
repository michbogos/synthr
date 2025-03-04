cmake_minimum_required(VERSION 3.11)

project(synthr_gui)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_FLAG "${CMAKE_CXX_FLAGS} -std=c++11 -g -Wall -Wextra")

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


file(GLOB CPP_FILES "*.cpp")
file(GLOB INCLUDE_FILES "*.h")

add_executable(program ${CPP_FILES})
target_include_directories(program PUBLIC ${CMAKE_SOURCE_DIR}/)
target_include_directories(program PUBLIC ${CMAKE_SOURCE_DIR}/imgui)
get_filename_component(PARENT_DIR ../ ABSOLUTE)
message(PARENT_DIR="${PARENT_DIR}/include/")
target_include_directories(program PUBLIC ${PARENT_DIR}/include/)
target_include_directories(program PUBLIC ${soundio_SOURCE_DIR} ${rtmidi_SOURCE_DIR})
target_sources(program PUBLIC ${CPP_FILES})
target_link_libraries(program m)
target_link_libraries(program libsoundio_static)
target_link_libraries(program libsoundio_shared)
target_link_libraries(program rtmidi)
target_link_directories(program PUBLIC ${PARENT_DIR})
target_link_libraries(program synthr)
# target_link_libraries(synthr synthr)