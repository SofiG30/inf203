#include "transformation.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

Liste_D* init_liste() {    
    Liste_D *new_list = malloc(sizeof(Liste_D));
    new_list->first = NULL;
    new_list->last = NULL;
    new_list->length = 0;
    new_list->size_grid = 0;
    return new_list;
}

void add_cell_D(Liste_D *L, Cellule_D *c) {
    if (L->first == NULL) {
        L->first = c;
        L->last = c;
        L->length = 1;
    } else {
        L->last->suiv = c;
        L->last = L->last->suiv;
        L->length = L->length + 1;
    }
}


void add_cell_C(Liste_C *L, Cellule_C *c) {
    if (L == NULL || c == NULL) {
        return;
    }

    if (L->first == NULL) {
        L->first = c;
        L->last = c;
        L->size = 1;
    } else {
        L->last->suiv = c;
        L->last = L->last->suiv;
        L->size = L->size + 1;
    }
}


Liste_C* init_empty_clause() {
    Liste_C *new_list = malloc(sizeof(Liste_C));
    new_list->first = NULL;
    new_list->last = NULL;
    new_list->size = 0;
    return new_list;
}


void add_regions(Liste_C *Grid, Liste_C *Se) {

    if ( Se->first == NULL && Grid->first == NULL) {
        printf("Error, lists are empty\n");
        return;
    }

    Cellule_C *curr_Grid = Grid->first;
    Cellule_C *curr = Se->first;
    while (curr != NULL && curr_Grid != NULL) {
       // printf("col grid %d",curr_Grid->col);
       // printf("line grid %d",curr_Grid->li);

        if (curr_Grid->data.first->col == curr->data.first->col && curr_Grid->data.first->li == curr->data.first->li) {
            //printf("Reg = %d",  curr_Grid->reg);
            curr->data.first->reg = curr_Grid->data.first->reg;
            curr_Grid->data.first->el = curr->data.first->el;
            curr = curr->suiv;
            curr_Grid = curr_Grid->suiv;
        } else {
            // The grid is stored in order of lines (only needed to change the cell of the grid)
            curr_Grid = curr_Grid->suiv;
        }
    }
    while (curr_Grid != NULL) {
        curr_Grid = curr_Grid->suiv;
    }
}

// Precondition : le fichier doit etre ouvert en mode lecture
Liste_C* read_sudoku(FILE *f, Liste_C *Grid) {
    Liste_C *Se = init_empty_clause();
    if (f == NULL) {
        printf("Erreur dans le pointeur du fichier\n");
    } else {
        //printf("Error\n");

        // Size of the grid 
        fscanf(f, "%d", &Se->first->data.size_grid);
        Grid->first->data.size_grid = Se->first->data.size_grid;
        //printf("Error\n");
        // GRID
        char val;
        // i = line, j = column, r = region
        int i = 1,j =1;
        //printf("Error\n");
        while (fscanf(f, " %c", &val) != EOF && val != '#') {
            switch(val) {
                case '?':
                    j++;
                    break;
                // New line
                case '\\':
                    j = 1;
                    i++;
                    break;
                default:
                    if (isdigit(val)) {
                        // A value already in the grid 
                        Liste_D *dis = init_liste(); // Assuming init_liste initializes a Liste_D properly
                        Cellule_D *cell = malloc(sizeof(Cellule_D));
                        cell->col = j;
                        cell->el = atoi(&val);
                        cell->li = i;
                        cell->suiv = NULL; 

                        add_cell_D(dis, cell); // Assuming add_cell_D adds cell to dis properly

                        Cellule_C *cell2 = malloc(sizeof(Cellule_C));
                        cell2->data = *dis; // Assigning properly initialized Liste_D to cell2->data
                        cell2->suiv = NULL;
   
                        add_cell_C(Se, cell2);
                        j++;

                        free(cell);
                        free(cell2);
                    }
                    break;
            }
       }
        //printf("Error\n");
        // REGIONS 
        i = 1; 
        j = 1;
        while (fscanf(f, " %c", &val) != EOF) {
            if (val != '\\') {  
                //printf(" val  = %c" , val);
                Liste_D *dis = init_liste(); // Assuming init_liste initializes a Liste_D properly
                Cellule_D *cell = malloc(sizeof(Cellule_D));
                cell->col = j;
                cell->el = 0;
                cell->li = i;
                cell->suiv = NULL;    
                add_cell_D(dis, cell); // Assuming add_cell_D adds cell to dis prope    
                Cellule_C *cell2 = malloc(sizeof(Cellule_C));
                cell2->data = *dis; // Assigning properly initialized Liste_D to cell2->data
                cell2->suiv = NULL;   
                add_cell_C(Grid, cell2);
                j++;

                free(cell);
                free(cell2);
            } else {
                i++;
                j = 1;
        }
}

        // Add the regions to the cells in the Se list 
        //printf("Error\n");
        add_regions(Grid, Se);
    }
    return Se;

}

 
void afficher_liste(Liste_C *L) {

    if (L->first == NULL) {
        printf("Error, the list is empty\n");
        return;
    } else {
        Cellule_C *curr = L->first;
        while (curr != NULL) {
            if (curr->data.first != NULL) {
                printf("(%d, %d, %d) , ", curr->data.first->li, curr->data.first->col, curr->data.first->el);
            } else {
                printf("NULL"); // Handle case where curr->data.first is NULL
            }
            curr = curr->suiv;
        }
    } 
    printf("\n");

}


/*Function which determines if an element is already in the line, column or region specified, returns 1 if the number in in LCR, 0 else*/
int is_number_in_lcr (int li, int col, int el, int reg, Liste_C *Grid) {
    // create a cell with the possible element
    Cellule_D *new_cell = malloc(sizeof(Cellule_D));
    new_cell->col = col;
    new_cell->li = li;
    new_cell->el = el;
    new_cell->reg = reg;
    new_cell->suiv = NULL;

    for (Cellule_C *curr = Grid->first; curr != NULL; curr = curr->suiv) { // for each element in the Grid
        Cellule_D * cell_to_compare = curr->data.first; 
        if (new_cell == cell_to_compare) { // if both cells are the same then the number is already present in either the line, column or region
            return 1; 
        }
    }
    return 0;
}


/*Function that creates a disjunction for each cell (and returns a Liste_ of disjunctions (ie list of list of cells)*/
Liste_C* construct_clause (Liste_C *L){
    //L is the list of the elements already defined in the grid 
    Liste_C *clause = init_empty_clause();
    for (Cellule_C* curr = L->first; curr != NULL; curr = curr->suiv) { // for each cell in the sudoku
        if (curr->data.first->el == 0) { // if the element is unknown (i.e: el = 0), then we enumerate all the possibilities of numbers in this cell by creating a disjunction
            for (int i = 1; i<=L->first->data.size_grid; i++) { //
                if (is_number_in_lcr(curr->data.first->li, curr->data.first->col, i, curr->data.first->reg, L)) {

                }
            }
        }
    }
    
}


/* Function which takes the clause list as an argument and writes the DIMACS file 
(this list will have already been transformed into x’s with readable indices: x1, x2, x3,….)*/
void create_dimacs (Liste_Clause2 LC, char* file_name) {

    FILE*f=fopen(file_name,"w");
    if (f==NULL){
        printf("erreur lors de l'ouverture du fichier\n");
    }

    //Write the DIMACS file
    fprintf (f,"p cnf %d %d\n",LC.nb_var,LC.size);
    
    //Parcours de tous les éléments 
    if (LC.first == NULL) {
        printf("Error, the list is empty\n");
        fclose(f);
        return;
    } else {
        Cellule_Clause2 *current_clause = LC.first;

        for (int i=1; current_clause != NULL && i<=LC.size; i++){
        //i is the number of the clause we're treating 
        if (current_clause->disjunction.first == NULL) {
            printf("Error, the clause is empty\n");
            fclose(f);
            return;
        } else {
            Cellule2 *current_cell = current_clause->disjunction.first;
            for (int j=1; current_cell != NULL && j <=current_clause->disjunction.size; j++) {
                //j goes from 1 to the size of the clause (nb of variables)
            
                fprintf (f,"%d ",current_cell->var);

                current_cell = current_cell->suiv;
            }
            //we printed a whole clause
            fprintf (f,"0\n");

           current_clause = current_clause->suiv;
            //we go back in the loop but for the next clause until there is no more clauses to be treated
        }
        }   
    }
    //The DIMACS file is written
    fclose(f);

}
