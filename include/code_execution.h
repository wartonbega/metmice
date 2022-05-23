
string normal_code_execution(vector<Node *> ast, bool function, map<string, Mtmc_variable *> &variable_function, int recursive, bool loop, vector<string> reference);

bool variable_exist_function(string name, map<string, Mtmc_variable *> variable_function);

void variable_override_function(Mtmc_variable *var, map<string, Mtmc_variable *> &variable_function, string name);

void variable_asignement_function(Mtmc_variable *var, map<string, Mtmc_variable *> &variable_function, string name);
Mtmc_variable *variable_get_value_function(string name, map<string, Mtmc_variable *> &variable_function);
Mtmc_variable *funcall_base_function(Node *instruction, map<string, Mtmc_variable *> variable_function, int recursive, vector<string> reference);
Mtmc_variable *compute_function(Node *trunc, bool empty_stack, bool funcall, map<string, Mtmc_variable *> &variable_function, int recursive, vector<string> reference);
Mtmc_variable *execute_function(vector<Node *> ast, vector<Mtmc_variable *> args, vector<Node *> normal_args, map<string, Mtmc_variable *> variable_function, int recursive, vector<string> reference);
string for_loop(vector<Node *> ast, bool function, map<string, Mtmc_variable *> &variable_function, int recursive, vector<Node *> args, vector<string> reference);
vector<Mtmc_variable *> compute_listed_values_function(vector<Node *> listed_values, map<string, Mtmc_variable *> &variable_function, int recursive, vector<string> reference);
string while_loop(vector<Node *> ast, bool function, map<string, Mtmc_variable *> &variable_function, int recursive, vector<Node *> args, vector<string> reference);
