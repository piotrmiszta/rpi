#include "server_menagment.h"
#include "server_defs.h"
#include "error_codes.h"
#include "client_connection.h"
#include "util_list.h"
#include <time.h> //for thrd_sleep()
#include <utility.h> //for loger()
#include <stdlib.h> //for free()
#include <unistd.h> //for write()

/* *****************************************************************************************
 * static global variable declaration
***************************************************************************************** */
static bool server_menagment_run = true;
static ListS* client_list = NULL;

/* *****************************************************************************************
 * static function declaration
***************************************************************************************** */
/**
 * @brief   function used in client list,
 * @param [in] client_struct pointer to ServerConnections* casted to void
 * @param [in] thread_id pointer to thrd_t casted to void
 * @return  integer, 0 if thrd_t in client_struct is the same value as *thread_id
 *          -1 othrewise
 *
 * @see ServerConnectionS
*/
static inline int server_menagment_thread_comp(void* client_struct, void* thread_id);

/**
 * @brief   function used in client list, free ServerConnectionS and destroy mutex
 * @param [in] arg pointer to ServerConnectionS casted to void
*/
static inline void server_menagment_thread_free(void* arg);

/**
 * @brief   function pop from client list all clients and ending their threads
 * @return  0 on success, error_code otherwise
*/
static inline int server_menagment_close_all_client(void);

/**
 * @brief   function check connection of all clients
 *          if can't connect ending client thread
 * @return  0 on success, error_code otherwise
*/
static inline int server_menagment_check_connection(void);

/**
 * @brief   function end connection with client by end client thread
 * @param [in] client pointer to ServerConnectionS*, with client information we want end job
 * @return  0 on success, error_code otherwise
*/
static inline int server_menagment_end_client_thread(ServerConnectionS* client);

/* *****************************************************************************************
 * global function definition
***************************************************************************************** */
int server_menagment_start_thread(void* arg) {
    LOG_INFO("Thread: server menagment started");
    client_list = util_list_create(server_menagment_thread_free, server_menagment_thread_comp);
    ServerS* server = arg;
    (void) server;
    while(server_menagment_run) {
        thrd_sleep(&(struct timespec){.tv_sec=2}, NULL);
        /* sleep, after it check connection, if can't send msg, end thread */
        server_menagment_check_connection();
    }
    LOG_INFO("Closing server menagment thread");
    /* need to join all client threads */
    server_menagment_close_all_client();
    LOG_INFO("Successful ending all clients threads");
    return 0;
}

void server_menagment_end_thread(void) {
    server_menagment_run = 0;
}

int server_menagment_add_client(ServerConnectionS* client) {
    assert_ss(client_list);
    assert_ss(client);
    client->thread_run = true;
    assert_ss(mtx_init(&client->mutex, mtx_plain) == 0);
    util_list_push_back(client_list, client);
    int res = thrd_create(&client->thread, client_connection_start_thread, client);
    if(res) {
        LOG_ERROR("Can't create new thread for client error: %s", print_err());
        return ESCAD;
    }
    else {
        LOG_INFO("Successfull created new thread: %lu, for client: sock = %d", client->thread, client->sock);
    }
    return SUCCESS;
}

/* *****************************************************************************************
 * static function definition
***************************************************************************************** */
static inline int server_menagment_close_all_client(void) {
    /* check number of clients */
    size_t nr_clients = util_list_get_size(client_list);
    /* iterate for all clients */
    for(size_t i = 0; i < nr_clients; i++) {
        ServerConnectionS* client = util_list_pop_back(client_list);
        /* lock mutex to be sure that nothing will be send to this client */
        server_menagment_end_client_thread(client);
    }
    util_list_destroy(client_list);
    return SUCCESS;
}

static inline int server_menagment_thread_comp(void* client_struct, void* thread_id) {
    thrd_t* _thread_id = thread_id;
    ServerConnectionS* _client_struct = client_struct;
    if( *_thread_id == _client_struct->thread) {
        return 0;
    }
    else {
        return -1;
    }
}

static inline void server_menagment_thread_free(void* arg) {
    ServerConnectionS* client = arg;
    mtx_destroy(&client->mutex);
    free(client);
}

static inline int server_menagment_check_connection(void) {
    size_t nr_clients = util_list_get_size(client_list);
    for(size_t i = 0; i < nr_clients; i++) {
        ServerConnectionS* client = util_list_get_back(client_list);
        /*  this is temporary solution, uint64_t will be msg payload,
            always when sending msg, first send size of sending msg, and its uint64_t
            this always need to be protected by mutex, if not this thread can send testing msg
            between sending size and payload. Now send uint64_t (size) = 0x00,
            this never can happen to send 0 payload so its good point to test connection bcs
            client can ignore this value
            TODO: Change uint64_t to size type if its will be implemented */
        if(client->thread_run) {
            uint64_t msg = 0x00;
            ssize_t writed = send(client->sock, &msg, sizeof(uint64_t), MSG_NOSIGNAL);
            if(writed == -1) {
                /* end thread */
                LOG_INFO("Client is not reachable err: %s, ending thread ", print_err());
                client = util_list_pop_back(client_list);
                server_menagment_end_client_thread(client);
            }
        }
        else {
            LOG_INFO("Client is not reachablee thread was end previous, free space for client");
            client = util_list_pop_back(client_list);
            server_menagment_end_client_thread(client);
        }
    }
    return SUCCESS;
}

static inline int server_menagment_end_client_thread(ServerConnectionS* client) {
    int client_status;
    client->thread_run = false;
    mtx_lock(&client->mutex);
    // NOTE: maybe here change sockopt to non block
    thrd_join(client->thread, &client_status);
    mtx_unlock(&client->mutex);
    /* destroy this function */
    server_menagment_thread_free(client);
    return client_status;
}
