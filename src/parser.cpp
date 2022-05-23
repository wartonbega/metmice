
#include <string>
#include <vector>

using namespace std;

#include "../include/node.h"
#include "../include/variables.hpp"
#include "../include/main.hpp"
#include "../include/usefull.h"
#include "../include/parser.h"

const char *get_completion(string token)
{
	if (token == "{")
	{
		return "}";
	}
	else if (token == "(")
	{
		return ")";
	}
	else if (token == "[")
	{
		return "]";
	}
	return "]";
}

bool is_keyword_p(string c)
{
	for (auto i : keywords_parser)
	{
		if (c == i)
		{
			return true;
		}
	}
	return false;
}

string tokenize(string token)
{
	if (token == ";")
	{
		return "end";
	}
	else if (is_keyword_p(token))
	{
		return "keyword";
	}
	else if (token == "~" || token == "+" || token == "-" || token == "*" || token == "^" || token == "/" || token == "%" || token == "or" || token == "and" || token == "//")
	{
		return "expr";
	}
	else if (token == "->")
	{
		return "asignement";
	} /*else if (token[0] == '!') {
		return "funcall";
	}*/
	else
		return "expr";
}

Node *parser(vector<string> lexeme, Node *ast_t, int min, int max, vector<string> code_ref)
{
	/*
	Le vecteur "lexeme" est la liste de lexeme que compose le code
	Le node pointer ast_t est la souche de l'arbre de syntaxe abstraite
	les ints min et max sont les bornes dans lequelles regarder les lexemes
	*/
	assertion(lexeme.size() >= max, "le max doit être plus petit que la liste");
	int index = min;
	bool lam = false;
	Node *lam_node = new Node();
	lam_node->ref = working_dir + "" + current_file_name + ":" + code_ref[index];
	Node *last_node = new Node();
	last_node->value = "undefined";
	last_node->ref = code_ref[index];
	while (index < max)
	{
		const char *token = lexeme[index].c_str();

		// if the token is an opening bracket or parentethis
		if (lexeme[index] == "{" or lexeme[index] == "(" or lexeme[index] == "[")
		{
			int index2 = 0;
			int b = 0;
			const char *comp = get_completion(lexeme[index]);

			// Find the closing bracket
			while (index2 < int(lexeme.size()))
			{
				if (lexeme[index + index2] == lexeme[index])
				{
					b++;
				}
				else if (lexeme[index + index2] == string(comp))
				{
					b--;
					if (b == 0)
					{
						break;
					}
				}
				else if (index + index2 == lexeme.size())
				{
					string err = "uncompleted token '" + lexeme[index] + "'";
					Error("parser", err.c_str());
				}
				index2++;
			}
			// Generates the name
			string s = "bracket?"; // Valeur de défaut pour le débug
			if (lexeme[index] == "{")
			{
				s = "bracket{}";
			}
			else if (lexeme[index] == "[")
			{
				s = "bracket[]";
			}
			else
			{
				s = "bracket()";
			}

			// Setting the value in the new branch
			Node *nw = new Node();
			nw = parser(lexeme, nw, index + 1, index + index2, code_ref);
			nw->value = s;
			nw->type = "bracket";
			nw->ref = working_dir + "" + current_file_name + ":" + code_ref[index];
			// Push the branche in the ast
			if (last_node->value[0] == '!') // Si c'est un appel de fonction
			{
				push_child(last_node, nw);
				last_node->type = "funcall";
				lam = true;
			}
			else if (s == "bracket[]" and last_node->value != "bracket()")
			{
				push_child(last_node, nw);
				last_node->type = "expr";
				if (!lam)
				{
					lam_node = new Node();
					lam = true;
					lam_node->value = "*";
					lam_node->type = "*";
					lam_node->ref = working_dir + "" + current_file_name + ":" + code_ref[index];
					
				}
				Node *nf = new Node();
				nf->value = "";
				for (int j = index; j < index2 + index + 1; j++)
				{
					nf->value += lexeme[j];
				}
				nf->children = nw->children;
				nf->type = "array";
				nf->ref = working_dir + "" + current_file_name + ":" + code_ref[index];
				push_child(lam_node, nf);
				last_node = nf;
			}
			else
			{
				if (lam)
				{
					push_child(ast_t, lam_node);
					lam = false;
				}
				push_child(ast_t, nw);
			}
			last_node = nw;
			index += index2;
		}
		else if (tokenize(token) != "expr")
		{
			if (lam)
			{
				push_child(ast_t, lam_node);
				lam = false;
			}
			Node *ni = new Node();
			ni->value = token;
			ni->type = "keyword";
			ni->ref = working_dir + "" + current_file_name + ":" + code_ref[index];
			push_child(ast_t, ni);
			last_node = ni;
		}
		else
		{
			if (!lam)
			{
				lam_node = new Node();
				lam = true;
				lam_node->value = "*";
				lam_node->type = "*";
				lam_node->ref = working_dir + "" + current_file_name + ":" + code_ref[index];
			}
			Node *nf = new Node();
			nf->value = token;
			nf->type = tokenize(token).c_str();
			push_child(lam_node, nf);
			nf->ref = working_dir + "" + current_file_name + ":" + code_ref[index];
			last_node = nf;
		}
		index++;
	}
	if (lam)
	{
		push_child(ast_t, lam_node);
	}
	return ast_t;
}
