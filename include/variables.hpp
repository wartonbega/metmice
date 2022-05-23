#include <sstream>
#include <string>
#include <vector>
#include <map>

using namespace std;
//struct Mtmc_listed_node;
struct Mtmc_variable;
//struct Mtmc_listed;
struct Mtmc_class_template;


struct Mtmc_variable
{
    string type;
    void *content;
    /*
    Pour convertir dans le type d'origine :
        - Vérfier le type
        - (type *)var->content; Un cast vers un void pointer

    Les types possible :
        - int
        - float
        - string
        - bool
        - list
        - none
        - type:[à définir]
        > quand le type est à définir, c'est que c'est une structure définie par l'utilisateur

    */
};


void Mtmc_variable_init(Mtmc_variable *var);

void Mtmc_variable_edit(Mtmc_variable *var, string content);

Mtmc_variable *new_mtmc_variable_from_string(string content);

string Mtmc_variable_string(Mtmc_variable *var);

bool Mtmc_variable_type_check(Mtmc_variable *var, string type);

void Mtmc_variable_delete_atributes(Mtmc_variable *var);

struct Mtmc_listed_node
{
    Mtmc_listed_node *next;
    Mtmc_listed_node *prev;
    Mtmc_variable *content;
};

struct Mtmc_listed
{
    Mtmc_listed_node *begin;
    Mtmc_listed_node *end;
    int64_t size = 0;
};

void *convert(string value, string type);
string Mtmc_listed_string(Mtmc_listed *parent);
void Mtmc_listed_init(Mtmc_listed *t);

void Mtmc_listed_push_back(Mtmc_variable *cont, Mtmc_listed *parent);
Mtmc_variable *Mtmc_listed_get(int index, Mtmc_listed *parent);
void Mtmc_listed_assigne(int64_t index, Mtmc_listed *parent, Mtmc_variable *n);
void Mtmc_listed_destuctor(int index, Mtmc_listed *parent);
void Mtmc_listed_print(Mtmc_listed *parent);
string Mtmc_listed_string(Mtmc_listed *parent);
Mtmc_listed *new_mtmc_listed_from_string(string content);
void *convert(string value, string type);

struct Mtmc_functions
{
    // Les fonctions crées dans le programme
    string name;
    vector<Node *> value;
    vector<Node *> args;
    vector<Node *> antiargs;
    bool is_structure_generator;
    Mtmc_class_template *temp;
};

struct Mtmc_class_template
{
    // Les classes définies dans le programme
    string name;
    map<string, Mtmc_variable*> atributes_name;
    vector<Node *> computings_to_do;
    vector<Mtmc_functions *> functions;
        
    bool function_exists(string name)
    {
        for (int i = 0; i < this->functions.size(); ++i)
        {
            if (this->functions[i]->name == name)
            {
                return true;
            }
        }
        return false;
    }

    void function_override(string name, Mtmc_functions *f)
    { // Soit réécris la fonctions dans la struct, soit la définis
        for (int i = 0; i < this->functions.size(); ++i)
        {
            if (("!" + this->functions[i]->name) == name)
            {
                this->functions[i] = f;
                return;
            } 
        }
        this->functions.push_back(f);
    }

    Mtmc_functions *function(string name);
};

struct Mtmc_class_variable
{
    Mtmc_class_template *parent_class;
    string name;
    map<string, Mtmc_variable *> atributes;

   
    bool variable_exists(string name)
    {
        if (this->atributes.find(name) != this->atributes.end())
        {
            return true;
        }
        return false;
    }

    Mtmc_variable *variable_get_value(string name)
    {
        return this->atributes[name];
        
    }

    void variable_override(Mtmc_variable* var, string name)
    {
        this->atributes[name] = var;
    }

    string normal_name()
    { // renvoie une chaine de caractère sous la forme "object<type_name at 0x0000>"
        stringstream ss;
        ss << (const void*)this;
        return "object<"+this->name+" at "+ss.str()+">";
    }


    string convert_to_string(vector<string> reference);
};
