#include "connection.h"
#include <utility.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>
#include "error_codes.h"
static inline int connection_socket(ConnectionArgS* args);
static inline int connection_connect(ConnectionArgS* args);
int connection_start_thread(void* arg) {
    LOG_INFO("Thread connection started: thread id %lu", thrd_current());
    ConnectionArgS* args = (ConnectionArgS*)(arg);
    if(connection_socket(args)) {
        /* TODO: error, now shut down thread */
        thrd_exit(FAILURE);
    }
    if(connection_connect(args)) {
        thrd_exit(FAILURE);
    }
    LOG_INFO("Successful connected to server");
    while(args->run) {
        /* write, read, etc */
        /* we can think about created buffer in args, so we could send pointer to another thread */
        /* and he can easly send message based on this args */
        char temp[] = "test";
        uint16_t len = 5;
        write(args->socket, &len, sizeof(uint16_t));
        write(args->socket, temp, sizeof(char) * len);
        thrd_sleep(&(struct timespec){.tv_sec=2}, NULL);
    }
    return SUCCESS;
}

static inline int connection_socket(ConnectionArgS* args) {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd < 0) {
        LOG_ERROR("Can't create socket: %s", print_err());
        //NOTE: Dont exit, now its for debugging, when no logic of program is currently implemented
        args->run = false;
        return ESOCK;
    }
    else {
        args->socket = fd;
        return SUCCESS;
    }
}

static inline int connection_connect(ConnectionArgS* args) {
    struct sockaddr_in temp = {.sin_port = htons(args->port),
            .sin_family = AF_INET };
    if( inet_pton(AF_INET, args->ip_address, &(temp.sin_addr)) <= 0 ) {
        LOG_ERROR("Error occured when called inet_pton() error: %s", print_err());
        return ECONN;
    }
    args->address = temp;
    if( connect(args->socket, (struct sockaddr*)(&args->address), sizeof(args->address))  < 0) {
        LOG_ERROR("Error occured when trying to connect to server, error: %s", print_err());
        return ECONN;
    }
    return SUCCESS;
}
