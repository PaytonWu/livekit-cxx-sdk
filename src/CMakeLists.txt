
add_subdirectory(ffi)

aux_source_directory(./ SRCS)
aux_source_directory(./utils UTILS)
aux_source_directory(./api API_SRCS)

add_library(${LIVEKIT_CXX_SDK_LIBRARY} ${SRCS} ${UTILS} ${API_SRCS})
add_dependencies(${LIVEKIT_CXX_SDK_LIBRARY} ${LIVEKIT_CXX_FFI_LIBRARY} abc)
target_link_libraries(${LIVEKIT_CXX_SDK_LIBRARY} PRIVATE abc crypto ${LIVEKIT_CXX_FFI_LIBRARY} fmt::fmt)
