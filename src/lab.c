#include <pthread.h>
#include "lab.h"

struct queue {
    void **b_buffer;    // bounded buffer
    size_t capacity;    // maximum length of queue
    size_t size;        // current length of queue
    size_t head;        // head of queue (for dequeueing)
    size_t tail;        // tail of queue (for enqueueing)
    bool shutdown;      // true if queue is shutdown
    
    pthread_mutex_t mutex;      // used when critical code is executed
    pthread_cond_t not_full;    // signals that queue is not full
    pthread_cond_t not_empty;   // signals that queue is not empty
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

    // initialize mutex and condition variables
    pthread_mutex_init(&q->mutex, NULL);
    pthread_cond_init(&q->not_full, NULL);
    pthread_cond_init(&q->not_empty, NULL);

    return q;
}

void queue_destroy(queue_t q) {
    if (!q) {
        return;
    }

    queue_shutdown(q);
    pthread_cond_destroy(&q->not_empty);
    pthread_cond_destroy(&q->not_full);
    pthread_mutex_destroy(&q->mutex);
    free(q->b_buffer);
    free(q);
}

void enqueue(queue_t q, void *data) {
    if (!q || !data) {
        return;
    }

    pthread_mutex_lock(&q->mutex);

    // if full, wait for q to not be full
    while (q->size >= q->capacity && !q->shutdown) {    // CR - Only wait if shutdown is inactive
        pthread_cond_wait(&q->not_full, &q->mutex);
    }

    // CR - Disallow enqueues if shutdown is active
    if (q->shutdown) {
        pthread_mutex_unlock(&q->mutex);
        return;
    }

    q->b_buffer[q->tail] = data;
    q->tail = (q->tail + 1) % q->capacity;    // CR - Allow wraparound
    q->size++;

    // signal that q is not empty
    pthread_cond_signal(&q->not_empty);

    pthread_mutex_unlock(&q->mutex);
}

void *dequeue(queue_t q) {
    if (!q) {
        return NULL;
    }

    pthread_mutex_lock(&q->mutex);

    // if empty, wait for q to not be empty
    while (q->size <= 0 && !q->shutdown) {  // CR - Only wait if shutdown is inactive
        pthread_cond_wait(&q->not_empty, &q->mutex);
    }

    // CR - Disallow dequeues if queue is empty (and shutdown is active)
    if (q->size <= 0) {
        pthread_mutex_unlock(&q->mutex);
        return NULL;
    }

    void *ret = q->b_buffer[q->head];
    q->head = (q->head + 1) % q->capacity;    // CR - Allow wraparound
    q->size--;

    // signal that q is not full
    pthread_cond_signal(&q->not_full);

    pthread_mutex_unlock(&q->mutex);

    return ret;
}

void queue_shutdown(queue_t q) {
    if (!q) {
        return;
    }

    pthread_mutex_lock(&q->mutex);
    q->shutdown = true;

    // CR - Force all waiting threads to complete (lack of which causes deadlock!!!)
    pthread_cond_broadcast(&q->not_full);
    pthread_cond_broadcast(&q->not_empty);

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