set(SOURCE_DIR ${CONNECTION_DIR}/src)
set(INCLUDE_DIR ${CONNECTION_DIR}/include)

file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${INCLUDE_DIR}/*.h")
file(GLOB src "${SOURCE_DIR}/*.c")
set(CONNECTION_INCLUDES ${CMAKE_SOURCE_DIR}/lib/**/include)

add_library(client_lib_connection STATIC ${src} ${HEADER_LIST})
target_include_directories(client_lib_connection PUBLIC ${LIST_INCLUDE_DIR})
target_link_libraries(client_lib_connection PUBLIC "utility")
target_link_libraries(client_lib_connection PUBLIC "lib_util")


list(APPEND CMAKE_MODULE_PATH cmake)
find_package(cmocka 1.1.0 REQUIRED)
include(${CMAKE_SOURCE_DIR}/cmake/AddCMockaTest.cmake)
add_subdirectory(test)
