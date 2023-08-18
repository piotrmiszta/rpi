set(CONNECTION_DIR ${CMAKE_SOURCE_DIR}/client/connection/)

list(APPEND LIST_INCLUDE_DIR ${CONNECTION_DIR}/include)

list(APPEND LIST_LINK_LIBS "lib_util")
list(APPEND LIST_LINK_LIBS "utility")

add_subdirectory(connection)

add_library(client_lib INTERFACE)
target_link_libraries(client_lib INTERFACE "client_lib_connection")


