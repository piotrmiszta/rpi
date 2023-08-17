#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>
#include <stdlib.h>
static void simple_test_fun(void** state) {
  (void)state;
    assert_int_equal(1,1);
}

int main(void)
{
  const struct CMUnitTest tests[] = {
    unit_test(simple_test_fun),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
