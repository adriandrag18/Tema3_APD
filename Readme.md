Draghicescu Adrian-Ioan 331CB

                                             TEMA 3 APD

    Nu mi se puncteaza citirea in paralel, dar in cod citesc in paralel, fiecare thread deschide fisierul dat.

Codul este facut dupa standarul checker ului. Adica:
    - la paragrafele de tip science-fiction inversez ficare al 7 lea cuvant de pe fiecare linie
    - inversarea cuvintelor se dace cu tot cu semne de punctuatie
    - despartirea cuvintelor se face doar dupa ' '(spatiu)

    Folosesc urmatoarea structura  pentru a stoca paragrafele atat dupa citire cat si dupa ce sunt trimise si primite
dint-un nod in altul:

typedef struct p{
    char type[20];
    int order_number;  // al catelea paragraf a fost citit in fisierul initial
    int num_lines;
    char *lines[MAX_NUM_LINES];
    int num_chars[MAX_NUM_LINES];  // numarul de caractere din fiecare linie
    struct p *next;
} PARAGRAPH;

    In fisierul paragraph.h este definita structura.
    In fisierul paragraph_list.c sunt definite toate functiile care au legatura cu paragrafele, cu initializarea lor,
adugarea lor in lista, cu adugarea unei lini, cu afisarea lor si cu eliberarea memoriei.
    In fisierele worker.c si master.c sunt functiile worker(), respectiv mater().

    Acesata structura functioneaza ca o lista pentru a putea sa le ordonez usor cand primesc paragrafele inapoi in
master.

    In functia master() citesc tot textul si pe masura ce se completeaza un paragraf ac tip cu tread ul se trimite in
procesul worker corespunzator si trimite un -1 dupa ce s au terminat paragrafele.
    Dupa fiecare thread primeste inapoi paragrafele procesate de la procescul corescunzator si le pune in aceasi lista
se foloseste un mutex cand se apeleaza functia de adaugare in lista. Dupa ce fiecare thread primieste -1, thread ul 0
printeaza rezultatul.

    In worker() thread ul 0 primeste datele, dupa care restul tread-urilor proceseaza ficare paragraf in paralel (20 de
linii pe rand). Dupa ce s-au terminat toate datele de procesat thread-ul 0 le timite inapoi in procesul master (fiecare
proces worker comuinica cu un singur thread din master).

