#include "transformation.h"
#include "types.h"
#include "dict.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>



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
                        add_cell(Se, new_cell);
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
            add_cell(Grid, new_cell);
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


void afficher_liste(Liste_D *L) {

    if (L->first == NULL) {
        printf("Error, the list is empty\n");
        return;
    } else {
        Cellule_D *curr = L->first;
        while (curr != NULL) {
            printf(" (%d, %d, %d, %d) , ", curr->li, curr->col, curr->el, curr->reg);
            curr = curr->suiv;
        }
    } 
    printf("\n");

}





/*1. Function which takes the clause list as an argument and writes the DIMACS file 
(this list will have already been transformed into x’s with readable indices: x1, x2, x3,….)*/
void create_dimacs (Liste_Di_2 *LC, char* file_name) {

    FILE*f=fopen(file_name,"w");
    if (f==NULL){
        printf("erreur lors de l'ouverture du fichier\n");
    }

    //Write the DIMACS file
    fprintf (f,"p cnf %d %d\n",LC->nb_var, LC->size);
    
    //Parcours de tous les éléments 
    if (LC->first == NULL) {
        printf("Erreur, la liste est vide\n");
        fclose(f);
        return;
    } else {
        Cellule_Di_2 *current_clause = LC->first;

        for (int i=1; current_clause != NULL && i<=LC->size; i++){
        //i is the number of the clause we're treating 
        if (current_clause->clause.first == NULL) {
            printf("Error, the clause is empty\n");
            fclose(f);
            return;
        } else {
            Cell_Di *current_cell = current_clause->clause.first;
            for (int j=1; current_cell != NULL && j <=current_clause->clause.size; j++) {
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



Liste_Di_2* rewrite_var(Liste_C *L) {

    Liste_Di_2 *new_list = init_listeDi_2();
    dictionary *D = init_dict();
    

    if (L->first == NULL) {
        printf("Error, the list is empty\n");
        return new_list;
    }

    int variable = 1;
    Cellule_C *curr = L->first;
    while (curr != NULL) {
        Liste_Di *clause = init_liste_Di();
        if (curr->data->first == NULL) {
            printf("Error, the list is empty\n");
            return new_list;
        } 
        clause->size = curr->data->length;
        Cellule_D *curr_elem = curr->data->first;
        while (curr_elem !=  NULL) {
            /* Error here / Logic of searching in the dict is not correct*/
            // Check the var is not already in dict
            int curr_key = get_key(D, curr_elem); 
            if (curr_key != 0) {
                // The value is already in the dict
                add_cell_Di(clause, curr_key);
            } else {
                add_cell_Di(clause, variable);
                add_key(D,variable, curr_elem);
                variable++;           
            }

            curr_elem = curr_elem->suiv;
        }
        // Add the clause to the list of clauses
        add_liste( clause, new_list);
        curr = curr->suiv;
    }
    return new_list;
}



