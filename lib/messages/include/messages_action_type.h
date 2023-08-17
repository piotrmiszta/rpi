#ifndef MESSAGES_ACTION_TYPE_H_
#define MESSAGES_ACTION_TYPE_H_
#include <stdint.h>

enum {
    JOB_ACTION_CHANGE_PIN = 1,
}

typedef struct MessageJobActionS{
    
    uint16_t pin;
    int8_t on;

}MessageJobActionS;

#endif //MESSAGES_ACTION_TYPE_H_