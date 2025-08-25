cmake_minimum_required(VERSION 3.16)
project(
    LIVEKIT_CXX_SDK
    VERSION 0.0.1
    DESCRIPTION "LiveKit C++ SDK based on LiveKit Rust FFI"
    LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_VERBOSE_MAKEFILE OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(LIVEKIT_CXX_SDK_LIBRARY livekit_cxx_sdk)

include(ExternalProject)

ExternalProject_Add(
    ABC
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third-party/abc
    BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/third-party/abc/build
    CMAKE_ARGS
        -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
    INSTALL_COMMAND ""
)
add_library(abc STATIC IMPORTED)
set_property(TARGET abc PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_BINARY_DIR}/third-party/abc/build/libabc.a)
add_dependencies(abc ABC)

ExternalProject_Add(
    JWT-CPP
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third-party/jwt-cpp
    BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/third-party/jwt-cpp/build
    CMAKE_ARGS
        -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
        -DJWT_DISABLE_PICOJSON=ON
        -DJWT_EXTERNAL_NLOHMANN_JSON=ON
        -DJWT_BUILD_EXAMPLES=OFF
    INSTALL_COMMAND ""
)
add_library(jwt-cpp INTERFACE IMPORTED)
# set_property(TARGET jwt-cpp PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_BINARY_DIR}/third-party/jwt-cpp/build/libjwt-cpp.a)
add_dependencies(jwt-cpp JWT-CPP)

include_directories(include)
include_directories(third-party/abc/include)
include_directories(third-party/rust-sdks/livekit-ffi/include)
include_directories(third-party/stdexec/include)
include_directories(third-party/jwt-cpp/include)

add_subdirectory(src)

if (WITH_TESTS)
    enable_testing()
    add_subdirectory(tests)
    add_subdirectory(examples)
endif()
