#ifndef DEVICES_H_
#define DEVICES_H_
#include <stdint.h>
#include <utility.h>
#define MAX_DEVICE_NAME_LEN     (20)
#include "types.h"

enum{
    PROTOCOL_ETH = 0,
    PROTOCOL_UART = 1,
    PROTOCOL_USB = 2,
    PROTOCOL_CUSTOM = 3,
};

typedef struct {
    device_t fd;
    protocol_t protocol;
    Queue* messages;
    semaphore_t que_empty;
    semaphore_t que_full;
    mtx_t mutex;
    char name[MAX_DEVICE_NAME_LEN];
}DeviceS;

DeviceS* devices_create(const char* name, protocol_t protocol);

#endif //DEVICES_H_