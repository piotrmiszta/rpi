#ifndef HANDLE_JOB_INFO_H_
#define HANDLE_JOB_INFO_H_
#include "messages.h"
#include "messages_types.h"
#include "handle_job_info_device_list.h"

static inline MessageS* handle_job_info(const MessageS* const msg_in) {
    LOG_DEBUG("Handling job_info request");
    MessageS* msg_out = NULL;
    switch (msg_in->header.job_type) {
    case JOB_INFO_DEVICES:
        msg_out = handle_job_info_device_list(msg_in);
        break;

    default:
        break;
    }
    return msg_out;
}

#endif //HANDLE_JOB_INFO_H_
