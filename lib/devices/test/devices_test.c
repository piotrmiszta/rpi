#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>
#include "devices_boot.h"
#include "devices.h"


static int setup(void** state) {
    LoggerParams param = {
        .author = "Piotr Miszta",
        .contact = "miszta.piotr.pm@gmail.com",
        .filename = "log.log",
        .version = 1.00,
    };
    logger_init(&param);
    devices_boot("/home/Piotr/rpi/lib/devices/test/files/device.xml"); //TODO: think about PATH
    devices_add(devices_create("STM1", PROTOCOL_ETH));
    devices_add(devices_create("STM2", PROTOCOL_ETH));
    devices_add(devices_create("STM3", PROTOCOL_ETH));
    devices_add(devices_create("STM4", PROTOCOL_ETH));

    return 0;
}
static int teardown(void** state) {
    devices_teardown();
    logger_close();
    return 0;
}
static void expected_fd(void ** state) {
    for(int i = 0; i < 3; i++) {
        DeviceS* device = devices_get(i);
        assert_int_equal(device->fd, i);
        assert_int_equal(device->protocol, PROTOCOL_ETH);
    }
}

static void expected_null(void** state) {
    DeviceS* device = devices_get(10);
    assert_ptr_equal(device, NULL);
}

int main(void)
{
    const struct CMUnitTest tests[] = {
            unit_test(expected_fd),
            unit_test(expected_null),
    };
    return cmocka_run_group_tests(tests, setup, teardown);
}
