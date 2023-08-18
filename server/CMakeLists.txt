set(SERVER_DIR ${CMAKE_SOURCE_DIR}/server/server_menagment/)
set(CLIENT_DIR ${CMAKE_SOURCE_DIR}/server/client_connection/)

list(APPEND LIST_INCLUDE_DIR_SERVER ${SERVER_DIR}/include)
list(APPEND LIST_INCLUDE_DIR_SERVER ${CLIENT_DIR}/include)

list(APPEND LIST_LINK_LIBS "server_lib_server")
list(APPEND LIST_LINK_LIBS "server_lib_client")
list(APPEND LIST_LINK_LIBS "utility")

add_subdirectory(server_menagment)
add_subdirectory(client_connection)

add_library(server_lib INTERFACE)
target_link_libraries(server_lib INTERFACE "server_lib_server")
target_link_libraries(server_lib INTERFACE "server_lib_client")

