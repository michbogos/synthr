cmake_minimum_required(VERSION 3.11)

file(GLOB CPP_FILES "*.cpp")

add_subdirectory(glfw)

add_library(imgui STATIC ${CPP_FILES})
find_package(OpenGL REQUIRED)
target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(imgui glfw)