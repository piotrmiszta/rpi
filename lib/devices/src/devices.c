#include "devices.h"
#include <stdlib.h>
#include "error_codes.h"
#include "messages.h"
#include <string.h>
#include "util_list.h"
#define allocator(size)         malloc(size)
#define deallocator(pointer)    free(pointer)

static ListS* list = NULL;

/**
 * @brief get device descriptor from last device in list
 * @return device_t last fd pushed to list
*/
static inline device_t device_get_last_fd(void);

/**
 * @brief function to dealloc device, used to list
 * @param arg pointer to void* casted from DeviceS*
*/
static inline void device_dealloc(void* arg);

/**
 * @brief function to dealloc message, used to message_que
 * @param arg pointer to void* casted from MessageS**
*/
static inline void device_msg_dealloc(void* arg);

/**
 * @brief funtion to compare device->fd and fd
 * @param device_ pointer to void* casted from DeviceS*
 * @param fd_ pointer to void* casted from device_t*
 * @return 0 when equal, 1 when device->fd > fd else -1
*/
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
    semaphore_init(&device->que_full, 0);
    device->protocol = protocol;
    strcpy(device->name, name);
    device->fd = device_get_last_fd() + 1;
    device->run = true;
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
    size_t nr_devices = util_list_get_size(list);
    for(size_t i = 0; i < nr_devices; i++) {
        DeviceS* device = util_list_pop_back(list);
        device->run = false;
        semaphore_post(&device->que_full);
        thrd_join(device->thread, NULL);
        device_dealloc(device);
    }
    util_list_destroy(list);
}

void devices_push_message(MessageS* message, device_t fd) {
    DeviceS* device = devices_get(fd);
    assert_ss(device);
    mtx_lock(&device->mutex);
    queue_push(device->messages, message);
    mtx_unlock(&device->mutex);
    semaphore_post(&device->que_full);
}

MessageS* devices_pop_message(DeviceS* device, error_t* err) {
    semaphore_wait(&device->que_full);
    if(device->run == false) {
        *err = ETHRD;
        return NULL;
    }
    mtx_lock(&device->mutex);
    MessageS* message = queue_pop(device->messages);
    mtx_unlock(&device->mutex);
    return message;
}
