#include "clause.h"
#include "types.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>




Liste_D* extract_list( Liste_C *L,  Liste_D *Se, int i) {

    Liste_D *new_list = init_liste();

    Cellule_C *curr_list = L->first;
    while (curr_list != NULL) {
        Cellule_D *curr_cell = curr_list->data->first;
        while (curr_cell != NULL) {
            if (curr_cell->el == i && in_list(Se, curr_cell) == false) {
                add_cell_D(new_list, curr_cell);
            }
            curr_cell = curr_cell->suiv;
        }
        curr_list = curr_list->suiv;
    }
    return new_list;
}


bool in_list(Liste_D *L, Cellule_D *cell) {
    if (L->first == NULL) {
        printf("Error, the list is empty");
        return false;
   } 
   Cellule_D *curr = L->first;
   while (curr != NULL) {
        if (curr->col == cell->col && curr->el == cell->el && curr->li == cell->li && curr->reg == cell->reg) {
            return true;
        }
        curr = curr->suiv;
   }
   return false;
}


// Extract the head of the list
Cellule_D* pop_cell(Liste_D *L) {

    if (L->first == NULL ) {
        printf("Error, the list is empty");
        return NULL;
    }
    Cellule_D *head = L->first;
    L->first = L->first->suiv;
    head->suiv = NULL;    
    return head;
}



bool is_number_in_lcr (Cellule_D *cell, Liste_D *Grid) {

   // printf("Error\n");

    // Line
   // printf("Size grid %d\n", Grid->size_grid);
    for (int i= 1; i <= Grid->size_grid; i++) {
     //   printf("col %d\n", cell->col);
        if (i != cell->col) {
          //     printf("Error\n");
            Cellule_D *curr = Grid->first;
            while (curr != NULL) {
                if (curr->li == cell->li && curr->el == cell->el && curr->col == i) {
            //           printf("Error\n");
                    return true;
                }
                  // printf("Error\n"); (* Infinite loop here for case where the val is = size_grid)
                curr = curr->suiv;
            }
        }

    }


   // printf("Error\n");
    // Column
    for (int i= 1; i <= Grid->size_grid; i++) {
        if (i != cell->li) {
            //printf("li %d\n", cell->li);
            Cellule_D *curr = Grid->first;
            while (curr != NULL) {
                if (curr->col == cell->col && curr->el == cell->el && curr->li == i) {
                    return true;
                }
                curr = curr->suiv;
            }
        }
    }
   //  printf("Error\n");
  // Region
    Cellule_D *curr = Grid->first;
    while (curr != NULL) {
        if (curr->el == cell->el && curr->reg == cell->reg) {
        //    printf(" curr = (%d, %d, %d, %d) , cell = (%d, %d, %d, %d) \n ", curr->li, curr->col, curr->el, curr->reg, cell->li, cell->col, cell->el, cell->reg);
            if (curr->col != cell->col || curr->li != cell->li) {
                return true;
            }
        }
        curr = curr->suiv;
    }
    return false;
    }



bool same_lcr(Cellule_D *c1, Cellule_D *c2) {

    if (c1 == NULL && c2 == NULL) {
        printf("Errror, cells might be empty\n");
        return false;
    }

    if (c1->col == c2->col || c1->li == c2->li || c1->reg == c2->reg) {
        return true;
    } 
    return false;
}

