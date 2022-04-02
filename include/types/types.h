#include "conversion.h"
#include "types_check.h"

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
    return get_from_string(word, 1, word.size() - 1);
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
