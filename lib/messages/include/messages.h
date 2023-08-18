#ifndef MESSAGES_H_
#define MESSAGES_H_
#include <stdint.h>
#include "messages.h"
#include "error_codes.h"
#include <utility.h>
#include <unistd.h>
#include <sys/socket.h>
#include "types.h"
/**
 * @struct MessageS
*/
typedef struct MessageS {
    struct MessageHeaderS{
        payload_size_t size;        /**< size of payload */
        message_type_t msg_type;    /**< message type specified by MessageTypeE  */
        request_type_t req_type;
        job_t job_type;
    } header;
    char payload[];                 /**< flexible array with payload */
}MessageS;

enum {
    MESSAGE_TYPE_REQ = 1,       /** @enum request */
    MESSAGE_TYPE_CFM = 2,       /** @enum confirmation */
    MESSAGE_TYPE_REJ = 3,       /** @enum reject */
};

enum {
    MESSAGE_REQUEST_ACTION = 1,     /** @enum action for device connected to server */
    MESSAGE_REQUEST_FILE = 2,       /** @enum get, send, show info about file */
    MESSAGE_REQUEST_INFO = 3,       /** @enum get info about system */
};

#define PAYLOAD_SIZE_MAX            (1000)  /** @def Maximum size of payload in one msg */
#define MSG_SIZE_MAX                (PAYLOAD_SIZE_MAX + sizeof(MessageS))       /** @def maximum size of message (payload + header)*/
#define MSG_SIZE(payload_size)      \
    ((payload_size_t)payload_size + (payload_size_t)sizeof(MessageS))  /** @def macro to set get message size based on paylolad size*/

/**
 * @brief messages_alloc function allocate memory for message
 * @param size memory to be allocated, its good to always put MSG_SIZE() macro here with payload size
 * @return pointer to allocated memory or null if can't allocated
*/
MessageS* messages_alloc(uint16_t size);

/**
 * @brief messages_free function free memory used by message
 * @param msg pointer to MessageS that will be deallocated
*/
void messages_free(MessageS* msg);

/**
 * @brief messages_set_payload setting payload to message: copy payload, and setting size of payload
 * @param message pointer to MessageS with message that we want to set payload
 * @param payload_in pointer to payload
 * @param size size of payload, its good practice to try always used local payload and setting here sizeof(payload)
 * @return  0 on SUCCESS 
 *          -1 on failure
*/
int messages_set_payload(MessageS* message, void* payload_in, payload_size_t size);

/**
 * @brief messages_get_payload function is used to copy payload from message to pointer payload_out
 * @param message pointer to MessageS we want to copy from payload
 * @param payload_out pointer to object we want to copy in payload, cant be NULL
 * @return  0 on success
 *          -1 on failure
*/
int messages_get_payload(MessageS* message, void* payload_out);

/**
 * @brief messages_read function is used to read incoming message
 * @param sock socket from we want to read
 * @param err   pointer to int, used to specify error when try to read 
 *              can be ETIMEO when timeout is reached, or EREAD on other error set by read() syscall
 *              or 0 on success
 * @return pointer to MessageS with setted payload or NULL pointer when error
*/
MessageS* messages_read(const int sock, int* err);

/**
 * @brief messages_write function write message to sock
 * @param sock socket to send message
 * @param msg pointer to MessageS which we want to send to specify socket
 * @return number of send bytes or -1 when error occured
*/
ssize_t messages_write(int sock, MessageS* msg);

#endif // MESSAGES_H_
