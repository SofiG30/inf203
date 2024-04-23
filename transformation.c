#include "transformation.h"
#include <stdio.h>
#include <stdlib.h>

Liste* init_liste() {    
    Liste *new_list = malloc(sizeof(Liste));
    new_list->first = NULL;
    new_list->last = NULL;
    new_list->size = 0;
    return new_list;
}

void add_cell(Liste *L, Cellule *c) {

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

// Precondition : le fichier doit etre ouvert en mode lecture
Liste* read_sudoku(FILE *f) {
    Liste *Se = init_liste();
    if ( f == NULL) {
        printf("Erreur dans le pointeur du fichier\n");
    } else {
        // Size of the grid 
        fscanf(f, "%d", &Se->size);

        char val;
        // i = line, j = column
        int i = 1,j =1;
       while (fscanf(f, " %c", &val) != EOF) {
        switch(val) {
                case '?':
                    j++;
                    break;
                case '|':
                case '.':
                    break;
                // New line
                case '\\':
                    j = 1;
                    i++;
                    break;
                default:
                    if ( val >= '1' && val <= '5') {
                        // A value already in the grid 
                        Cellule *new_cell = malloc(sizeof(Cellule));
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
    }
    return Se;

}


void afficher_liste(Liste *L) {

    if (L->first == NULL) {
        printf("Error, the list is empty\n");
        return;
    } else {
        Cellule *curr = L->first;
        while (curr != NULL) {
            printf(" (%d, %d, %d) , ", curr->li, curr->col, curr->el);
            curr = curr->suiv;
        }
    } 
    printf("\n");

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
        if (current_clause->clause.first == NULL) {
            printf("Error, the clause is empty\n");
            fclose(f);
            return;
        } else {
            Cellule2 *current_cell = current_clause->clause.first;
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
