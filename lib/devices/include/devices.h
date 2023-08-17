#ifndef DEVICES_H_
#define DEVICES_H_
#include <stdint.h>
#define MAX_DEVICE_NAME_LEN     (20)

enum{
    PROTOCOL_ETH = 0,
    PROTOCOL_UART = 1,
    PROTOCOL_USB = 2,
    PROTOCOL_CUSTOM = 3,
};

typedef struct {
    uint32_t fd;
    uint8_t protocol;
    char name[MAX_DEVICE_NAME_LEN];
}DeviceS;

DeviceS* devices_create(void);

#endif //DEVICES_H_