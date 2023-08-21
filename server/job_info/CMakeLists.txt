set(SOURCE_DIR ${JOB_INFO_DIR}/src)
set(INCLUDE_DIR ${JOB_INFO_DIR}/include)

file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${INCLUDE_DIR}/*.h")
file(GLOB src "${SOURCE_DIR}/*.c")
set(SERVER_INCLUDES ${CMAKE_SOURCE_DIR}/server/**/include)

add_library(server_lib_job_info STATIC ${src} ${HEADER_LIST})
target_include_directories(server_lib_job_info PUBLIC ${LIST_INCLUDE_DIR_SERVER})
target_link_libraries(server_lib_job_info PUBLIC "utility")
target_link_libraries(server_lib_job_info PUBLIC "lib_util")
target_link_libraries(server_lib_job_info PUBLIC "lib_devices")
target_link_libraries(server_lib_job_info PUBLIC "lib_messages")



list(APPEND CMAKE_MODULE_PATH cmake)
find_package(cmocka 1.1.0 REQUIRED)
include(${CMAKE_SOURCE_DIR}/cmake/AddCMockaTest.cmake)
add_subdirectory(test)
