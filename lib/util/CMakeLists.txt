set(SOURCE_DIR ${UTIL_DIR}/src)
set(INCLUDE_DIR ${UTIL_DIR}/include)


file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${INCLUDE_DIR}/*.h")
file(GLOB src "${SOURCE_DIR}/*.c")

add_library(lib_util STATIC ${src} ${HEADER_LIST})
target_include_directories(lib_util PUBLIC ${LIST_INCLUDE_DIR}/)
target_link_libraries(lib_util PUBLIC "utility")

list(APPEND CMAKE_MODULE_PATH cmake)
find_package(cmocka 1.1.0 REQUIRED)
include(${CMAKE_SOURCE_DIR}/cmake/AddCMockaTest.cmake)
add_subdirectory(test)
