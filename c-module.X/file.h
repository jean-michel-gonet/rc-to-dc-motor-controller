#ifndef __QUEUE_H
#define	__QUEUE_H

#define QUEUE_SIZE 32

typedef struct {
    /** Memory buffer to store the queue. */
    char queue[QUEUE_SIZE];

    /** Points to the first free position in queue to put a new char in. */
    unsigned char queueIn;

    /** Points to the next char to dequeue. */
    unsigned char queueOut;

    /** Flag if queue is empty. */
    char queueEmpty;

    /** Flag if queue is full. */
    char queueFull;
} Queue;

void enqueue(Queue *file, char c);
char dequeue(Queue *file);
char queueIsEmpty(Queue *file);
char queueIsFull(Queue *file);
void queueReset(Queue *file);

#ifdef TEST
void test_queue();
#endif

#endif
