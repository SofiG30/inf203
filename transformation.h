#ifndef _TRANSFORMATION_H_
#define _TRANSFORMATION_H_
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "types.h"
          
void add_regions(Liste_D *Grid, Liste_D *Se);

// Reads an instance of a sudoku file
Liste_D* read_sudoku (FILE *f, Liste_D *Grid);

// Function just to verify the list is correctly formed
void afficher_liste_D(Liste_D *L);

void afficher_liste_C(Liste_C *L);

bool is_number_in_lcr (Cellule_D *cell, Liste_D *Grid);

Liste_C* construct_clause(Liste_D* L);
// Create the clauses of each case in the grid
//Liste_C* construct_clause(Liste_D *Se, Liste_D *Grid);

void afficher_liste(Liste_D *L);

void create_dimacs (Liste_Di_2* LC, char* file_name);

Liste_Di_2* rewrite_var(Liste_C *L);

void give_solution (FILE *ans_sat, dictionary *D);


#endif
