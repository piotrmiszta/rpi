#include "connection.h"
#include <utility.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>
#include "error_codes.h"
#include "messages_info_type.h"
#include "messages.h"
#include <stdlib.h>
#include <stdio.h>
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
        int x = 0;
        printf("Choose request\n");
        scanf("%d", &x);
        if(x == 1) {
            MessageS* message = messages_alloc(MSG_SIZE(0));
            message->header.job_type = JOB_INFO_DEVICES;
            message->header.msg_type = MESSAGE_TYPE_REQ;
            message->header.req_type = MESSAGE_REQUEST_INFO;
            message->header.size = 0;
            messages_write(args->socket, message);
            int err;
            while(1) {
                MessageS* msg_out = messages_read(args->socket, &err);
                if(msg_out) {
                    MessagesJobInfoDeviceS* device = malloc(msg_out->header.size);
                    messages_get_payload(msg_out, device);
                    for(int i = 0 ; i < device->size; i++) {
                        printf("%d, %s", device->device[i].fd, device->device[i].name);
                    }
                    break;
                }

            }

        }
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
