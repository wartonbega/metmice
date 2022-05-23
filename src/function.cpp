
#include <vector>
#include <string>
#include <map>

#include "../include/node.h"
#include "../include/variables.hpp"
#include "../include/code_execution.h"
#include "../include/visitor.h"
#include "../include/usefull.h"
#include "../include/main.hpp"
#include "../include/types.h"
#include "../include/types_check.h"
#include "../include/structures.h"
#include "../include/paterns.h"
#include "../include/paterns_debug.h"
#include "../include/conversion.h"
#include "../include/keywords.h"


bool variable_exist_function(string name, map<string, Mtmc_variable *> variable_function)
{
    if (variable_function.find(name) != variable_function.end())
    {
        return true;
    }
    if (variable_exist(name))
    {
        return true;
    }
    return false;
}

void variable_asignement_function(Mtmc_variable *var, map<string, Mtmc_variable *> &variable_function, string name)
{
    if (!mtmc_accept_var_name(name))
    {
        string err = "invalid name for a variable `" + name + "`";
        debug_error("execution", err);
    }
    variable_function[name] = var;
}

void variable_override_function(Mtmc_variable *var, map<string, Mtmc_variable *> &variable_function, string name)
{
    // On peut directement acceder à l'élement, vus que avant d'overrdie, on as checké si elle existe
    variables_t[name] = var;


    if (variable_exist(name))
    { // Si la variable est globale, alors on appel plutôt la définition d'une nouvelle variable à l'interieur de la fonction
        variable_asignement_function(var, variable_function, name); // On ne veut pas réécrire une variable globale
    }
}

Mtmc_variable *variable_get_value_function(string name, map<string, Mtmc_variable *> &variable_function)
{

    if (variable_function.find(name) != variable_function.end())
    {
        return variable_function[name];
    }
    if (variable_exist(name))
    {
        return variable_get_value(name);
    }
    return new Mtmc_variable();
}

Mtmc_variable *funcall_base_function(Node *instruction, map<string, Mtmc_variable *> variable_function, int recursive, vector<string> reference)
{
    if (is_object_attribute(instruction->value))
    { // soit quand on appel une fonction-attribut : "!obj.function()"
        vector<string> names = object_atribute_separator_function(instruction->value);

        Mtmc_variable *var = nullptr;
        if (variable_exist_function(names[0], variable_function))
        {
            var = variable_get_value_function(names[0], variable_function);
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

        Mtmc_class_variable *c_var = (Mtmc_class_variable *)var->content;

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
        {
            Mtmc_functions *func = c_var->parent_class->function(names[names.size() - 1]);

            vector<Node *> args;
            vector<Mtmc_variable *> args2;
            if (instruction->children[0]->children.size() != 0)
            {
                args = get_listed_values(instruction->children[0]->children[0]);
                args2 = compute_listed_values_function(args, variable_function, recursive, reference);
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
        // Récupérer le 'numéro' de la fonction : son index dans les tables de stockage des fonctions
        int index = function_index(instruction->value);

        // Si c'est une fonction normale :
        if (functions[index]->is_structure_generator == false)
        {
            vector<Node *> args;
            vector<Mtmc_variable *> args2;
            if (instruction->children[0]->children.size() != 0)
            {
                args = get_listed_values(instruction->children[0]->children[0]);
                args2 = compute_listed_values_function(args, variable_function, recursive, reference);
            }

            // Le nom des arguments de la fonction
            vector<Node *> normal_args = functions[index]->args;
            if (args.size() != normal_args.size())
            {
                string err = "the number of arguments of the function being called '" + instruction->value + "' is not the same of the function that has been precedently defined \nexpected " + to_string(normal_args.size()) + " arguments, got " + to_string(args.size());
                Error("execution", err);
            }
            map<string, Mtmc_variable *> variablen_function;
            Mtmc_variable *returning = execute_function(functions[index]->value, args2, normal_args, variablen_function, recursive + 1, reference);
            return returning;
        }
        else
        { // C'est donc un générateur de structures
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

Mtmc_variable *compute_function(Node *trunc, bool empty_stack, bool funcall, map<string, Mtmc_variable *> &variable_function, int recursive, vector<string> reference)
{
    // [note] : à réfléchir :  si il y a une varibale, récupérer sa valeur lors de l'utilisation de cette dernière
    // ou lors de l'empilement ?
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
            Mtmc_variable *ret = funcall_base_function(trunc->children[i], variable_function, recursive, reference);
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
                        computes.push(magic_method_execution(names[i], b, a, funcall, map<string, Mtmc_variable *>(), recursive));
                    }
                    else
                    {
                        computes.push(a);
                        computes.push(b);
                        compute_operator_rpn(names, i, reference, types);
                    }
                }
            }
            else
            {
                compute_operator_normal(names, types, i, true, variable_function, trunc, recursive, reference);
            }
        }
        else if (names[i] == ",")
        {
            a = computes.top();
            computes.pop();
            b = computes.top();
            computes.pop();
            computes.push(a);
            computes.push(b);
        }
        else
        {
            if (variable_exist_function(names[i], variable_function))
            {
                computes.push(variable_get_value_function(names[i], variable_function));
            }
            else if (explicit_variable(names[i]))
            {
                computes.push(new_mtmc_variable_from_string(names[i]));
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
                if (!variable_exist_function(names_of_objects[0], variable_function))
                {
                    string err = "unknown variable '" + names_of_objects[0] + "'";
                    Error("execution", err);
                }
                Mtmc_variable *n = variable_get_value_function(names_of_objects[0], variable_function);
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
                        string err = "type '" + current->type + "' has no attribute";
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
                computes.push(current);
            }
            else
            {
                string err = "unkown variable or expression `" + names[i] + "`";
                Error("execution", err);
            }
        }
    }
    Mtmc_variable *ret = nullptr;
    if (!computes.empty())
    {
        ret = computes.top();
    }
    if (empty_stack)
        empty_compute_stack();
    return ret;
}

vector<Mtmc_variable *> compute_listed_values_function(vector<Node *> listed_values, map<string, Mtmc_variable *> &variable_function, int recursive, vector<string> reference)
{
    vector<Mtmc_variable *> ret;
    for (auto val : listed_values)
    {
        ret.push_back(compute_function(val, true, false, variable_function, recursive, reference));
    }
    return ret;
}

//—————————————————————— Visitor ——————————————————————

Mtmc_variable *execute_function(vector<Node *> ast, vector<Mtmc_variable *> args, vector<Node *> normal_args, map<string, Mtmc_variable *> variable_function, int recursive, vector<string> reference)
{
    /*
    On présupose que la fonction est correcte, que le nombre d'arguments est le bon
    On executera la fonction ci-dessus
    On ne peux pas déclarer de nouvelles fonctions dans une fonction
    */

    if (recursive > maximum_recursion_depth)
    {
        string err = "maximum recursion depth reached : " + to_string(recursive);
        Error("execution", err);
    }

    Mtmc_variable *what_to_return = new_mtmc_variable_from_string("none"); // À la base, une fonction retourne "none"

    // Asigniation des arguments
    for (int i = 0; i < args.size(); ++i)
    {
        string name = normal_args[i]->children[0]->value;
        Mtmc_variable *var = new Mtmc_variable(*args[i]);
        if (variable_exist_function(name, variable_function))
        {
            variable_override_function(var, variable_function, name);
        }
        variable_asignement_function(var, variable_function, name);
    }

    int index = 0;

    // Début du "scan" de la fonction
    while (index != ast.size())
    {
        Node *instruction = ast[index];

        current_reference = instruction->ref;
        threaded_references.push(current_reference);

        if (instruction->value == "expr")
        {
            Mtmc_variable *result = compute_function(instruction->children[0], true, false, variable_function, recursive, reference);
            if (instruction->children.size() > 1 and instruction->children[1]->value == "->")
            {
                if (instruction->children[2]->children.size() == 3) // pour les assigantions des listes genre '3 -> liste*3'
                {
                    // Normalement, sa se présente soit liste*index soit liste*[index]
                    // On crée donc un sous noeud qui contient une fois la liste, pour pouvoir utiliser la fonction de calcule
                    Node *n = new Node();
                    n->children.push_back(instruction->children[2]->children[0]);
                    Mtmc_variable *list_to_mod = compute_function(n, false, false, variable_function, recursive, reference);

                    if (list_to_mod->type != "list")
                    {
                        string err = "the type of the list being modified must be 'list', not '" + list_to_mod->type + "'";
                        Error("execution", err);
                    }

                    // Pareil
                    n = new Node();
                    n->children.push_back(instruction->children[2]->children[2]);
                    Mtmc_variable *list_index = compute_function(n, false, false, variable_function, recursive, reference);

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

                    if (!is_object_attribute(instruction->children[2]->children[0]->value))
                    {
                        Mtmc_variable *r = new Mtmc_variable();
                        r->type = result->type;
                        r->content = result->content;
                        string name = instruction->children[2]->children[0]->value;
                        if (variable_exist_function(name, variable_function))
                        {
                            variable_override_function(r, variable_function, name);
                        }
                        else
                            variable_asignement_function(r, variable_function, name);
                    }
                    else
                    { // Si c'est un attrubut d'objet que on modifie
                        vector<string> names_of_objects = object_atribute_separator(instruction->children[2]->children[0]->value);
                        if (!variable_exist_function(names_of_objects[0], variable_function))
                        {
                            string err = "unknown variable '" + names_of_objects[0] + "'";
                            Error("execution", err);
                        }
                        Mtmc_variable *n = variable_get_value_function(names_of_objects[0], variable_function);
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
        }
        else if (instruction->value == "keyword")
        {
            Node *args_s = instruction->children[1];
            vector<string> s_args = childs_value(args_s);
            compute_function(args_s, false, false, variable_function, recursive, reference);

            if (instruction->children[0]->value == "push")
            {
                keyword_push(s_args, reference);
            }
            else if (instruction->children[0]->value == "convert")
            {
                keyword_convert(s_args);
            }
            else if (instruction->children[0]->value == "type")
            {
                keyword_type(s_args);
            }
            else if (instruction->children[0]->value == "return")
            {
                if (s_args.size() >= 1)
                {
                    what_to_return = computes.top();
                    computes.pop();
                }
                threaded_references.pop();
                break;
            }
            else if (instruction->children[0]->value == "length")
            {
                keyword_length(s_args);
            }
            else if (instruction->children[0]->value == "pop")
            {
                keyword_pop(s_args);
            }
            else if (instruction->children[0]->value == "global")
            {
                string name;
                if (int(s_args.size()) == 1)
                {
                    name = s_args[s_args.size() - 1];
                    computes.pop(); // pas besoin de la valeur mise sur le stack alors que on as juste besoin du nom de la variable
                }
                else
                    Error("execution", "global keyword needs one and only one argument"); // Eh oui ..
                if (variable_exist_function(name, variable_function))
                {
                    // On définis donc une variable global sur le nom et la valeur de celle qui existe dans la fonction
                    Mtmc_variable *value = variable_get_value_function(name, variable_function);
                    if (variable_exist(name))
                    {
                        variable_override(value, name);
                    }
                    else
                        variable_asignement(value, name);
                }
                else
                {
                    string err = "unknown variable `" + name + "` for a global asignement";
                    Error("execution", err);
                }
            }
            else
            {
                string err = "the keyword `" + instruction->children[0]->value + "` is not usable in the current state (function call, global execution, if-else statement ...)";
                Error("execution", err);
            }
        }
        else if (instruction->value == "if_statement")
        {
            Node *condition = instruction->children[1]->children[0];
            Mtmc_variable *condition_value = compute_function(condition, true, false, variable_function, recursive, reference);
            if (condition_value->type != "bool")
            {
                Error("execution", "condition for an if-else statement must be a boolean");
            }
            if (*(string *)condition_value->content == "true")
            {
                vector<Node *> paterns = recognize_paternes(instruction->children[2], true);
                string ret = normal_code_execution(paterns, true, variable_function, recursive, false, reference); // Qui correspond au {}
                if (ret == "return")
                {
                    what_to_return = computes.top();
                    computes.pop();
                    threaded_references.pop();
                    break;
                }
            }
            else if (instruction->children.size() > 3 and instruction->children[3]->value == "else")
            {
                vector<Node *> paterns = recognize_paternes(instruction->children[4], true);
                string ret = normal_code_execution(paterns, true, variable_function, 0, false, reference); // Qui correspond au {} du else statementif (ret == "return")
                if (ret == "return")
                {
                    if (computes.size() >= 1)
                    {
                        what_to_return = computes.top();
                        computes.pop();
                        threaded_references.pop();
                        break;
                    }
                    else
                        Error("execution", "compute stack can't be empty to pop");
                }
            }
        }
        else if (instruction->value == "func")
        {
            Error("execution", "can't define function inside a function");
        }
        else if (instruction->value == "funcall")
        {
            Error("internal", "funcall not implemeted yet");
        }
        else if (instruction->value == "while_loop")
        {
            vector<Node *> paterns = recognize_paternes(instruction->children[2], true);
            string ret = while_loop(paterns, true, variable_function, recursive, get_listed_values(instruction->children[1]->children[0]), reference);
            if (ret == "return")
            {
                what_to_return = computes.top();
                computes.pop();
                threaded_references.pop();
                break;
            }
        }
        else if (instruction->value == "for_loop")
        {
            vector<Node *> paterns = recognize_paternes(instruction->children[2], true);
            string ret = for_loop(paterns, true, variable_function, recursive, get_listed_values(instruction->children[1]->children[0]), reference); // Qui correspond au {} du else statementif (ret == "return")
            if (ret == "return")
            {
                what_to_return = computes.top();
                computes.pop();
                threaded_references.pop();
                break;
            }
        }
        else if (instruction->value == "struct")
        {
            string err = "can't have a structure definition inside of a function";
            Error("execution", err);
        }
        output_display_pile();
        manage_system_pile();
        index++;
        threaded_references.pop();
    }

    return what_to_return;
}
