#include <map>
#include <string>
#include <vector>

using namespace std;

#include "../include/node.h"
#include "../include/variables.hpp"

#include "../include/main.hpp"
#include "../include/keywords.h"

#include "../include/visitor.h"
#include "../include/code_execution.h"
#include "../include/usefull.h"

#include "../include/types.h"
#include "../include/types_check.h"
#include "../include/conversion.h"


void keyword_push(vector<string> s_args, vector<string> reference)
{
    // push stack (int) element (any)
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
        push_display_pile(arg2, reference);
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

void keyword_type(vector<string> s_args)
{
    // type element (any)
    if (s_args.size() < 1 and computes.size() < 1)
    {
        Error("execution", "'type' keywords need at least one arguments");
    }
    Mtmc_variable *arg = computes.top();
    computes.pop();
    empty_compute_stack();

    Mtmc_variable *type = new_mtmc_variable_from_string(to_string(arg->type));
    computes.push(type);
}

void keyword_convert(vector<string> s_args)
{
    // convert "type" (string) element (any)
    if (s_args.size() < 2 and computes.size() < 2)
    {
        Error("execution", "'convert' keyword needs at least one argument");
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
    if (arg1->type != "string")
    {
        Error("execution", "the type indicated must be represented as a string. \nExample : convert \"string\" 3;");
    }
    string obj_type = del_string(*(string *)arg1->content);
    Mtmc_variable * converted = convert_to_spec(arg2, obj_type);

    computes.push(converted);
}

void keyword_include(vector<string> s_args)
{
    // include "librairy.mtmc"
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

void keyword_pop(vector<string> s_args)
{
    // pop index (int) liste (list)
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
    Mtmc_variable *element = Mtmc_listed_get(int(carg1), list);
    computes.push(element);
    Mtmc_listed_destuctor(int(carg1), list);
}

void keyword_operator(vector<string> s_args)
{
    // operator "operator" (string) "!function" (string)
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

void keyword_length(vector<string> s_args)
{
    // length liste (list)
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