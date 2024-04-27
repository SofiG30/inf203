// Type that will store the relations between the variables and the cases of the sudoku grid 
// Type like dictionary
#ifndef _DICT_H_
#define _DICT_H_
#include <stdio.h>
#include "transformation.h"


dictionary* init_dict();

void detruire_dict(dictionary *D);

// Functions that use the dict type
cell_dict* find_relation(dictionary *D, int key);

Cellule_D* get_value(dictionary *D, int key);

int get_key(dictionary *D, Cellule_D *c);

void add_key(dictionary *D, int key, Cellule_D *Value);

#endif
