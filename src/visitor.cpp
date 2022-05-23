#include <string>
#include <vector>
#include <map>

#include "../include/node.h"
#include "../include/variables.hpp"
#include "../include/main.hpp"
#include "../include/usefull.h"
#include "../include/types.h"
#include "../include/paterns_debug.h"
#include "../include/paterns.h"
#include "../include/visitor.h"
#include "../include/alu.h"
#include "../include/code_execution.h"
#include "../include/stdout.h"
#include "../include/structures.h"
#include "../include/conversion.h"
#include "../include/types_check.h"

#include "../include/keywords.h"

void log(string content)
{
    if (show_logs)
    {
        printf("log : --> %s", content.c_str());
    }
}

vector<Mtmc_variable *> extract_values(map<string, Mtmc_variable *> m)
{
    // Retourne les valeurs de la map des variables
    vector<Mtmc_variable *> value;
    for (std::map<string, Mtmc_variable *>::iterator it = m.begin(); it != m.end(); ++it)
    {
        value.push_back(it->second);
    }
    return value;
}

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

void empty_compute_stack_deletion()
{
    vector<Mtmc_variable *> existing_values = extract_values(variables_t);
    if (clear_stack)
    {
        while (!computes.empty())
        {
            Mtmc_variable *v = computes.top();
            if (!count(existing_values.begin(), existing_values.end(), v))
            {
                delete v;
            }
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
    string patent = concatenate_without_blank(display);
    cout << patent;
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
            string *con = new string(content);
            Mtmc_variable *res = new Mtmc_variable();
            res->content = (void *)con; // Casté en void *
            res->type = "string";
            computes.push(res);
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
            Mtmc_variable_init(var);
            res = to_string(res);
            string *m = new string(res);
            var->content = (void *)m;
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
        { // Donne le nom de l'os en cours
            Mtmc_variable *var = new Mtmc_variable();
            Mtmc_variable_init(var);
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
            string *res = new string(to_string(working_dir));
            var->type = "string";
            var->content = (void *)res;
            computes.push(var);
        }
        else if (i == 9)
        { // Pour avoir le nom du fichier courrant
            Mtmc_variable *var = new Mtmc_variable();
            Mtmc_variable_init(var);
            string *res = new string(to_string(current_file_name));
            var->content = (void *)res;
            computes.push(var);
        }
        else if (i == 10)
        { // Change la norme de calcule
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
            Mtmc_variable_init(var);

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
            Mtmc_variable_init(var);

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
        else if (i == 12)
        { // Récupération du temps écoulé depuis 1970 (en secondes)
            Mtmc_variable *var = new Mtmc_variable();
            int64_t *t = new int64_t(time(NULL));
            // dématerialisation (cast)
            var->content = (void *)t;
            var->type = "int";
            computes.push(var);
        }
    }
    system_p.clear();
}

Mtmc_variable *variable_get_value(string name)
{
    return variables_t[name];
}

bool variable_exist(string name)
{
    if (variables_t.find(name) != variables_t.end())
    {
        return true;
    }
    return false;
}

void variable_asignement(Mtmc_variable *var, string name)
{
    if (!mtmc_accept_var_name(name))
    {
        string err = "invalid name for a variable `" + name + "`";
        debug_error("execution", err);
    }
    variables_t[name] = var;
}

void variable_override(Mtmc_variable *var, string name)
{
    // On peut directement acceder à l'élement, vus que avant d'overrdie, on as checké si elle existe
    variables_t[name] = var;
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

Mtmc_functions *function_get(string name)
{
    for (int i = 0; i < functions.size(); ++i)
    {
        if (functions[i]->name == name)
        {
            return functions[i];
        }
    }
    return nullptr;
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

Mtmc_variable *funcall_base(Node *instruction, int recursive, vector<string> reference)
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

        if (!is_typed_non_predefined(var->type)) // Doit être un type définis post-compile
        {
            string err = "type '" + var->type + "' has no attribute '" + names[1] + "'";
            Error("execution", err);
        }

        Mtmc_class_variable *c_var = (Mtmc_class_variable *)var->content; // C'est donc le premier enfant de la variable de base

        Mtmc_variable *current = new Mtmc_variable(*var);

        for (int y = 1; y < names.size() - 1; ++y)
        { // PLusieurs attributs (!obj.atr.function())
            if (!is_typed_non_predefined(current->type))
            {
                string err = "type '" + current->type + "' has no attribute";
                Error("execution", err);
            }

            c_var = (Mtmc_class_variable *)current->content;
            if (!c_var->variable_exists(names[y]))
            {
                string err = "object '" + add_to(names, y - 1) + "' has not attribute '" + names[y] + "'";
                Error("execution", err);
            }
            current = c_var->variable_get_value(names[y]);
        }

        if (!is_typed_non_predefined(current->type))
        {
            string err = "type '" + names[0] + "' has not attribute '" + names[names.size() - 1] + "'";
            Error("execution", err);
        }

        c_var = (Mtmc_class_variable *)current->content;

        if (c_var->parent_class->function_exists(names[names.size() - 1]))
        { // On exectute maintenant la fonction qui est extraite de la variable :
            // La fonction extraite de la variable
            Mtmc_functions *func = c_var->parent_class->function(names[names.size() - 1]);

            vector<Node *> args;
            vector<Mtmc_variable *> args2;
            if (instruction->children[0]->children.size() != 0)
            {
                args = get_listed_values(instruction->children[0]->children[0]);
                args2 = compute_listed_values(args, reference);
            }
            vector<Mtmc_variable *> args3;
            args3.push_back(current); // Correspond au 'self'
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
            map<string, Mtmc_variable *> variable_bis;
            Mtmc_variable *var = execute_function(func->value, args3, normal_args, variable_bis, recursive, reference);
            return var;
        }
        else
        {
            string err = "the object '" + names[0] + "' (of type '" + c_var->name + "') have no attribute '" + names[names.size() - 1] + "'";
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
                args2 = compute_listed_values(args, reference);
            }
            vector<Node *> normal_args = functions[index]->args;
            if (args.size() != normal_args.size())
            {
                string err = "the number of arguments of the function being called '" + instruction->value + "' is not the same of the function that has been precedently defined \nexpected " + to_string(normal_args.size()) + " arguments, got " + to_string(args.size());
                Error("execution", err);
            }
            map<string, Mtmc_variable *> variable_bis;
            Mtmc_variable *var = execute_function(functions[index]->value, args2, normal_args, variable_bis, recursive, reference);
            return var;
        }
        else
        {
            // C'est donc un pochoir à structures
            Mtmc_class_template *temp = functions[index]->temp;
            Mtmc_class_variable *var = generate_structured_variable(temp, reference);

            Mtmc_variable *n = new Mtmc_variable();
            n->type = "type:" + var->name;
            n->content = (void *)var;
            return n;
        }
    }

    return new Mtmc_variable();
}

Mtmc_variable *funcall_operator(string name, Mtmc_variable *arg1, Mtmc_variable *arg2, vector<string> reference)
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
    map<string, Mtmc_variable *> variable_bis;
    Mtmc_variable *var = execute_function(functions[index]->value, args2, normal_args, variable_bis, 0, reference);
    return var;
}

void compute_operator_rpn(vector<string> names, int &i, vector<string> reference, vector<string> types)
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
            computes.push(funcall_operator(f, a, b, reference));
        }
        else if (names[i] == "~")
        {
            if (i + 1 < names.size())
            {
                Mtmc_variable *r = alu_negativ(get_value_from_undefined_string(names, types, i));
                computes.push(r);
                i += 1;
            }
            else
            {
                Error("execution", "can't have a lonely operator `~`\nWarning : current compute mode is in rpn");
            }
        }
        else
        {
            string err = "unknown operator '" + names[i] + "'";
            Error("execution", err);
        }
    }
    else 
    {
        if (names[i] == "~")
        {
            if (i + 1 < names.size())
            {
                Mtmc_variable *r = alu_negativ(get_value_from_undefined_string(names, types, i));
                computes.push(r);
                i += 1;
            }
            else
            {
                Error("execution", "can't have a lonely operator `~`\nWarning : current compute mode is in rpn");
            }
        }
        else 
        {
            string err = "unknown operator '" + names[i] + "'";
            Error("execution", err);
        }
    }
}

void compute_operator_normal(vector<string> names, vector<string> types, int &i, bool funcall, map<string, Mtmc_variable *> &variable_function, Node *trunc, int recursive, vector<string> reference)
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
            Mtmc_variable_init(b);
            // Assigner la variable ou appeler la fonction
            if (funcall)
            {
                if (b_prime[0] == '!')
                {
                    // On sauvegarde l'état actuel du stack
                    stack<Mtmc_variable *> save = computes;
                    Mtmc_variable *ret = funcall_base_function(trunc->children[i + 1], variable_function, recursive, reference);
                    computes = save;
                    b = ret;
                }
                else if (variable_exist_function(b_prime, variable_function))
                {
                    b = variable_get_value_function(b_prime, variable_function);
                }
                else if (explicit_variable(b_prime))
                {
                    b = new_mtmc_variable_from_string(b_prime);
                }
                else if (types[i + 1] == "array")
                {
                    Mtmc_listed *childes = new_mtmc_listed_from_string(names[i + 1]);
                    Mtmc_variable *n = new Mtmc_variable();
                    n->type = "list";
                    n->content = (void *)childes;
                    computes.push(n);
                }
                else if (is_object_attribute(b_prime))
                {
                    vector<string> names = object_atribute_separator(b_prime);
                    if (!variable_exist_function(names[0], variable_function))
                    {
                        string err = "unknown variable '" + names[0] + "'";
                        Error("execution", err);
                    }
                    Mtmc_variable *n = variable_get_value_function(names[0], variable_function);
                    Mtmc_class_variable *n_prime;

                    if (!is_typed_non_predefined(n->type))
                    {
                        string err = "type '" + n->type + "' has no attribute '" + names[1] + "'";
                        Error("execution", err);
                    }
                    n_prime = (Mtmc_class_variable *)n->content;

                    Mtmc_variable *current = new Mtmc_variable(*n);

                    for (int y = 1; y < names.size(); ++y)
                    { // PLusieurs attributs
                        if (!is_typed_non_predefined(current->type))
                        {
                            string err = "type '" + n->type + "' has no attribute";
                            Error("execution", err);
                        }
                        n_prime = (Mtmc_class_variable *)current->content;
                        if (!n_prime->variable_exists(names[y]))
                        {
                            string err = "object '" + add_to(names, y - 1) + "' has not attribute '" + names[y] + "'";
                            Error("execution", err);
                        }
                        current = n_prime->variable_get_value(names[y]);
                    }
                    b = current;
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
                    Mtmc_variable *ret = funcall_base(trunc->children[i + 1], 0, reference);
                    computes = save;
                    b = ret;
                }
                else if (variable_exist(b_prime))
                {
                    b = variable_get_value(b_prime);
                }
                else if (explicit_variable(b_prime))
                {
                    b = new_mtmc_variable_from_string(b_prime);
                }
                else if (types[i + 1] == "array")
                {
                    Mtmc_listed *childes = new_mtmc_listed_from_string(names[i + 1]);
                    b->type = "list";
                    b->content = (void *)childes;
                }
                else if (is_object_attribute(b_prime))
                { // se présente sous la forme obj.atribute
                    vector<string> names = object_atribute_separator(b_prime);
                    if (!variable_exist(names[0]))
                    {
                        string err = "unknown variable '" + names[0] + "'";
                        Error("execution", err);
                    }
                    Mtmc_variable *n = variable_get_value(names[0]);
                    Mtmc_class_variable *n_prime;

                    if (!is_typed_non_predefined(n->type))
                    {
                        string err = "type '" + n->type + "' has no attribute '" + names[1] + "'";
                        Error("execution", err);
                    }
                    n_prime = (Mtmc_class_variable *)n->content;

                    Mtmc_variable *current = new Mtmc_variable(*n);

                    for (int y = 1; y < names.size(); ++y)
                    { // PLusieurs attributs
                        if (!is_typed_non_predefined(current->type))
                        {
                            string err = "type '" + n->type + "' has no attribute";
                            Error("execution", err);
                        }
                        n_prime = (Mtmc_class_variable *)current->content;
                        if (!n_prime->variable_exists(names[y]))
                        {
                            string err = "object '" + add_to(names, y - 1) + "' has not attribute '" + names[y] + "'";
                            Error("execution", err);
                        }
                        current = n_prime->variable_get_value(names[y]);
                    }
                    b = current;
                }
                else
                {
                    string err = "unkown variable or expression `" + b_prime + "`";
                    Error("execution", err);
                }
            }
            if (is_typed_non_predefined(a->type))
            {
                computes.push(magic_method_execution(names[i], b, a, funcall, variable_function, recursive));
            }
            else
            {
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
                else if (names[i] == "~")
                {
                    if (i + 1 < names.size())
                    {
                        Mtmc_variable *r = alu_negativ(get_value_from_undefined_string(names, types, i));
                        computes.push(r);
                    }
                    else
                    {
                        Error("execution", "can't have a lonely operator `~`\nWarning : current compute mode is in normal numeration mode");
                    }
                }
                else if (operator_exists(names[i]))
                {
                    string f = operator_function(names[i]);
                    computes.push(funcall_operator(f, a, b, reference));
                }
                else
                {
                    string err = "unknown operator '" + names[i] + "'";
                    Error("execution", err);
                }

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
        if (names[i] == "~")
        {
            if (i + 1 < names.size())
            {
                Mtmc_variable *r = alu_negativ(get_value_from_undefined_string(names, types, i));
                computes.push(r);
            }
            else
            {
                Error("execution", "can't have a lonely operator `~`\nWarning : current compute mode is in normal numeration mode");
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

Mtmc_variable *get_value_from_undefined_string(vector<string> names, vector<string> types, int &i)
{
    if (variable_exist(names[i]))
    {
        return variable_get_value(names[i]);
    }
    else if (explicit_variable(names[i]))
    {
        Mtmc_variable *n = new_mtmc_variable_from_string(names[i]);
        return n;
    }
    else if (types[i] == "array")
    {
        Mtmc_listed *childes = new_mtmc_listed_from_string(names[i]);
        Mtmc_variable *n = new Mtmc_variable();
        n->type = "list";
        n->content = (void *)childes;
        return n;
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
        Mtmc_class_variable *n_prime;

        if (!is_typed_non_predefined(n->type))
        {
            string err = "type '" + n->type + "' has no attribute '" + names_of_objects[1] + "'";
            Error("execution", err);
        }
        n_prime = (Mtmc_class_variable *)n->content;

        Mtmc_variable *current = new Mtmc_variable(*n);

        for (int y = 1; y < names_of_objects.size(); ++y)
        { // PLusieurs attributs
            if (!is_typed_non_predefined(current->type))
            {
                string err = "type '" + n->type + "' has no attribute";
                Error("execution", err);
            }
            n_prime = (Mtmc_class_variable *)current->content;
            if (!n_prime->variable_exists(names_of_objects[y]))
            {
                string err = "object '" + add_to(names_of_objects, y - 1) + "' has not attribute '" + names_of_objects[y] + "'";
                Error("execution", err);
            }
            current = n_prime->variable_get_value(names_of_objects[y]);
        }
        return current;
    }
    else
    {
        string err = "unkown variable or expression `" + names[i] + "`";
        Error("execution", err);
    }
    return nullptr;
}

Mtmc_variable *compute(Node *trunc, bool empty_stack, bool funcall, vector<string> reference)
{
    vector<string> names = childs_value(trunc);
    vector<string> types = childs_type(trunc);

    Mtmc_variable *a = new Mtmc_variable();
    Mtmc_variable *b = new Mtmc_variable();
    for (int i = 0; i < names.size(); i++)
    {
        if (types[i] == "funcall")
        {
            // On sauvegarde l'état actuel du stack
            stack<Mtmc_variable *> save = computes;
            Mtmc_variable *ret = funcall_base(trunc->children[i], 0, reference);
            computes = save;
            computes.push(ret);
        }
        else if (is_operator(names[i]))
        {
            if (compute_norme == 0) // rpn
            {
                if (computes.size() > 1)
                {
                    b = computes.top();
                    computes.pop();
                    a = computes.top();
                    if (is_typed_non_predefined(a->type))
                    {
                        computes.push(magic_method_execution(names[i], b, a, funcall, map<string, Mtmc_variable *>(), 0));
                    }
                    else
                    {
                        computes.push(a);
                        computes.push(b);
                        compute_operator_rpn(names, i, reference, types);
                    }
                }
            }
            else // pas rpn
            {
                map<string, Mtmc_variable *> prout;
                compute_operator_normal(names, types, i, false, prout, trunc, 0, reference);
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
            computes.push(get_value_from_undefined_string(names, types, i));
        }
    }
    Mtmc_variable *ret = computes.top();
    if (empty_stack)
    {
    }
    // empty_compute_stack();
    return ret;
}

vector<Mtmc_variable *> compute_listed_values(vector<Node *> listed_values, vector<string> reference)
{
    // le vector est sous la forme * ¬ 3
    //                               ¬ 4
    //                               ¬ +
    // args[0]->children[0]->value = 3
    vector<Mtmc_variable *> ret;
    for (auto val : listed_values)
    {
        ret.push_back(compute(val, true, false, reference));
    }
    return ret;
}

// ————————————————————————————————————————————————————————————————————————————————————————————————

int visitor(vector<Node *> ast, bool ast_show, bool debug, bool strict, vector<string> reference)
{
    int index = 0;
    while (index != ast.size())
    {
        Node *instruction = ast[index];
        current_reference = instruction->ref;
        threaded_references.push(current_reference);
        if (instruction->value == "expr")
        {
            Mtmc_variable *result = compute(instruction->children[0], true, false, reference);
            if (instruction->children.size() > 1 and instruction->children[1]->value == "->")
            {
                if (instruction->children[2]->children.size() == 3) // pour les assigantions des listes genre '3 -> liste*3'
                {
                    // Normalement, sa se présente soit liste*index soit liste*[index]
                    // On crée donc un sous noeud qui contient une fois la liste, pour pouvoir utiliser la fonction de calcule
                    Node *n = new Node();
                    n->children.push_back(instruction->children[2]->children[0]);
                    Mtmc_variable *list_to_mod = compute(n, false, false, reference);

                    if (list_to_mod->type != "list")
                    {
                        string err = "the type of the list being modified must be 'list', not '" + list_to_mod->type + "'";
                        Error("execution", err);
                    }

                    // Pareil
                    n = new Node();
                    n->children.push_back(instruction->children[2]->children[2]);
                    Mtmc_variable *list_index = compute(n, false, false, reference);

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
                        Mtmc_variable *r = new Mtmc_variable();
                        r->type = result->type;
                        r->content = result->content;
                        string name = instruction->children[2]->children[0]->value;
                        if (variable_exist(name))
                        {
                            variable_override(r, name);
                        }
                        variable_asignement(r, name);
                    }
                    else
                    { // Si c'est un attribut d'objet que on modifie
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
                        Mtmc_variable *r = new Mtmc_variable();
                        r->type = result->type;
                        r->content = result->content;
                        string name = names_of_objects[1];
                        n_prime->variable_override(r, name);
                    }
                }
            }
            empty_compute_stack_deletion();
        }
        else if (instruction->value == "keyword")
        {
            Node *args = instruction->children[1];
            empty_compute_stack();
            vector<string> s_args = childs_value(args);
            compute(args, false, false, reference);

            if (instruction->children[0]->value == "push")
            {
                keyword_push(s_args, reference);
            }
            else if (instruction->children[0]->value == "type")
            {
                keyword_type(s_args);
            }
            else if (instruction->children[0]->value == "convert")
            {
                keyword_convert(s_args);
            }
            else if (instruction->children[0]->value == "include")
            {
                keyword_include(s_args);
            }
            else if (instruction->children[0]->value == "length")
            {
                keyword_length(s_args);
            }
            else if (instruction->children[0]->value == "pop")
            {
                keyword_pop(s_args);
            }
            else if (instruction->children[0]->value == "operator")
            {
                keyword_operator(s_args);
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
            Mtmc_variable *condition_value = compute(condition, true, false, reference);
            if (!Mtmc_variable_type_check(condition_value, "bool"))
            {
                Error("execution", "condition of an if statement needs to be a boolean");
            }
            string cond = *(string *)condition_value->content;
            map<string, Mtmc_variable *> prout;
            if (cond == "true")
            {
                vector<Node *> paterns = recognize_paternes(instruction->children[2], true);
                normal_code_execution(paterns, false, prout, 0, false, reference); // Qui correspond au {}
            }
            else if (instruction->children.size() > 3 and instruction->children[3]->value == "else")
            {
                vector<Node *> paterns = recognize_paternes(instruction->children[4], true);
                normal_code_execution(paterns, false, prout, 0, false, reference); // Qui correspond au {} du else statement
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
                args = get_listed_values(instruction->children[1]->children[0]); // (*)
                if (instruction->children[2]->children.size() == 0)
                {
                    Error("execution", "when there are arguments in your function definition, there needs to be therefore anti-arguments (between brackets)");
                }
                antiargs = get_listed_values(instruction->children[2]->children[0]); // [*]
            }
            vector<Node *> paterns_value = recognize_paternes(instruction->children[3], true); // {*}
            if (instruction->children[0]->children[0]->value == "MTMC_DEBUG_FUNC")
            {
                Node *trunc = new Node();
                trunc->children = paterns_value;
                cout << ast_t_rep(trunc, 0) << endl;
            }
            function_asignement(instruction->children[0]->children[0]->value, paterns_value, args, antiargs);
        }
        else if (instruction->value == "while_loop")
        {
            vector<Node *> paterns = recognize_paternes(instruction->children[2], true);
            map<string, Mtmc_variable *> variable_function;
            string ret = while_loop(paterns, false, variable_function, 0, get_listed_values(instruction->children[1]->children[0]), reference);
        }
        else if (instruction->value == "for_loop")
        {
            vector<Node *> paterns = recognize_paternes(instruction->children[2], true);
            map<string, Mtmc_variable *> variable_function;
            string ret = for_loop(paterns, false, variable_function, 0, get_listed_values(instruction->children[1]->children[0]), reference); // Qui correspond au {} du else statementif (ret == "return")
        }
        else if (instruction->value == "struct")
        {
            string name = instruction->children[1]->children[0]->value;
            vector<Node *> trunc = recognize_paternes(instruction->children[2], true);

            Mtmc_class_template *s_template = generate_structure(name, trunc, reference);
            Mtmc_functions *f = new Mtmc_functions();
            f->name = "!" + name;
            f->is_structure_generator = true;
            f->temp = s_template;
            functions.push_back(f);
        }
        output_display_pile();
        manage_system_pile();
        index++;
        threaded_references.pop();
    }
    return 0;
}
