#include <string>
#include <vector>
#include "../include/types_check.h"
#include "../include/node.h"
#include "../include/variables.hpp"
#include "../include/usefull.h"
#include "../include/main.hpp"


bool is_int(string c)
{
    if (consti(c, "0123456789-"))
    {
        return true;
    }
    return false;
}

bool is_string(string c)
{
    if (c[0] == '"' && c[c.length() - 1] == '"')
    {
        return true;
    }
    return false;
}

bool is_float(string c)
{
    if (consti(c, "0123456789.-") and number_of('.', c) == 1)
    {
        return true;
    }
    return false;
}

bool is_bool(string c)
{
    if (c == "true" or c == "false")
    {
        return true;
    }
    return false;
}

bool is_funcall(string c)
{
    if (c[0] == '!')
    {
        return true;
    }
    return false;
}

bool is_operator(string c)
{
    if (c == "-" || c == "+" || c == "=" || c == "*" || c == "^" || c == "/" || c == "%" || c == "|" || c == "<" || c == ">" || c == "~")
    {
        return true;
    }
    else
    {
        for (auto i : operators_n)
        {
            if (i == c)
            {
                return true;
            }
        }
        return false;
    }
    return false;
}

bool is_none(string c)
{
    if (c == "none")
    {
        return true;
    }
    return false;
}

bool is_list(string c)
{
    if (c[0] == '[' and c[c.size() - 1] == ']')
    {
        return true;
    }
    return false;
}

bool is_typed(string c)
{
    if (is_int(c) || is_string(c) || is_none(c) || is_float(c) || is_bool(c) || is_list(c))
    {
        return true;
    }
    return false;
}

bool is_indexing(string c)
{
    // syntax: name_of_the_list*[index]
    // or : name_of_the_list*index

    int count = 0;
    for (auto i : c)
    {
        if (i == '*')
        {
            count++;
        }
    }
    if (count == 1)
    {
        return true;
    }
    return false;
}

int separate_indexing(string c)
{
    for (int i = 0; i < c.length(); i++)
    {
        if (c[i] == '*')
        {
            return i;
        }
    }
    return 0; // Pour le compilateur :)
}

bool is_object_attribute(string c)
{
    bool is_t = false;
    for (auto i : c)
    {
        if (i == '.')
        {
            return true;
        }
    }
    return is_t;
}

vector<string> object_atribute_separator_function(string c)
{
    // Liste de plusieurs noms : avec le nom de la fonction à la fin
    vector<string> splited;
    splited.push_back("");
    for (int i = 1; i < c.size(); i++)
    {
        if (c[i] == '.')
        {
            splited.push_back("");
        }
        else
        {
            splited[splited.size() - 1].push_back(c[i]);
        }
    }
    splited[splited.size() - 1] = "!" + splited[splited.size() - 1];
    return splited;
}

vector<string> object_atribute_separator(string c)
{

    vector<string> splited;
    splited.push_back("");
    for (int i = 0; i < c.size(); i++)
    {
        if (c[i] == '.')
        {
            splited.push_back("");
        }
        else
        {
            splited[splited.size() - 1].push_back(c[i]);
        }
    }
    return splited;
}

bool is_typed_non_predefined(string type)
{
    if (type[0] == 't' and type[1] == 'y' and type[2] == 'p' and type[3] == 'e' and type[4] == ':')
    {
        return true;
    }
    return false;
}
