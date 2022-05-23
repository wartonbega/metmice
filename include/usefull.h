
using namespace std;
void assertion(bool expr, string error_s);

void Error(string emplacement, string err);

void debug_error(string emplacement, string err);

string get_from(vector<string> jsp, int index1, int index2);

vector<Node *> get_from_vector(vector<Node *> jsp, int index1, int index2);
string get_from_string(string jsp, int index1, int index2);
Node *get_from_node(Node *trunc, int index1, int index2);
bool in(char one, const char *two);
bool in_vector(string one, vector<string> sample);

bool consti(string word, string chars);

string concatenate(vector<string> v);

string concatenate_without_blank(vector<string> v);
int number_of(char c, string word);
vector<string> get_keys(map<string, Mtmc_variable *> m);

int make_max(int n, int max);
int make_floor(int n, int min);
int get_line_of_error_reference(string reference);
string give_file_error(string filename, int line);


#ifndef USEFULL_H
#define USEFULL_H

#define TERMINAL_RESET   string("\033[0m")
#define TERMINAL_BLACK   string("\033[30m")      
#define TERMINAL_RED     string("\033[31m")      
#define TERMINAL_GREEN   string("\033[32m")      
#define TERMINAL_YELLOW  string("\033[33m")      
#define TERMINAL_BLUE    string("\033[34m")      
#define TERMINAL_MAGENTA string("\033[35m")      
#define TERMINAL_CYAN    string("\033[36m")      
#define TERMINAL_WHITE   string("\033[37m")      
#define TERMINAL_BOLDBLACK   string("\033[1m\033[30m")
#define TERMINAL_BOLDRED     string("\033[1m\033[31m")
#define TERMINAL_BOLDGREEN   string("\033[1m\033[32m")
#define TERMINAL_BOLDYELLOW  string("\033[1m\033[33m")
#define TERMINAL_BOLDBLUE    string("\033[1m\033[34m")
#define TERMINAL_BOLDMAGENTA string("\033[1m\033[35m")
#define TERMINAL_BOLDCYAN    string("\033[1m\033[36m")
#define TERMINAL_BOLDWHITE   string("\033[1m\033[37m")


#endif