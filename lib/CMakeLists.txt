set(MESSAGES_DIR ${CMAKE_SOURCE_DIR}/lib/messages/)
set(UTIL_DIR ${CMAKE_SOURCE_DIR}/lib/util/)
set(DEVICES_DIR ${CMAKE_SOURCE_DIR}/lib/devices/)


list(APPEND LIST_INCLUDE_DIR ${MESSAGES_DIR}/include)
list(APPEND LIST_INCLUDE_DIR ${UTIL_DIR}/include)
list(APPEND LIST_INCLUDE_DIR ${DEVICES_DIR}/include)


list(APPEND LIST_LINK_LIBS "lib_util")
list(APPEND LIST_LINK_LIBS "lib_messages")
list(APPEND LIST_LINK_LIBS "lib_devices")
list(APPEND LIST_LINK_LIBS "utility")

add_subdirectory(util)
add_subdirectory(messages)
add_subdirectory(devices)

add_library(lib INTERFACE)
target_link_libraries(lib INTERFACE "lib_util")
target_link_libraries(lib INTERFACE "lib_messages")
target_link_libraries(lib INTERFACE "lib_devices")

