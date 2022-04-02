#include "stdio.h"
#include <cstring>
#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <math.h>
#include <stack>
#include <unistd.h>
#include <limits.h>
#include <filesystem>
#include <sstream>

using namespace std;

bool ast_show = 0; // Montre l'arbre de syntaxe abstraite du code (dev only)
bool debug = 0;    // Met des fonctions de debug du code
bool strict = 0;   // Analyse plus poussée du code et de sa syntaxe
bool show_logs = 0;

int compute_norme = 0; // 0 = rpn, 1 = normal
int clear_stack = 0; // 1 supprime souvent le stack, 0 jamais


string mtmc_version_date = "march 2022";
string mtmc_version = "0.1.2";
string mtmc_wdid = "Add of lists, integrations of lists.\nAdded the keyword 'operator' and included the operator definitions\nBugs correction and fixes"; // metmice : what did I do ?

string working_dir;
string current_file_name;

vector<string> operators_n; // Table dédiée aux noms d'operateurs définis
vector<string> operators_f; // Table dédiée aux fonctions des operateurs


#include "./node.h"
#include "./usefull.h"
#include "./types/types.h"
#include "./variables/variables.cpp"

void utilise_file(string name, bool ast_show, bool debug, bool strict, bool including); // Pour pouvoir inclure des bibliothèque
string read_file(const char *name, bool);

stack<Mtmc_variable *> computes; // Premier stack dédié aux calculs
vector<string> display;          // Deuxième stack dédié à l'affichage
vector<int> system_p;         // Troisième stack dédié au système

map<string, Mtmc_variable *> variables_t; // Table pour le stockage des variables

vector<Mtmc_functions *> functions; // Table dédiée aux fonctions

int maximum_recursion_depth; // Le niveau de récursion ne peux pas le dépasser

vector<string> keywords_parser; // Les mots clés du parser (et donc du langage)

void keywords_init()
{
    keywords_parser.push_back("if");
    keywords_parser.push_back("return");
    keywords_parser.push_back("while");
    keywords_parser.push_back("break");
    keywords_parser.push_back("exit");
    keywords_parser.push_back("continue");
    keywords_parser.push_back("push");
    keywords_parser.push_back("include");
    keywords_parser.push_back("type");
    keywords_parser.push_back("else");
    keywords_parser.push_back("global");
    keywords_parser.push_back("for");
    keywords_parser.push_back("length");
    keywords_parser.push_back("pop");
    keywords_parser.push_back("operator");
    keywords_parser.push_back("struct");
}


#include "./parser.h"
#include "./lexer.h"
#include "./paterns.h"
#include "./visitor.h"


using namespace std;
