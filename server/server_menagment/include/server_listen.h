#ifndef SERVER_LISTEN_H_
#define SERVER_LISTEN_H_
/**
 * @file    server_listen.h
 * @brief   file specify listening and accepted incoming clients
*/

/**
 * @brief   function start routine server listen thread
 *          this thread is used to listen for client connection, accept this connection
 *          and call server_menagment_add_client function if accept was successful
 *
 * @param [in] arg pointer to ServerS casted to arg with server infromation
 * @return integer, 0 on success or error code
 *
 * @see error_codes.h
 * @see ServerS
*/
int server_listen_start_thread(void* arg);
/**
 * @brief   function end server_listen connection and close socket(), is important
 *          server_listen thread if die, die all connection
 *
 * @param [in] sock integer value of socket that socket() function was returned
*/
void server_listen_end_thread(int sock);

#endif //SERVER_LISTEN_H_
