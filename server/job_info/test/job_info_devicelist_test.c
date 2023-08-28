#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>
#include "devices.h"
#include <stdio.h>
#include "handle_job_info_device_list.h"
#include "messages_types.h"
#include <stdlib.h>
static int setup(void** state) {
    devices_create_list();
    devices_add(devices_create("STM1", PROTOCOL_ETH));
    devices_add(devices_create("STM2", PROTOCOL_ETH));
    devices_add(devices_create("STM3", PROTOCOL_ETH));
    devices_add(devices_create("STM4", PROTOCOL_ETH));
    devices_add(devices_create("STM5", PROTOCOL_ETH));
    devices_add(devices_create("STM6", PROTOCOL_ETH));


    MessageS* msg = messages_alloc(MSG_SIZE(0));
    msg->header.req_type = MESSAGE_REQUEST_INFO;
    msg->header.msg_type = MESSAGE_TYPE_REQ;
    msg->header.job_type = JOB_INFO_DEVICES;
    (*state) = msg;
    return 0;
}

static int teardown(void** state) {
    devices_teardown();
    messages_free(*state);
    return 0;
}

static void test_handle_job_devicelist(void ** state) {
    MessageS* msg_in = (*state);
    MessageS* msg = handle_job_info_device_list(msg_in);
    assert_int_equal(msg->header.job_type, JOB_INFO_DEVICES);
    assert_int_equal(msg->header.msg_type, MESSAGE_TYPE_CFM);
    assert_int_equal(msg->header.req_type, MESSAGE_REQUEST_INFO);

    MessagesJobInfoDeviceS* devices = malloc(msg->header.size);
    messages_get_payload(msg, devices);
    char* temp[] = { "STM1", "STM2", "STM3", "STM4", "STM5", "STM6"};
    for(int i = 0; i < 6; i++) {
        assert_string_equal(devices->device[i].name, temp[i]);
        assert_int_equal(devices->device[i].fd, i);
        printf("%s - %d\n", devices->device[i].name, devices->device[i].fd);
    }
    free(devices);
    free(msg);
}

int main(void)
{
    const struct CMUnitTest tests[] = {
            unit_test(test_handle_job_devicelist),
    };
    return cmocka_run_group_tests(tests, setup, teardown);
}
