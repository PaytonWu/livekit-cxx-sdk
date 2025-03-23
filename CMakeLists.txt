cmake_minimum_required(VERSION 3.16)
project(
        LIVEKIT_CXX_SDK
        VERSION 0.0.1
        DESCRIPTION "LiveKit C++ SDK based on LiveKit Rust FFI"
        LANGUAGES CXX
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

include_directories(include)
include_directories(third_party/rust-sdks/livekit-ffi/include)

aux_source_directory(src SRCS)
add_library(${LIVEKIT_CXX_SDK_LIBRARY} ${SRCS})
#target_link_libraries(${LIVEKIT_CXX_SDK_LIBRARY} PRIVATE fmt::fmt-header-only)

if (WITH_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
