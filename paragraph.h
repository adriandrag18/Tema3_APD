//
// Created by adrian on 19.01.2021.
//

#ifndef UNTITLED_PARAGRAPH_H
#define UNTITLED_PARAGRAPH_H

#define MASTER 0
#define TAG 0
#define MAX_NUM_LINES 5000
#define MAX_LENGTH_LINE 5000
#define THREADS_MASTER 4
#define MIN(a, b) (((a) > (b)) ? (b) : (a))

typedef struct p{
    char type[20];
    int order_number;  // al catelea paragraf a fost citit in fisierul initial
    int num_lines;
    char *lines[MAX_NUM_LINES];
    int num_chars[MAX_NUM_LINES];  // numarul de caractere din fiecare linie
    struct p *next;
} PARAGRAPH;

extern int maxThreads, rank;
extern PARAGRAPH *root;
extern char in_file[100];
extern char *types[4];
extern pthread_mutex_t mutex;
extern pthread_barrier_t barrier;

#endif //UNTITLED_PARAGRAPH_H
