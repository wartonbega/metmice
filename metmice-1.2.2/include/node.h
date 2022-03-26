using namespace std;

struct Node
{
	vector<Node *> children;
	string value;
	string type;
	int n;
};


Node *add_child(Node *tree)
{
	Node *ne = new Node;
	tree->children.push_back(ne);
	return ne;
}

void push_child(Node *tree, Node *c) {
	tree->children.push_back(c);
}

void ast_t_rep(Node *ast_t, int tab)
{
	string t;
	for (int i = 0; i < tab; ++i)
	{
		t += "│   ";
	}
	if (tab != 0)
	{
		t += "├── ";
	}
	printf("%s%s (%s)\n", t.c_str(), ast_t->value.c_str(), ast_t->type.c_str());
	for (int i = 0; i < ast_t->children.size(); ++i)
	{
		ast_t_rep(ast_t->children[i], tab + 1);
	}
}

vector<string> childs_value(Node *trunc){
	vector<string> values;
	for (int i = 0; i<trunc->children.size(); ++i){
		values.push_back(trunc->children[i]->value);
	}
	return values;
}

vector<string> childs_type(Node *trunc){
	vector<string> types;
	for (int i = 0; i<trunc->children.size(); ++i){
		types.push_back(trunc->children[i]->type);
	}
	return types;
}