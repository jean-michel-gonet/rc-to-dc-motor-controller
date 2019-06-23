#include "test.h"
#include "file.h"

/**
 * If there is room in the queue, queues one char.
 * @param c The char.
 */
void enqueue(Queue *file, char c) {
    file->queueEmpty = 0;
    if (!file->queueFull) {
        file->queue[file->queueIn++] = c;
        if (file->queueIn >= QUEUE_SIZE) {
            file->queueIn = 0;
        }
        if (file->queueIn == file->queueOut) {
            file->queueFull = 255;
        }
    }
}

/**
 * If queue is not empty, returns one char from queue.
 * @return The char removed from queue, or 0 if queue is empty.
 */
char dequeue(Queue *file) {
    char c;
    file->queueFull = 0;
    if (!file->queueEmpty) {
        c = file->queue[file->queueOut++];
        if (file->queueOut >= QUEUE_SIZE) {
            file->queueOut = 0;
        }
        if (file->queueOut == file->queueIn) {
            file->queueEmpty = 255;
        }
        return c;
    }
    return 0;
}

/**
 * Flags if queue is empty.
 */
char queueIsEmpty(Queue *file) {
    return file->queueEmpty;
}

/**
 * Flags if queue is full.
 */
char queueIsFull(Queue *file) {
    return file->queueFull;
}

/**
 * Clears and resets the queue.
 */
void queueReset(Queue *file) {
    file->queueIn = 0;
    file->queueOut = 0;
    file->queueEmpty = 255;
    file->queueFull = 0;
    
}

#ifdef TEST
void test_can_enqueue_and_dequeue() {
    Queue file;
    queueReset(&file);
    
    assertEquals("FIL01", queueIsEmpty(&file), 255);    
    assertEquals("FIL02", dequeue(&file), 0);
    assertEquals("FIL03", dequeue(&file), 0);

    enqueue(&file, 10);
    enqueue(&file, 20);

    assertEquals("FIL04", queueIsEmpty(&file), 0);
    assertEquals("FIL05", dequeue(&file), 10);
    assertEquals("FIL06", dequeue(&file), 20);
    assertEquals("FIL07", queueIsEmpty(&file), 255);
    assertEquals("FIL08", dequeue(&file), 0);
}

void test_can_enqueue_and_dequeue_a_lot() {
    Queue file;
    int n = 0;
    char c = 0;
    
    queueReset(&file);

    for (n = 0; n < QUEUE_SIZE * 4; n++) {
        enqueue(&file, c);
        if (assertEquals("FBC001", dequeue(&file), c)) {
            return;
        }
        c++;
    }
}

void test_can_retrieve_chars_even_after_overflow() {
    Queue file;
    char c = 1;
    
    queueReset(&file);
    while(!queueIsFull(&file)) {
        enqueue(&file, c++);
    }

    assertEquals("FDB001", dequeue(&file), 1);
    assertEquals("FDB002", dequeue(&file), 2);
    
    while(!queueIsEmpty(&file)) {
        c = dequeue(&file);
    }
    enqueue(&file, 1);      // Ces caractères sont ignorés...
    enqueue(&file, 1);      // ... car la file est pleine.

    assertEquals("FDB003", c, QUEUE_SIZE);
}

void test_queue() {
    test_can_enqueue_and_dequeue();
    test_can_enqueue_and_dequeue_a_lot();
    test_can_retrieve_chars_even_after_overflow();
}
#endif
