set(TEST_DIR "${CMAKE_SOURCE_DIR}/server/job_info/test/")

list(APPEND TEST_LIBS "cmocka")
list(APPEND TEST_LIBS "utility")
list(APPEND TEST_LIBS "lib_messages")
list(APPEND TEST_LIBS "lib_util")
list(APPEND TEST_LIBS "lib_devices")
list(APPEND TEST_LIBS "server_lib_job_info")


set(CMAKE_C_STANDARD 17)
list(APPEND TEST_DIRS "${CMOCKA_INCLUDE_DIRS}")
list(APPEND TEST_DIRS "${CMAKE_SOURCE_DIR}/src")
include_directories(${CMAKE_SOURCE_DIR}/src)

add_cmocka_test(lib_server_job_info_devicelist_test
  SOURCES ${TEST_DIR}/job_info_devicelist_test.c
  COMPILE_OPTIONS ${DEFAULT_C_COMPILE_FLAGS}
  LINK_OPTIONS ${DEFAULT_LINK_FLAGS}
  LINK_LIBRARIES ${TEST_LIBS})

add_cmocka_test_environment(lib_server_job_info_devicelist_test)
target_include_directories(lib_server_job_info_devicelist_test PUBLIC "${TEST_DIRS}")
