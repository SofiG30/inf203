#include <stdio.h>
#include "transformation.h"

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

    Liste_D *Se = read_sudoku(f, Grid);
    afficher_liste_D(Se);
    afficher_liste_D(Grid);

    printf("\n\n\n");

    Liste_C *lc2 = construct_clause(Grid);
    afficher_liste_C(lc2);    
    return 0;

}
