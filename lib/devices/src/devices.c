#include "devices.h"
#include <stdlib.h>
#include "error_codes.h"
#include "messages.h"
#include <string.h>
#include "util_list.h"
#define allocator(size)         malloc(size)
#define deallocator(pointer)    free(pointer)

static ListS* list;

static inline device_t device_get_last_fd(void);

static void device_dealloc(void* arg) {
    MessageS* mess = (MessageS* )arg;
    messages_free(mess);
}

DeviceS* devices_create(const char* name, protocol_t protocol) {
    if(strlen(name) > MAX_DEVICE_NAME_LEN) {
        LOG_ERROR("Device name is too long!");
        return NULL;
    }
    DeviceS* device = allocator(sizeof(*device));
    assert_ss(device);
    device->messages = queue_create(device_dealloc);
    mtx_init(&device->mutex, mtx_plain);
    semaphore_init(&device->que_empty, 10);
    semaphore_init(&device->que_full, 0);
    device->protocol = protocol;
    strcpy(device->name, name);
    device->fd = device_get_last_fd() + 1;
    return NULL;
}

static inline device_t device_get_last_fd(void) {
    DeviceS* device = util_list_get_back(list);
    return device->fd;
}
