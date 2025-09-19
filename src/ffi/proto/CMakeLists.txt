aux_source_directory(./ PROTO_SRCS)
add_library(${LIVEKIT_CXX_PROTO_LIBRARY} ${PROTO_SRCS})
# target_link_libraries(${LIVEKIT_CXX_PROTO_LIBRARY} PRIVATE ${PROTOBUF_LIBRARY})
# target_include_directories(${LIVEKIT_CXX_PROTO_LIBRARY} PRIVATE ${PROTOBUF_INCLUDE_DIR})
