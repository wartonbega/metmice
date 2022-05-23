using namespace std;

bool is_int(string c);
bool is_string(string c);
bool is_float(string c);
bool is_bool(string c);
bool is_funcall(string c);
bool is_operator(string c);
bool is_none(string c);
bool is_list(string c);
bool is_typed(string c);
bool is_indexing(string c);

int separate_indexing(string c);
bool is_object_attribute(string c);
vector<string> object_atribute_separator_function(string c);
vector<string> object_atribute_separator(string c);
bool is_typed_non_predefined(string type);
