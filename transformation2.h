#ifndef _TRANSFORMATION_H_
#define _TRANSFORMATION_H_
#include <stdio.h>
#include <stdbool.h>


// TYPES FOR WHAT IS INSIDE THE CLAUSES

typedef struct Cellule_Liste_ {
	//cell = (li, col, el)
	int li;
    int col;
    int el; 
	int reg ;
	struct Cellule_Liste_* suiv;
} Cellule_D;


typedef struct Liste_ {
	unsigned int size_grid;
	unsigned int length;       
	Cellule_D *first; 
	Cellule_D *last;                         
} Liste_D;




// TYPES FOR THE CLAUSE ITSELF

typedef struct Cellule_Liste_Clause_ {
    Liste_D data;   
	struct Cellule_Liste_Clause_* suiv;
} Cellule_C;


typedef struct Liste_Clause_ {
	unsigned int size;       
	Cellule_C *first; 
	Cellule_C *last;  
	                       
} Liste_C;


// ?? TYPES FOR THE 2ND TYPE OF FORMULA ?? (variables are just integers (+ or -))

typedef struct Cellule_Liste2_ {
    int var;   
	struct Cellule_Liste2_* suiv;
} Cellule2;


typedef struct Liste2_ {
	unsigned int size;       
	Cellule2 *first; 
	//Cellule_Liste2_ *last;  
	                       
} Liste2;

// TYPES FOR THE CLAUSE ITSELF
typedef struct Cellule_Liste_Clause2_ {
    Liste2 disjunction;   
	struct Cellule_Liste_Clause2_* suiv;
} Cellule_Clause2;


typedef struct Liste_Clause2_ {
	unsigned int size;  //nb of clauses in the formula     
	unsigned int nb_var;
	Cellule_Clause2 *first; 
	Cellule_Clause2 *last;  
	                       
} Liste_Clause2;


Liste_D* init_liste();

Liste_C* init_empty_clause();

void add_cell_D(Liste_D *L, Cellule_D *c) ;

void add_cell_C(Liste_C *L, Cellule_C *c);

Liste_D* read_sudoku (FILE *f, Liste_D *Grid);

bool cellule_est_dans_liste(Cellule_D *cellule, Liste_D *liste); 

bool is_number_in_lcr (Cellule_D *cell, Liste_D *Grid);

void afficher_liste_D(Liste_D *L);

void afficher_liste_C(Liste_C *L);

Liste_C* construct_clause (Liste_D *L);

void create_dimacs (Liste_Clause2 LC, char* file_name);

#endif
