/*
Anton Appel le 15/03/2022
Les structures vont faire entrer le metmice dans l'ère de la poo
*/

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

Mtmc_class_template *generate_structure(string name, vector<Node *> trunc)
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
            Mtmc_variable *r = compute(instruction->children[0], true, false);
            if (instruction->children.size() > 1 and instruction->children[1]->value == "->")
            {
                if (instruction->children[2]->children.size() == 1)
                {
                    // Asignation du nom à la variable
                    string name = instruction->children[2]->children[0]->value;
                    result->atributes_name[name] = r;
                }
                else
                {
                    Error("structure definition", "attributes can't have a complex declaration");
                }
            }
        }
        else if (instruction->value == "func")
        {
            vector<Node *> args;
            vector<Node *> antiargs;
            if (instruction->children[1]->children.size() != 0)
            {
                args = get_listed_values(instruction->children[1]->children[0]);     // (*)
                antiargs = get_listed_values(instruction->children[2]->children[0]); // [*]
            }
            vector<Node *> paterns_value = recognize_paternes(instruction->children[3], true); // {*}
            Mtmc_functions *f = structure_function_asignement(instruction->children[0]->children[0]->value, paterns_value, args, antiargs);
            result->functions.push_back(f);
        }
        else
        {
            string err = "invalid instruction type '" + instruction->value + "' in a structure definition\nPossible declarations are: `expr` and `func`";
            Error("structure definition", err);
        }
        index++;
    }
    return result;
}

Mtmc_class_variable *generate_structured_variable(Mtmc_class_template *parent)
{
    Mtmc_class_variable *result = new Mtmc_class_variable();
    result->name = parent->name;
    result->parent_class = parent;
    // Copie les variables de la classe parente dans la variable fille
    vector<string> names = get_keys(parent->atributes_name);
    for (int i = 0; i < names.size(); i++)
    {
        string name = names[i];
        Mtmc_variable *v = parent->atributes_name[name];
        Mtmc_variable *v_copy = new Mtmc_variable(*v);
        result->atributes[name] = v_copy;
    }
    return result;
}
