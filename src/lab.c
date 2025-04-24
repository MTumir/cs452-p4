#include <pthread.h>
#include <stdio.h>

#include "lab.h"

struct queue_t {
    size_t size;        // Length of queue
    pthread_t head;     // Head of queue (for popping)
};

queue_t queue_init(int capacity) {
    if (capacity == 0) {
        return NULL;
    }

}

void queue_destory(queue_t q) {
    if (!q) {
        return;
    }

}

void enqueue(queue_t q, void *data) {
    if (!q || !data) {
        return;
    }

}

void *dequeue(queue_t q) {
    if (!q) {
        return NULL;
    }

    return NULL;
}

void queue_shutdown(queue_t q) {
    if (!q) {
        return;
    }

}

bool is_empty(queue_t q) {
    if (!q) {
        return true;
    }

    return false;
}

bool is_shutdown(queue_t q) {
    if (!q) {
        return true;
    }

    return false;
}