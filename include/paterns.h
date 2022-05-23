


bool recognize_if(vector<string> c_values, int index);
Node *give_if(Node *trunc, int index);
bool recognize_while(vector<string> c_values, int index);
Node *give_while(Node *trunc, int index);
bool recognize_for(vector<string> c_values, int index);
Node *give_for(Node *trunc, int index);
bool recognize_expr(vector<string> c_values, int index);
Node *give_expr(Node *trunc, int index);
bool recognize_func(vector<string> c_values, int index);
Node *give_func(Node *trunc, int index);
bool recognize_keyword(vector<string> c_values, int index);
Node *give_keyword(Node *trunc, int index);
bool recognize_struct(vector<string> c_values, int index);
Node *give_struct(Node *trunc, int index);
vector<Node *> recognize_paternes(Node *trunc, bool debug);

