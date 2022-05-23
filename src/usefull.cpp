
#include <string>
#include <iostream>
#include <map>
#include <vector>
#include "../include/node.h"
#include "../include/variables.hpp"
#include "../include/main.hpp"
#include "../include/usefull.h"

void assertion(bool expr, string error_s)
{
	if (!expr)
	{
		cout << error_s << endl;
		exit(1);
	}
}

int make_floor(int n, int min)
{
	if (n < min)
	{
		return min;
	}
	return n;
}

int make_max(int n, int max)
{
	if (n > max)
	{
		return max;
	}
	return n;
}

int get_line_of_error_reference(string reference)
{
	int index = 0;
	while (reference[index] != ':')
	{
		index++;
	}
	string line;
	index += 1;
	while (reference[index] != ':')
	{
		line += reference[index];
		index++;
	}
	int ret = stoi(line);
	return ret;
}

string get_filename_of_error_reference(string reference)
{
	string filename;
	int index = 0;
	while (reference[index] != ':')
	{
		filename += reference[index];
		index++;
	}
	return filename;
}

string give_file_error(string filename, int line)
{
	string file_content = read_file(filename.c_str(), false) + "\n";
	vector<string> lines;
	string ret;
	int i = 0;
	while (i < file_content.size())
	{
		if (file_content[i] == '\n')
		{
			lines.push_back(ret);
			ret = "";
		}
		else
		{
			ret += file_content[i];
		}
		i++;
	}

	int min_line = make_floor(line - 3, 0);
	int max_line = make_max(line + 3, lines.size());
	string rend = TERMINAL_GREEN + filename + "\n";

	rend += TERMINAL_BOLDWHITE;
	for (int i = min_line; i < max_line; i++)
	{
		string patent = to_string(i + 1) + " | " + lines[i];
		if (i + 1 == line)
		{
			string head = "> " + patent + TERMINAL_BOLDWHITE;
			patent = TERMINAL_BOLDRED + head;
		}
		rend += patent + "\n";
	}
	rend += TERMINAL_RESET;
	return rend;
}

void Error(string emplacement, string err)
{
	if (emplacement == "execution")
	{
		emplacement = current_reference;
	}
	string Error_s = TERMINAL_BOLDRED + "\n[" + emplacement + "]: " + TERMINAL_RESET+ err;
	
	int line;
	string filename;
	string full_error;

	try 
	{
		line = get_line_of_error_reference(emplacement);			// on regarde le nom du fichier
		filename = get_filename_of_error_reference(emplacement); // on regarde la ligne ou l'erreur est
		full_error = give_file_error(filename, line); // on se donne les lignes concernées uniquement (celles indiqu)
	}
	catch (std::invalid_argument)
	{
		cout << "Caught exception, unknown reference format '" << emplacement << "'. This is an internal error." << endl;
	}

	Error_s += "\n" + full_error;

	if (full_error_call_stack)
	{ // avoir tout l'appel
		string call_threaded;
		Error_s += "\n";
		while (!threaded_references.empty())
		{
			string ref = threaded_references.top();
			threaded_references.pop();
			call_threaded += TERMINAL_BOLDYELLOW + "referenced from : " + TERMINAL_RESET + ref + " :\n" + give_file_error(get_filename_of_error_reference(ref), get_line_of_error_reference(ref));
		}
		Error_s += call_threaded;
	}

	throw Error_s;
}

void debug_error(string emplacement, string err)
{
	if (debug)
	{
		string err_mess2 = "\n[mahtematica/:" + emplacement + "]: " + err;
		cout << err_mess2 << endl;
		cout << "Error message printed because debug was enabled" << endl;
		exit(1);
	}
}

string get_from(vector<string> jsp, int index1, int index2)
{
	string result;
	for (int i = index1; i < index2; ++i)
	{
		if ((i + 1) < index2)
		{
			result += jsp[i] + " ";
		}
		else
			result += jsp[i];
	}
	return result;
}

vector<Node *> get_from_vector(vector<Node *> jsp, int index1, int index2)
{
	vector<Node *> result;
	for (int i = index1; i < index2; ++i)
	{
		result.push_back(jsp[i]);
	}
	return result;
}

string get_from_string(string jsp, int index1, int index2)
{
	string result;
	if (index2 > jsp.size())
	{
		Error("execution", "the second index can't be greater than the string length");
	}
	for (int i = index1; i < index2; ++i)
	{
		result += jsp[i];
	}
	return result;
}

Node *get_from_node(Node *trunc, int index1, int index2)
{
	Node *ne = new Node();
	ne->value = trunc->value;
	ne->type = trunc->type;
	ne->children = get_from_vector(trunc->children, index1, index2);
	return ne;
}

bool in(char one, const char *two)
{
	for (int i = 0; i < int(strlen(two)); ++i)
	{
		if (*(two + i) == one)
		{
			return true;
		}
	}
	return false;
}

bool in_vector(string one, vector<string> sample)
{
	/* Regarde si le string est dans le vector
	 */
	for (int i = 0; i < sample.size(); ++i)
	{
		if (one == sample[i])
		{
			return true;
		}
	}
	return false;
}

bool consti(string word, string chars)
{
	/* Prend en compte un mot et une liste de caractères.
	Si le mot est uniquement composé des caractères qui sont dans la liste, renvoie true.
	Sinon false
	*/
	int i = 0;
	while (i < word.size())
	{
		if (!in(word[i], chars.c_str()))
		{
			return false;
		}
		++i;
	}
	return true;
}

string concatenate(vector<string> v)
{
	string word;
	for (int i = 0; i < v.size(); i++)
	{
		word += v[i] + " ";
	}
	return word;
}

string concatenate_without_blank(vector<string> v)
{
	string word;
	for (int i = 0; i < v.size(); i++)
	{
		word += v[i];
	}
	return word;
}

int number_of(char c, string word)
{
	/* Retourne le nombre de caractère 'c' qui sont compris dans 'word'
	 */
	int count = 0;
	for (int i = 0; i < word.size(); ++i)
	{
		if (word[i] == c)
		{
			count += 1;
		}
	}
	return count;
}

vector<string> get_keys(map<string, Mtmc_variable *> m)
{
	vector<string> key;
	for (map<string, Mtmc_variable *>::iterator it = m.begin(); it != m.end(); ++it)
	{
		key.push_back(it->first);
	}

	return key;
}
