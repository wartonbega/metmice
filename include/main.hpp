#include "stdio.h"
#include <cstring>
#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <math.h>
#include <stack>
#include <unistd.h>
#include <limits.h>
#include <filesystem>
#include <sstream>
#include "stdlib.h"
#include <ctime>


using namespace std;
#ifndef MAIN_HPP
    #define MAIN_HPP
    extern bool ast_show; // Montre l'arbre de syntaxe abstraite du code (dev only)
    extern bool debug;    // Met des fonctions de debug du code
    extern bool strict;   // Analyse plus poussée du code et de sa syntaxe
    extern bool show_logs;
    extern int clear_stack; // 1 supprime souvent le stack, 0 jamais
    extern int maximum_recursion_depth ; // Le niveau de récursion ne peux pas le dépasser
    extern int compute_norme;
    extern int full_error_call_stack;
    
    extern vector<string> display;          // Deuxième stack dédié à l'affichage
    extern vector<int> system_p;         // Troisième stack dédié au système
    extern vector<string> operators_n; // Table dédiée aux noms d'operateurs définis
    extern vector<string> operators_f; // Table dédiée aux fonctions des operateurs
    extern stack<Mtmc_variable *> computes; // Premier stack dédié aux calculs
    extern map<string, Mtmc_variable *> variables_t; // Table pour le stockage des variables
    extern vector<Mtmc_functions *> functions; // Table dédiée aux fonctions
    extern vector<string> keywords_parser; // Les mots clés du parser (et donc du langage)

    extern string working_dir;
    extern string current_file_name;

    extern string FullStringResult;
    extern string current_reference;
    extern stack<string> threaded_references;
    #ifdef __unix__

    #define w_OS 1

    #elif __MACH__

    #define w_OS 0

    #elif _WIN64

    #define w_OS 2

    #endif

    
#endif

void utilise_file(string name, bool ast_show, bool debug, bool strict, bool including);
string read_file(const char *name, bool including);