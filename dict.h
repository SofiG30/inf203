// Type that will store the relations between the variables and the cases of the sudoku grid 
// Type like dictionary
#ifndef _DICT_H_
#define _DICT_H_
#include <stdio.h>
#include "transformation.h"


typedef struct cell_dict {
    int key; // Variable
    Cellule_D *c; 
    struct cell_dict *next;   
} cell_dict;

typedef struct dictionary {
    cell_dict *head;
    cell_dict *tail;
    int nb_keys;
} dictionary;


dictionary* init_dict();

void detruire_dict(dictionary *D);

// Functions that use the dict type
cell_dict* find_relation(dictionary *D, int key);

int get_key(dictionary *D, Cellule_D *c) 

Cellule_D* get_value(dictionary *D, int key);

void add_key(dictionary *D, int key, Cellule_D *Value);

#endif
