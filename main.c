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
    Liste_C *Se = read_sudoku(f, Grid);
    afficher_liste(Se);
    printf("\n\n");
    afficher_liste(Grid);
    printf("Length of grid list: %d, Length of Se list: %d\n", Grid->first->data.length, Se->first->data.size_grid);

    // TEST DE LA FONCTION is_number_in_lcr

    Liste_D *dis = init_liste(); // Assuming init_liste initializes a Liste_D properly
    Cellule_D *cell = malloc(sizeof(Cellule_D));
    cell->col = 1;
    cell->el = 1;
    cell->li = 1;
    cell->reg = 1;
    cell->suiv = NULL; 

    add_cell_D(dis, cell); // Assuming add_cell_D adds cell to dis properly

    Cellule_C *cell2 = malloc(sizeof(Cellule_C));
    cell2->data = *dis; // Assigning properly initialized Liste_D to cell2->data
    cell2->suiv = NULL;

    
    add_cell_C(Grid, cell2); // Adding cell2 to Grid

    afficher_liste(Grid); // Displaying the content of Grid

    int k = is_number_in_lcr(1, 1, 2, 1, Grid);
    printf("\n\n%d\n\n", k);
    
    return 0;

}
