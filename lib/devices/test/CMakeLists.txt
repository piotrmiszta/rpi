set(TEST_DIR "${CMAKE_SOURCE_DIR}/lib/devices/test/")

list(APPEND TEST_LIBS "cmocka")
list(APPEND TEST_LIBS "utility")
list(APPEND TEST_LIBS "lib_messages")
list(APPEND TEST_LIBS "lib_devices")

list(APPEND TEST_LIBS "lib_util")
set(CMAKE_C_STANDARD 17)
list(APPEND TEST_DIRS "${CMOCKA_INCLUDE_DIRS}")
list(APPEND TEST_DIRS "${CMAKE_SOURCE_DIR}/src")
include_directories(${CMAKE_SOURCE_DIR}/src)

add_cmocka_test(lib_devices_test
  SOURCES ${TEST_DIR}/devices_test.c
  COMPILE_OPTIONS ${DEFAULT_C_COMPILE_FLAGS}
  LINK_OPTIONS ${DEFAULT_LINK_FLAGS}
  LINK_LIBRARIES ${TEST_LIBS})

add_cmocka_test_environment(lib_devices_test)
target_include_directories(lib_devices_test PUBLIC "${TEST_DIRS}")
