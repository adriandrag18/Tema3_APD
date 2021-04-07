//
// Created by adrian on 19.01.2021.
//

#ifndef UNTITLED_PARAGRAPH_LIST_H
#define UNTITLED_PARAGRAPH_LIST_H

#include <stdio.h>

extern void add_list(PARAGRAPH **head, PARAGRAPH *paragraph);
extern void free_paragraph(PARAGRAPH *paragraph);
extern void free_list(PARAGRAPH *head);
extern void set_paragraph(PARAGRAPH *paragraph, int order_number, char *type);
extern void print_paragraph(PARAGRAPH *paragraph, FILE *fout);
extern void print_list(PARAGRAPH *head, FILE *fout) ;
extern void add_line(PARAGRAPH *paragraph, char *line);
extern void send_paragraph(PARAGRAPH *paragraph, int rank_receiver);
extern int receive_paragraph(PARAGRAPH *paragraph, int rank_sender);

#endif //UNTITLED_PARAGRAPH_LIST_H
