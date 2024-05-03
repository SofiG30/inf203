#ifndef _TRANSFORMATION_H_
#define _TRANSFORMATION_H_
#include <stdio.h>
#include "types.h"
#include "clause.h"
#include "dict.h"
          




void add_regions(Liste_D *Grid, Liste_D *Se);

// Reads an instance of a sudoku file
Liste_D* read_sudoku (FILE *f, Liste_D *Grid);


// Create the clauses of each case in the grid
Liste_C* positive_clauses (Liste_D *L, Liste_D *Se);

Liste_C* construct_clause_neg (Liste_C *L, Liste_D *Se);

// Create the clauses of each case in the grid
Liste_Di_2* rewrite_var(Liste_C *L, dictionary *D);

void create_dimacs (Liste_Di_2 *Pos_clauses, Liste_Di_2 *Neg_clauses, char* file_name);

//void give_solution (FILE *ans_sat, dictionary *D);

#endif
