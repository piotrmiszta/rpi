#ifndef DEVICES_H_
#define DEVICES_H_
#include <stdint.h>
#include <utility.h>
#define MAX_DEVICE_NAME_LEN     (20)
#include "types.h"

enum{
    PROTOCOL_ETH = 0,   /** @enum Ethernet connection */
    PROTOCOL_UART = 1,  /** @enum Uart/Usart connection */
    PROTOCOL_USB = 2,   /** @enum USB connection */
    PROTOCOL_CUSTOM = 3,/** @enum Other connection */
    PROTOCOL_USART = 4,
    PROTOCOL_UNKNOWN = 100 /** @enum Wrong protocol specified*/
};
/**
 * @brief DeviceS is struct that describes devices connected to source hw
*/
typedef struct {
    device_t fd;            /** < device descriptor, [id]*/
    protocol_t protocol;    /** < protocol used to connect to device*/
    Queue* messages;        /** < que for messages that will be send to device */
    semaphore_t que_empty;  /** < semaphore for que */
    semaphore_t que_full;   /** < semaphore for que */
    mtx_t mutex;            /** < mtx for que */
    char name[MAX_DEVICE_NAME_LEN]; /** < name for device */
    thrd_t thread;
    bool run;
}DeviceS;

/**
 * @brief funtion to initialize static variables for devices
 *
 * @return error_t: error code
 *         EDBOOT - for error with list initialize
 *         SUCCESS - for no errors
*/
error_t devices_create_list(void);

/**
 * @brief allocate memory for device and initialize members
 * @param name device name
 * @param protocol protocol_t type of protocol
 * @return pointer to DeviceS
*/
DeviceS* devices_create(const char* name, protocol_t protocol);

/**
 * @brief get device with specified device descriptor
 * @param fd device descriptor
 * @return pointer to DeviceS or NULL when not found
*/
DeviceS* devices_get(const device_t fd);

/**
 * @brief add device to list
 * @return SUCCESS or Error code
*/
error_t devices_add(DeviceS* device);

/**
 * @brief delete device from list
 * @param fd device descriptor
 * @return 0 on success or error code
*/
error_t device_del(const device_t fd);

/**
 * @brief check that device is in list
 * @param fd device descriptor
 * @return 0 on success or -1 Failure when don;t exsist
*/
error_t device_check(const device_t fd);

size_t device_get_size(void);

DeviceS* devices_get_index(size_t index);

void devices_teardown(void);
#endif //DEVICES_H_
