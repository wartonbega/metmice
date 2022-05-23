#include <string>
#include <vector>

#include "../include/node.h"
#include "../include/variables.hpp"
#include "../include/structures.h"
#include "../include/paterns.h"
#include "../include/visitor.h"
#include "../include/code_execution.h"
#include "../include/usefull.h"
#include "../include/types_check.h"

Mtmc_functions *structure_function_asignement(string name, vector<Node *> paterns, vector<Node *> args, vector<Node *> antiargs)
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
    return f;
}

string operator_to_magic_name(string op)
{
    if (op == "+")
    {
        return "!magic_plus";
    }
    if (op == "-")
    {
        return "!magic_minus";
    }
    if (op == "*")
    {
        return "!magic_times";
    }
    if (op == "=")
    {
        return "!magic_equals";
    }
    if (op == "/")
    {
        return "!magic_div";
    }
    if (op == "|")
    {
        return "!magic_floordiv";
    }
    if (op == "%")
    {
        return "!magic_modulo";
    }
    if (op == ">")
    {
        return "!magic_gt";
    }
    if (op == "<")
    {
        return "!magic_lt";
    }
    if (op == "^")
    {
        return "!magic_lt";
    }
    string err = "unknown operator '" + op + "' for a structure's magic-method. Maybe it not a predefined operator (defined using the keyword 'operator'). Beware, it can come from the standard librairy";
    Error("execution", err);
    return "";
}

Mtmc_variable *magic_method_execution(string op, Mtmc_variable *arg2, Mtmc_variable *main_arg, bool function, map<string, Mtmc_variable *> variables_function, int recursive)
{
    // la vérification du type de main_arg doit être fait au préalable : main_arg -> Mtmc_class_variable ou !is_type_non_predifined(type) (viens de type_check.h)
    Mtmc_class_variable *arg1 = (Mtmc_class_variable *)main_arg->content;
    Mtmc_functions *func = arg1->parent_class->function(operator_to_magic_name(op));

    if (func->args.size() != 2)
    {
        Error("execution", "magic-methods can only support 2 arguments : 'self' and the second element of the operation");
    }
    vector<Mtmc_variable *> arguments;
    arguments.push_back(main_arg); // 'self'
    arguments.push_back(arg2);     // le deuxième element de l'opération ( a + b <--)
    Mtmc_variable *ret = execute_function(func->value, arguments, func->args, variables_function, recursive, vector<string>());
    return ret;

}

void magic_methods(Mtmc_class_template *temp)
{
    /*
    On as comme magic methods :
        magic_plus
        magic_minus
        magic_times
        magic_equals
        magic_power
        magic_div
        magic_floordiv
        magic_modulo
        magic_gt
        magic_lt
    Chacune prend en argument le deuxième élément.
    Si c'est deux objets qui ont chacun une magic-method, c'est le premier qui est appelé qui prend le contrôle du calcule

    La fonction "template" qui génère les magic_methods est :
        MTMC_MAGIC_FUNCTION_GLOBAL_TEMPLATE
        => Doit être include de la std lib
    */

    if (!function_exists("!MTMC_MAGIC_FUNCTION_GLOBAL_TEMPLATE"))
    {
        string err = "you need to include file 'struct_magic_functions.mtmc' or more generally 'std.mtmc' in order to create magic methods for your structs";
        Error("execution", err);
    }
    // la fonction template qui est dans "lib/struct_magic_functions.mtmc"
    Mtmc_functions *mag_mt_temp = function_get("!MTMC_MAGIC_FUNCTION_GLOBAL_TEMPLATE");

    // Toutes les magic-methods qui sont générés depuis le meme template
    // Elles ne font que renvoyer l'objet (return self)
    // Peuvent être override
    Mtmc_functions *magic_plus = new Mtmc_functions(*mag_mt_temp);
    magic_plus->name = "!magic_plus";
    Mtmc_functions *magic_minus = new Mtmc_functions(*mag_mt_temp);
    magic_minus->name = "!magic_minus";
    Mtmc_functions *magic_times = new Mtmc_functions(*mag_mt_temp);
    magic_times->name = "!magic_times";
    Mtmc_functions *magic_equals = new Mtmc_functions(*mag_mt_temp);
    magic_equals->name = "!magic_equals";
    Mtmc_functions *magic_power = new Mtmc_functions(*mag_mt_temp);
    magic_power->name = "!magic_power";
    Mtmc_functions *magic_div = new Mtmc_functions(*mag_mt_temp);
    magic_div->name = "!magic_div";
    Mtmc_functions *magic_floordiv = new Mtmc_functions(*mag_mt_temp);
    magic_floordiv->name = "!magic_floordiv";
    Mtmc_functions *magic_modulo = new Mtmc_functions(*mag_mt_temp);
    magic_modulo->name = "!magic_modulo";
    Mtmc_functions *magic_gt = new Mtmc_functions(*mag_mt_temp);
    magic_gt->name = "!magic_gt";
    Mtmc_functions *magic_lt = new Mtmc_functions(*mag_mt_temp);
    magic_lt->name = "!magic_lt";

    if (!temp->function_exists("!magic_plus"))
        temp->function_override("!magic_plus", magic_plus);
    if (!temp->function_exists("!magic_minus"))
        temp->function_override("!magic_minus", magic_minus);
    if (!temp->function_exists("!magic_times"))
        temp->function_override("!magic_times", magic_times);
    if (!temp->function_exists("!magic_equals"))
        temp->function_override("!magic_equals", magic_equals);
    if (!temp->function_exists("!magic_power"))
        temp->function_override("!magic_power", magic_power);
    if (!temp->function_exists("!magic_div"))
        temp->function_override("!magic_div", magic_div);
    if (!temp->function_exists("!magic_floordiv"))
        temp->function_override("!magic_floordiv", magic_floordiv);
    if (!temp->function_exists("!magic_modulo"))
        temp->function_override("!magic_modulo", magic_modulo);
    if (!temp->function_exists("!magic_gt"))
        temp->function_override("!magic_gt", magic_gt);
    if (!temp->function_exists("!magic_lt"))
        temp->function_override("!magic_lt", magic_lt);
}

Mtmc_class_template *generate_structure(string name, vector<Node *> trunc, vector<string> reference)
{
    /*
    génère un pochoir à structure
    */
    Mtmc_class_template *result = new Mtmc_class_template();
    result->name = name; // Le nom de la structure

    // Le code autorise uniquement la déclaration de fonctions et de variables
    int index = 0;
    while (index != trunc.size())
    {
        Node *instruction = trunc[index];
        if (instruction->value == "expr")
        {
            result->computings_to_do.push_back(instruction);
            // On ne fait que stocker le code brut des variables, c'est à dire leurs noms, et valeur non calculées,
            // pour ne pas avoir des erreurs d'objets dupliqué d'une structure à l'autre
        }
        else if (instruction->value == "func")
        {
            // La définition des méthodes
            vector<Node *> args;
            vector<Node *> antiargs;
            if (instruction->children[1]->children.size() != 0)
            {
                args = get_listed_values(instruction->children[1]->children[0]);     // (*)
                antiargs = get_listed_values(instruction->children[2]->children[0]); // [*]
            }
            // On n'analyse pas entièremement le code (pas de compute) mais on garde le `raw` code en mémoire pour ne pas à évaluer à chaques créations
            vector<Node *> paterns_value = recognize_paternes(instruction->children[3], true); // {*}
            Mtmc_functions *f = structure_function_asignement(instruction->children[0]->children[0]->value, paterns_value, args, antiargs);
            result->function_override(f->name, f); // Soit la fonction est réécrite, soit elle est définie
        }
        else
        {
            string err = "invalid instruction type '" + instruction->value + "' in a structure definition\nPossible declarations are: `expr` and `func`";
            Error("execution", err);
        }
        index++;
    }
    // On définis les magic_methods qui n'ont pas encore été définies :
    magic_methods(result);
    return result;
}

Mtmc_class_variable *generate_structured_variable(Mtmc_class_template *parent, vector<string> reference)
{
    Mtmc_class_variable *result = new Mtmc_class_variable();
    result->name = parent->name;
    result->parent_class = parent;
    map<string, Mtmc_variable *> new_variables;

    for (int i = 0; i < parent->computings_to_do.size(); i++)
    {
        // Ici, on récupère le code brut de la définition des attributs pour les calculers
        Node *instruction = parent->computings_to_do[i];
        Mtmc_variable *r = compute(instruction->children[0], true, false, reference);
        if (instruction->children.size() > 1 and instruction->children[1]->value == "->")
        {
            // On assigne à une nouvelle map de variables pour avoir la main mise sur les valeurs
            if (instruction->children[2]->children.size() == 1)
            {
                // Asignation du nom à la variable
                string name = instruction->children[2]->children[0]->value;
                new_variables[name] = r;
            }
            else
            {
                Error("execution", "attributes can't have a complex declaration");
            }
        }
    }

    // Copie les variables de la classe parente dans la variable fille
    vector<string> names = get_keys(new_variables);

    for (int i = 0; i < names.size(); i++)
    { // On fait une deepcopy de un à les les nouveaux éléments pour ne pas avoir des problèmes de mémoire désallouée
        string name = names[i];
        Mtmc_variable *v = new_variables[name];
        result->atributes[name] = v;
    }
    return result;
}