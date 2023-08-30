#include <utility.h>
#include "unistd.h"
#include "server_boot.h"
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include "error_codes.h"
#include "messages_info_type.h"
#include "devices_boot.h"
#define VALGRINDs
static inline void main_init(void);
//static inline void main_destroy(void);

static ServerS server;

static void sigint_handler(int sig);

int main(void) {
    struct sigaction act = {0};
    act.sa_handler = sigint_handler;
    sigaction(SIGINT, &act, NULL);
    main_init();
    if(devices_boot("/home/Piotr/rpi/files/server/devices/devices.xml")) {
        sigint_handler(SIGINT);
    }
    if(server_boot(&server)) {
        sigint_handler(SIGINT);
    }

    #ifdef VALGRIND
    sleep(40);
    sigint_handler(SIGINT);
    #else
    while(1) {}
    #endif

}

static inline void main_init(void) {
    LoggerParams param = {
        .author = "Piotr Miszta",
        .contact = "miszta.piotr.pm@gmail.com",
        .filename = "log.log",
        .version = 1.00,
    };
    logger_init(&param);
}

static void sigint_handler(int sig) {
    assert(sig == SIGINT);
    LOG_INFO(   "\n"
                "\t---------------------------------------------------------\n"
                "\tDetected Ctrl-C, closing server, deallcate all memory ...\n"
                "\t---------------------------------------------------------\n");
    server_close(&server);
    devices_teardown();
    logger_close();
    exit(0);
}
