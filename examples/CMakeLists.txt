add_executable(basic_room basic_room.cpp)
target_link_directories(basic_room PRIVATE ${PROJECT_BINARY_DIR})
target_link_libraries(basic_room PRIVATE ${LIVEKIT_CXX_SDK_LIBRARY} ${LIVEKIT_CXX_FFI_LIBRARY} ${PROTOBUF_LIBRARY} fmt::fmt)

add_executable(access_token_example access_token_example.cpp)
target_link_directories(access_token_example PRIVATE ${PROJECT_BINARY_DIR})
target_link_libraries(access_token_example PRIVATE ${LIVEKIT_CXX_SDK_LIBRARY} ${LIVEKIT_CXX_FFI_LIBRARY} ${PROTOBUF_LIBRARY} fmt::fmt)
