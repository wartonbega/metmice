#include <string>
#include <vector>
#include "../include/node.h"
#include "../include/variables.hpp"
#include "../include/main.hpp"

Node *add_child(Node *tree)
{
	Node *ne = new Node;
	tree->children.push_back(ne);
	return ne;
}

void push_child(Node *tree, Node *c)
{
	tree->children.push_back(c);
}

string ast_t_rep(Node *ast_t, int tab)
{
	string t;
	for (int i = 0; i < tab; ++i)
	{
		t += "├\t";
	}
	if (tab != 0)
	{
		t += "├─ ";
	}
	string str = ast_t->value;
	str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());

	string res = t + str + " (" + ast_t->type + ")\t" + ast_t->ref + "\n";
	for (int i = 0; i < ast_t->children.size(); ++i)
	{
		res += ast_t_rep(ast_t->children[i], tab + 1);
	}
	return res;
}

vector<string> childs_value(Node *trunc)
{
	vector<string> values;
	for (int i = 0; i < trunc->children.size(); ++i)
	{
		values.push_back(trunc->children[i]->value);
	}
	return values;
}

vector<string> childs_type(Node *trunc)
{
	vector<string> types;
	for (int i = 0; i < trunc->children.size(); ++i)
	{
		types.push_back(trunc->children[i]->type);
	}
	return types;
}

vector<string> childs_ref(Node *trunc)
{
	vector<string> ref;
	for (int i = 0; i < trunc->children.size(); ++i)
	{
		ref.push_back(trunc->children[i]->ref);
	}
	return ref;
}