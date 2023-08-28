#include "devices.h"
#include <stdlib.h>
#include "error_codes.h"
#include "messages.h"
#include <string.h>
#include "util_list.h"
#define allocator(size)         malloc(size)
#define deallocator(pointer)    free(pointer)

static ListS* list = NULL;

static inline device_t device_get_last_fd(void);
static inline void device_dealloc(void* arg);
static inline void device_msg_dealloc(void* arg);
static inline int device_search(void* device_, void* fd_);

error_t devices_create_list(void) {
    list = util_list_create(device_dealloc, device_search);
    if(!list) {
        LOG_ERROR("Device can't create list for devices!");
        return EDBOOT;
    }
    return SUCCESS;
}

DeviceS* devices_create(const char* name, protocol_t protocol) {
    if(strlen(name) > MAX_DEVICE_NAME_LEN) {
        LOG_ERROR("Device name is too long!");
        return NULL;
    }
    DeviceS* device = allocator(sizeof(*device));
    assert_ss(device);
    device->messages = queue_create(device_msg_dealloc);
    mtx_init(&device->mutex, mtx_plain);
    semaphore_init(&device->que_empty, 10);
    semaphore_init(&device->que_full, 0);
    device->protocol = protocol;
    strcpy(device->name, name);
    device->fd = device_get_last_fd() + 1;
    return device;
}
#include <stdio.h>
DeviceS* devices_get(const device_t fd) {
    assert_ss(list);
    index_t index = util_list_find(list, (void*)(&fd));
    if(index < -1) {
        LOG_ERROR("Don't find device");
        return NULL;
    }
    else{
        return util_list_get_index(list, index);
    }
}

error_t devices_add(DeviceS* device) {
    assert_ss(list);
    assert_ss(device);
    return util_list_push_back(list, device);
}

error_t device_del(const device_t fd) {
    index_t index = util_list_find(list, (void*)(&fd));
    if(index > -1) {
        if(util_list_pop_index(list, index)) {
            return SUCCESS;
        }
    }
    return EDDEL;
}
error_t device_check(const device_t fd) {
    index_t index = util_list_find(list, (void*)(&fd));
    if(index > -1) {
        return SUCCESS;
    }
    else {
        return FAILURE;
    }
}

static inline device_t device_get_last_fd(void) {
    DeviceS* device = util_list_get_back(list);
    if(device)
        return device->fd;
    else
        return -1;
}

static inline int device_search(void* device_, void* fd_) {
    DeviceS* device = (DeviceS* )device_;
    device_t fd = *((device_t*)(fd_));
    if (device->fd == fd) {
        return 0;
    }
    else if (device->fd > fd) {
        return 1;
    }
    else {
        return -1;
    }
}

static inline void device_dealloc(void* arg) {
    DeviceS* device = (DeviceS*)arg;
    mtx_destroy(&device->mutex);
    semaphore_destroy(&device->que_empty);
    semaphore_destroy(&device->que_full);
    queue_destroy(device->messages);
    deallocator(device);
}

static inline void device_msg_dealloc(void* arg) {
    MessageS* mess = (MessageS* )arg;
    messages_free(mess);
}

size_t device_get_size(void) {
    return util_list_get_size(list);
}

DeviceS* devices_get_index(size_t index) {
    return util_list_get_index(list, index);
}

void devices_teardown(void) {
    util_list_destroy(list);
}
