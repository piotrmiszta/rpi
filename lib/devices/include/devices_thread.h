#ifndef DEVICES_THREAD_H_
#define DEVICES_THREAD_H_
/**
 * @brief   run other thread to other device
 *          currently only one thread per device
*/

int devices_thread_run(void* arg);

#endif //DEVICES_THREAD_H_