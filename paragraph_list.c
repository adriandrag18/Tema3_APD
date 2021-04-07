#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "paragraph.h"

void add_list(PARAGRAPH **head, PARAGRAPH *paragraph) {
    if (!*head) {
        *head = paragraph;
    } else {
        if ((*head)->order_number > paragraph->order_number) {
            PARAGRAPH *aux = *head;
            *head = paragraph;
            (*head)->next = aux;
        } else {
            int i = 0;
            for (PARAGRAPH *p = *head; p; p = p->next) {
                i++;
                if (!p->next) {
                    p->next = paragraph;
                    break;
                } else if (p->next->order_number > paragraph->order_number) {
                    PARAGRAPH *aux = p->next;
                    p->next = paragraph;
                    p->next->next = aux;
                    break;
                }
            }
        }
    }
}

void free_paragraph(PARAGRAPH *paragraph) {
    for (int i = 0; i < paragraph->num_lines; i++)
        free(paragraph->lines[i]);

    free(paragraph);
}

void free_list(PARAGRAPH *head) {
    if (!head)
        return;
    while (head) {
        PARAGRAPH * aux = head->next;
        free_paragraph(head);
        head = aux;
    }
}

void set_paragraph(PARAGRAPH *paragraph, int order_number, char *type) {
    paragraph->order_number = order_number;
    paragraph->num_lines = 0;
    paragraph->next = NULL;
    strcpy(paragraph->type, type);
}

void print_paragraph(PARAGRAPH *paragraph, FILE *fout) {
    fprintf(fout, "%s", paragraph->type);
    for (int i = 0; i < paragraph->num_lines; i++)
        fprintf(fout, "%s", paragraph->lines[i]);
    if (paragraph->next)
        fprintf(fout, "\n");
}

void print_list(PARAGRAPH *head, FILE *fout) {
    for (PARAGRAPH *p = head; p; p = p->next)
        print_paragraph(p, fout);
}

void add_line(PARAGRAPH *paragraph, char *line) {
    int len = (int) strlen(line);
    paragraph->lines[paragraph->num_lines] = (char *) malloc(len * sizeof(char) + 1);
    strcpy(paragraph->lines[paragraph->num_lines], line);
    paragraph->num_chars[paragraph->num_lines] = len;
    paragraph->num_lines++;
}

void send_paragraph(PARAGRAPH *paragraph, int rank_receiver) {
    MPI_Send(&paragraph->order_number, 1, MPI_INT, rank_receiver, TAG, MPI_COMM_WORLD);
    MPI_Send(&paragraph->num_lines, 1, MPI_INT, rank_receiver, TAG, MPI_COMM_WORLD);
    for (int i = 0; i < paragraph->num_lines; i++) {
        MPI_Send(&paragraph->num_chars[i], 1, MPI_INT, rank_receiver, TAG, MPI_COMM_WORLD);
        MPI_Send(paragraph->lines[i], paragraph->num_chars[i] + 1, MPI_CHAR, rank_receiver, TAG, MPI_COMM_WORLD);
    }
}

int receive_paragraph(PARAGRAPH *paragraph, int rank_sender) {
    MPI_Recv(&paragraph->order_number, 1, MPI_INT, rank_sender, TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    if (paragraph->order_number == -1) {
        return -1;
    }
    MPI_Recv(&paragraph->num_lines, 1, MPI_INT, rank_sender, TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    for (int i = 0; i < paragraph->num_lines; i++) {
        MPI_Recv(&paragraph->num_chars[i], 1, MPI_INT, rank_sender, TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        paragraph->lines[i] = malloc(paragraph->num_chars[i] * 2 + 1);
        MPI_Recv(paragraph->lines[i], paragraph->num_chars[i] + 1, MPI_CHAR, rank_sender, TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    return 1;
}
