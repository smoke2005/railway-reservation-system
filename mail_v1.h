#ifndef MAIL_V1_H_INCLUDED
#define MAIL_V1_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include "curl.h"

extern char payload_text[10000];

struct upload_status {
    int lines_read;
};

extern size_t payload_source(void *ptr, size_t size, size_t nmemb, void *userp) {
    struct upload_status *upload_ctx = (struct upload_status *)userp;
    const char *data;
    size_t room = size * nmemb;

    if ((size == 0) || (nmemb == 0) || ((size * nmemb) < 1)) {
        return 0;
    }

    data = &payload_text[upload_ctx->lines_read];

    if (*data) {
        size_t len = strlen(data);
        if (len > room) {
            len = room;
        }
        memcpy(ptr, data, len);
        upload_ctx->lines_read += len;
        return len;
    }

    return 0;
}



#endif // MAIL_V1_H_INCLUDED
