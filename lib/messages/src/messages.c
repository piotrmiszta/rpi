#include "messages.h"
#include "error_codes.h"
#include <stdlib.h>
#include <unistd.h>


MessageS* messages_alloc(uint16_t size) {
    assert_ss(size >= sizeof(MessageS));
    assert_ss(size < MSG_SIZE_MAX);
    MessageS* msg = calloc(1, size);
    return msg;
}

void messages_free(MessageS* msg) {
    free(msg);
}

int messages_set_payload(MessageS* message, void* payload_in, payload_size_t size) {
    memcpy(message->payload, payload_in, size);
    message->header.size = size;
    return SUCCESS;
}

int messages_get_payload(MessageS* message, void* payload_out) {
    assert_ss(message->header.size > 0);
    assert_ss(payload_out);
    memcpy(payload_out, message->payload, message->header.size);
    return SUCCESS;
}

MessageS* messages_read(const int sock, int* err) {
    *err = SUCCESS;
    payload_size_t payload_size;
    ssize_t recv_val = recv(sock, &payload_size, sizeof(payload_size), MSG_NOSIGNAL);
    uint16_t msg_size = MSG_SIZE(payload_size);
    if(recv_val > 0) {
        /* read */
        if(payload_size > MSG_SIZE_MAX) {
            LOG_ERROR("Messages is too long! skipping!!!");
            *err = ELMSG;
            return NULL;
        }
        LOG_TRACE("Received size of msg payload_size = %u, msg_size = %u", payload_size, msg_size);
        MessageS* msg = messages_alloc(msg_size);
        assert_ss(msg);
        ssize_t msg_recv = recv(sock, msg, msg_size, MSG_NOSIGNAL);
        if(msg_recv > 0) {
            LOG_TRACE("Recaived msg size = %d, type = %d, req_type = %d", msg_recv,
                msg->header.msg_type,
                msg->header.req_type);
            *err = SUCCESS;
            return msg;
        }
        else {
            if(errno == EAGAIN || errno == EWOULDBLOCK || errno == EINPROGRESS) {
                LOG_WARNING("Timeout reached for message read, errno = %s", print_err());
                *err = ERTIMO;
            }
            else {
                LOG_ERROR("Failed to read msg!!! error: %s", print_err());
                *err = EREAD;
            }
            return NULL;
        }
    }
    else {
        if(errno == EAGAIN || errno == EWOULDBLOCK || errno == EINPROGRESS) {
            LOG_WARNING("Timeout reached for message size read, errno = %s", print_err());
            *err = ERTIMO;
            errno = SUCCESS;
        }
        else {
            LOG_ERROR("Failed to read msg size!!! error: %s", print_err());
            *err = EREAD;
        }
        return NULL;
    }
}

ssize_t messages_write(int sock, MessageS* msg) {
    assert_ss(msg);
    ssize_t send_size_val = send(sock, &(msg->header.size), sizeof(msg->header.size), MSG_NOSIGNAL);
    if(send_size_val > 0) {
        LOG_TRACE("Send msg size = %d", msg->header.size);
        size_t msg_size = MSG_SIZE(msg->header.size);
        ssize_t send_val = send(sock, msg, msg_size, MSG_NOSIGNAL);
        if( send_val > 0) {
            LOG_TRACE("Recaived msg size = %d, type = %d, req_type = %d", msg_size,
                msg->header.msg_type,
                msg->header.req_type);
            return send_val;
        }
        else {
            LOG_ERROR("Failed to write msg!!!");
            return send_val;
        }
    }
    else {
        LOG_ERROR("Failed to write msg size !!!");
        return send_size_val;
    }
}


