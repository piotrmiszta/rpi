#include "devices_thread.h"
#include "devices.h"
#include "error_codes.h"
#include "messages.h"
#include <time.h>

static MessageS* devices_thread_parse_msg(const MessageS* const msg_in);

int devices_thread_run(void* arg) {
    DeviceS* device = (DeviceS*) arg;
    while(device->run) {
        error_t local_errno;
        MessageS* msg_in = devices_pop_message(device, &local_errno);
        if(msg_in == NULL && local_errno == ETHRD) {
            break;
        }
        assert_ss(msg_in->header.msg_type == MESSAGE_TYPE_INTERNAL);
        MessageS* msg_out = devices_thread_parse_msg(msg_in);
        if(!msg_out) {
            LOG_ERROR("Error when parsing messages!");
        }
        device->write(device->socket, msg_out);
    }
    LOG_DEBUG("Thread for device nr %d ended!!", device->fd);
    return SUCCESS;
}

static MessageS* devices_thread_parse_msg(const MessageS* const msg_in) {
    return NULL;
}