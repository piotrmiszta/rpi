#ifndef SERVER_BOOT_H_
#define SERVER_BOOT_H_
#include "error_codes.h"
#include "server_defs.h"
/**
 * @file    server_boot.h
 * @brief   file specify boot of server, allocating variables and starting threads that are
 *          required to server working correctly
*/


/**
 * @brief   allocated and setting all params that is needed to start server job
 *          and start server_menagment thread and server_listen thread
 *
 * @param [in] server_params pointer to ServerS that store information about server
 * @return  integer, 0 on success or error
 *
 * @see error_codes.h
 * @see ServerS
*/
int server_boot(ServerS* server_params);
/**
 * @brief   closing server that means closing server_menagment thread
 *          and closing server_listen thread
 *
 * @param [in] server_params need to close server socket
 * @return intereger, 0 on success or error code
 *
 * @see error_codes.h
 * @see server_menagment_end_thread
 * @see server_listen_end_thread
*/
int server_close(ServerS* server_params);

#endif // SERVER_BOOT_H_
