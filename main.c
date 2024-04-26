#include <stdio.h>
#include <stdlib.h>
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
    Liste_C *Grid = init_empty_clause();
    //Liste_C *Se = read_sudoku(f, Grid);
    //afficher_liste(Se);
    printf("\n\n");
    afficher_liste(Grid);
    //printf("Length of grid list: %d, Length of Se list: %d\n", Grid->first->data.length, Se->first->data.size_grid);

    //int k = is_number_in_lcr(1, 1, 2, 1, Grid);
    //printf("\n\n%d\n\n", k);

    return 0;

}
