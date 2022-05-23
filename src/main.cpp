#include <map>

#include <string>

using namespace std;
#include "../include/node.h"
#include "../include/variables.hpp"
#include "../include/main.hpp"
#include "../include/usefull.h"
#include "../include/parser.h"
#include "../include/paterns.h"
#include "../include/paterns_debug.h"
#include "../include/types.h"
#include "../include/conversion.h"
#include "../include/types_check.h"
#include "../include/stdout.h"
#include "../include/visitor.h"
#include "../include/code_execution.h"
#include "../include/structures.h"
#include "../include/lexer.h"
#include "../include/lib_standard.hpp"


int compute_norme;
bool ast_show = 1; // Montre l'arbre de syntaxe abstraite du code (dev only)
bool debug = 1;    // Met des fonctions de debug du code
bool strict = 1;   // Analyse plus poussée du code et de sa syntaxe
bool show_logs = 0;
int clear_stack = 1; // 1 supprime souvent le stack, 0 jamais
int maximum_recursion_depth = 50; // Le niveau de récursion ne peux pas le dépasser

int full_error_call_stack = 0;
vector<string> display;          // Deuxième stack dédié à l'affichage
vector<int> system_p;         // Troisième stack dédié au système
vector<string> operators_n; // Table dédiée aux noms d'operateurs définis
vector<string> operators_f; // Table dédiée aux fonctions des operateurs
stack<Mtmc_variable *> computes; // Premier stack dédié aux calculs
map<string, Mtmc_variable *> variables_t; // Table pour le stockage des variables
vector<Mtmc_functions *> functions; // Table dédiée aux fonctions
vector<string> keywords_parser; // Les mots clés du parser (et donc du langage)

string FullStringResult;
string working_dir;
string current_file_name;

string current_reference;
stack<string> threaded_references;

string get_working_dir(string fullname)
{
	assertion(w_OS != 2, "Not implemented for windows");
	bool get = false;
	string dir = "";
	for (int i = fullname.size(); i >= 0; --i)
	{
		if (fullname[i] == '/')
		{
			get = true;
		}
		if (get)
		{
			dir = fullname[i] + dir;
		}
	}
	return dir;
}

string read_file(const char *name, bool including)
{
	ifstream fichier(name);
	if (fichier)
	{
		char lettre;
		string file;
		while (fichier.get(lettre))
		{
			file += lettre;
		}
		return file;
	}
	else
	{
		if (including)
		{
			if (w_OS == 1 || w_OS == 0)
			{
				char username[50];
				getlogin_r(username, 50);
				string s_name = working_dir + string(name);
				return read_file(s_name.c_str(), false); // On veux pouvoir avoir des erreurs !
			}
		}
		string err = "can't open file '" + string(name) + "'";
		string n = string(name) + ":" + "0:0";
		Error("execution", err);
		return "Nope";
	}
}

void utilise_file(string name, bool ast_show, bool debug, bool strict, bool including)
// Including c'est si le fichier viens d'une bibiothèque
{
    string source = read_file(name.c_str(), including);

	string old_dir = working_dir;
	working_dir += get_working_dir(name); // Pour ouvrir des fichiers sans avoir a chaques fois besoin de spécifier le répertoir d'avant

	string old_file_name = current_file_name;
	current_file_name = name; // On garde le nom du fichier courant pour les erreurs

    vector<string> code_ref = vector<string>();
    vector<string> lexeme = lex(source.c_str(), code_ref);
    Node *ast_t = new Node();
    ast_t->value = "main";
    ast_t->type = "root";
    Node *p = parser(lexeme, ast_t, 0, int(lexeme.size()), code_ref);
    ast_t = p;
    if (ast_show)
    {
        string ast_t_string = ast_t_rep(ast_t, 0);
        ast_t_string += "\n";
        display.push_back(ast_t_string);
		cout << ast_t_string << endl;
    }
    // Trie basiquement les expressions. Vérifie la syntaxe si `strict` est true.
    vector<Node *> paterns = recognize_paternes(ast_t, strict);
    free(ast_t); // Plus besoin de cette m*rde
    if (ast_show)
    {
        cout << "Arbre de syntaxe des différentes expressions : " << endl;
        for (int i = 0; i < paterns.size(); ++i)
        {
            string ast_t_string = ast_t_rep(paterns[i], 0);
			ast_t_string += "\n";
			display.push_back(ast_t_string);
			cout << ast_t_string << endl;
        }
    }
    visitor(paterns, ast_show, debug, strict, code_ref);
    working_dir = old_dir;
	current_file_name = old_file_name;
}


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
    keywords_parser.push_back("convert");
}

void initialize()
{
    display.clear();
    system_p.clear();
    operators_n.clear();
    operators_f.clear();
    empty_compute_stack();
    variables_t.clear();
    functions.clear();
    keywords_parser.clear();
    FullStringResult.clear();
}

int main(int argc, char *argv[])
{
	ast_show = 0; // Montre l'arbre de syntaxe abstraite du code (dev only)
	debug = 1;	  // Met des fonctions de debug du code
	strict = 1;	  // Analyse plus poussée du code et de sa syntaxe
	maximum_recursion_depth = 50;
	clear_stack = 0;
	full_error_call_stack = 0;
	keywords_init();
	
	int compilation = 0;

	if (argc < 2)
	{
		printf("Usage : \n\tmetmice filename.mtmc [options]\n");
		exit(1);
	} 
	for (int i = 2; i < argc; ++i)
	{
		if (string(argv[i]) == "--strict_disabled" or string(argv[i]) == "-S")
		{
			strict = 0;
		}
		else if (string(argv[i]) == "--print_full_error" or string(argv[i]) == "-e")
		{
			full_error_call_stack = 1;
		}
		else if (string(argv[i]) == "--debug_disabled" or string(argv[i]) == "-d")
		{
			debug = 0;
		}
		else if (string(argv[i]) == "--ast_show" or string(argv[i]) == "-A")
		{
			ast_show = 1;
		}
		else if (string(argv[i]) == "--clear_stack_desabled" or string(argv[i]) == "-c")
		{
			clear_stack = 0;
		}
		else if (string(argv[i]) == "--compile" or string(argv[i]) == "-C")
		{
			compilation = 1;
		}
		else if (string(argv[i]) == "--help" or string(argv[i]) == "-h")
		{
			cout << "Help : " << endl;
			cout << "\t--strict_disabled or -S to disable a strict analyse of the code " << endl;
			cout << "\t--debug_disabled or -d to not debug the code" << endl;
			cout << "\t--ast_show or -A to show the abstract syntax tree" << endl;
			cout << "\t--clear_stack_disabled or -c to desable the compute stack clearing" << endl;
			cout << "\t--print_full_error or -e to print the full error calling stack" << endl;
			cout << "\t--compile or -C to compile the code in c++ and the into an executable" << endl;
			cout << "\t--help or -h to show this message" << endl;
			printf("Usage : \n\tmetmice filename.mtmc\n");
			exit(0);
		}
		else
		{
			string err = "unknown argument `" + string(argv[i]) + "`, try --help to get the help";
			Error("execution", err);
		}
	}

	const char *filename = argv[1] ;
	if (!compilation)
	{
		try 
		{
			utilise_file(filename, ast_show, debug, strict, false);
		}
		catch (string warn)
		{
			cout << warn << endl;
		}
	}
	else 
	{
		
	}
	return 0;
}
