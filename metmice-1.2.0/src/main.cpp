#include "../include/main.h"

using namespace std;

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
		Error("opening", err);
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

	vector<string> code_ref; // Pourquoi pas ensuite récupérer les lignes et colones pour plus de contexte pour les erreures. ... ? [TODO]
	vector<string> lexeme = lex(source.c_str(), code_ref);
	Node *ast_t = new Node();
	ast_t->value = "main";
	ast_t->type = "root";
	Node *p = parser(lexeme, ast_t, 0, lexeme.size());
	ast_t = p;
	if (ast_show)
	{
		cout << "Arbre de syntaxe abstraite du document `main` : " << endl;
		ast_t_rep(ast_t, 0);
		cout << endl;
	}

	// Trie basiquement les expressions. Vérifie la syntaxe si `strict` est true.
	vector<Node *> paterns = recognize_paternes(ast_t, strict);
	free(ast_t); // Plus besoin de cette m*rde
	if (ast_show)
	{
		cout << "Arbre de syntaxe des différentes expressions : " << endl;
		for (int i = 0; i < paterns.size(); ++i)
		{
			ast_t_rep(paterns[i], 0);
			cout << endl;
		}
	}
	visitor(paterns, ast_show, debug, strict);
	working_dir = old_dir;
	current_file_name = old_file_name;
}

vector<string> parse_args(string arg)
{
	vector<string> args;
	string begin;
	for (int i = 0; i < arg.size(); i++)
	{
		if (arg[i] == '=')
		{
			args.push_back(begin);
			begin = "";
		}
		begin += arg[i];
	}
	args.push_back(begin);
	return args;
}

void setup()
{ // initie des variables de base
	Mtmc_variable *v = new_mtmc_variable_from_string("mtmc_version", mtmc_version);
	Mtmc_variable *d = new_mtmc_variable_from_string("mtmc_version_date", mtmc_version_date);

	variable_asignement(v);
	variable_asignement(d);
}

int main(int argc, char *argv[])
{

	ast_show = 0; // Montre l'arbre de syntaxe abstraite du code (dev only)
	debug = 1;	  // Met des fonctions de debug du code
	strict = 1;	  // Analyse plus poussée du code et de sa syntaxe
	maximum_recursion_depth = 50;
	clear_stack = 1;
	keywords_init();
	
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
		else if (string(argv[i]) == "--help" or string(argv[i]) == "-h")
		{
			cout << "Help : " << endl;
			cout << "\t--strict_disabled or -S to disable a strict analyse of the code " << endl;
			cout << "\t--debug_disabled or -d to not debug the code" << endl;
			cout << "\t--ast_show or -A to show the abstract syntax tree" << endl;
			cout << "\t--clear_stack_disabled or -c to desable the compute stack clearing" << endl;
			cout << "\t--help or -h to show this message" << endl;
			printf("Usage : \n\tmetmice filename.mtmc\n");
			exit(0);
		}
		else
		{
			string err = "unknown argument `" + string(argv[i]) + "`, try --help to get the help";
			Error("arguments", err);
		}
	}

	const char *filename = argv[1] ;
	setup();
	utilise_file(filename, ast_show, debug, strict, false);

	return 0;
}
