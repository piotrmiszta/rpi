#include "handle_job_info_device_list.h"
#include "messages_types.h"
#include "devices.h"
#include <string.h>
#include <stdlib.h>

MessageS* handle_job_info_device_list(const MessageS* const msg) {
    (void)msg;
    size_t size = device_get_size();
    if(size * SIZE_ONE_DEVICE > PAYLOAD_SIZE_MAX) {
        //TODO: Handling of to big message
    }
    payload_size_t payload_size = sizeof(MessagesJobInfoDeviceS) + size * SIZE_ONE_DEVICE;
    MessagesJobInfoDeviceS* device_list = malloc(payload_size);
    assert_ss(device_list);
    for(size_t i = 0; i < size; i++) {
        DeviceS* device = devices_get_index(i);
        strcpy(device_list->device[i].name, device->name);
        device_list->device[i].fd = device->fd;
    }
    MessageS* msg_out = messages_alloc(MSG_SIZE(payload_size));
    msg_out->header.job_type = JOB_INFO_DEVICES;
    msg_out->header.msg_type = MESSAGE_TYPE_CFM;
    msg_out->header.req_type = MESSAGE_REQUEST_INFO;
    messages_set_payload(msg_out, device_list, payload_size);
    free(device_list);
    return msg_out;
}
