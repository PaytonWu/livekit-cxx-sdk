find_package(GTest REQUIRED)
include(GoogleTest)

aux_source_directory(./abc ABC_TEST_SOURCES)

add_executable(tests ${ABC_TEST_SOURCES})

target_include_directories(tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include)
target_link_directories(tests PRIVATE ${PROJECT_BINARY_DIR})
target_link_libraries(tests PRIVATE ${LIVEKIT_CXX_SDK_LIBRARY} GTest::GTest GTest::Main)

gtest_discover_tests(tests)
