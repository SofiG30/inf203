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
                for (Cellule_D *c = curr->data.first; c != NULL; c = c->suiv) {
                    printf("(%d, %d, %d) , ", c->li, c->col, c->el);
                }
            } else {
                printf("NULL"); // Handle case where curr->data.first is NULL
            }
            curr = curr->suiv;
        }
    }
    printf("\n");

}


bool is_number_in_lcr (Cellule_D *cell, Liste_D *Grid) {
    


    // VERIFY THE NUMBER IS NOT PRESENT ON THE SAME LINE
    for (int i = 1; i <= Grid->size_grid; i++) {
        cell->col = i;
        printf("col: %d, li: %d, el: %d\n", i, cell->li, cell->el);

        Cellule_D *curr = Grid->first;
        while (curr != NULL) {
            if (curr->li == cell->li && curr->el == cell->el) {
                return true;
            }
            curr = curr->suiv;
        }
    }

    // VERIFY THE NUMBER IS NOT PRESENT IN THE SAME COLUMN
    for (int i = 1; i <= Grid->size_grid; i++) {
        cell->li = i;
        printf("col: %d, li: %d, el: %d\n", cell->col, cell->li, cell->el);
        
        Cellule_D *curr = Grid->first;
        while (curr != NULL) {
            if (curr->li == cell->col && curr->el == cell->el) {
                return true;
            }
            curr = curr->suiv;
        }
    }

    // VERIFY THE NUMBER IS NOT PRESENT IN THE SAME REGION
    for (int i = 1; i <= Grid->size_grid; i++) {
        cell->reg = i;
        printf("col: %d, li: %d, el: %d\n", cell->col, cell->li, cell->el);
        Cellule_D *curr = Grid->first;
        while (curr != NULL) {
            if (curr->reg == cell->li && curr->el == cell->el) {
                printf("true\n");
                return true;
            }
            curr = curr->suiv;
        }
    }

    return false;
}


/*Function that creates a disjunction for each cell (and returns a Liste_D of clauses (ie list of list of cells)*/
Liste_C* construct_clause (Liste_D *L){
    //L is the list of the elements already defined in the grid
    Liste_C *clause = init_listeClause();
    Liste_D *disjunction = init_liste();
    for (Cellule_D *curr = L->first; curr != NULL; curr = curr->suiv) {
        if (curr->el == 0) { // if the element is unknown (i.e the cell is empty)
            for (int i = 1; i <= L->size_grid; i++) {
                curr->el = i;    
                if (is_number_in_lcr(curr, L) == false) { // if the number is not present on the line, column, or region then add it to the clause
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


//Precondition : the file must be open in lecture mode
void give_solution (FILE *ans_sat, dictionary *D){

    if ( ans_sat == NULL) {
        printf("Erreur dans le pointeur du fichier\n");
    } else {
        
        // Satisfiability
        char[6] is_sat; //it will be SAT or UNSAT (max 5 characters)
        int var;

        fscanf(ans_sat, "%s", is_sat);
        if (is_sat == "SAT"){
            fscanf(ans_sat, " %c", var);
            while (var != 0){
                /*we are only interested in the variables that are positive (i.e which are true)
                they gives which combination of line, col, region and element is true*/
                cell_dict *cell_d = find_relation(D, var);
                
                fscanf(ans_sat, " %c", var);
            }

        } else {
            //the formula given is unsatisfiable

        }
    }
}

