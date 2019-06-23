#ifndef __FILE_H
#define	__FILE_H

#define FILE_TAILLE 32

typedef struct {
    /** Espace de mémoire pour stocker la file. */
    char file[FILE_TAILLE];

    /** Pointeur d'entrée de la file. */
    unsigned char fileEntree;

    /** Pointeur de sortie de la file. */
    unsigned char fileSortie;

    /** Indique si la file est vide. */
    char fileVide;

    /** Indique si la file est pleine. */
    char filePleine;
} File;

void enqueue(File *file, char c);
char dequeue(File *file);
char queueIsEmpty(File *file);
char queueIsFull(File *file);
void queueReset(File *file);

#ifdef TEST
void test_queue();
#endif

#endif
