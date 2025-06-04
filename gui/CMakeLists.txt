cmake_minimum_required(VERSION 3.11)

project(synthr_gui)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_FLAG "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -Wextra -fPIC")

include(ExternalProject)
include(FetchContent)

FetchContent_Declare(
    juce_imgui
    GIT_REPOSITORY https://github.com/Krasjet/imgui_juce.git
    GIT_PROGRESS   TRUE
    GIT_SHALLOW    TRUE
)
FetchContent_Populate(juce_imgui)


FetchContent_Declare(
    juce
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_PROGRESS   TRUE
    GIT_SHALLOW    TRUE
)
FetchContent_Populate(juce)

FetchContent_Declare(
    soundio
    URL https://github.com/andrewrk/libsoundio/archive/refs/tags/2.0.1-7.tar.gz
    GIT_PROGRESS   TRUE
    GIT_SHALLOW    TRUE
)
FetchContent_Populate(soundio)

FetchContent_Declare(
    rtmidi
    URL https://github.com/thestk/rtmidi/archive/refs/tags/6.0.0.tar.gz
    GIT_PROGRESS   TRUE
    GIT_SHALLOW    TRUE
)
FetchContent_Populate(rtmidi)

add_subdirectory(${soundio_SOURCE_DIR})
add_subdirectory(${rtmidi_SOURCE_DIR})
add_subdirectory(${juce_SOURCE_DIR})
add_subdirectory(${CMAKE_SOURCE_DIR}/glfw)
add_subdirectory(${juce_imgui_SOURCE_DIR})


file(GLOB CPP_FILES "*.cpp")
file(GLOB INCLUDE_FILES "*.h")
file(GLOB IMGUI_FILES "./imgui/*.cpp")
file(GLOB IMPLOT_FILES "./implot/*.cpp")
file(GLOB NODE_EDITOR_FILES "./imgui-node-editor/*.cpp")

get_filename_component(PARENT_DIR ../ ABSOLUTE)
file(GLOB SYNTHR_FILES "${PARENT_DIR}/src/*.c")
list(FILTER SYNTHR_FILES EXCLUDE REGEX "main\\.c")

add_library(imgui ${IMGUI_FILES} ${CMAKE_SOURCE_DIR}/imgui/backends/imgui_impl_glfw.cpp ${CMAKE_SOURCE_DIR}/imgui/backends/imgui_impl_opengl3.cpp)
target_include_directories(imgui PUBLIC ${CMAKE_SOURCE_DIR}/imgui/)

add_library(node_editor ${NODE_EDITOR_FILES})
target_include_directories(node_editor PUBLIC ${CMAKE_SOURCE_DIR}/imgui/ ${CMAKE_SOURCE_DIR}/imgui-node-editor/)
target_link_libraries(node_editor PUBLIC imgui)

add_library(implot ${IMPLOT_FILES})
target_include_directories(implot PUBLIC ${CMAKE_SOURCE_DIR}/imgui/ ${CMAKE_SOURCE_DIR}/implot/)
target_link_libraries(implot PUBLIC imgui)

add_library(synthr ${SYNTHR_FILES})
target_include_directories(synthr PUBLIC ${PARENT_DIR}/include)
# target_link_libraries(synthr PUBLIC libsoundio_static)
# target_link_libraries(synthr PUBLIC libsoundio_shared)
# target_link_libraries(synthr PUBLIC rtmidi)

# add_executable(program ${CPP_FILES})
# target_include_directories(program PUBLIC ${CMAKE_SOURCE_DIR}/)
# target_include_directories(program PUBLIC ${CMAKE_SOURCE_DIR}/imgui/)
# target_include_directories(program PUBLIC ${CMAKE_SOURCE_DIR}/imgui/backends)
# target_include_directories(program PUBLIC ${CMAKE_SOURCE_DIR}/glfw/include/)
# target_include_directories(program PUBLIC ${CMAKE_SOURCE_DIR}/imnodes/)
# target_include_directories(program PUBLIC ${CMAKE_SOURCE_DIR}/imgui-node-editor/)
# target_include_directories(program PUBLIC ${PARENT_DIR}/include/)
# target_include_directories(program PUBLIC ${soundio_SOURCE_DIR} ${rtmidi_SOURCE_DIR})
# target_sources(program PUBLIC ${CPP_FILES})
# target_link_libraries(program PUBLIC m)
# target_link_libraries(program PUBLIC libsoundio_static)
# target_link_libraries(program PUBLIC libsoundio_shared)
# target_link_libraries(program PUBLIC rtmidi)
# target_link_libraries(program PUBLIC imgui)
# target_link_libraries(program PUBLIC GL)
# target_link_libraries(program PUBLIC imgui)
# target_link_libraries(program PUBLIC implot)
# target_link_libraries(program PUBLIC node_editor)
# target_link_libraries(program PUBLIC glfw)
# target_link_libraries(program PUBLIC synthr)

set(FORMATS "Standalone")
set(JUCE_WEB_BROWSER 0)

juce_add_gui_app(standalone
        VERSION 1.0.0
        IS_SYNTH TRUE
        NEEDS_BROWSER FALSE
        FORMATS ${FORMATS}
        PLUGIN_NAME "Synthr"
        PRODUCT_NAME "Synthr")

juce_generate_juce_header(standalone)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
pkg_check_modules(WEBKIT2GTK REQUIRED webkit2gtk-4.1)

include_directories(${GTK3_INCLUDE_DIRS} ${WEBKIT2GTK_INCLUDE_DIRS})

target_link_libraries(standalone PUBLIC ${OPENGL_LIBRARIES})

target_include_directories(standalone PUBLIC ${CMAKE_SOURCE_DIR}/)
target_include_directories(standalone PUBLIC ${CMAKE_SOURCE_DIR}/imgui/)
target_include_directories(standalone PUBLIC ${CMAKE_SOURCE_DIR}/imgui/backends)
target_include_directories(standalone PUBLIC ${CMAKE_SOURCE_DIR}/glfw/include/)
target_include_directories(standalone PUBLIC ${CMAKE_SOURCE_DIR}/imnodes/)
target_include_directories(standalone PUBLIC ${CMAKE_SOURCE_DIR}/imgui-node-editor/)
target_include_directories(standalone PUBLIC ${PARENT_DIR}/include/)
target_include_directories(standalone PUBLIC ${soundio_SOURCE_DIR} ${rtmidi_SOURCE_DIR})
target_sources(standalone PUBLIC ${CPP_FILES})
target_link_libraries(standalone PUBLIC m)
target_link_libraries(standalone PUBLIC libsoundio_static)
target_link_libraries(standalone PUBLIC libsoundio_shared)
target_link_libraries(standalone PUBLIC rtmidi)
target_link_libraries(standalone PUBLIC imgui)
target_link_libraries(standalone PUBLIC GL)
target_link_libraries(standalone PUBLIC imgui)
target_link_libraries(standalone PUBLIC implot)
target_link_libraries(standalone PUBLIC node_editor)
target_link_libraries(standalone PUBLIC glfw)
target_link_libraries(standalone PUBLIC synthr)
target_link_libraries(standalone
        PRIVATE
        juce::juce_dsp
        juce::juce_audio_processors
        juce::juce_audio_plugin_client
        juce::juce_audio_utils
        juce::juce_opengl
        imgui_impl_juce
        PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
        curl)

set(FORMATS "VST3")
juce_add_plugin(plugin
        VERSION 1.0.0
        IS_SYNTH TRUE
        NEEDS_BROWSER FALSE
        NEEDS_MIDI_INPUT TRUE
        FORMATS ${FORMATS}
        PLUGIN_NAME "Synthr"
        PRODUCT_NAME "Synthr")

target_compile_definitions(plugin PUBLIC JUCE_COPY_PLUGIN_AFTER_BUILD=1 JUCE_VST3_CAN_REPLACE_VST2=0 JucePlugin_Build_VST3=1 JUCE_PLUGIN_BUILD_VST3=1 JUCEPLUGIN_BUILD_VST3=1)


juce_generate_juce_header(plugin)

set(JUCE_COPY_PLUGIN_AFTER_BUILD ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
pkg_check_modules(WEBKIT2GTK REQUIRED webkit2gtk-4.1)

include_directories(${GTK3_INCLUDE_DIRS} ${WEBKIT2GTK_INCLUDE_DIRS})

target_link_libraries(plugin PUBLIC ${OPENGL_LIBRARIES})

target_include_directories(plugin PUBLIC ${CMAKE_SOURCE_DIR}/)
target_include_directories(plugin PUBLIC ${CMAKE_SOURCE_DIR}/imgui/)
target_include_directories(plugin PUBLIC ${CMAKE_SOURCE_DIR}/imgui/backends)
target_include_directories(plugin PUBLIC ${CMAKE_SOURCE_DIR}/glfw/include/)
target_include_directories(plugin PUBLIC ${CMAKE_SOURCE_DIR}/imnodes/)
target_include_directories(plugin PUBLIC ${CMAKE_SOURCE_DIR}/imgui-node-editor/)
target_include_directories(plugin PUBLIC ${PARENT_DIR}/include/)
target_include_directories(plugin PUBLIC ${soundio_SOURCE_DIR} ${rtmidi_SOURCE_DIR})
target_sources(plugin PUBLIC ${CPP_FILES})
target_link_libraries(plugin PUBLIC m)
# target_link_libraries(plugin PUBLIC libsoundio_static)
# target_link_libraries(plugin PUBLIC libsoundio_shared)
# target_link_libraries(plugin PUBLIC rtmidi)
target_link_libraries(plugin PUBLIC imgui)
target_link_libraries(plugin PUBLIC GL)
target_link_libraries(plugin PUBLIC imgui)
target_link_libraries(plugin PUBLIC implot)
target_link_libraries(plugin PUBLIC node_editor)
target_link_libraries(plugin PUBLIC glfw)
target_link_libraries(plugin PUBLIC synthr)
target_link_libraries(plugin
        PRIVATE
        juce::juce_dsp
        juce::juce_audio_processors
        juce::juce_audio_plugin_client
        juce::juce_audio_utils
        juce::juce_opengl
        imgui_impl_juce
        PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
        curl)

# juce_add_plugin(sine
#   FORMATS VST3 Standalone
#   COMPANY_NAME kst
#   PLUGIN_MANUFACTURER_CODE Krjt
#   PLUGIN_CODE Ksin
#   PRODUCT_NAME "Sine"
#   VERSION 1.0.0
#   IS_SYNTH TRUE
#   # we don't use midi but ableton won't load vst3 synth
#   # without midi input
#   NEEDS_MIDI_INPUT TRUE)

#   target_sources(sine PUBLIC ${CMAKE_SOURCE_DIR}/Main.cpp ${CMAKE_SOURCE_DIR}/PluginProcessor.cpp ${CMAKE_SOURCE_DIR}/PluginEditor.cpp)
#   target_include_directories(sine PUBLIC ${CMAKE_SOURCE_DIR}/)

# juce_generate_juce_header(sine)

# target_compile_definitions(sine
#   PUBLIC
#     JUCE_WEB_BROWSER=0
#     JUCE_USE_CURL=0
#     JUCE_REPORT_APP_USAGE=0
#     JUCE_VST3_CAN_REPLACE_VST2=0)

# target_link_libraries(sine
#   PRIVATE
#     juce::juce_audio_utils
#     juce::juce_opengl
#     imgui
#     imgui_impl_juce
#   PUBLIC
#     juce::juce_recommended_config_flags
#     juce::juce_recommended_lto_flags
#     juce::juce_recommended_warning_flags)