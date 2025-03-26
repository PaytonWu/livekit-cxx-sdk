
set(LIVEKIT_CXX_FFI_LIBRARY livekit_cxx_ffi)

add_subdirectory(ffi)

aux_source_directory(./ SRCS)

add_library(${LIVEKIT_CXX_SDK_LIBRARY} ${SRCS})
add_dependencies(${LIVEKIT_CXX_SDK_LIBRARY} ${LIVEKIT_CXX_FFI_LIBRARY})