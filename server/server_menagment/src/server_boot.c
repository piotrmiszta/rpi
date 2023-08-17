#include "server_boot.h"
#include "server_listen.h"
#include "server_menagment.h"
#include <errno.h>
#include <utility.h>
#include <threads.h>

#define SERVER_PORT     8080
/* *****************************************************************************************
 * static global variable declaration
***************************************************************************************** */
static thrd_t thread_server_listen;
static thrd_t thread_server_menagment;

/* *****************************************************************************************
 * static function declaration
***************************************************************************************** */
/**
 * @brief   function create socket, wraper to socket()
 * @param [out] sock socket description
 * @return 0 on success error_code otherwise
*/
static inline int server_socket(int* sock);

/**
 * @brief function bind with port wrapper to bind()
 * @param [in] server_params pointer to ServerS with information about server
 * @return 0 on success error_code otherwise
*/
static inline int server_bind(ServerS* server_params);

/**
 * @brief function assign params to server struct
 * @param [out] server_params pointer to ServerS* struct
 * @return 0 on success error_code otherwise
*/
static inline int server_param(ServerS* server_params);

/* *****************************************************************************************
 * global function definition
***************************************************************************************** */
int server_boot(ServerS* server_params) {
    if(!server_params) {
        return ENARG;
    }
    else {
        server_param(server_params);
        if(server_socket(&server_params->sock)) {
            return EF_SB;
        }
        if(server_bind(server_params)) {
            return EF_SB;
        }
        thrd_create(&thread_server_listen, server_listen_start_thread, server_params);
        thrd_create(&thread_server_menagment, server_menagment_start_thread, server_params);
        LOG_INFO("Successful booting server !!!");
        return SUCCESS;
    }
}

int server_close(ServerS* params) {
    server_menagment_end_thread();
    server_listen_end_thread(params->sock);
    thrd_join(thread_server_menagment, NULL);
    thrd_join(thread_server_listen, NULL);
    return SUCCESS;
}

/* *****************************************************************************************
 * static function definition
***************************************************************************************** */
static inline int server_socket(int* sock) {
    int res = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(res <= 0) {
        LOG_ERROR("Can't create socket error: %s", print_err());
        return ESOCK;
    }
    else {
        LOG_DEBUG("Create socket for server");
        *sock = res;
        return SUCCESS;
    }
}

static inline int server_bind(ServerS* server_params) {
    int res = bind(server_params->sock,
        (struct sockaddr*)(&server_params->address),
        sizeof(server_params->address));

    if(res) {
        LOG_ERROR("Can't bind socket error: %s", print_err());
        return EBIND;
    }
    else {
        LOG_DEBUG("Successful bind server to socket");
        return SUCCESS;
    }
}

static inline int server_param(ServerS* server_params) {
    server_params->address.sin_family = AF_INET;
    server_params->address.sin_addr.s_addr = INADDR_ANY;
    server_params->address.sin_port = htons(SERVER_PORT);
    LOG_DEBUG(  "Setting server params: "
                "sin_family: %d "
                "sin_addr.s_addr: %d "
                "sin_port: %d",
                server_params->address.sin_family,
                server_params->address.sin_addr.s_addr,
                server_params->address.sin_port);
    return SUCCESS;
}


