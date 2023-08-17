#ifndef CLIENT_CONNECTION_H
#define CLIENT_CONNECTION_H
#include <assert.h>
#include <threads.h>
#include <stdbool.h>
#include <stdint.h>
#include <netinet/in.h>

#define CONNECTION_PORT 8080
#define IP_ADDRESS "127.0.0.1"

typedef struct {
    int socket; /**< [out] fd of socket */
    int port; /**< [in] port eg. 8080 */
    const char ip_address[15]; /**< [in] adres ip eg. "127.0.0.1" */
    struct sockaddr_in address; /**< [out] params to connect with server */
    volatile bool run; /** [in, out] flag to end thread */
}ConnectionArgS;
/* TODO: check padding */

int connection_start_thread(void* arg);

#endif //CLIENT_CONNECTION_H
