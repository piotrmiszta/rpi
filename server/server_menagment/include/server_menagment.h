#ifndef SERVER_MENAGMENT_H_
#define SERVER_MENAGMENT_H_
#include "server_defs.h"
/**
 * @file    server_menagment.h
 * @brief   used to check client connection and endind client threads
*/

/**
 * @brief   function that start routine new thread server_menagment
 *          this thread is use to periodic check connection with clients
 *          and end this connection, deallocate memory used by connection with clients
 *
 * @param   [in] arg casted to void* pointer to ServerS struct allocated in server_boot.c
 * @return  0 on success, or error code
 *
 * @see     error_codes.h
 * @see     server_boot
*/
int server_menagment_start_thread(void* arg);
/**
 * @brief   function end server menagment thread by setting global variable to false
 *
*/
void server_menagment_end_thread(void);
/**
 * @brief   function push new client connection struct ServerConnectionS to
 *          client list store in server_menagment.c
 *
 * @param   [in] client pointer to ServerConnectionS with accepted client information
 * @return  0 on success or error code
 *
 * @see     error_codes.h
 * @see     ServerConnectionS
 *
*/
int server_menagment_add_client(ServerConnectionS* client);

#endif //SERVER_MENAGMENT_H_
