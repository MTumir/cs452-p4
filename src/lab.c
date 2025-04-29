#include <pthread.h>
#include "lab.h"

struct queue {
    void **b_buffer;    // bounded buffer
    size_t capacity;    // maximum length of queue
    size_t size;        // current length of queue
    size_t head;        // head of queue (for dequeueing)
    size_t tail;        // tail of queue (for enqueueing)
    bool shutdown;      // true if queue is shutdown
    
    pthread_mutex_t lock;   // used when critical code is executed
};

queue_t queue_init(int capacity) {
    if (capacity <= 0) {
        return NULL;
    }

    queue_t q = (queue_t)malloc(sizeof(struct queue));
    if (!q) {
        return NULL;
    }

    q->b_buffer = (void **)malloc(sizeof(void *) * capacity);
    if (!q->b_buffer) {
        free(q);
        return NULL;
    }

    q->capacity = capacity;
    q->size = 0;
    q->head = 0;
    q->tail = 0;
    q->shutdown = false;

    // do locking stuff here later its too late for this rn

    return q;
}

void queue_destroy(queue_t q) {
    if (!q) {
        return;
    }

    // TODO LOCK

    free(q->b_buffer);
    free(q);

    // TODO UNLOCK (or not cuz the lock is in q)
}

void enqueue(queue_t q, void *data) {
    if (!q || !data) {
        return;
    }

    // TODO LOCK

    if (q->size >= q->capacity) {
        // TODO UNLOCK
        return;
    }

    q->b_buffer[q->tail] = data;
    q->tail = q->tail + sizeof(data);
    q->size++;

    // TODO UNLOCK

}

void *dequeue(queue_t q) {
    if (!q) {
        return NULL;
    }

    // TODO LOCK

    if (q->size <= 0) {
        // TODO UNLOCK
        return NULL;
    }

    void *ret = q->b_buffer[q->head];
    q->head = q->head + sizeof(ret);
    q->size--;

    // TODO UNLOCK

    return ret;
}

void queue_shutdown(queue_t q) {
    if (!q) {
        return;
    }

    // TODO LOCK
    q->shutdown = true;
    // TODO UNLOCK
}

bool is_empty(queue_t q) {
    if (!q) {
        return true;
    }

    // TODO LOCK
    bool ret = q->size == 0;
    // TODO UNLOCK

    return ret;
}

bool is_shutdown(queue_t q) {
    if (!q) {
        return true;
    }

    // TODO LOCK
    bool ret = q->shutdown;
    // TODO UNLOCK

    return ret;
}