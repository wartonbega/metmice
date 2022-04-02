

void error_generator_alu(string ope, Mtmc_variable *one, Mtmc_variable *two, string type1, string type2)
{
    string full = "`" + ope + "` operator can't manage type '" + type1 + "' (`";
    if (type1 == "float")
    {
        full += to_string(*(float *)one->content);
    }
    else if (type1 == "int")
    {
        full += to_string(*(int *)one->content);
    }
    else if (type1 == "list")
    {
        full += "list";
    }
    else
    {
        full += *(string *)one->content;
    }
    full += "`) and '" + type2 + "' (`";
    if (type2 == "float")
    {
        full += to_string(*(float *)two->content);
    }
    else if (type2 == "int")
    {
        full += to_string(*(int *)two->content);
    }
    else if (type2 == "list")
    {
        full += "list";
    }
    else
    {
        full += *(string *)two->content;
    }
    full += "`)";
    Error("execution", full);
}

Mtmc_variable *alu_plus(Mtmc_variable *arg1, Mtmc_variable *arg2)
{
    string type1 = arg1->type;
    string type2 = arg2->type;
    Mtmc_variable *result = new Mtmc_variable();
    Mtmc_variable_init(result);
    if (type1 != type2 and (type1 != "list" and type2 != "list")) // Les listes supportent tous les types
    {
        error_generator_alu("+", arg1, arg2, type1, type2);
    }
    if (type1 == "int")
    {
        int64_t *res = new int64_t(*(int64_t *)arg1->content + *(int64_t *)arg2->content);
        result->content = (void *)res;
        result->type = "int";
    }
    else if (type1 == "float")
    {
        float *res = new float(*(float *)arg1->content + *(float *)arg2->content);
        result->content = (void *)res;
        result->type = "float";
    }
    else if (type1 == "string")
    {
        string *res = new string('"' + get_string_content(*(string *)arg1->content) + get_string_content(*(string *)arg2->content) + '"');
        result->content = (void *)res;
        result->type = "string";
    }
    else if (type1 == "list")
    {
        Mtmc_listed_push_back(arg2, (Mtmc_listed *)arg1->content);
        result->content = (void *)arg1->content;
        result->type = "list";
    }
    else if (type2 == "list")
    {
        Mtmc_listed_push_back(arg1, (Mtmc_listed *)arg2->content);
        result->content = (void *)arg2->content;
        result->type = "list";
    }
    else
    {
        error_generator_alu("+", arg1, arg2, type1, type2);
    }
    return result;
}

Mtmc_variable *alu_minus(Mtmc_variable *arg1, Mtmc_variable *arg2)
{
    string type1 = arg1->type;
    string type2 = arg2->type;
    Mtmc_variable *result = new Mtmc_variable();
    Mtmc_variable_init(result);
    if (type1 != type2)
    {
        error_generator_alu("-", arg1, arg2, type1, type2);
    }
    if (type1 == "int")
    {
        int64_t *res = new int64_t(*(int64_t *)arg1->content - *(int64_t *)arg2->content);
        result->content = (void *)res;
        result->type = "int";
    }
    else if (type1 == "float")
    {
        float *res = new float(*(float *)arg1->content - *(float *)arg2->content);
        result->content = (void *)res;
        result->type = "float";
    }
    else
    {
        error_generator_alu("-", arg1, arg2, type1, type2);
    }
    return result;
}

Mtmc_variable *alu_times(Mtmc_variable *arg1, Mtmc_variable *arg2)
{
    string type1 = arg1->type;
    string type2 = arg2->type;
    Mtmc_variable *result = new Mtmc_variable();
    Mtmc_variable_init(result);
    if (type1 != type2 and (type1 != "list" and type2 != "int"))
    {
        error_generator_alu("*", arg1, arg2, type1, type2);
    }
    if (type1 == "int")
    {
        int64_t *res = new int64_t(*(int64_t *)arg1->content * *(int64_t *)arg2->content);
        result->content = (void *)res;
        result->type = "int";
    }
    else if (type1 == "float")
    {
        float *res = new float(*(float *)arg1->content * *(float *)arg2->content);
        result->content = (void *)res;
        result->type = "float";
    }
    else if (type1 == "list")
    {
        /*
        deux type d'indexages :
            - list*[index]
            - list*index
        */
        if (type2 == "list")
        {
            // Récupère l'élément de la liste et la renvoie
            if (((Mtmc_listed *)arg2->content)->size != 1)
            {
                Error("execution", "the index of the list must be of lenght one");
            }

            Mtmc_listed *indexes = (Mtmc_listed *)arg2->content;

            Mtmc_variable *index = indexes->begin->next->content;

            if (index->type == "none")
            {
                Error("not implemented", "indexing with a content still waiting to be evaluated (include/alu.h/alu_times)");
            }
            else if (index->type != "int")
            {
                string err = "the index of the list must be of type 'int' not type '" + index->type + "'";
                Error("execution", err);
            }
            int64_t index_int = int64_t(*(int64_t *)index->content); // Cast vers du int (l'index)

            result = Mtmc_listed_get(index_int, (Mtmc_listed *)arg1->content);
        }
        else if (type2 == "int")
        {
            Mtmc_listed *list = (Mtmc_listed *)arg1->content;
            int64_t index = int64_t(*(int64_t *)arg2->content); // Cast vers du int
            result = Mtmc_listed_get(index, list);
        }
    }
    else
    {
        error_generator_alu("*", arg1, arg2, type1, type2);
    }
    return result;
}

Mtmc_variable *alu_equals(Mtmc_variable *arg1, Mtmc_variable *arg2)
{
    string type1 = arg1->type;
    string type2 = arg2->type;
    Mtmc_variable *result;

    if (type1 != type2)
    {
        result = new_mtmc_variable_from_string("false");
    }
    if (type1 == "int")
    {
        if (*(int64_t *)arg1->content == *(int64_t *)arg2->content)
        {
            result = new_mtmc_variable_from_string("true"); // Initie le nom puis le contenu
        }
        else
        {
            result = new_mtmc_variable_from_string("false");
        }
    }
    else if (type1 == "float")
    {
        if (*(float *)arg1->content == *(float *)arg2->content)
        {
            result = new_mtmc_variable_from_string("true");
        }
        else
        {
            result = new_mtmc_variable_from_string("false");
        }
    }
    else if (type1 == "list")
    {
        Mtmc_listed *list1 = (Mtmc_listed *)arg1->content;
        Mtmc_listed *list2 = (Mtmc_listed *)arg2->content;
        // On vérifie d'abords que les deux listes soient de la même taille
        if (list1->size == list2->size)
        {
            for (int i = 0; i < list1->size; ++i)
            {
                Mtmc_variable *n1 = Mtmc_listed_get(i, list1);
                Mtmc_variable *n2 = Mtmc_listed_get(i, list2);
                Mtmc_variable *r = alu_equals(n1, n2);
                result = new_mtmc_variable_from_string("true");
                if (*(string *)r->content == "false")
                {
                    result = new_mtmc_variable_from_string("false");
                    break;
                }
            }
        }
        else
        {
            result = new_mtmc_variable_from_string("false");
        }
    }
    else
    {
        if (*(string *)arg1->content == *(string *)arg2->content)
        {
            result = new_mtmc_variable_from_string("true");
        }
        else
        {
            result = new_mtmc_variable_from_string("false");
        }
    }
    result->type = "bool";
    return result;
}

Mtmc_variable *alu_power(Mtmc_variable *arg1, Mtmc_variable *arg2)
{
    string type1 = arg1->type;
    string type2 = arg2->type;
    Mtmc_variable *result = new Mtmc_variable();
    Mtmc_variable_init(result);
    if (type1 != type2)
    {
        error_generator_alu("^", arg1, arg2, type1, type2);
    }
    if (type1 == "int")
    {
        int64_t *res = new int64_t(pow(*(int64_t *)arg1->content, *(int64_t *)arg2->content));
        result->content = (void *)res;
        result->type = "int";
    }
    else if (type1 == "float")
    {
        float *res = new float(pow(*(float *)arg1->content, *(float *)arg2->content));
        result->content = (void *)res;
        result->type = "float";
    }
    else if (type1 == "string")
    {
        error_generator_alu("^", arg1, arg2, type1, type2);
    }
    else
    {
        error_generator_alu("^", arg1, arg2, type1, type2);
    }
    return result;
}

Mtmc_variable *alu_div(Mtmc_variable *arg1, Mtmc_variable *arg2)
{
    string type1 = arg1->type;
    string type2 = arg2->type;
    Mtmc_variable *result = new Mtmc_variable();
    Mtmc_variable_init(result);

    if (type1 != type2)
    {
        error_generator_alu("/", arg1, arg2, type1, type2);
    }
    if (type1 == "int")
    {
        int64_t *res = new int64_t(*(int64_t *)arg1->content / *(int64_t *)arg2->content);
        result->content = (void *)res;
        result->type = "int";
    }
    else if (type1 == "float")
    {
        float *res = new float(*(float *)arg1->content / *(float *)arg2->content);
        result->content = (void *)res;
        result->type = "float";
    }
    else
    {
        error_generator_alu("/", arg1, arg2, type1, type2);
    }
    return result;
}

Mtmc_variable *alu_floordiv(Mtmc_variable *arg1, Mtmc_variable *arg2)
{
    string type1 = arg1->type;
    string type2 = arg2->type;
    Mtmc_variable *result = new Mtmc_variable();
    Mtmc_variable_init(result);
    if (type1 != type2)
    {
        error_generator_alu("//", arg1, arg2, type1, type2);
    }
    if (type1 == "int")
    {
        // Division euclidienne de deux nombres entiers
        int64_t *res = new int64_t(*(int64_t *)arg1->content / *(int64_t *)arg2->content);
        result->content = (void *)res;
        result->type = "int";
    }
    else
    {
        error_generator_alu("//", arg1, arg2, type1, type2);
    }
    return result;
}

Mtmc_variable *alu_modulo(Mtmc_variable *arg1, Mtmc_variable *arg2)
{
    string type1 = arg1->type;
    string type2 = arg2->type;
    Mtmc_variable *result = new Mtmc_variable();
    Mtmc_variable_init(result);
    if (type1 != type2)
    {
        error_generator_alu("%", arg1, arg2, type1, type2);
    }
    if (type1 == "int")
    {
        int64_t *res = new int64_t(*(int64_t *)arg1->content % *(int64_t *)arg2->content);
        result->content = (void *)res;
        result->type = "int";
    }
    else if (type1 == "float")
    { // Pour calculer le modulo d'un float : fmod (soit en mtmc : float % float)
        float *res = new float(fmod(*(float *)arg1->content, *(float *)arg2->content));
        result->content = (void *)res;
        result->type = "float";
    }
    else
    {
        error_generator_alu("%", arg1, arg2, type1, type2);
    }
    return result;
}

Mtmc_variable *alu_gt(Mtmc_variable *arg1, Mtmc_variable *arg2)
{
    string type1 = arg1->type;
    string type2 = arg2->type;
    Mtmc_variable *result = nullptr;
    if (type1 != type2)
    {
        error_generator_alu(">", arg1, arg2, type1, type2);
    }
    if (type1 == "int")
    {
        if (*(int64_t *)arg1->content > *(int64_t *)arg2->content)
        {
            result = new_mtmc_variable_from_string("true");
        }
        else
        {
            result = new_mtmc_variable_from_string("false");
        }
    }
    else if (type1 == "float")
    {
        if (*(float *)arg1->content > *(float *)arg2->content)
        {
            result = new_mtmc_variable_from_string("true");
        }
        else
        {
            result = new_mtmc_variable_from_string("false");
        }
    }
    else if (type1 == "string")
    {
        if ((*(string *)arg1->content).size() > (*(string *)arg2->content).size())
        {
            result = new_mtmc_variable_from_string("true");
        }
        else
        {
            result = new_mtmc_variable_from_string("false");
        }
    }
    else
    {
        error_generator_alu(">", arg1, arg2, type1, type2);
    }
    result->type = "bool";
    return result;
}

Mtmc_variable *alu_lt(Mtmc_variable *arg1, Mtmc_variable *arg2)
{
    string type1 = arg1->type;
    string type2 = arg2->type;
    Mtmc_variable *result = nullptr;
    if (type1 != type2)
    {
        error_generator_alu("<", arg1, arg2, type1, type2);
    }
    if (type1 == "int")
    {
        if (*(int64_t *)arg1->content < *(int64_t *)arg2->content)
        {
            result = new_mtmc_variable_from_string("true");
        }
        else
        {
            result = new_mtmc_variable_from_string("false");
        }
    }
    else if (type1 == "float")
    {
        if (*(float *)arg1->content < *(float *)arg2->content)
        {
            result = new_mtmc_variable_from_string("true");
        }
        else
        {
            result = new_mtmc_variable_from_string("false");
        }
    }
    else if (type1 == "string")
    {
        if ((*(string *)arg1->content).size() < (*(string *)arg2->content).size())
        {
            result = new_mtmc_variable_from_string("true");
        }
        else
        {
            result = new_mtmc_variable_from_string("false");
        }
    }
    else
    {
        error_generator_alu("<", arg1, arg2, type1, type2);
    }
    result->type = "bool";
    return result;
}

Mtmc_variable *alu_negativ(Mtmc_variable *arg1)
{
    string type1 = arg1->type;
    Mtmc_variable *result = new Mtmc_variable();
    if (type1 == "int")
    {
        int64_t *res = new int64_t(-*(int64_t *)arg1->content);
        result->content = (void *)res;
        result->type = "int";
    }
    else if (type1 == "float")
    {
        float *res = new float(-*(float *)arg1->content);
        result->content = (void *)res;
        result->type = "float";
    }
    else
    {
        error_generator_alu("-", arg1, NULL, type1, "");
    }
    return result;
}