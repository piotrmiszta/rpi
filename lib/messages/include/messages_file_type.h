#ifndef MESSAGES_FILE_TYPE_H_
#define MESSAGES_FILE_TYPE_H_
#include "types.h"

enum {
    JOB_FILE_GET = 1,       /** @enum get file*/
    JOB_FILE_CHECK = 2,     /** @enum check file exisit */
    JOB_FILE_GET_INFO = 3,  /** @enum return info like size name format etc. */
    JOB_FILE_LIST = 4,      /** @enum list files availble for client */
};

typedef struct {
    char* filename;
}MessagesJobFileS;

#endif // MESSAGES_FILE_TYPE_H_