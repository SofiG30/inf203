#include "transformation.h"
#include "types.h"
#include <string.h>
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



/*Function that creates a disjunction for each cell (and returns a Liste_D of clauses (ie list of list of cells)*/
Liste_C* positive_clauses (Liste_D *L, Liste_D *Se){
    //L is the list that represents the Grid
    Liste_C *clause = init_empty_clause();

    if (L->first == NULL) {
        printf("Error, the list is empty\n");
        return clause;
    }

    Cellule_D *curr = L->first;
    while (curr != NULL) {
        Liste_D *literal = init_liste();
        if (curr->el == 0) { // if the element is unknown (i.e the cell is empty)
            for (int i = 1; i <= L->size_grid; i++) {
                Cellule_D *tmp = malloc(sizeof(Cellule_D)); // Allocate memory for tmp
                tmp->li = curr->li;
                tmp->col = curr->col;
                tmp->el = i;  
                tmp->reg = curr->reg;
                if (!is_number_in_lcr(tmp,L)) { 
                    // if the number is not present on the line, column, or region then add it to the clause
                    add_cell_D(literal, tmp);
                }
            
            }
        } 
        
        Cellule_C *new_cell = malloc(sizeof(Cellule_C));
        new_cell->data = literal;
        new_cell->suiv = NULL; 
        add_cell_C(clause, new_cell);
        curr = curr->suiv;
   
    }

    Cellule_D *curr_Se;    
    for (curr_Se = Se->first; curr_Se != NULL; curr_Se = curr_Se->suiv) {
    // Allocate memory for a new list and a new cell
    Liste_D *new_list = malloc(sizeof(Liste_D));
    Cellule_C *new_cell = malloc(sizeof(Cellule_C));

    Cellule_D *tmp = malloc(sizeof(Cellule_D)); // Allocate memory for tmp
    tmp->col = curr_Se->col;
    tmp->li = curr_Se->li;
    tmp->reg = curr_Se->reg;
    tmp->el = curr_Se->el;
    add_cell_D(new_list, tmp);   
   
    
    // Initialize the new cell
    new_cell->data = new_list;
    new_cell->suiv = NULL;
    
    // Add the new cell to the clause list
    add_cell_C(clause, new_cell);
}
    return clause;
}



Liste_C* construct_clause_neg(Liste_C *L_pos, Liste_D* Se) {

    // Initialiser la sequence 
    Liste_C* neg_clauses = init_empty_clause();

    if (L_pos->first == NULL) {
        printf("Error, there are no clauses on the list\n");
        return neg_clauses;
    }

    for (int i = 1; i <= Se->size_grid; i++) {
        // We want all the values that contain the cells with element i
        Liste_D *List = extract_list(L_pos, Se, i);
            
        while (List->length >= 2) {
            // Extract the first element of the cell
            Cellule_D* curr = pop_cell(List); // Here List is modified -> doesnt contain curr anymore
            // Compare curr with all cells in List 
            Cellule_D* cell_l = List->first;   
            // We go through the whole list
            while (cell_l != NULL) {
                // Check if the cells share the same l,c,r
                if (same_lcr(curr,cell_l)) {
                    // Add the cells to a new list
                    Liste_D *literal = init_liste();
                    if (literal == NULL) {
                        printf("Memory allocation error\n");
                        free_list(literal);
                        return neg_clauses;
                    }

                    // temporary cell of curr;
                    Cellule_D *tmp_curr = malloc(sizeof(Cellule_D));
                    tmp_curr->li = curr->li;
                    tmp_curr->col = curr->col;
                    tmp_curr->el = curr->el;
                    tmp_curr->reg = curr->reg;
                    tmp_curr->suiv = NULL;
                    add_cell_D(literal, tmp_curr);  

                                  
                    // Temporary cell of cell_l
                    Cellule_D *tmp = malloc(sizeof(Cellule_D));
                    tmp->li =  cell_l->li;
                    tmp->col = cell_l->col;
                   tmp->el = cell_l->el;
                    tmp->reg = cell_l->reg;
                    tmp->suiv = NULL;

                    add_cell_D(literal, tmp); 

                    // Create a cell_c containing the literal list
                    Cellule_C *new_cell = malloc(sizeof(Cellule_C));
                    new_cell->data = literal;
                    new_cell->suiv = NULL;
                    add_cell_C(neg_clauses, new_cell);
                    //free_list(literal);
                }
                cell_l = cell_l->suiv;
            }
        }
        free_list(List);
    }
    return neg_clauses;
}



/*1. Function which takes the clause list as an argument and writes the DIMACS file 
(this list will have already been transformed into x’s with readable indices: x1, x2, x3,….)*/
/*void create_dimacs (Liste_Di_2 *Pos_clauses, Liste_Di_2 *Neg_clauses, char* file_name) {

    FILE*f=fopen(file_name,"w");
    if (f==NULL){
        printf("erreur lors de l'ouverture du fichier\n");
    }

    //Write the DIMACS file
    // The number of var doesnt change
    
    fprintf (f,"p cnf %d %d\n", Pos_clauses->nb_var, (Pos_clauses->size + Neg_clauses->size));
    
    //Parcours de tous les éléments 
    if (Pos_clauses->first == NULL || Neg_clauses->first == NULL) {
        printf("Erreur, la liste est vide\n");
        fclose(f);
        return;
    } else {
         // We start by the pos clauses
        Cellule_Di_2 *current_clause = Pos_clauses->first;

        for (int i=1; current_clause != NULL && i<=Pos_clauses->size; i++){
        //i is the number of the clause we're treating 
        if (current_clause->clause->first == NULL) {
            printf("Error, the clause is empty\n");
            fclose(f);
            return;
        } else {
            Cell_Di *current_cell = current_clause->clause->first;
            for (int j=1; current_cell != NULL && j <=current_clause->clause->size; j++) {
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
        // Negative clauses
        current_clause = Neg_clauses->first;

        for (int i=1; current_clause != NULL && i<=Neg_clauses->size; i++){
        //i is the number of the clause we're treating 
        if (current_clause->clause->first == NULL) {
            printf("Error, the clause is empty\n");
            fclose(f);
            return;
        } else {
            Cell_Di *current_cell = current_clause->clause->size;
            for (int j=1; current_cell != NULL && j <=current_clause->clause->size; j++) {
                //j goes from 1 to the size of the clause (nb of variables)
            
                fprintf (f,"-%d ",current_cell->var);

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
*/



Liste_Di_2* rewrite_var(Liste_C *L, dictionary *D) {

    Liste_Di_2 *new_list = init_listeDi_2();    
    if (L->first == NULL) {
        printf("Error, the list is empty\n");
        return new_list;
    }
    
    int variable = 1;
    Cellule_C *curr = L->first;
    
    while (curr != NULL) {
        
        if (curr->data == NULL) {
            // Is going in here
            printf("Error, the list is empty\n");
            return new_list;
        } 
        Liste_Di *clause = init_liste_Di();
        clause->size = curr->data->length;


        Cellule_D *curr_elem = curr->data->first;
        while (curr_elem !=  NULL) {
            if (curr_elem == NULL) {
                printf("Error, current element is empty\n");
                return new_list;
            }          
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
/*void give_solution (FILE *ans_sat, dictionary *D){

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
                we are only interested in the variables that are positive (i.e which are true)
                they gives which combination of line, col, region and element is true
                cell_dict *cell_d = find_relation(D, var);
                
                fscanf(ans_sat, " %c", var);
            }

        } else {
            //the formula given is unsatisfiable

        }
    }
}
*/



