cmake_minimum_required(VERSION 3.8)
project(matrix_look_look LANGUAGES CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-color=always -pedantic -Wall -pthread -O3")
set(EXTERNAL_MODULE_PATH ${CMAKE_SOURCE_DIR}/external)

find_program(CCACHE ccache)
if(CCACHE)
    message(STATUS "Found ccache: " ${CCACHE})
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ${CCACHE})
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ${CCACHE})
endif(CCACHE)

find_package(Eigen3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)

add_library(imgui STATIC
    ${EXTERNAL_MODULE_PATH}/imgui/imconfig.h
    ${EXTERNAL_MODULE_PATH}/imgui/imgui_internal.h
    ${EXTERNAL_MODULE_PATH}/imgui/imstb_rectpack.h
    ${EXTERNAL_MODULE_PATH}/imgui/imstb_textedit.h
    ${EXTERNAL_MODULE_PATH}/imgui/imstb_truetype.h
    ${EXTERNAL_MODULE_PATH}/imgui/imgui.h
    ${EXTERNAL_MODULE_PATH}/imgui/imgui.cpp
    ${EXTERNAL_MODULE_PATH}/imgui/imgui_demo.cpp
    ${EXTERNAL_MODULE_PATH}/imgui/imgui_draw.cpp
    ${EXTERNAL_MODULE_PATH}/imgui/imgui_widgets.cpp)

add_library(loguru STATIC
    ${EXTERNAL_MODULE_PATH}/loguru.hpp
    ${EXTERNAL_MODULE_PATH}/loguru.cpp)
target_link_libraries(loguru PUBLIC dl pthread)

add_library(mll STATIC
    ${CMAKE_SOURCE_DIR}/imgui_binding.hpp
    ${CMAKE_SOURCE_DIR}/imgui_binding_glfw.cpp
    ${CMAKE_SOURCE_DIR}/imgui_binding_opengl3.cpp
    ${CMAKE_SOURCE_DIR}/imgui_magic.hpp
    ${CMAKE_SOURCE_DIR}/imgui_magic.cpp
    ${CMAKE_SOURCE_DIR}/logging.hpp
    ${CMAKE_SOURCE_DIR}/shader.hpp
    ${CMAKE_SOURCE_DIR}/shader.cpp)
target_include_directories(mll SYSTEM PUBLIC ${EXTERNAL_MODULE_PATH})
target_link_libraries(mll PUBLIC loguru Eigen3::Eigen OpenGL::OpenGL GLEW::GLEW glfw)

add_executable(demo demo.cc)
target_link_libraries(demo imgui loguru mll)

if(NOT EXISTS "${CMAKE_BINARY_DIR}/Roboto-Regular.ttf")
    file(COPY "${EXTERNAL_MODULE_PATH}/Roboto-Regular.ttf" DESTINATION ${CMAKE_BINARY_DIR})
endif()
