#include "paterns_debug.h"
/*
Notes pour la reconnaissance des motifs :
	- Ne sont pas inclus les caractères tels que ";"
	- Les expressions sans interets, sont mises dans des branches à part de l'ast notées : "*"

Les if statements :
	if(){};

Les fonctions :
	*()[]{};
	Attention pas d'assignations possibles !

Les listed values : (Pas forcément utile (à voire))
	*(*,*)


*/

bool recognize_if(vector<string> c_values, int index)
{
	if (c_values[index] == "if")
	{
		if (c_values[index + 1] == "bracket()")
		{
			if (c_values[index + 2] == "bracket{}")
			{
				if (c_values[index + 3] == ";")
				{
					return true;
				}
				else if (c_values[index + 3] == "else")
				{
					if (c_values[index + 4] == "bracket{}")
					{
						if (c_values[index + 5] == ";")
						{
							return true;
						}
					}
				}
			}
		}
	}
	return false;
}

Node *give_if(Node *trunc, int index)
{
	Node *current_node = new Node();
	current_node->value = "if_statement";
	push_child(current_node, trunc->children[index]);
	push_child(current_node, trunc->children[index + 1]);
	push_child(current_node, trunc->children[index + 2]);
	if (trunc->children[index + 3]->value == "else")
	{
		push_child(current_node, trunc->children[index + 3]); // Else
		push_child(current_node, trunc->children[index + 4]); // Suivis de ses actions
	}
	return current_node;
}

bool recognize_while(vector<string> c_values, int index)
{
	if (c_values[index] == "while")
	{
		if (c_values[index + 1] == "bracket()")
		{
			if (c_values[index + 2] == "bracket{}")
			{
				if (c_values[index + 3] == ";")
				{
					return true;
				}
			}
		}
	}
	return false;
}

Node *give_while(Node *trunc, int index)
{
	Node *current_node = new Node();
	current_node->value = "while_loop";
	push_child(current_node, trunc->children[index]);
	push_child(current_node, trunc->children[index + 1]);
	push_child(current_node, trunc->children[index + 2]);
	return current_node;
}

bool recognize_for(vector<string> c_values, int index)
{
	if (c_values[index] == "for")
	{
		if (c_values[index + 1] == "bracket()")
		{
			if (c_values[index + 2] == "bracket{}")
			{
				if (c_values[index + 3] == ";")
				{
					return true;
				}
			}
		}
	}
	return false;
}

Node *give_for(Node *trunc, int index)
{
	Node *current_node = new Node();
	current_node->value = "for_loop";
	push_child(current_node, trunc->children[index]);
	push_child(current_node, trunc->children[index + 1]);
	push_child(current_node, trunc->children[index + 2]);
	return current_node;
}

bool recognize_expr(vector<string> c_values, int index)
{
	if (c_values[index] == "*")
	{
		if (c_values[index + 1] == "->" and c_values[index + 2] == "*")
		{
			if (c_values[index + 3] == ";")
			{
				return true;
			}
		}
		else if (c_values[index + 1] == ";")
		{
			return true;
		}
	}
	return false;
}

Node *give_expr(Node *trunc, int index)
{
	/*
	———— *
	| ... optionnel
	———— ->
	|
	———— nom
	*/
	// Voir pour les mutliples assignations
	if (trunc->children[index + 1]->value == "->" and trunc->children[index + 2]->children.size() < 1)
	{
		Error("parser", "needs at least a value to asign for");
	}

	Node *current_node = new Node();
	current_node->value = "expr";
	push_child(current_node, trunc->children[index]);
	if (trunc->children[index + 1]->value == "->")
	{
		push_child(current_node, trunc->children[index + 1]);
		push_child(current_node, trunc->children[index + 2]);
	}
	return current_node;
}

bool recognize_func(vector<string> c_values, int index)
{
	if (c_values[index] == "*")
	{
		if (c_values[index + 1] == "bracket()")
		{
			if (c_values[index + 2] == "bracket[]")
			{
				if (c_values[index + 3] == "bracket{}")
				{
					if (c_values[index + 4] == ";")
					{
						return true;
					}
				}
			}
		}
	}
	return false;
}

Node *give_func(Node *trunc, int index)
{
	if (trunc->children[index]->children.size() != 1)
	{
		Error("parser", "function definition cannot contain multiple names");
	}
	Node *current_node = new Node();
	current_node->value = "func";
	push_child(current_node, trunc->children[index]);
	push_child(current_node, trunc->children[index + 1]);
	push_child(current_node, trunc->children[index + 2]);
	push_child(current_node, trunc->children[index + 3]);
	return current_node;
}

bool recognize_keyword(vector<string> c_values, int index)
{
	if (is_keyword(c_values[index]))
	{
		if (c_values[index + 1] == "*")
		{
			if (c_values[index + 2] == ";")
			{
				return true;
			}
		}
		else
		{
			if (c_values[index + 1] == ";")
			{
				return false;
			}
		}
	}
	return false;
}

Node *give_keyword(Node *trunc, int index)
{
	Node *current_node = new Node();
	current_node->value = "keyword";
	push_child(current_node, trunc->children[index]);
	if (trunc->children[index + 1]->value == "*")
	{
		push_child(current_node, trunc->children[index + 1]);
	}
	return current_node;
}

bool recognize_struct(vector<string> c_values, int index)
{
	/*
	globalement ça ressemble à ça : 
		struct nom
		{
			*
		};
	*/
	if (c_values[index] == "struct")
	{
		if (c_values[index + 1] == "*")
		{
			if (c_values[index + 2] == "bracket{}")
			{
				if (c_values[index + 3] == ";")
				{
					return true;
				}
			}
		
		}
	}
	return false;
}

Node *give_struct(Node *trunc, int index)
{
	Node *current_node = new Node();

	if (trunc->children[index + 1]->children.size() != 1)
	{
		Error("parser", "structure definition cannot contain multiple names");
	}

	current_node->value = "struct";
	push_child(current_node, trunc->children[index]); // "struct"
	push_child(current_node, trunc->children[index + 1]); // le nom de la structure
	push_child(current_node, trunc->children[index + 2]); // le corps de la structure
	return current_node;
}

vector<Node *> recognize_paternes(Node *trunc, bool debug)
{
	vector<Node *> branches;
	vector<string> c_values = childs_value(trunc);
	vector<string> c_types = childs_type(trunc);
	Node *last = new Node();
	int index = 0;
	while (index < c_values.size())
	{
		if (debug)
		{
			if (recognize_struct_debug(c_values, index))
			{
				last = give_struct(trunc, index);
			}
			else if (recognize_if_debug(c_values, index))
			{
				last = give_if(trunc, index);
			}
			else if (recognize_func_debug(c_values, index))
			{
				last = give_func(trunc, index);
			}
			else if (recognize_while_debug(c_values, index))
			{
				last = give_while(trunc, index);
			}
			else if (recognize_for_debug(c_values, index))
			{
				last = give_for(trunc, index);
			}
			else if (recognize_keyword_debug(c_values, index))
			{
				last = give_keyword(trunc, index);
			}
			else if (recognize_expr_debug(c_values, index))
			{
				last = give_expr(trunc, index);
			}
			else
			{
				string err = "unknown instruction `" + get_from(c_values, index, c_values.size()) + "` \n(Warning : this instruction can be shorten)";
				Error("parser", err);
			}
		}
		else
		{
			if (recognize_struct(c_values, index))
			{
				last = give_struct(trunc, index);
			}
			else if (recognize_if(c_values, index))
			{
				last = give_if(trunc, index);
			}
			else if (recognize_func(c_values, index))
			{
				last = give_func(trunc, index);
			}
			else if (recognize_while(c_values, index))
			{
				last = give_while(trunc, index);
			}
			else if (recognize_for(c_values, index))
			{
				last = give_for(trunc, index);
			}
			else if (recognize_keyword(c_values, index))
			{
				last = give_keyword(trunc, index);
			}
			else if (recognize_expr(c_values, index))
			{
				last = give_expr(trunc, index);
			}
			else
			{
				string err = "unknown instruction `" + get_from(c_values, index, c_values.size()) + "` \n(Warning : this instruction can be shorten)";
				Error("parser", err);
			}
		}
		index += last->children.size();
		branches.push_back(last);

		++index;
	}
	return branches;
}