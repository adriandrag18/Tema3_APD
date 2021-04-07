#include "mpi.h"
#include <stdlib.h>
#include "string.h"
#include <pthread.h>
#include <ctype.h>
#include "paragraph.h"
#include "paragraph_list.h"

int is_vowel(int ch)
{
    int c = toupper(ch);
    return (c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U');
}

void process_horror(char *line, int len) {
    char line_out[2 * len];
    int j = 0;
    for (int i = 0; line[i]; i++, j++) {
        line_out[j] = line[i];
        if (isalpha(line[i]) && !is_vowel(line[i])) {
            j++;
            line_out[j] = (char) tolower(line[i]);
        }
    }
    line_out[j] = 0;
    strcpy(line, line_out);
}

void process_comedy(char* line) {
    int position = 0;
    for (int i = 0; line[i]; i++, position++) {
        if (position % 2 == 1 && isalpha(line[i]))
            line[i] = (char) toupper(line[i]);
        if (line[i] == ' ')
            position = -1;
    }
}

void process_fantasy(char* line) {
    for (int i = 0; line[i]; i++)
        if (isalpha(line[i]) && (line[i - 1] == ' ' || i == 0))
            line[i] = (char) toupper(line[i]);
}

void process_science_fiction(char* line) {
    int count = 1, start_word = 0, i;
    for (i = 0; line[i]; i++) {
        if (line[i] == ' ' || line[i] == '\n') {
            if (count % 7 == 0) {
                int len_word = i - start_word;
                char aux;
                for (int j = 0; j < (len_word + 1) / 2; j++) {
                    aux = line[start_word + j];
                    line[start_word + j] = line[i - 1 - j];
                    line[i - 1 - j] = aux;
                }
            }
            count++;
            start_word = i + 1;
        }
    }
}

void *thread_worker(void *arg) {
    int id = *(int *) arg;
    if (id == 0) {
        root = NULL;
        PARAGRAPH *paragraph = malloc(sizeof(PARAGRAPH));
        set_paragraph(paragraph, 0, types[rank - 1]);
        while (receive_paragraph(paragraph, MASTER) > 0) {
            add_list(&root, paragraph);
            paragraph = malloc(sizeof(PARAGRAPH));
            set_paragraph(paragraph, 0, types[rank - 1]);
        }
        free(paragraph);
    }
    pthread_barrier_wait(&barrier);
    if (id != 0) {
        for (PARAGRAPH *p = root; p; p = p->next) {
            int start = (id - 1) * 20, end = MIN(id * 20, p->num_lines);
            int end_last_thread = MIN((maxThreads - 1) * 20, p->num_lines);
            while (1) {
                for (int i = start; i < end; i++)
                    switch (rank) {
                        case 1:
                            process_horror(p->lines[i], p->num_chars[i]);
                            p->num_chars[i] = strlen(p->lines[i]);
                            break;
                        case 2:
                            process_comedy(p->lines[i]);
                            break;
                        case 3:
                            process_fantasy(p->lines[i]);
                            break;
                        case 4:
                            process_science_fiction(p->lines[i]);
                            break;
                        default:
                            exit(-1);
                    }
                if (end_last_thread == p->num_lines)
                    break;
                end = MIN(end + (maxThreads - 1) * 20, p->num_lines);
                end_last_thread = MIN(end_last_thread + (maxThreads - 1) * 20, p->num_lines);
                start += (maxThreads - 1) * 20;
            }
        }
    }
    pthread_barrier_wait(&barrier);
    if (id == 0) {
        for (PARAGRAPH *p = root; p; p = p->next)
            send_paragraph(p, MASTER);

        int end = -1;
        MPI_Send(&end, 1, MPI_INT, MASTER, TAG, MPI_COMM_WORLD);
        free_list(root);
    }

    pthread_exit(NULL);
}

void worker() {
    pthread_t threads[maxThreads];
    void *status;
    int arguments[maxThreads], id;

    pthread_mutex_init(&mutex, NULL);
    pthread_barrier_init(&barrier, NULL, maxThreads);

    for (id = 0; id < maxThreads; id++) {
        arguments[id] = id;
        pthread_create(&threads[id], NULL, thread_worker, (void *) &arguments[id]);
    }

    for (id = 0; id < maxThreads; id++)
        pthread_join(threads[id], &status);
}
