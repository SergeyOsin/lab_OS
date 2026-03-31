#include <pthread.h>

struct Mailbox {
    pthread_mutex_t mutex;
    bool has_data;
    char message[256];
};
