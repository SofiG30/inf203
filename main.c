#include <stdio.h>
#include <stdlib.h>
#include "dict.h"
#include "types.h"
#include "transformation.h"
#include "clause.h"

int main(int argc, char *argv[]) {
    
    if (argc < 3) {
        printf("Error, not enough argument\n");
        return 1;
    } 

    FILE *f = fopen(argv[1], "r");
    if (f == NULL) {
        printf("Error when opening the file\n");
        return 1;
    }

    // Variables 
    Liste_D *Grid = init_liste();
    Liste_D *Se = init_liste();
    Liste_C *pos_cl = init_empty_clause();
    Liste_C *neg_cl = init_empty_clause();
   // dictionary *D = init_dict();
   // Liste_Di_2 *pos = init_listeDi_2();
   // Liste_Di_2 *neg = init_listeDi_2();

    // We start by reading the sudoku grid
    Se = read_sudoku(f, Grid);

    /*
    afficher_liste_D(Se);
    printf("\n\n");
    afficher_liste_D(Grid);
    */

    // Create the clause lists 
    // Positive 
    pos_cl = positive_clauses(Grid, Se);    
    //afficher_liste_C(pos_cl);

    // Negative
    //printf("Error\n");
    neg_cl = construct_clause_neg(pos_cl, Se);  
    // Works only when the value is 1
    //afficher_liste_C(pos_cl);

    /*
    // Turn the lists into Di_lists (Only integers represent the cases) + store in dict
     printf("Error\n");
    pos = rewrite_var(pos_cl,D);
     printf("Error\n");
    neg = rewrite_var(neg_cl, D);

    // Write the dimacs file 
    create_dimacs(pos, neg, argv[2]);
    */

    // Free all the lists -> prevent memory problems
    free(Se);
    free(Grid);
    free(pos_cl);
    free(neg_cl);
   // free(pos);
   // free(neg);
    fclose(f);

return 0;
}

