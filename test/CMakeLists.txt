set(TEST_DIR "${CMAKE_SOURCE_DIR}/test/test_cases")

list(APPEND TEST_LIBS "cmocka")
list(APPEND TEST_LIBS "utility")
set(CMAKE_C_STANDARD 17)
list(APPEND TEST_DIRS "${CMOCKA_INCLUDE_DIRS}")
list(APPEND TEST_DIRS "${CMAKE_SOURCE_DIR}/src")
include_directories(${CMAKE_SOURCE_DIR}/src)

add_cmocka_test(simple_test
  SOURCES ${TEST_DIR}/simple_test.c
  COMPILE_OPTIONS ${DEFAULT_C_COMPILE_FLAGS}
  LINK_OPTIONS ${DEFAULT_LINK_FLAGS}
  LINK_LIBRARIES ${TEST_LIBS})

add_cmocka_test_environment(simple_test)
target_include_directories(simple_test PUBLIC "${TEST_DIRS}")
