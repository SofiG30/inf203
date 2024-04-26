#include "transformation.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

Liste_D* init_liste() {    
    Liste_D *new_list = malloc(sizeof(Liste_D));
    new_list->first = NULL;
    new_list->last = NULL;
    new_list->length = 0;
    return new_list;
}


Liste_C* init_empty_clause() {
    Liste_C *new_list = malloc(sizeof(Liste_C));
    new_list->first = NULL;
    new_list->last = NULL;
    new_list->size = 0;
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


void add_regions(Liste_D *Grid, Liste_D *Se) {

    if ( Se->first == NULL && Grid->first == NULL) {
        printf("Error, a list is empty\n");
        return;
    }

    Cellule_D *curr_Grid = Grid->first;
    Cellule_D *curr = Se->first;
    while (curr != NULL && curr_Grid != NULL) {
       // printf("col grid %d",curr_Grid->col);
       // printf("line grid %d",curr_Grid->li);

        if (curr_Grid->col == curr->col && curr_Grid->li == curr->li) {
            //printf("Reg = %d",  curr_Grid->reg);
            curr->reg = curr_Grid->reg;
            curr_Grid->el = curr->el;
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
Liste_D* read_sudoku(FILE *f, Liste_D *Grid) {
    Liste_D *Se = init_liste();
    if ( f == NULL) {
        printf("Erreur dans le pointeur du fichier\n");
    } else {
        //printf("Error\n");

        // Size of the grid 
        fscanf(f, "%d", &Se->size_grid);
        Grid->size_grid = Se->size_grid;
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
                        Cellule_D *new_cell = malloc(sizeof(Cellule_D));
                        new_cell->col = j;
                        new_cell->li = i;
                        new_cell->el = atoi(&val);
                        new_cell->suiv = NULL;
                        add_cell_D(Se, new_cell);
                        j++;
                    }
                    break;
            }
       }
        //printf("Error\n");
        // Regions 
       i = 1; 
       j = 1;
        while (fscanf(f, " %c", &val) != EOF) {
            if (val != '\\') {  
            //printf(" val  = %c" , val);
            Cellule_D *new_cell = malloc(sizeof(Cellule_D));
            new_cell->col = j;
           // printf(" line = %d, col = %d", i, j);
            new_cell->li = i;
            new_cell->reg = atoi(&val);
            //printf(" reg = %d\n", new_cell->reg);
            new_cell->el = 0;
            new_cell->suiv = NULL;
            add_cell_D(Grid, new_cell);
            j++;
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

 
void afficher_liste_D(Liste_D *L) {

    if (L->first == NULL) {
        printf("Error, the list is empty\n");
        return;
    } else {
        Cellule_D *curr = L->first;
        while (curr != NULL) {
            printf(" (%d, %d, %d) , ", curr->li, curr->col, curr->el);
            curr = curr->suiv;
        }
    } 
    printf("\n");

}


void afficher_liste_C(Liste_C *L) {

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


bool cellule_est_dans_liste(Cellule_D *cellule, Liste_D *liste) {
    if (cellule == NULL || liste == NULL) {
        return false;
    }

    Cellule_D *courant = liste->first;
    while (courant != NULL) {
        if (courant == cellule) {
            return true;
        }
        courant = courant->suiv;
    }

    return false;
}


bool is_number_in_lcr (Cellule_D *cell, Liste_D *Grid) {
    // VERIFY THE NUMBER IS NOT PRESENT ON THE SAME LINE
    for (int i = 1; i < Grid->size_grid; i++) {
        cell->col = i;
        if (cellule_est_dans_liste(cell, Grid)) {
            return true;
        }
    }

    // VERIFY THE NUMBER IS NOT PRESENT IN THE SAME COLUMN
    for (int i = 1; i < Grid->size_grid; i++) {
        cell->li = i;
        if (cellule_est_dans_liste(cell, Grid)) {
            return true;
        }
    }

    // VERIFY THE NUMBER IS NOT PRESENT IN THE SAME REGION
    for (int i = 1; i < Grid->size_grid; i++) {
        cell->reg = i;
        if (cellule_est_dans_liste(cell, Grid)) {
            return true;
        }
    }

    return false;
}


/*Function that creates a disjunction for each cell (and returns a Liste_D of clauses (ie list of list of cells)*/
Liste_C* construct_clause (Liste_D *L){
    //L is the list of the elements already defined in the grid
    Liste_C *clause = init_empty_clause();
    Liste_D *disjunction = init_liste();
    for (Cellule_D *curr = L->first; curr != NULL; curr = curr->suiv) {
        if (curr->el == 0) { // if the element is unknown
            for (int i = 1; i < L->size_grid; i++) {
                if (is_number_in_lcr(curr, L) == false) { // if the number is not present on the line, column, or region then add it to the clause
                    curr->el = i;
                    add_cell_D(disjunction, curr);
                    Cellule_C *cell_clause = malloc(sizeof(Cellule_C));
                    cell_clause->data = *disjunction;
                    cell_clause->suiv = NULL;
                    add_cell_C(clause, cell_clause);
                }
            }
        }
    }
    return clause;
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
