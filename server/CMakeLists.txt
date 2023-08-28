set(SERVER_DIR ${CMAKE_SOURCE_DIR}/server/server_menagment/)
set(CLIENT_DIR ${CMAKE_SOURCE_DIR}/server/client_connection/)
set(JOB_INFO_DIR ${CMAKE_SOURCE_DIR}/server/job_info/)


list(APPEND LIST_INCLUDE_DIR_SERVER ${SERVER_DIR}/include)
list(APPEND LIST_INCLUDE_DIR_SERVER ${CLIENT_DIR}/include)
list(APPEND LIST_INCLUDE_DIR_SERVER ${JOB_INFO_DIR}/include)


list(APPEND LIST_LINK_LIBS "server_lib_server")
list(APPEND LIST_LINK_LIBS "server_lib_client")
list(APPEND LIST_LINK_LIBS "server_server_lib_job_infolib_c")

list(APPEND LIST_LINK_LIBS "utility")

add_subdirectory(server_menagment)
add_subdirectory(client_connection)
add_subdirectory(job_info)


add_library(server_lib INTERFACE)
target_link_libraries(server_lib INTERFACE "server_lib_server")
target_link_libraries(server_lib INTERFACE "server_lib_client")
target_link_libraries(server_lib INTERFACE "server_lib_job_info")



