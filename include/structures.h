

Mtmc_class_template *generate_structure(string name, vector<Node *> trunc, vector<string> reference);
Mtmc_functions *structure_function_asignement(string name, vector<Node *> paterns, vector<Node *> args, vector<Node *> antiargs);
Mtmc_class_variable *generate_structured_variable(Mtmc_class_template *parent, vector<string> reference);

void magic_methods(Mtmc_class_template *temp);
Mtmc_variable *magic_method_execution(string op, Mtmc_variable *arg2, Mtmc_variable *main_arg, bool function, map<string, Mtmc_variable *> variables_function, int recursive);
