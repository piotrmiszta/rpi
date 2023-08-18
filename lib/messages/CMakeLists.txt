set(SOURCE_DIR ${MESSAGES_DIR}/src)
set(INCLUDE_DIR ${MESSAGES_DIR}/include)

file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${INCLUDE_DIR}/*.h")
file(GLOB src "${SOURCE_DIR}/*.c")
set(MESSAGES_INCLUDES ${CMAKE_SOURCE_DIR}/lib/**/include)

add_library(lib_messages STATIC ${src} ${HEADER_LIST})
target_include_directories(lib_messages PUBLIC ${LIST_INCLUDE_DIR})
target_link_libraries(lib_messages PUBLIC "utility")

list(APPEND CMAKE_MODULE_PATH cmake)
find_package(cmocka 1.1.0 REQUIRED)
include(${CMAKE_SOURCE_DIR}/cmake/AddCMockaTest.cmake)
add_subdirectory(test)
