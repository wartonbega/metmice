#include <string>
#include <vector>

struct Mtmc_variable;

using namespace std;

void assertion(bool expr, string error_s)
{
	if (!expr)
	{
		cout << error_s << endl;
		exit(1);
	}
}

void Error(string emplacement, string err)
{
	string err_mess2 = "\n[mahtematica: " + current_file_name + " :" + emplacement + "]: " + err;
	cout << err_mess2 << endl;
	exit(1);
}

void debug_error(string emplacement, string err)
{
	if (debug)
	{
		string err_mess2 = "\n[mahtematica/" + current_file_name + ":" + emplacement + "]: " + err;
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
		Error("internal:get_from_string", "the second index can't be greater than the string length");
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
	for (map<string, Mtmc_variable*>::iterator it = m.begin(); it != m.end(); ++it)
	{
		key.push_back(it->first);
	}

	return key;
}
