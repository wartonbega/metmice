#include <string>
#include <map>
#include <vector>

#include "../include/node.h"
#include "../include/variables.hpp"
#include "../include/usefull.h"

#include "../include/visitor.h"
#include "../include/types.h"

#include "../include/main.hpp"

#include "../include/code_execution.h"
#include "../include/paterns.h"
#include "../include/paterns_debug.h"

#include "../include/types.h"
#include "../include/types_check.h"
#include "../include/conversion.h"

#include "../include/keywords.h"

string normal_code_execution(vector<Node *> ast, bool function, map<string, Mtmc_variable *> &variable_function, int recursive, bool loop, vector<string> reference)
{
    int index = 0;
    string what_to_return = "";
    while (index != ast.size())
    {
        Node *instruction = ast[index];

        current_reference = instruction->ref;
        threaded_references.push(current_reference);
        if (instruction->value == "expr")
        {
            Mtmc_variable *result;
            if (function)
            {                                                                                                  // Si on se trouve dans l'execution d'une fonction
                result = compute_function(instruction->children[0], true, true, variable_function, recursive, reference); // Des calculs chiants
            }
            else
            {
                result = compute(instruction->children[0], true, false, reference);
            }
            // Dans le cas ou l'utilisateur assigne cette valeur à une variable
            if (instruction->children.size() > 1 and instruction->children[1]->value == "->")
            {
                if (instruction->children[2]->children.size() == 3) // pour les assigantions des listes genre '3 -> liste*3'
                {
                    // Normalement, sa se présente soit liste*index soit liste*[index]
                    // On crée donc un sous noeud qui contient une fois la liste, pour pouvoir utiliser la fonction de calcule
                    Node *n = new Node();
                    n->children.push_back(instruction->children[2]->children[0]);
                    Mtmc_variable *list_to_mod;
                    if (function)
                    {
                        list_to_mod = compute_function(n, false, function, variable_function, recursive, reference);
                    }
                    else
                    {
                        list_to_mod = compute(n, false, false, reference);
                    }

                    if (list_to_mod->type != "list")
                    {
                        string err = "the type of the list being modified must be 'list', not '" + list_to_mod->type + "'";
                        Error("execution", err);
                    }

                    // Pareil
                    n = new Node();
                    n->children.push_back(instruction->children[2]->children[2]);
                    Mtmc_variable *list_index;
                    if (function)
                    {
                        list_index = compute_function(n, false, true, variable_function, recursive, reference);
                    } else 
                    {
                        list_index = compute(n, false, false, reference);
                    }

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
                    else 
                    {
                        string err = "the index of the list must be of type 'int' or 'list' not type '" + list_index->type + "'";
                        Error("execution", err);
                    }
                }
                else if (instruction->children[2]->children.size() == 1)
                {
                    // Asignation du nom à la variable

                    if (!is_object_attribute(instruction->children[2]->children[0]->value))
                    {
                        string name = instruction->children[2]->children[0]->value; // Asignation du nom à la variable
                        if (function)
                        {
                            if (variable_exist_function(name, variable_function))
                            {
                                variable_override_function(result, variable_function, name);
                            }
                            else
                                variable_asignement_function(result, variable_function, name);
                        }
                        else if (variable_exist(name))
                        {
                            variable_override(result, name);
                        }
                        variable_asignement(result, name);
                    }
                    else
                    {
                        if (!function)
                        { 
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
                        else
                        {
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
        }
        else if (instruction->value == "keyword")
        {
            
            Node *args;
            vector<string> s_args;
            if (instruction->children.size() > 1)
            { // Il y a des mot-clés qui n'ont pas d'argument ou qui peuvent ne pas en avoir : break, continue, return
                args = instruction->children[1];
                s_args = childs_value(args);
                if (function)
                {
                    compute_function(args, false, false, variable_function, recursive, reference);
                }
                else
                    compute(args, false, false, reference);
            }

            if (instruction->children[0]->value == "push")
            {
                keyword_push(s_args, reference);
            }
            else if (instruction->children[0]->value == "convert")
            {
                keyword_convert(s_args);
            }
            else if (instruction->children[0]->value == "length")
            {
                keyword_length(s_args);
            }
            else if (instruction->children[0]->value == "type")
            {
                keyword_type(s_args);
            }
            else if (instruction->children[0]->value == "return")
            {
                if (function)
                {
                    what_to_return = "return";
                    threaded_references.pop();
                    break;
                }
            }
            else if (instruction->children[0]->value == "break")
            {
                if (loop)
                {
                    what_to_return = "break";
                    threaded_references.pop();
                    break;
                }
                else 
                {
                    Error("execution", "'break' keyword can only be used in a loop");
                }
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
            if (!function)
            {
                Mtmc_variable *condition_value = compute(condition, true, function, reference);
                string result;
                if (!Mtmc_variable_type_check(condition_value, "bool"))
                {
                    Error("execution", "condition of an if statement needs to be a boolean");
                }
                string cond = *(string *)condition_value->content;
                if (cond == "true")
                {
                    vector<Node *> paterns = recognize_paternes(instruction->children[2], true);
                    result = normal_code_execution(paterns, 0, variable_function, 0, loop, reference); // Qui correspond au {}
                }
                else if (instruction->children.size() > 3 and instruction->children[3]->value == "else")
                {
                    vector<Node *> paterns = recognize_paternes(instruction->children[4], true);
                    result = normal_code_execution(paterns, 0, variable_function, 0, loop, reference); // Qui correspond au {} du else statement
                }
                if (result == "return")
                {
                    what_to_return = "return";
                    threaded_references.pop();
                    break;
                }
                else if (result == "break")
                {
                    what_to_return = "break";
                    threaded_references.pop();
                    break;
                }
            }
            else
            {
                Mtmc_variable *condition_value = compute_function(condition, true, function, variable_function, recursive, reference);
                string result;
                if (!Mtmc_variable_type_check(condition_value, "bool"))
                {
                    Error("execution", "condition of an if statement needs to be a boolean");
                }
                string cond = *(string *)condition_value->content;

                if (cond == "true")
                {
                    vector<Node *> paterns = recognize_paternes(instruction->children[2], true);
                    result = normal_code_execution(paterns, function, variable_function, 0, loop, reference); // Qui correspond au {}
                }
                else if (instruction->children.size() > 3 and instruction->children[3]->value == "else")
                {
                    vector<Node *> paterns = recognize_paternes(instruction->children[4], true);
                    result = normal_code_execution(paterns, function, variable_function, 0, loop, reference); // Qui correspond au {} du else statement
                }
                if (result == "return")
                {
                    what_to_return = "return";
                    threaded_references.pop();
                    break;
                }
                else if (result == "break")
                {
                    what_to_return = "break";
                    threaded_references.pop();
                    break;
                }
            }
        }
        else if (instruction->value == "for_loop")
        {
            vector<Node *> paterns = recognize_paternes(instruction->children[2], debug);
            vector<Node *> args_for_loop = get_listed_values(instruction->children[1]->children[0]);
            string ret = for_loop(paterns, function, variable_function, recursive, args_for_loop, reference);
            if (ret == "return")
            {
                what_to_return = "return";
                threaded_references.pop();
                break;
            }
        }
        output_display_pile();
        manage_system_pile();
        index++;
        threaded_references.pop();
    }
    return what_to_return;
}
