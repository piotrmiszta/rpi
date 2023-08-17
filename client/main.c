#include <stdio.h>
#include <utility.h>
#include "connection.h"
int main(int argc, char ** argv) {
    LoggerParams param = {
            .author = "Piotr Miszta",
            .contact = "miszta.piotr.pm@gmail.com",
            .filename = "log.log",
            .version = 1.00,
    };
    logger_init(&param);
    ConnectionArgS args = {.socket = 0, .run = 1, .port = CONNECTION_PORT, .ip_address = IP_ADDRESS};
    thrd_t thread;
    thrd_create(&thread, connection_start_thread, &args);
    thrd_join(thread, NULL);
    return 0;
}
