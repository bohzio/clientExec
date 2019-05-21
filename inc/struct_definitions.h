#ifndef _REQUEST_RESPONSE_HH
#define _REQUEST_RESPONSE_HH

#include <sys/types.h>


struct SharedItem {
    char idUser[11];
    char key[20];
    time_t timestamp;
};

#endif
