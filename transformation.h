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


// ?? TYPES FOR THE 2ND TYPE OF FORMULA ?? (variables are just integers (+ or -))

typedef struct Cellule_Liste2_ {
    int var;   
	struct Cellule_Liste2_* suiv;
} Cellule2;


typedef struct Liste2_ {
	unsigned int size;       
	Cellule_Liste2_ *first; 
	//Cellule_Liste2_ *last;  
	                       
} Liste2;


// TYPES FOR THE CLAUSE ITSELF

typedef struct Cellule_Liste_Clause2_ {
    Liste2 clause;   
	struct Cellule_Liste_Clause2_* suiv;
} Cellule_Clause2;


typedef struct Liste_Clause2_ {
	unsigned int size;  //nb of clauses in the formula     
	unsigned int nb_var;
	Cellule_Liste_Clause2_ *first; 
	Cellule_Liste_Clause2_ *last;  
	                       
} Liste_Clause2;



void read_sudoku ();

void create_dimacs (Liste_Clause2 LC, char* file_name);

#endif
