#include "test.h"
#include "file.h"

/**
 * If there is room in the queue, queues one char.
 * @param c The char.
 */
void enqueue(File *file, char c) {
    file->fileVide = 0;
    if (!file->filePleine) {
        file->file[file->fileEntree++] = c;
        if (file->fileEntree >= FILE_TAILLE) {
            file->fileEntree = 0;
        }
        if (file->fileEntree == file->fileSortie) {
            file->filePleine = 255;
        }
    }
}

/**
 * If queue is not empty, returns one char from queue.
 * @return The char removed from queue, or 0 if queue is empty.
 */
char dequeue(File *file) {
    char c;
    file->filePleine = 0;
    if (!file->fileVide) {
        c = file->file[file->fileSortie++];
        if (file->fileSortie >= FILE_TAILLE) {
            file->fileSortie = 0;
        }
        if (file->fileSortie == file->fileEntree) {
            file->fileVide = 255;
        }
        return c;
    }
    return 0;
}

/**
 * Flags if queue is empty.
 */
char queueIsEmpty(File *file) {
    return file->fileVide;
}

/**
 * Flags if queue is full.
 */
char queueIsFull(File *file) {
    return file->filePleine;
}

/**
 * Clears and resets the queue.
 */
void queueReset(File *file) {
    file->fileEntree = 0;
    file->fileSortie = 0;
    file->fileVide = 255;
    file->filePleine = 0;
    
}

#ifdef TEST
void test_can_enqueue_and_dequeue() {
    File file;
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
    File file;
    int n = 0;
    char c = 0;
    
    queueReset(&file);

    for (n = 0; n < FILE_TAILLE * 4; n++) {
        enqueue(&file, c);
        if (assertEquals("FBC001", dequeue(&file), c)) {
            return;
        }
        c++;
    }
}

void test_can_retrieve_chars_even_after_overflow() {
    File file;
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

    assertEquals("FDB003", c, FILE_TAILLE);
}

void test_queue() {
    test_can_enqueue_and_dequeue();
    test_can_enqueue_and_dequeue_a_lot();
    test_can_retrieve_chars_even_after_overflow();
}
#endif
