cmake_minimum_required(VERSION 3.11)

project(synthr)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAG "${CMAKE_CXX_FLAGS} -std=c89 -O3")

include(ExternalProject)

file(GLOB C_FILES "${CMAKE_SOURCE_DIR}/src/*.c")
file(GLOB INCLUDE_FILES "${CMAKE_SOURCE_DIR}/include/*.h")

add_library(synthr ${C_FILES})
target_include_directories(synthr PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_sources(synthr PUBLIC ${C_FILES})
set_target_properties(synthr PROPERTIES LINKER_LANGUAGE C)

add_executable(program ${C_FILES})
target_include_directories(program PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_directories(program PUBLIC ${CMAKE_SOURCE_DIR}/libsoundio/)
target_sources(program PUBLIC ${C_FILES})
target_link_libraries(program m)
add_subdirectory(libsoundio)
target_link_libraries(program soundio)
set_target_properties(program PROPERTIES LINKER_LANGUAGE C)
# target_link_libraries(synthr libsynthr)