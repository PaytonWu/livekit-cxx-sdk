aux_source_directory(./ EXAMPLES_SRCS)

add_executable(examples ${EXAMPLES_SRCS})

target_include_directories(examples PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include)
target_link_directories(examples PRIVATE ${PROJECT_BINARY_DIR})
target_link_libraries(examples PRIVATE ${LIVEKIT_CXX_SDK_LIBRARY})
