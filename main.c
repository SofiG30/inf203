#include <stdio.h>
#include <stdlib.h>
#include "dict.h"
#include "types.h"
#include "transformation.h"

//only 1 argument: sudoku_file.txt 
int main(int argc, char *argv[]) {
    
    if (argc < 2) {
        printf("Error, not enough argument\n");
        return 1;
    } 

    FILE *f = fopen(argv[1], "r");
    if (f == NULL) {
        printf("Error while opening the file\n");
        return 1;
    }

    Liste_D *Grid = init_liste();
    if ( f == NULL) {
        printf("Erreur dans le pointeur du fichier\n");
    }
    
    Liste_D *Se = read_sudoku(f, Grid);

    /*Function that creates a disjunction for each cell (and returns a Liste_D of clauses (ie list of list of cells)*/
    Liste_C *clause = construct_clause (Se);

    afficher_liste_C(clause);

    /* Test for rewrite_var function
 
    Liste_C *L = init_listeClause();
    for (int j = 0 ; j < 3 ; j++) {
        // printf("Error here\n");
        
        Liste_D *clause = init_liste(); 

        for (int i = 0; i < 3; i++) {
            Cellule_D *new_cell = malloc(sizeof(Cellule_D));
            printf("Add values: \n");
            scanf("%d" , &new_cell->li);
            scanf("%d" , &new_cell->col);
            scanf("%d" , &new_cell->el);
            scanf("%d" , &new_cell->reg);
            new_cell->suiv = NULL;
            add_cell(clause, new_cell);
        }
        add_cell_Clause(L, clause);
    }
Liste_Di_2 *final_list = rewrite_var(L);

create_dimacs(final_list, argv[1]); */

    return 0;

}
