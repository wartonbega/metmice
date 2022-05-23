#include <vector>
#include <string>

const char *get_completion(string token);

bool is_keyword_p(string c);

string tokenize(string token);

Node *parser(vector<string>, Node *, int, int, vector<string>);

