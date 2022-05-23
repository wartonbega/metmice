#include <string>
#include <math.h>
#include <iostream>
using namespace std;

#include "../include/node.h"
#include "../include/variables.hpp"
#include "../include/types.h"
#include "../include/usefull.h"

#include "../include/conversion.h"
#include "../include/types_check.h"

bool explicit_variable(string v)
{
    if (is_string(v) || is_int(v) || is_bool(v) || is_float(v) || is_none(v) || is_list(v))
    {
        return true;
    }
    return false;
}

string get_string_content(string word)
{
    return get_from_string(word, 1, int(word.size()) - 1);
}

string get_type(string expr)
{
    if (explicit_variable(expr))
    {
        if (is_int(expr))
        {
            return "int";
        }
        else if (is_float(expr))
        {
            return "float";
        }
        else if (is_bool(expr))
        {
            return "bool";
        }
        else if (is_string(expr))
        {
            return "string";
        }
        else if (is_list(expr))
        {
            return "list";
        }
        else
            return "none";
    }
    else
        return "none";
}

Mtmc_variable *convert_to_spec(Mtmc_variable *start, string result_type)
{
    Mtmc_variable *full_result = new_mtmc_variable_from_string("none");
    string current_type = start->type;
    if (current_type == "int")
    {
        int64_t content = *(int64_t *)start->content;
        if (result_type == "string")
        {
            string *result = new string(to_string(to_string(content)));
            full_result->type = result_type;
            full_result->content = (void *)result;
            return full_result;
        }
        if (result_type == "float")
        {
            // cast the value content to a float
            float *result = new float(content);
            full_result->type = result_type;
            full_result->content = (void *)result;
            return full_result;
        }
        string err = "cannot convert type '" + current_type + "' to type '" + result_type + "'";
        Error("execution", err);
    }
    if (current_type == "float")
    {
        float content = *(float *)start->content;
        if (result_type == "string")
        {
            string *result = new string(to_string(content));
            full_result->type = result_type;
            full_result->content = (void *)result;
            return full_result;
        }
        if (result_type == "int")
        {
            int64_t *result = new int64_t(content);
            full_result->type = result_type;
            full_result->content = (void *)result;
            return full_result;
        }
        string err = "cannot convert type '" + current_type + "' to type '" + result_type + "'";
        Error("execution", err);
    }
    if (current_type == "bool")
    {
        string content = *(string *)start->content;
        if (result_type == "string")
        {
            string *result = new string(to_string(content));
            full_result->type = result_type;
            full_result->content = (void *)result;
            return full_result;
        }
        string err = "cannot convert type '" + current_type + "' to type '" + result_type + "'";
        Error("execution", err);
    }

    if (current_type == "list")
    {

        Mtmc_listed *content = (Mtmc_listed *)start->content;
        if (result_type == "string")
        {
            string *result = new string(to_string(Mtmc_listed_string(content)));
            full_result->type = result_type;
            full_result->content = (void *)result;
            return full_result;
        }
        string err = "cannot convert type '" + current_type + "' to type '" + result_type + "'";
        Error("execution", err);
    }

    // On est obligé de lancer une erreur sinon on aurais juste des variables de type 'none' qui se baladent partout
    string err = "cannot convert type '" + current_type + "' to type '" + result_type + "'";
    Error("execution", err);
    return full_result;
};