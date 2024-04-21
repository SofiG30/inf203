#include "transformation.h"
#include <stdio.h>

//Fonction qui prend une liste de clauses et le nom d'un fichier cible et qui ecrit le DIMACS file correspondant
/*D'abord transformer les variables -> plus des struct mais des int (1, 2, 3...)*/
void creer_dimacs (Liste_Clause LC, char* file_name) {

    FILE*f=fopen(file_name,"w");
    if (f==NULL){
        printf("erreur lors de l'ouverture du fichier");
    }

    //fprintf ("p cnf %d %d",LC.size);

}
