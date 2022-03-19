#include "stdlib.h"

void manage_system_pile();
void empty_compute_stack();
void output_display_pile();
void push_display_pile(Mtmc_variable *element);
Mtmc_variable *variable_get_value(string name);
bool variable_exist(string name);
void variable_asignement(Mtmc_variable *var);
void variable_override(Mtmc_variable *var);
bool function_exists(string name);
void function_asignement(string name, vector<Node *> paterns, vector<Node *> args, vector<Node *> antiargs);
vector<Node *> get_listed_values(Node *listed_value);
int function_index(string name);
Mtmc_variable *funcall_base(Node *instruction, int recursive);
Mtmc_variable *compute(Node *trunc, bool empty_stack, bool funcall);
vector<Mtmc_variable *> compute_listed_values(vector<Node *> listed_values);
bool mtmc_accept_var_name(string name);
void compute_operator_rpn(vector<string> names, int &i);
void compute_operator_normal(vector<string> names, vector<string> types, int &i, bool funcall, vector<Mtmc_variable *> &variable_function, Node *trunc, int recursive);
string while_loop(vector<Node *> ast, bool function, vector<Mtmc_variable *> &variable_function, int recursive, vector<Node *> args);


// fonctions qui viennent de execution/structure.h
Mtmc_class_template *generate_structure(string name, Node *trunc);
Mtmc_functions *structure_function_asignement(string name, vector<Node *> paterns, vector<Node *> args, vector<Node *> antiargs);
Mtmc_class_variable *generate_structured_variable(Mtmc_class_template *parent);

#include "./alu.h"
#include "execution/code_execution.h"
#include "execution/function.h"
#include "execution/if.h"
#include "execution/loops.h"
#include "stdout.h"
#include "execution/structures.h"

// 0 pour mac, 1 pour linux, 2 pour windows
#ifdef __unix__

#define w_OS 1

#elif __MACH__

#define w_OS 0

#elif _WIN64

#define w_OS 2

#endif

void log(string content)
{
    if (show_logs)
    {
        printf("log : --> %s", content.c_str());
    }
}

int variable_pool = 0;
int functions_pool = 0;

void empty_compute_stack()
{
    if (clear_stack)
    {
        while (!computes.empty())
        {
            computes.pop();
        }
    }
}

void empty_sys_stack()
{
    system_p.empty();
}

void output_display_pile()
{
    cout << concatenate_without_blank(display);
    display.clear();
}

void manage_system_pile()
{
    for (auto i : system_p)
    {
        if (i == 0)
        {
            exit(0);
        }
        else if (i == 1)
        {
            exit(1);
        }
        else if (i == 2)
        { // mettre le contenu d'un fichier sur le stack
            string name;
            if (computes.size() >= 1)
            {
                if (computes.top()->type == "string")
                {
                    name = del_string(*(string *)computes.top()->content);
                    computes.pop();
                }
                else
                {
                    Error("execution", "the name of the file imported must be a string");
                }
            }
            else
                Error("execution", "the compute stack must to not be empty");
            string content = read_file(name.c_str(), false);
            content = to_string(content);
            computes.push(new Mtmc_variable());
            computes.top()->content = (void *)&content; // Casté en void *
            computes.top()->type = "string";
        }
        else if (i == 3)
        { // Executer la dernière commande sur le stack
            string name;
            if (computes.size() >= 1)
            {
                if (computes.top()->type == "string")
                {
                    name = del_string(*(string *)computes.top()->content);
                    computes.pop();
                }
                else
                {
                    Error("execution", "the name of the file imported must be a string");
                    exit(1);
                }
            }
            else
                Error("execution", "the compute stack must to not be empty");

            int ret = system(name.c_str());
            if (ret == 1 or ret == -1)
            {
                Error("execution", "the system call failed");
            }
            else
            {
                computes.push(new Mtmc_variable());
                computes.top()->content = (void *)&ret;
                computes.top()->type = "int";
            }
        }
        else if (i == 4)
        { // Met en pause le programme pour un laps de temps
            int name = 0;
            if (computes.size() >= 1)
            {
                if (computes.top()->type == "int")
                {
                    name = *(int *)computes.top()->content;
                    computes.pop();
                }
                else
                {
                    Error("execution", "the lapse of time to slepp must be an integer");
                }
            }
            else
                Error("execution", "the compute stack must to not be empty");

            sleep(name); // Pas besoin de else, l'erreur arrête le programme
        }
        else if (i == 5)
        { // Attend un input de l'utilisateur
            string res;
            getline(cin, res);
            Mtmc_variable *var = new Mtmc_variable();
            Mtmc_variable_init(var, "");
            res = to_string(res);
            var->content = (void *)&res;
            var->type = "string";
            computes.push(var);
        }
        else if (i == 6)
        { // Change le maximum de recursion
            int name = 0;
            if (computes.size() >= 1)
            {
                if (computes.top()->type == "int")
                {
                    name = *(int *)computes.top()->content;
                    computes.pop();
                }
                else
                {
                    Error("execution", "the maximum recursion value must be an integer");
                }
            }
            else
                Error("execution", "the compute stack must to not be empty");
            if (name > 2265)
            {
                Error("execution", "Max recursion depth can't be over 2266");
            }
            maximum_recursion_depth = name;
        }
        else if (i == 7)
        {
            Mtmc_variable *var = new Mtmc_variable();
            Mtmc_variable_init(var, "");
            string os;
            if (w_OS == 0)
            {
                os = "\"Macos\"";
            }
            else if (w_OS == 1)
            {
                os = "\"Linux\"";
            }
            else if (w_OS == 2)
            {
                os = "\"Windows\"";
            }
            else
                os = "\"Unknown\"";
            string *ost = new string(os);
            var->type = "string";
            var->content = (void *)ost;
            computes.push(var);
        }
        else if (i == 8)
        { // Pour avoir le répertoir courrant
            Mtmc_variable *var = new Mtmc_variable();
            Mtmc_variable_init(var, "");
            string *res = new string(to_string(working_dir));
            var->content = (void *)res;
            computes.push(var);
        }
        else if (i == 9)
        { // Pour avoir le nom du fichier courrant
            Mtmc_variable *var = new Mtmc_variable();
            Mtmc_variable_init(var, "");
            string *res = new string(to_string(current_file_name));
            var->content = (void *)res;
            computes.push(var);
        }
        else if (i == 10) // Change la norme de calcule
        {
            int name = 0;
            if (computes.size() >= 1)
            {
                if (computes.top()->type == "int")
                {
                    name = *(int *)computes.top()->content; // Casté en int
                    computes.pop();
                }
                else
                {
                    Error("execution", "the numeration norm must be an integer (either 1 or 0 for rpn or natural)");
                }
            }
            else
                Error("execution", "the compute stack must to not be empty");

            Mtmc_variable *var = new Mtmc_variable();
            Mtmc_variable_init(var, "");

            if (compute_norme == 0)
            { // On récupère la norme actuelle
                int64_t *current_norm = new int64_t(0);
                var->content = (void *)current_norm;
            }
            else
            {
                int64_t *current_norm = new int64_t(1);
                var->content = (void *)current_norm;
            }
            var->type = "int";
            computes.push(var);

            if (name == 1)
                compute_norme = 1; // Pour le mode de calcule "normal"
            else if (name == 0)
                compute_norme = 0; // Pour le calcule en rpn
            else
                Error("execution", "the compute mode should be either 0 (for rpn) or 1 (for straight numeration)");
        }
        else if (i == 11)
        { // Changement de la supression du stack de calcule
            int name = 0;
            if (computes.size() >= 1)
            {
                if (computes.top()->type == "int")
                {
                    name = *(int *)computes.top()->content; // Casté en int
                    computes.pop();
                }
                else
                {
                    Error("execution", "the stack clearing mode should be either 0 (for no garbage collection at all) or 1 (to often clear the stack)");
                }
            }
            else
                Error("execution", "the compute stack must to not be empty");

            Mtmc_variable *var = new Mtmc_variable();
            Mtmc_variable_init(var, "");

            if (clear_stack == 0)
            {
                int64_t *current_norm = new int64_t(0);
                var->content = (void *)current_norm;
            }
            else
            {
                int64_t *current_norm = new int64_t(1);
                var->content = (void *)current_norm;
            }
            var->type = "int";
            computes.push(var);

            if (name == 1)
                clear_stack = 1;
            else if (name == 0)
                clear_stack = 0;
            else
                Error("execution", "the stack clearing mode should be either 0 (for no garbage collection at all) or 1 (to often clear the stack)");
        }
    }
    system_p.clear();
}

Mtmc_variable *variable_get_value(string name)
{
    for (int i = 0; i < variables_t.size(); ++i)
    {
        if (variables_t[i]->name == name)
        {
            return variables_t[i];
        }
    }
    Error("execution", "the variable " + name + " doesn't exist");
    return NULL;
}

bool variable_exist(string name)
{
    for (int i = 0; i < variables_t.size(); ++i)
    {
        if (variables_t[i]->name == name)
        {
            return true;
        }
    }
    return false;
}

bool mtmc_accept_var_name(string name)
{
    // Vérifie que :
    //      - Il n'y as pas de caractère interdits genre !
    // vérifie que ce n'est pas que un nombre
    // vérifie que ce n'est pas un mot-clé
    if (is_typed(name))
    {
        return false;
    }
    if (name[0] == '!')
    {
        return false;
    }
    if (is_keyword(name))
    {
        return false;
    }
    if (in('.', name.c_str()))
    // On peut accepter les points dans les noms de variables que pour
    // les structures
    {
        return false;
    }

    return true;
}

void variable_asignement(Mtmc_variable *var)
{
    if (!mtmc_accept_var_name(var->name))
    {
        string err = "invalid name for a variable `" + var->name + "`";
        debug_error("execution", err);
    }
    variables_t.push_back(var);
}

void variable_override(Mtmc_variable *var)
{
    int index = 0;
    for (int i = 0; i < variables_t.size(); ++i)
    {
        if (variables_t[i]->name == var->name) // Que si la variable existe déja et que la valeur est la même
        {
            index = i;
            break;
        }
    }
    variables_t[index] = var;
}

bool function_exists(string name)
{
    for (int i = 0; i < functions.size(); ++i)
    {
        if (functions[i]->name == name)
        {
            return true;
        }
    }
    return false;
}

bool operator_exists(string name)
{
    for (int i = 0; i < operators_n.size(); ++i)
    {
        if (operators_n[i] == name)
        {
            return true;
        }
    }
    return false;
}

string operator_function(string name)
{
    for (int i = 0; i < operators_n.size(); ++i)
    {
        if (operators_n[i] == name)
        {
            return operators_f[i];
        }
    }
    return "none";
}

void function_asignement(string name, vector<Node *> paterns, vector<Node *> args, vector<Node *> antiargs)
{
    // Pour appeler une fonction on auras besoin de ses arguments, son nom, son code, et ses antiargs pour les calculs
    if (!mtmc_accept_var_name(name))
    {
        string err = "invalid name for a function `" + name + "`";
        debug_error("execution", err);
    }

    Mtmc_functions *f = new Mtmc_functions();
    f->name = "!" + name;
    f->value = paterns;
    f->args = args;
    f->antiargs = antiargs;
    f->is_structure_generator = false; // Ce qui veut dire que quand on appelera la fonction, elle ne renverra pas de structure generator
    functions.push_back(f);
}

vector<Node *> get_listed_values(Node *listed_value)
{
    /*
    Renvoie une liste découpée d'expressions (par des virgules)
    */
    vector<Node *> returnings; // La liste qui vas être renvoyée
    if (listed_value->children.size() == 0)
    {
        return returnings;
    }
    vector<string> childs = childs_value(listed_value); // On ne travaillera que avec les index de ce vecteur
    Node *value = new Node();                           // les branches vont etre poussées une à une
    value->value = "*";
    int last;
    for (int i = 0; i < childs.size(); ++i)
    {
        if (childs[i] == ",")
        {
            returnings.push_back(value);
            value = new Node();
            value->value = "*";
        }
        else
        {
            push_child(value, listed_value->children[i]); // On met la branche dans l'arbre
        }
        last = i;
    }
    if (value->children.size() != 0)
    {
        returnings.push_back(value);
    }
    return returnings;
}

int function_index(string name)
{
    for (int i = 0; i < functions.size(); ++i)
    {
        if (functions[i]->name == name)
        {
            return i;
        }
    }
    return 0;
}

Mtmc_variable *funcall_base(Node *instruction, int recursive)
{
    if (is_object_attribute(instruction->value))
    { // Si c'est genre !obj.fonction()
        vector<string> names = object_atribute_separator_function(instruction->value);

        Mtmc_variable *var = nullptr;
        if (variable_exist(names[0]))
        {
            var = variable_get_value(names[0]);
        }
        else
        {
            string err = "unknown variable '" + names[0] + "'";
            Error("execution", err);
        }

        Mtmc_class_variable *c_var = (Mtmc_class_variable *)var->content;

        if (c_var->parent_class->function_exists(names[1]))
        {
            Mtmc_functions *func = c_var->parent_class->function(names[1]);

            vector<Node *> args;
            vector<Mtmc_variable *> args2;
            if (instruction->children[0]->children.size() != 0)
            {
                args = get_listed_values(instruction->children[0]->children[0]);
                args2 = compute_listed_values(args);
            }
            vector<Mtmc_variable *> args3;
            args3.push_back(var); // Correspond au 'self'
            for (int i = 0; i < args2.size(); i++)
            {
                args3.push_back(args2[i]);
            }
            vector<Node *> normal_args = func->args;
            if (args3.size() != normal_args.size())
            {
                string err = "the number of arguments of the function being called '" + instruction->value + "' is not the same of the function that has been precedently defined \nexpected " + to_string(normal_args.size()) + " arguments, got " + to_string(args.size());
                Error("execution", err);
            }
            vector<Mtmc_variable *> variable_bis;
            Mtmc_variable *var = execute_function(func->value, args3, normal_args, variable_bis, recursive);
            return var;
        }
        else
        {
            string err = "the object '" + names[0] + "' (of type '" + c_var->name + "') have no attribute '" + names[1] + "'";
            Error("execution", err);
        }
    }
    else
    {
        if (!function_exists(instruction->value))
        {
            string err = "unknown function '" + instruction->value + "'";
            Error("execution", err);
        }
        int index = function_index(instruction->value);
        
        // Si c'est une fonction normale :
        if (functions[index]->is_structure_generator == false)
        {
            vector<Node *> args;
            vector<Mtmc_variable *> args2;
            if (instruction->children[0]->children.size() != 0)
            {
                args = get_listed_values(instruction->children[0]->children[0]);
                args2 = compute_listed_values(args);
            }
            vector<Node *> normal_args = functions[index]->args;
            if (args.size() != normal_args.size())
            {
                string err = "the number of arguments of the function being called '" + instruction->value + "' is not the same of the function that has been precedently defined \nexpected " + to_string(normal_args.size()) + " arguments, got " + to_string(args.size());
                Error("execution", err);
            }
            vector<Mtmc_variable *> variable_bis;
            Mtmc_variable *var = execute_function(functions[index]->value, args2, normal_args, variable_bis, recursive);
            return var;
        }
        else
        {
            // C'est donc un pochoir à structures
            Mtmc_class_template *temp = functions[index]->temp;
            Mtmc_class_variable *var = generate_structured_variable(temp);

            Mtmc_variable *n = new Mtmc_variable();
            n->type = "type:" + var->name;
            n->content = (void *)var;
            return n;
        }
    }

    return new Mtmc_variable();
}

Mtmc_variable *funcall_operator(string name, Mtmc_variable *arg1, Mtmc_variable *arg2)
{
    if (!function_exists(name))
    {
        string err = "unknown function '" + name + "'";
        Error("execution", err);
    }
    int index = function_index(name);
    vector<Mtmc_variable *> args2;
    args2.push_back(arg1);
    args2.push_back(arg2);
    vector<Node *> normal_args = functions[index]->args;
    if (normal_args.size() != 2)
    {
        string err = "the number of arguments of the function being called as an operator : '" + name + "' needs two arguments";
        Error("execution", err);
    }
    vector<Mtmc_variable *> variable_bis;
    Mtmc_variable *var = execute_function(functions[index]->value, args2, normal_args, variable_bis, 0);
    return var;
}

void compute_operator_rpn(vector<string> names, int &i)
{
    if (computes.size() >= 2)
    {
        Mtmc_variable *b = computes.top();
        computes.pop();
        Mtmc_variable *a = computes.top();
        computes.pop();
        if (names[i] == "-")
        {
            computes.push(alu_minus(a, b));
        }
        else if (names[i] == "+")
        {
            computes.push(alu_plus(a, b));
        }
        else if (names[i] == "=")
        {
            computes.push(alu_equals(a, b));
        }
        else if (names[i] == "*")
        {
            computes.push(alu_times(a, b));
        }
        else if (names[i] == "^")
        {
            computes.push(alu_power(a, b));
        }
        else if (names[i] == "/")
        {
            computes.push(alu_div(a, b));
        }
        else if (names[i] == "%")
        {
            computes.push(alu_modulo(a, b));
        }
        else if (names[i] == "//")
        {
            computes.push(alu_floordiv(a, b));
        }
        else if (names[i] == "<")
        {
            computes.push(alu_lt(a, b));
        }
        else if (names[i] == ">")
        {
            computes.push(alu_gt(a, b));
        }
        else if (operator_exists(names[i]))
        {
            string f = operator_function(names[i]);
            computes.push(funcall_operator(f, a, b));
        }
    }
    else
    {
        if (names[i] == "-")
        {
            if (i + 1 < names.size())
            {
                Mtmc_variable *r = alu_negativ(new_mtmc_variable_from_string("", names[i + 1]));
                computes.push(r);
                i += 1;
            }
            else
            {
                Error("execution", "can't have a lonely operator `-`\nWarning : current compute mode is in rpn");
            }
        }
        else
        {
            string err = "`" + names[i] + "` binary operator needs two arguments\nWarning : current compute mode is in rpn";
            Error("execution", err);
        }
    }
}

void compute_operator_normal(vector<string> names, vector<string> types, int &i, bool funcall, vector<Mtmc_variable *> &variable_function, Node *trunc, int recursive)
{
    // Méthode "normal" de calcule
    if (computes.size() >= 1)
    {
        Mtmc_variable *a = computes.top();
        computes.pop();
        if (names.size() - i > 1)
        {
            string b_prime = names[i + 1]; // le numéro d'après
            Mtmc_variable *b = new Mtmc_variable();
            Mtmc_variable_init(b, "");
            // Assigner la variable ou appeler la fonction
            if (funcall)
            {
                if (b_prime[0] == '!')
                {
                    // On sauvegarde l'état actuel du stack
                    stack<Mtmc_variable *> save = computes;
                    Mtmc_variable *ret = funcall_base_function(trunc->children[i + 1], variable_function, recursive);
                    computes = save;
                    b = ret;
                }
                else if (variable_exist_function(b_prime, variable_function))
                {
                    b = variable_get_value_function(b_prime, variable_function);
                }
                else if (explicit_variable(b_prime))
                {
                    b = new_mtmc_variable_from_string("", b_prime);
                }
                else if (types[i + 1] == "array")
                {
                    Mtmc_listed *childes = new_mtmc_listed_from_string(names[i + 1]);
                    Mtmc_variable *n = new Mtmc_variable();
                    n->type = "list";
                    n->content = (void *)childes;
                    computes.push(n);
                }
                else
                {
                    string err = "unkown variable or expression `" + b_prime + "`";
                    Error("execution", err);
                }
            }
            else
            {
                if (b_prime[0] == '!')
                {
                    // On sauvegarde l'état actuel du stack
                    stack<Mtmc_variable *> save = computes;
                    Mtmc_variable *ret = funcall_base(trunc->children[i + 1], 0);
                    computes = save;
                    b = ret;
                }
                else if (variable_exist(b_prime))
                {
                    b = variable_get_value(b_prime);
                }
                else if (explicit_variable(b_prime))
                {
                    b = new_mtmc_variable_from_string("", b_prime);
                }
                else if (types[i + 1] == "array")
                {
                    Mtmc_listed *childes = new_mtmc_listed_from_string(names[i + 1]);
                    b->type = "list";
                    b->content = (void *)childes;
                }
                else if (is_object_attribute(b_prime))
                { // se présente sous la forme obj.atribute
                    if (!funcall)
                    {
                        vector<string> names = object_atribute_separator(b_prime);
                        if (!variable_exist(names[0]))
                        {
                            string err = "unknown variable '" + names[0] + "'";
                            Error("execution", err);
                        }
                        Mtmc_variable *n = variable_get_value(names[0]);
                        if (!is_typed_non_predefined(n->type))
                        {
                            string err = "type '" + n->type + "' has no attribute '" + names[1] + "'";
                            Error("execution", err);
                        }
                        Mtmc_class_variable *n_prime = (Mtmc_class_variable *)n->content;
                        if (!n_prime->variable_exists(names[1]))
                        {
                            string err = "object '" + names[0] + "' has no attribute '" + names[1] + "'";
                            Error("execution", err);
                        }

                        b = n_prime->variable_get_value(names[1]);
                    }
                    else
                    {
                        vector<string> names = object_atribute_separator(b_prime);
                        if (!variable_exist_function(names[0], variable_function))
                        {
                            string err = "unknown variable '" + names[0] + "'";
                            Error("execution", err);
                        }
                        Mtmc_variable *n = variable_get_value_function(names[0], variable_function);
                        if (!is_typed_non_predefined(n->type))
                        {
                            string err = "type '" + n->type + "' has no attribute '" + names[1] + "'";
                            Error("execution", err);
                        }
                        Mtmc_class_variable *n_prime = (Mtmc_class_variable *)n->content;
                        if (!n_prime->variable_exists(names[1]))
                        {
                            string err = "object '" + names[0] + "' has no attribute '" + names[1] + "'";
                            Error("execution", err);
                        }

                        b = n_prime->variable_get_value(names[1]);
                    }
                }
                else
                {
                    string err = "unkown variable or expression `" + b_prime + "`";
                    Error("execution", err);
                }
            }

            if (names[i] == "-")
            {
                computes.push(alu_minus(a, b));
            }
            else if (names[i] == "+")
            {
                computes.push(alu_plus(a, b));
            }
            else if (names[i] == "=")
            {
                computes.push(alu_equals(a, b));
            }
            else if (names[i] == "*")
            {
                computes.push(alu_times(a, b));
            }
            else if (names[i] == "^")
            {
                computes.push(alu_power(a, b));
            }
            else if (names[i] == "/")
            {
                computes.push(alu_div(a, b));
            }
            else if (names[i] == "%")
            {
                computes.push(alu_modulo(a, b));
            }
            else if (names[i] == "//")
            {
                computes.push(alu_floordiv(a, b));
            }
            else if (names[i] == "<")
            {
                computes.push(alu_lt(a, b));
            }
            else if (names[i] == ">")
            {
                computes.push(alu_gt(a, b));
            }
            else if (operator_exists(names[i]))
            {
                string f = operator_function(names[i]);
                computes.push(funcall_operator(f, a, b));
            }
        }
        else
        {
            string err = "`" + names[i] + "` binary operator needs two arguments\nWarning : current compute mode is in normal numeration mode";
            Error("execution", err);
        }
    }
    else
    {
        if (names[i] == "-")
        {
            if (i + 1 < names.size())
            {
                Mtmc_variable *r = alu_negativ(new_mtmc_variable_from_string("", names[i + 1]));
                computes.push(r);
            }
            else
            {
                Error("execution", "can't have a lonely operator `-`\nWarning : current compute mode is in normal numeration mode");
            }
        }
        else
        {
            string err = "`" + names[i] + "` binary operator needs two arguments\nWarning : current compute mode is in normal numeration mode";
            Error("execution", err);
        }
    }
    i++;
}

Mtmc_variable *compute(Node *trunc, bool empty_stack, bool funcall)
{
    vector<string> names = childs_value(trunc);
    vector<string> types = childs_type(trunc);

    Mtmc_variable *a;
    Mtmc_variable *b;
    for (int i = 0; i < names.size(); i++)
    {
        if (types[i] == "funcall")
        {
            // On sauvegarde l'état actuel du stack
            stack<Mtmc_variable *> save = computes;
            Mtmc_variable *ret = funcall_base(trunc->children[i], 0);
            computes = save;
            computes.push(ret);
        }
        else if (is_operator(names[i]))
        {
            if (compute_norme == 0) // rpn
            {
                compute_operator_rpn(names, i);
            }
            else // pas rpn
            {
                vector<Mtmc_variable *> prout;
                compute_operator_normal(names, types, i, false, prout, trunc, 0);
            }
        }
        else if (names[i] == ",")
        { // équivalent a la commande `swap` sur les calculatrices rpn
            a = computes.top();
            computes.pop();
            b = computes.top();
            computes.pop();
            computes.push(a);
            computes.push(b);
        }
        else
        {
            if (variable_exist(names[i]))
            {
                computes.push(variable_get_value(names[i]));
            }

            else if (explicit_variable(names[i]))
            {
                Mtmc_variable *n = new_mtmc_variable_from_string("", names[i]);
                computes.push(n);
            }
            else if (types[i] == "array")
            {
                Mtmc_listed *childes = new_mtmc_listed_from_string(names[i]);
                Mtmc_variable *n = new Mtmc_variable();
                n->type = "list";
                n->content = (void *)childes;
                computes.push(n);
            }
            else if (is_object_attribute(names[i]))
            { // se présente sous la forme obj.atribute
                vector<string> names_of_objects = object_atribute_separator(names[i]);
                if (!variable_exist(names_of_objects[0]))
                {
                    string err = "unknown variable '" + names_of_objects[0] + "'";
                    Error("execution", err);
                }
                Mtmc_variable *n = variable_get_value(names_of_objects[0]);
                if (!is_typed_non_predefined(n->type))
                {
                    string err = "type '" + n->type + "' has no attribute '" + names_of_objects[1] + "'";
                    Error("execution", err);
                }
                Mtmc_class_variable *n_prime = (Mtmc_class_variable *)n->content;
                if (!n_prime->variable_exists(names_of_objects[1]))
                {
                    string err = "object '" + names_of_objects[0] + "' has no attribute '" + names_of_objects[1] + "'";
                    Error("execution", err);
                }

                computes.push(n_prime->variable_get_value(names_of_objects[1]));
            }
            else
            {
                string err = "unkown variable or expression `" + names[i] + "`";
                Error("execution", err);
            }
        }
    }
    Mtmc_variable *ret = computes.top();
    if (empty_stack)
        empty_compute_stack();
    return ret;
}

vector<Mtmc_variable *> compute_listed_values(vector<Node *> listed_values)
{
    // le vector est sous la forme * ¬ 3
    //                               ¬ 4
    //                               ¬ +
    // args[0]->children[0]->value = 3
    vector<Mtmc_variable *> ret;
    for (auto val : listed_values)
    {
        ret.push_back(compute(val, true, false));
    }
    return ret;
}

// ————————————————————————————————————————————————————————————————————————————————————————————————

int visitor(vector<Node *> ast, bool ast_show, bool debug, bool strict)
{
    int index = 0;
    while (index != ast.size())
    {
        Node *instruction = ast[index];

        if (instruction->value == "expr")
        {
            Mtmc_variable *result = compute(instruction->children[0], true, false);
            if (instruction->children.size() > 1 and instruction->children[1]->value == "->")
            {
                if (instruction->children[2]->children.size() == 3) // pour les assigantions des listes genre '3 -> liste*3'
                {
                    // Normalement, sa se présente soit liste*index soit liste*[index]
                    // On crée donc un sous noeud qui contient une fois la liste, pour pouvoir utiliser la fonction de calcule
                    Node *n = new Node();
                    n->children.push_back(instruction->children[2]->children[0]);
                    Mtmc_variable *list_to_mod = compute(n, false, false);

                    if (list_to_mod->type != "list")
                    {
                        string err = "the type of the list being modified must be 'list', not '" + list_to_mod->type + "'";
                        Error("execution", err);
                    }

                    // Pareil
                    n = new Node();
                    n->children.push_back(instruction->children[2]->children[2]);
                    Mtmc_variable *list_index = compute(n, false, false);

                    if (list_index->type == "int")
                    {
                        Mtmc_listed_assigne(*(int64_t *)list_index->content, (Mtmc_listed *)list_to_mod->content, result);
                    }
                    else if (list_index->type == "list")
                    {
                        Mtmc_listed *list_indexL = (Mtmc_listed *)list_index->content;
                        Mtmc_variable *index = list_indexL->begin->next->content;

                        if (index->type == "none")
                        {
                            Error("not implemented", "indexing with a content still waiting to be evaluated (include/visitor.h/visitor/asignation dans une liste)");
                        }
                        else if (index->type != "int")
                        {
                            string err = "the index of the list must be of type 'int' not type '" + index->type + "'";
                            Error("execution", err);
                        }
                        int64_t index_int = int64_t(*(int64_t *)index->content); // Cast vers du int (l'index)
                        Mtmc_listed_assigne(index_int, (Mtmc_listed *)list_to_mod->content, result);
                    }
                }
                else if (instruction->children[2]->children.size() == 1)
                {
                    // Asignation du nom à la variable

                    // Si le nom de la variable est simple (genre pas un objet)
                    if (!is_object_attribute(instruction->children[2]->children[0]->value))
                    {
                        result->name = instruction->children[2]->children[0]->value;
                        if (variable_exist(result->name))
                        {
                            variable_override(result);
                        }
                        variable_asignement(result);
                    }
                    else
                    { // Si c'est un attrubut d'objet que on modifie
                        vector<string> names_of_objects = object_atribute_separator(instruction->children[2]->children[0]->value);
                        if (!variable_exist(names_of_objects[0]))
                        {
                            string err = "unknown variable '" + names_of_objects[0] + "'";
                            Error("execution", err);
                        }
                        Mtmc_variable *n = variable_get_value(names_of_objects[0]);
                        if (!is_typed_non_predefined(n->type))
                        {
                            string err = "type '" + n->type + "' has no attribute '" + names_of_objects[1] + "'";
                            Error("execution", err);
                        }
                        Mtmc_class_variable *n_prime = (Mtmc_class_variable *)n->content;
                        if (!n_prime->variable_exists(names_of_objects[1]))
                        {
                            string err = "object '" + names_of_objects[0] + "' has no attribute '" + names_of_objects[1] + "'";
                            Error("execution", err);
                        }

                        result->name = names_of_objects[1];
                        n_prime->variable_override(result);
                    }
                }
            }
        }
        else if (instruction->value == "keyword")
        {
            Node *args = instruction->children[1];
            empty_compute_stack();
            vector<string> s_args = childs_value(args);
            compute(args, false, false);

            if (instruction->children[0]->value == "push")
            {
                if (s_args.size() < 2 and computes.size() < 2)
                {
                    Error("execution", "'push' keywords need at least two arguments");
                }
                Mtmc_variable *arg1;
                Mtmc_variable *arg2;
                if (s_args.size() == 1)
                {
                    arg1 = computes.top();
                    computes.pop();
                    arg2 = computes.top();
                    computes.pop();
                }
                else
                {
                    arg2 = computes.top();
                    computes.pop();
                    arg1 = computes.top();
                    computes.pop();
                }
                if (!Mtmc_variable_type_check(arg1, "int"))
                {
                    Error("execution", "arguments for 'push' keyword needs to be an integer");
                }
                int carg1 = *(int *)arg1->content;
                if (carg1 == 1)
                {
                    empty_compute_stack();
                    push_display_pile(arg2);
                }
                else if (carg1 == 2)
                {
                    empty_compute_stack();
                    computes.push(arg2);
                }
                else if (carg1 == 3)
                {
                    if (!Mtmc_variable_type_check(arg2, "int"))
                    {
                        Error("execution", "arguments for 'push' in the system pile needs to be an integer");
                    }
                    system_p.push_back(*(int *)arg2->content);
                }
            }
            else if (instruction->children[0]->value == "type")
            {
                if (s_args.size() < 1 and computes.size() < 1)
                {
                    Error("execution", "'type' keywords need at least one arguments");
                }
                Mtmc_variable *arg = computes.top();
                computes.pop();
                empty_compute_stack();

                Mtmc_variable *type = new_mtmc_variable_from_string("", to_string(arg->type));
                computes.push(type);
            }
            else if (instruction->children[0]->value == "include")
            {
                if (s_args.size() < 1 and computes.size() < 1)
                {
                    Error("execution", "'include' keywords need at least one arguments");
                }
                Mtmc_variable *arg = computes.top();
                empty_compute_stack();
                string type = arg->type;
                if (type != "string")
                {
                    Error("execution", "'include' keyword only support 'string' arguments");
                }

                string res = del_string(*(string *)arg->content);
                utilise_file(res, ast_show, debug, strict, true);
            }
            else if (instruction->children[0]->value == "length")
            {
                if (s_args.size() < 1 and computes.size() < 1)
                {
                    Error("execution", "'length' keywords need at least one arguments");
                }
                Mtmc_variable *arg = computes.top();
                empty_compute_stack();
                string type = arg->type;
                if (type != "list")
                {
                    Error("execution", "'length' keyword only support 'list' arguments");
                }
                Mtmc_listed *list = (Mtmc_listed *)arg->content;
                int64_t *length = new int64_t(list->size);
                Mtmc_variable *len = new Mtmc_variable();
                len->content = (void *)length;
                len->type = "int";
                computes.push(len);
            }
            else if (instruction->children[0]->value == "pop")
            {
                if (s_args.size() < 2 and computes.size() < 2)
                {
                    Error("execution", "'pop' keywords need at least two arguments");
                }
                Mtmc_variable *arg1;
                Mtmc_variable *arg2;
                arg2 = computes.top();
                computes.pop();
                arg1 = computes.top();
                computes.pop();

                if (!Mtmc_variable_type_check(arg1, "int"))
                {
                    Error("execution", "first argument for 'pop' keyword needs to be an integer");
                }
                int64_t carg1 = *(int64_t *)arg1->content;
                if (!Mtmc_variable_type_check(arg2, "list"))
                {
                    Error("execution", "second argument for 'pop' keyword needs to be a list");
                }
                Mtmc_listed *list = (Mtmc_listed *)arg2->content;
                Mtmc_variable *element = Mtmc_listed_get(carg1, list);
                computes.push(element);
                Mtmc_listed_destuctor(carg1, list);
            }
            else if (instruction->children[0]->value == "operator")
            {
                Mtmc_variable *arg1;
                Mtmc_variable *arg2;
                arg2 = computes.top();
                computes.pop();
                arg1 = computes.top();
                computes.pop();

                if (!Mtmc_variable_type_check(arg1, "string"))
                {
                    Error("execution", "first argument for 'operator' keyword needs to be an string");
                }
                string opera = *(string *)arg1->content;
                if (!Mtmc_variable_type_check(arg2, "string"))
                {
                    Error("execution", "second argument for 'operator' keyword needs to be a string");
                }
                string func = *(string *)arg2->content;
                opera = del_string(opera);
                func = del_string(func);

                if (!function_exists(func))
                {
                    string err = "function '" + func + "' does not exists";
                    Error("execution", err);
                }

                if (!mtmc_accept_var_name(opera))
                {
                    string err = "'" + opera + "' is not an acceptable name for an operator";
                    Error("execution", err);
                }

                operators_f.push_back(func);
                operators_n.push_back(opera);
            }
            else
            {
                string err = "unknown keyword `" + instruction->children[0]->value + "` \nMaybe the keyword is not usable in the current state";
                Error("execution", err);
            }
        }
        else if (instruction->value == "if_statement")
        {
            Node *condition = instruction->children[1]->children[0];
            Mtmc_variable *condition_value = compute(condition, true, false);
            if (!Mtmc_variable_type_check(condition_value, "bool"))
            {
                Error("execution", "condition of an if statement needs to be a boolean");
            }
            string cond = *(string *)condition_value->content;
            vector<Mtmc_variable *> prout;
            if (cond == "true")
            {
                vector<Node *> paterns = recognize_paternes(instruction->children[2], true);
                normal_code_execution(paterns, false, prout, 0); // Qui correspond au {}
            }
            else if (instruction->children.size() > 3 and instruction->children[3]->value == "else")
            {
                vector<Node *> paterns = recognize_paternes(instruction->children[4], true);
                normal_code_execution(paterns, false, prout, 0); // Qui correspond au {} du else statement
            }
        }
        else if (instruction->value == "func")
        {
            if (debug and function_exists(instruction->children[0]->children[0]->value))
            {
                string err = "overrinding the function '" + instruction->children[0]->children[0]->value + "' \n(program stopped because debugging was enabled)";
                Error("execution", err);
            }
            vector<Node *> args;
            vector<Node *> antiargs;
            if (instruction->children[1]->children.size() != 0)
            {
                args = get_listed_values(instruction->children[1]->children[0]);     // (*)
                antiargs = get_listed_values(instruction->children[2]->children[0]); // [*]
            }
            vector<Node *> paterns_value = recognize_paternes(instruction->children[3], true); // {*}
            function_asignement(instruction->children[0]->children[0]->value, paterns_value, args, antiargs);
        }
        else if (instruction->value == "while_loop")
        {
            vector<Node *> paterns = recognize_paternes(instruction->children[2], true);
            vector<Mtmc_variable *> variable_function;
            string ret = while_loop(paterns, false, variable_function, 0, get_listed_values(instruction->children[1]->children[0]));
        }
        else if (instruction->value == "for_loop")
        {
            vector<Node *> paterns = recognize_paternes(instruction->children[2], true);
            vector<Mtmc_variable *> variable_function;
            string ret = for_loop(paterns, false, variable_function, 0, get_listed_values(instruction->children[1]->children[0])); // Qui correspond au {} du else statementif (ret == "return")
        }
        else if (instruction->value == "struct")
        {
            string name = instruction->children[1]->children[0]->value;
            vector<Node *> trunc = recognize_paternes(instruction->children[2], true);

            Mtmc_class_template *s_template = generate_structure(name, trunc);
            Mtmc_functions *f = new Mtmc_functions();
            f->name = "!" + name;
            f->is_structure_generator = true;
            f->temp = s_template;
            functions.push_back(f);
        }
        output_display_pile();
        manage_system_pile();
        index++;
    }
    return 0;
}