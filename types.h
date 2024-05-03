#ifndef _TYPES_H_
#define _TYPES_H_
#include <stdio.h>


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
    Liste_D *data;   
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
} Cell_Di;


typedef struct Liste2_ {
	unsigned int size;       
	Cell_Di *first; 
	Cell_Di *last;        
} Liste_Di;

// TYPES FOR THE CLAUSE ITSELF
typedef struct Cellule_Liste_Clause2_ {
    Liste_Di *clause;   
	struct Cellule_Liste_Clause2_* suiv;
} Cellule_Di_2;


typedef struct Liste_Clause2_ {
	unsigned int size;  //nb of clauses in the formula     
	unsigned int nb_var;
	Cellule_Di_2 *first; 
	Cellule_Di_2 *last;  
	                       
} Liste_Di_2;

// Auxiliary functions
Liste_D* init_liste();

void add_cell_D(Liste_D *L, Cellule_D *c) ;

void add_cell_C(Liste_C *L, Cellule_C*c);

Liste_C* init_empty_clause();

// Function just to verify the list is correctly formed
void afficher_liste_D(Liste_D *L);

void afficher_liste_C(Liste_C *L);

void afficher_liste_Di(Liste_Di *L);

void afficher_liste_Di2(Liste_Di_2 *L);

// Aux function rewrite_var
Liste_Di_2* init_listeDi_2() ;

Liste_Di* init_liste_Di() ;

void add_cell_Di(Liste_Di *L, int val);

void add_liste(Liste_Di *clause, Liste_Di_2 *L);

// Function to test -> Not necessary afterwards
void add_cell_Clause(Liste_C *L, Liste_D *c);

void free_list(Liste_D *list) ;
 
#endif
