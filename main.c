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
    
    Liste *Se = read_sudoku(f);
    afficher_liste(Se);
    return 0;

}
