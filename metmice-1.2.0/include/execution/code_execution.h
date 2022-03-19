string normal_code_execution(vector<Node *> ast, bool function, vector<Mtmc_variable *> &variable_function, int recursive);
bool variable_exist_function(string name, vector<string> variable_name_function);
void variable_override_function(string name, string value, vector<string> &variable_name_function, vector<string> &variable_value_function);
void variable_asignement_function(string name, string value, vector<string> &variable_name_function, vector<string> &variable_value_function);
string variable_get_value_function(string name, vector<string> &variable_name_function, vector<string> &variable_value_function);
string funcall_base_function(Node *instruction, vector<string> variable_name_function, vector<string> variable_value_function, int recursive);
string compute_function(Node *trunc, bool empty_stack, bool funcall, vector<string> &variable_name_function, vector<string> &variable_value_function, int recursive);
vector<string> compute_listed_values_function(vector<Node *> listed_values, vector<string> &variable_name_function, vector<string> &variable_value_function, int recursive);
string execute_function(vector<Node *> ast, vector<string> args, vector<Node *> normal_args, vector<string> variable_name_function, vector<string> variable_value_function, int recursive);
string for_loop(vector<Node *> ast, bool function, vector<Mtmc_variable *> &variable_function, int recursive, vector<Node *> args);

