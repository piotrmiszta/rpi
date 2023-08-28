#include "devices_thread.h"
#include "devices.h"
#include "error_codes.h"
#include <time.h>

int devices_thread_run(void* arg) {
    DeviceS* device = (DeviceS*) arg;
    (void)device;
    while(device->run) {
        /* running */
        LOG_WARNING("Remove this !!!!");
        LOG_INFO("Running thread for device: %lu", device->fd);
        thrd_sleep(&(struct timespec){.tv_sec=2}, NULL);
    }
    return SUCCESS;
}