

string for_loop(vector<Node *> ast, bool function, vector<Mtmc_variable *> &variable_function, int recursive, vector<Node *> args)
{
    // On vérifie d'abord que les arguments donnés soient ok.
    // Le premier argument est le nom de la variable, le deuxième la valeur initiale, et le dernier la valeur max
    // On as aussi déjà l'ast fournis si gentillement par la fonction d'avant :)
    if (args.size() > 3)
    {
        debug_error("execution", "Only three arguments are needed for a 'for' loop");
    }
    if (args.size() < 3)
    {
        Error("execution", "three arguments are needed to call 'for' loop");
    }
    string name = args[0]->children[0]->value; // Extrait le nom de la variable
    Mtmc_variable *begin;
    Mtmc_variable *end;

    // Calcule les arguments
    if (function)
    { // Si c'est une fonction, le calcul est différent
        begin = compute_function(args[1], true, function, variable_function, recursive);
        end = compute_function(args[2], true, function, variable_function, recursive);
    }
    else
    { // Sinon c'est plutot simple :)
        begin = compute(args[1], true, false);
        end = compute(args[2], true, false);
    }
    // Il faut que les begin et end soient des integers
    if (begin->type != "int" || end->type != "int")
    {
        string err = "bad values for 'for' loop : expected (int, int), got (" + begin->type + ", " + end->type + ")";
        Error("execution", err);
    }

    // On utilise la section de calcule du langage pour vérifier que les arguments soient compatibles
    Mtmc_variable *cond = alu_gt(end, begin);
    if (*(string *)cond->content != "true")
    {
        debug_error("execution", "the begining of the loop is greater than the end"); // Au pire la loop s'arretera
    }

    begin->name = name;
    if (function)
    { // On définis la variable en fonction de l'endroit où on est
        if (variable_exist_function(name, variable_function))
        {
            variable_override_function(begin, variable_function);
        }
        variable_asignement_function(begin, variable_function);
    }
    else
    {
        if (variable_exist(name))
        {
            variable_override(begin);
        }
        variable_asignement(begin);
    }
    string what_to_return; // Peut être égal à "return" par exemple ou à "break"

    int64_t i = *(int64_t *)begin->content;
    Mtmc_variable *index = new Mtmc_variable(*begin);
    index->name = name;
    index->type = "int";
    int64_t end_as_int = *(int64_t *)end->content;

section_loop_begin: // On utilise les sections et les goto pour plus de praticité

    if (i < end_as_int)
    {
        if (function)
        { // De toute façon comme on l'as prédéfinis avant, on as déja écrit cette variable
            variable_override_function(index, variable_function);
        }
        else
            variable_override(index);

        // Tout vas bien, on as définis la variable
        string ret = normal_code_execution(ast, function, variable_function, recursive);
        if (ret == "return")
        {
            what_to_return = ret;
            goto section_loop_end;
        }
        i++;
        int64_t *v = new int64_t(i );
        index->content = (void *)v; // Ne pas oublier le '&' si je ne veux pas juste me retrouver avec un emplacement de mémoire aléatoire
        goto section_loop_begin;
    }
    else
    {
        goto section_loop_end;
    }

section_loop_end:
    return what_to_return;
}

Mtmc_variable *while_loop_compute_condition(bool function, vector<Mtmc_variable *> &variable_function, int recursive, vector<Node *> args)
{
    // Calcule la valeur de la condition de la boucle while
    // Check le type (bool)
    Mtmc_variable *cond;
    if (function)
    {
        cond = compute_function(args[0], true, function, variable_function, recursive);
    }
    else
    { // Pas d'appel de fonction, on peut donc simplement calculer
        cond = compute(args[0], true, false);
    }
    if (cond->type != "bool")
    {
        Error("execution", "the condition of the 'while' loop must be a boolean");
    }
    return cond;
}

string while_loop(vector<Node *> ast, bool function, vector<Mtmc_variable *> &variable_function, int recursive, vector<Node *> args)
{
    // Attention, l'argument doit être tel quel, sinon la boucle ne se finira jamais
    string what_to_return = "";

    if (args.size() > 1)
    {
        Error("execution", "Only one argument is needed for a 'while' loop");
    }
    if (args.size() < 1)
    {
        Error("execution", "one argument is needed to call 'while' loop");
    }

    Mtmc_variable *cond;
    goto section_while_loop_begin;

section_while_loop_begin:
    cond = while_loop_compute_condition(function, variable_function, recursive, args);
    if (cond->type != "bool")
    {
        Error("execution", "the condition of the 'while' loop must be a boolean");
    }
    if (*(string *)cond->content != "true")
    {
        goto section_while_loop_end;
    }
    else
    {
        string ret = normal_code_execution(ast, function, variable_function, recursive);
        if (ret == "return")
        {
            what_to_return = ret;
            goto section_while_loop_end;
        }
        goto section_while_loop_begin;
    }

section_while_loop_end:
    return what_to_return;
}