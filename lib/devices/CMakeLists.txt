set(SOURCE_DIR ${DEVICES_DIR}/src)
set(INCLUDE_DIR ${DEVICES_DIR}/include)

file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${INCLUDE_DIR}/*.h")
file(GLOB src "${SOURCE_DIR}/*.c")
set(DEVICES_INCLUDES ${CMAKE_SOURCE_DIR}/lib/**/include)

add_library(lib_devices STATIC ${src} ${HEADER_LIST})
target_include_directories(lib_devices PUBLIC ${LIST_INCLUDE_DIR})
target_link_libraries(lib_devices PUBLIC "utility")
target_link_libraries(lib_devices PUBLIC "lib_messages")
target_link_libraries(lib_devices PUBLIC "lib_util")

target_link_libraries(lib_devices PUBLIC "xml2")



list(APPEND CMAKE_MODULE_PATH cmake)
find_package(cmocka 1.1.0 REQUIRED)
include(${CMAKE_SOURCE_DIR}/cmake/AddCMockaTest.cmake)
add_subdirectory(test)
