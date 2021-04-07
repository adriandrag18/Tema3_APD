#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include <pthread.h>
#include "paragraph.h"
#include "paragraph_list.h"

void *reader(void *arg)
{
    int id = *(int*) arg, end = -1, order_number = 0;
    FILE *fin = fopen(in_file, "r");
    size_t len = MAX_LENGTH_LINE;
    char *line = malloc(MAX_LENGTH_LINE), type[30];

    getline(&line, &len, fin);
    strcpy(type, line);
    PARAGRAPH *paragraph = (PARAGRAPH *) malloc(sizeof(PARAGRAPH));
    set_paragraph(paragraph, order_number, type);

    while (getline(&line, &len, fin) > 0) {
        if (strcmp(line, "\n") == 0) {
            if (strcmp(paragraph->type, types[id]) == 0)
                send_paragraph(paragraph, id + 1);
            free(paragraph);
            order_number++;

            if (getline(&line, &len, fin) > 0) {
                strcpy(type, line);
                paragraph = (PARAGRAPH *) malloc(sizeof(PARAGRAPH));
                set_paragraph(paragraph, order_number, type);
            }
        } else {
            add_line(paragraph, line);
        }
    }
    if (paragraph->order_number == order_number) {
        if (strcmp(paragraph->type, types[id]) == 0)
            send_paragraph(paragraph, id + 1);
        free(paragraph);
    }
    MPI_Send(&end, 1, MPI_INT, id + 1, TAG, MPI_COMM_WORLD);

    fclose(fin);
    free(line);

    pthread_barrier_wait(&barrier);

    if (id == 0)
        root = NULL;

    pthread_barrier_wait(&barrier);

    paragraph = malloc(sizeof(PARAGRAPH));
    set_paragraph(paragraph, 0, types[id]);
    while (receive_paragraph(paragraph, id + 1) > 0) {
        pthread_mutex_lock(&mutex);
        add_list(&root, paragraph);
        pthread_mutex_unlock(&mutex);
        paragraph = malloc(sizeof(PARAGRAPH));
        set_paragraph(paragraph, 0, types[id]);
    }
    free(paragraph);

    pthread_barrier_wait(&barrier);
    if (id == 0) {
        char out_file[100];
        strncpy(out_file, in_file, strlen(in_file) - 3);
        out_file[strlen(in_file) - 3] = 0;
        strcpy(out_file + strlen(in_file) - 3, "out");

        FILE *fout = fopen(out_file, "w");
        print_list(root, fout);
        fclose(fout);
    }

    pthread_exit(NULL);
}

void master() {
    pthread_t threads[THREADS_MASTER];
    void *status;
    int arguments[THREADS_MASTER], id;

    pthread_mutex_init(&mutex, NULL);
    pthread_barrier_init(&barrier, NULL, THREADS_MASTER);

    for (id = 0; id < THREADS_MASTER; id++) {
        arguments[id] = id;
        pthread_create(&threads[id], NULL, reader, (void *) &arguments[id]);
    }

    for (id = 0; id < THREADS_MASTER; id++)
        pthread_join(threads[id], &status);

    pthread_mutex_destroy(&mutex);
    pthread_barrier_destroy(&barrier);
}
