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
add_subdirectory(${CMAKE_SOURCE_DIR}/glfw)


file(GLOB CPP_FILES "*.cpp")
file(GLOB INCLUDE_FILES "*.h")
file(GLOB IMGUI_FILES "./imgui/*.cpp")
file(GLOB IMPLOT_FILES "./implot/*.cpp")

add_library(imgui ${IMGUI_FILES} ${CMAKE_SOURCE_DIR}/imgui/backends/imgui_impl_glfw.cpp ${CMAKE_SOURCE_DIR}/imgui/backends/imgui_impl_opengl3.cpp)
target_include_directories(imgui PUBLIC ${CMAKE_SOURCE_DIR}/imgui/)

add_library(implot ${IMPLOT_FILES})
target_include_directories(implot PUBLIC ${CMAKE_SOURCE_DIR}/imgui/ ${CMAKE_SOURCE_DIR}/implot/)
target_link_libraries(implot PUBLIC imgui)


add_executable(program ${CPP_FILES})
target_include_directories(program PUBLIC ${CMAKE_SOURCE_DIR}/)
target_include_directories(program PUBLIC ${CMAKE_SOURCE_DIR}/imgui/)
target_include_directories(program PUBLIC ${CMAKE_SOURCE_DIR}/imgui/backends)
target_include_directories(program PUBLIC ${CMAKE_SOURCE_DIR}/glfw/include/)
target_include_directories(program PUBLIC ${CMAKE_SOURCE_DIR}/imnodes/)
get_filename_component(PARENT_DIR ../ ABSOLUTE)
message(PARENT_DIR="${PARENT_DIR}/include/")
target_include_directories(program PUBLIC ${PARENT_DIR}/include/)
target_include_directories(program PUBLIC ${soundio_SOURCE_DIR} ${rtmidi_SOURCE_DIR})
target_sources(program PUBLIC ${CPP_FILES})
target_link_libraries(program PUBLIC m)
target_link_libraries(program PUBLIC libsoundio_static)
target_link_libraries(program PUBLIC libsoundio_shared)
target_link_libraries(program PUBLIC rtmidi)
target_link_libraries(program PUBLIC imgui)
target_link_libraries(program PUBLIC GL)
target_link_libraries(program PUBLIC imgui)
target_link_libraries(program PUBLIC implot)
target_link_libraries(program PUBLIC glfw)
target_link_directories(program PUBLIC ${PARENT_DIR})
target_link_libraries(program PUBLIC synthr)
# target_link_libraries(synthr synthr)