cmake_minimum_required(VERSION 3.22)

set(CPP_SWITCH 0.0.1) 

project(cpp_switch LANGUAGES CXX VERSION ${CPP_SWITCH})

include(CTest)
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
find_package(Boost REQUIRED)
find_package(Catch2 REQUIRED)

add_library(cpp_switch INTERFACE)
target_include_directories(cpp_switch
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_compile_features(cpp_switch
    INTERFACE
        cxx_std_20
)

add_executable(test_cpp_switch "")
target_sources(test_cpp_switch
    PRIVATE
        switch.test.cpp
)

target_link_libraries(test_cpp_switch
    PRIVATE
        cpp_switch
        Catch2::Catch2
)