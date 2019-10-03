
/* Nom du fichier   : main.cpp
   Auteur           : PAZIMNA Solibia
   Date de creation : 31-03-2019
   Objet            : Fonction principal du programme.
*/

#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>

void OperationToDO(std::vector<std::string> detailCmdUser);

int main(int argc, char** argv)
{
    std::string current_exec_name = argv[0]; // Name of the current exec program
       std::string first_arge;
       std::vector<std::string> all_args;
       if (argc > 1) {
         first_arge = argv[1];
         all_args.assign(argv + 1, argv + argc);//Regroupement des parametres de dans un vecteur de string
       }
       OperationToDO(all_args);
    return 0;
}
