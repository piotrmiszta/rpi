#ifndef TYPES_H_
#define TYPES_H_
#include <stdint.h>

typedef int32_t error_t;
typedef long long int index_t;
typedef uint16_t payload_size_t;    /** @typedef type to use with payload size */
typedef uint8_t request_type_t;     /** @typedef type to use with request type*/
typedef uint8_t message_type_t;     /** @typedef type to use with message type*/
typedef uint32_t job_t;             /** @typedef type to use with job type */

typedef uint32_t device_t;
typedef uint8_t protocol_t;

typedef int32_t client_t;

#endif //TYPES_H_