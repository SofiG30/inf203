#ifndef _CLAUSE_H_
#define _CLAUSE_H_
#include <stdio.h>
#include <stdbool.h>
#include "types.h"

// all aux functions in the creation of the clauses are here
          
Liste_D* extract_list( Liste_C *L,  Liste_D *Se, int i) ;

bool in_list(Liste_D *L, Cellule_D *cell);

Cellule_D* pop_cell(Liste_D *L);

bool is_number_in_lcr (Cellule_D *cell, Liste_D *Grid) ;

bool same_lcr(Cellule_D *c1, Cellule_D *c2) ;




#endif