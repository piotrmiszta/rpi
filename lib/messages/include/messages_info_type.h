#ifndef MESSAGES_INFO_TYPE_H_
#define MESSAGES_INFO_TYPE_H_
#include "devices.h"
#include "types.h"

#define MAX_ACTION_NAME_LEN     100

enum {
    JOB_INFO_DEVICES = 1, /** @enum return information about availble devices */
    JOB_INFO_ACTIONS = 2, /** @enum return information about availble action for specify device */
};

typedef struct {
    client_t fd;
    struct {
        char name[MAX_DEVICE_NAME_LEN];
        device_t fd;
    }device[];
}MessagesJobInfoDeviceS;
#define SIZE_ONE_DEVICE         (sizeof(device_t) + (sizeof(char) * MAX_ACTION_NAME_LEN))
typedef struct {
    device_t fd;
    struct {
        char name[MAX_ACTION_NAME_LEN];
        char desc[MAX_DEVICE_NAME_LEN];
        job_t enum_code;
    }actions[];
}MessagesJobInfoActionS;

#endif //MESSAGES_INFO_TYPE_H_