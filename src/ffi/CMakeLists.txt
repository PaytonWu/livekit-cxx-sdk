aux_source_directory(./proto PROTO_SRCS)
aux_source_directory(./ FFI_SRCS)
add_library(${LIVEKIT_CXX_FFI_LIBRARY} ${FFI_SRCS} ${PROTO_SRCS})
target_link_libraries(${LIVEKIT_CXX_FFI_LIBRARY} PRIVATE protobuf)
