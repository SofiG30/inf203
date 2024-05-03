#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "types.h"


Liste_D* init_liste() {    
    Liste_D *new_list = malloc(sizeof(Liste_D));
    new_list->first = NULL;
    new_list->last = NULL;
    new_list->size_grid = 0;
    new_list->length = 0;
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


void add_cell_C(Liste_C *L, Cellule_C  *c) {
    if (L == NULL || c == NULL) {
        return;
    }

    if (L->first == NULL) {
        L->first = c;
        L->last = c;

        if (c->data->length != 0) {
            L->size = 1;
        }

    } else {
      
        L->last->suiv = c;
        L->last = L->last->suiv;

        if (c->data->length != 0) {
            L->size =  L->size + 1;
        }
    }

}

Liste_C* init_empty_clause() {
    Liste_C *L = malloc(sizeof(Liste_C));
    L->size = 0;
    L->first = NULL;
    L->last = NULL;
    return L;
}


void afficher_liste_D(Liste_D *L) {

    if (L->first == NULL) {
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


void afficher_liste_C(Liste_C *L) {
    
    if (L->first == NULL) {
        return;
    } else {
        Cellule_C *curr = L->first;
        while (curr != NULL) {
            afficher_liste_D(curr->data);
            curr = curr->suiv;
        }
    }
    printf("\n");

}

void afficher_liste_Di(Liste_Di *L) {
    if (L->first == NULL) {
        return;
    } 
    Cell_Di *curr = L->first;
    while (curr != NULL) {
        printf(" %d", curr->var);
        curr = curr->suiv;
    }
}

void afficher_liste_Di2(Liste_Di_2 *L) {
    if (L->first == NULL) {
        printf("Error, the list is empty\n");
        return;
    }

    printf(" number of clauses = %d\n", L->size);
    printf(" number of variables = %d\n", L->nb_var);
    Cellule_Di_2 *curr = L->first;
    while (curr != NULL) {
        afficher_liste_Di(curr->clause);
        printf("\n");
        curr = curr->suiv;
    }
}

Liste_Di_2* init_listeDi_2() {
    Liste_Di_2 *List = malloc(sizeof(List));
    List->first = NULL;
    List->last = NULL;
    List->nb_var = 0;
    List->size = 0;
    return List;
}

Liste_Di* init_liste_Di() {
    Liste_Di *L = malloc(sizeof(Liste_Di));
    L->first = NULL;
    L->last = NULL;
    L->size = 0;
    return L;
}

void add_cell_Di(Liste_Di *L, int val){

    Cell_Di *new_cell = malloc(sizeof(new_cell));
    new_cell->var = val;
    new_cell->suiv = NULL;

    if (L->first == NULL) {
        L->first = new_cell;
        L->last = new_cell;
        L->size = 1;
    } else  {
        L->last->suiv = new_cell;
        L->last = L->last->suiv;
        L->size = L->size + 1;
    }
}
 
void add_liste(Liste_Di *clause, Liste_Di_2 *L) {

    Cellule_Di_2 *new_cell = malloc(sizeof(Cellule_Di_2));
    new_cell->clause = clause;
    new_cell->suiv = NULL;

    if (L->first == NULL) {
        L->first = new_cell;
        L->last = new_cell;
        L->size = 1;
        L->nb_var = new_cell->clause->size;
    } else {
        L->last->suiv = new_cell;
        L->last = L->last->suiv;
        L->size = L->size + 1;
        L->nb_var =  L->nb_var + new_cell->clause->size;
    }
}


void add_cell_Clause(Liste_C *L, Liste_D *c) {

    Cellule_C *new_cell = malloc(sizeof(Cellule_C));
    new_cell->data = c;
    new_cell->suiv = NULL;


    if (L->first == NULL) {
        L->first = new_cell;
        L->last = new_cell;
        L->size = 1;
    } else {
        L->last->suiv = new_cell;
        L->last = L->last->suiv;
        L->size = L->size + 1;

    }

}    


void free_list(Liste_D *list) {
    Cellule_D *current = list->first;
    while (current != NULL) {
        Cellule_D *temp = current;
        current = current->suiv;
        free(temp); // Free memory for the current cell
    }
    free(list); // Free memory for the list structure
}

