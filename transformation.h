#ifndef _TRANSFORMATION_H_
#define _TRANSFORMATION_H_


// TYPES FOR WHAT IS INSIDE THE CLAUSES

typedef struct Cellule_Liste_ {
	//cell = (li, col, el)
	int li;
    int col;
    int el;   
	struct Cellule_Liste_* suiv;
} Cellule;


typedef struct Liste_ {
	unsigned int size;       
	Cellule_Liste_ *first; 
	Cellule_Liste_ *last;  
	                       
} Liste;


// TYPES FOR THE CLAUSE ITSELF

typedef struct Cellule_Liste_Clause_ {
    Liste data;   
	struct Cellule_Liste_Clause_* suiv;
} Cellule_Clause;


typedef struct Liste_Clause_ {
	unsigned int size;       
	Cellule_Liste_Clause_ *first; 
	Cellule_Liste_Clause_ *last;  
	                       
} Liste_Clause;


void lire_sudoku ();

void creer_dimacs (Liste_Clause LC, char* file_name);

#endif