
add_subdirectory(ffi)

aux_source_directory(./ DEFAULT_SRCS)
aux_source_directory(./abc ABC_SRCS)
aux_source_directory(./rtc RTC_SRCS)
aux_source_directory(./utils UTILS_SRCS)
aux_source_directory(./api API_SRCS)
add_library(${LIVEKIT_CXX_SDK_LIBRARY} ${DEFAULT_SRCS} ${ABC_SRCS} ${RTC_SRCS} ${UTILS_SRCS} ${API_SRCS})
add_dependencies(${LIVEKIT_CXX_SDK_LIBRARY} ${LIVEKIT_CXX_FFI_LIBRARY} abc)
target_link_libraries(${LIVEKIT_CXX_SDK_LIBRARY} PRIVATE ${LIVEKIT_CXX_FFI_LIBRARY} abc crypto fmt::fmt)
