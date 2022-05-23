#include <vector>
#include <string>
struct Node
{
	std::vector<Node *> children;
	std::string value;
	std::string type;
	int n;
	std::string ref;
};

Node *add_child(Node *tree);
void push_child(Node *tree, Node *c);
std::string ast_t_rep(Node *ast_t, int tab);
std::vector<std::string> childs_value(Node *trunc);
std::vector<std::string> childs_type(Node *trunc);
std::vector<std::string> childs_ref(Node *trunc);
