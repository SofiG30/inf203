#include "transformation.h"
#include <stdio.h>

//Fonction qui prend une liste de clauses et le nom d'un fichier cible et qui ecrit le DIMACS file correspondant
/*D'abord transformer les variables -> plus des struct mais des int (1, 2, 3...)*/

/*1. Function which takes the clause list as an argument and writes the DIMACS file 
(this list will have already been transformed into x’s with readable indices: x1, x2, x3,….)*/
void create_dimacs (Liste_Clause2 LC, char* file_name) {

    FILE*f=fopen(file_name,"w");
    if (f==NULL){
        printf("erreur lors de l'ouverture du fichier");
    }

    //Write the DIMACS file
    fprintf (f,"p cnf %d %d\n",LC.nb_var,LC.size);
    
    //Parcours de tous les éléments 
    Cellule_Clause2 current_clause = LC.first;
    for (int i=1; i<=LC.size; i++){
        //i is the number of the clause we're treating 

        Cellule2 current_cell = current_clause->clause->first;
        for (int j=1; j<=current_clause->clause->size; j++) {
            //j goes from 1 to the size of the clause (nb of variables)
            
            fprintf (f,"%d ",current_cell->var);

            current_cell = current_cell->suiv;
        }
        //we printed a whole clause
        fprintf (f,"0\n");

        current_clause = current_clause->suiv;
        //we go back in the loop but for the next clause until there is no more clauses to be treated

    }
    //The DIMACS file is written
    fclose(f);

}

