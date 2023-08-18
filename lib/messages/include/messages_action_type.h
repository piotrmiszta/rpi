#ifndef MESSAGES_ACTION_TYPE_H_
#define MESSAGES_ACTION_TYPE_H_
#include <stdint.h>
#include "devices.h"
#include "types.h"

enum {
    JOB_ACTION_CHANGE_PIN = 1,
};

typedef struct MessageJobActionS{
    device_t fd;

    uint16_t pin;
    int8_t on;
}MessageJobActionS;

#endif //MESSAGES_ACTION_TYPE_H_