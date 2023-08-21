cmake_minimum_required(VERSION 3.8)

project(rpi)

set(CMAKE_C_STANDARD 17)
#set(CMAKE_C_COMPILER clang)
#set(CMAKE_C_FLAGS "-Weverything -Wno-declaration-after-statement -Wno-date-time -g -Wno-disabled-macro-expansion -O3")
enable_testing()
set(CMAKE_C_FLAGS "-Wall -pedantic -g -Werror -O3 -Wno-non-literal-null-conversion -std=c11")
add_executable(server_exe server/main.c)
add_executable(client_exe client/main.c)

add_subdirectory(client)
add_subdirectory(server)
add_subdirectory(lib)

target_link_libraries(server_exe PUBLIC "utility")
target_link_libraries(server_exe PUBLIC "server_lib")
target_link_libraries(server_exe PUBLIC "lib")

target_link_libraries(client_exe PUBLIC "utility")
target_link_libraries(client_exe PUBLIC "lib")
target_link_libraries(client_exe PUBLIC "client_lib")

list(APPEND CMAKE_MODULE_PATH cmake)
find_package(cmocka 1.1.0 REQUIRED)
include(${CMAKE_SOURCE_DIR}/cmake/AddCMockaTest.cmake)
add_subdirectory(test)
