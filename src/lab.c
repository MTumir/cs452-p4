#include <pthread.h>
#include "lab.h"

struct queue {
    void **b_buffer;    // bounded buffer
    size_t capacity;    // maximum length of queue
    size_t size;        // current length of queue
    size_t head;        // head of queue (for dequeueing)
    size_t tail;        // tail of queue (for enqueueing)
    bool shutdown;      // true if queue is shutdown
    
    pthread_mutex_t mutex;   // used when critical code is executed
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

    pthread_mutex_init(&q->mutex, NULL);

    return q;
}

void queue_destroy(queue_t q) {
    if (!q) {
        return;
    }

    queue_shutdown(q);
    free(q->b_buffer);
    free(q);
}

void enqueue(queue_t q, void *data) {
    if (!q || !data) {
        return;
    }

    pthread_mutex_lock(&q->mutex);

    if (q->size >= q->capacity) {
        pthread_mutex_unlock(&q->mutex);
        return;
    }

    q->b_buffer[q->tail] = data;
    q->tail = q->tail + sizeof(data);
    q->size++;

    pthread_mutex_unlock(&q->mutex);
}

void *dequeue(queue_t q) {
    if (!q) {
        return NULL;
    }

    pthread_mutex_lock(&q->mutex);

    if (q->size <= 0) {
        pthread_mutex_unlock(&q->mutex);
        return NULL;
    }

    void *ret = q->b_buffer[q->head];
    q->head = q->head + sizeof(ret);
    q->size--;

    pthread_mutex_unlock(&q->mutex);

    return ret;
}

void queue_shutdown(queue_t q) {
    if (!q) {
        return;
    }

    pthread_mutex_lock(&q->mutex);
    q->shutdown = true;
    pthread_mutex_unlock(&q->mutex);
}

bool is_empty(queue_t q) {
    if (!q) {
        return true;
    }

    pthread_mutex_lock(&q->mutex);
    bool ret = q->size == 0;
    pthread_mutex_unlock(&q->mutex);

    return ret;
}

bool is_shutdown(queue_t q) {
    if (!q) {
        return true;
    }

    pthread_mutex_lock(&q->mutex);
    bool ret = q->shutdown;
    pthread_mutex_unlock(&q->mutex);

    return ret;
}