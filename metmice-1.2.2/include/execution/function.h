
vector<Mtmc_variable *> compute_listed_values_function(vector<Node *> listed_values, vector<Mtmc_variable *> &variable_function, int recursive);

bool variable_exist_function(string name, vector<Mtmc_variable *> variable_function)
{
    for (int i = 0; i < variable_function.size(); ++i)
    {
        if (variable_function[i]->name == name)
        {
            return true;
        }
    }
    for (int i = 0; i < variables_t.size(); ++i)
    {
        if (variables_t[i]->name == name)
        {
            return true;
        }
    }
    return false;
}

void variable_asignement_function(Mtmc_variable *var, vector<Mtmc_variable *> &variable_function)
{
    if (!mtmc_accept_var_name(var->name))
    {
        string err = "invalid name for a variable `" + var->name + "`";
        debug_error("function execution", err);
    }
    variable_function.push_back(var);
}

void variable_override_function(Mtmc_variable *var, vector<Mtmc_variable *> &variable_function)
{
    for (int i = 0; i < variable_function.size(); ++i)
    {
        if (variable_function[i]->name == var->name)
        {
            variable_function[i]->content = var->content;
            return;
        }
    }
    if (variable_exist(var->name))
    { // Si la variable est globale, alors on appel plutôt la définition d'une nouvelle variable à l'intètieur de la fonction
        variable_asignement_function(var, variable_function);
    }
}

Mtmc_variable *variable_get_value_function(string name, vector<Mtmc_variable *> &variable_function)
{
    for (int i = 0; i < variable_function.size(); ++i)
    {
        if (variable_function[i]->name == name)
        {
            return variable_function[i];
        }
    }
    for (int i = 0; i < variables_t.size(); ++i)
    {
        if (variables_t[i]->name == name)
        {
            return variables_t[i];
        }
    }
    return new Mtmc_variable();
}

Mtmc_variable *funcall_base_function(Node *instruction, vector<Mtmc_variable *> variable_function, int recursive)
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

        Mtmc_variable * current = new Mtmc_variable(*var);

        for (int y = 1; y < names.size()-1; ++y)
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
            string err = "type '" + names[0] + "' has not attribute '" + names[names.size()-1] + "'";
            Error("execution", err);
        }
        c_var = (Mtmc_class_variable*)current->content;

        if (c_var->parent_class->function_exists(names[names.size()-1]))
        {
            Mtmc_functions *func = c_var->parent_class->function(names[names.size()-1]);

            vector<Node *> args;
            vector<Mtmc_variable *> args2;
            if (instruction->children[0]->children.size() != 0)
            {
                args = get_listed_values(instruction->children[0]->children[0]);
                args2 = compute_listed_values_function(args, variable_function, recursive);
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
            vector<Mtmc_variable *> variable_bis;
            Mtmc_variable *var = execute_function(func->value, args3, normal_args, variable_bis, recursive);
            return var;
        }
        else
        {
            string err = "the object '" + names[0] + "' (of type '" + c_var->name + "') have no attribute '" + names[names.size()-1] + "'";
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
                args2 = compute_listed_values_function(args, variable_function, recursive);
            }

            // Le nom des arguments de la fonction
            vector<Node *> normal_args = functions[index]->args;
            if (args.size() != normal_args.size())
            {
                string err = "the number of arguments of the function being called '" + instruction->value + "' is not the same of the function that has been precedently defined \nexpected " + to_string(normal_args.size()) + " arguments, got " + to_string(args.size());
                Error("execution", err);
            }
            vector<Mtmc_variable *> variablen_function;
            Mtmc_variable *returning = execute_function(functions[index]->value, args2, normal_args, variablen_function, recursive + 1);
            return returning;
        }
        else
        { // C'est donc un générateur de structures
            Mtmc_class_template *temp = functions[index]->temp;
            Mtmc_class_variable *var = generate_structured_variable(temp);

            Mtmc_variable *n = new Mtmc_variable();
            n->type = "type:" + var->name;
            n->content = (void *)var;
            return n;
        }
    }
    return new Mtmc_variable();
}

Mtmc_variable *compute_function(Node *trunc, bool empty_stack, bool funcall, vector<Mtmc_variable *> &variable_function, int recursive)
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
            Mtmc_variable *ret = funcall_base_function(trunc->children[i], variable_function, recursive);
            computes = save;
            computes.push(ret);
        }
        else if (is_operator(names[i]))
        {
            if (compute_norme == 0) // rpn
            {
                compute_operator_rpn(names, i);
            }
            else
            {
                compute_operator_normal(names, types, i, true, variable_function, trunc, recursive);
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
                computes.push(new_mtmc_variable_from_string("", names[i]));
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
                
                for (int y = 1; y<names_of_objects.size(); ++y)
                { // PLusieurs attributs
                    if (!is_typed_non_predefined(current->type))
                    {
                        string err = "type '" + current->type + "' has no attribute";
                        Error("execution", err);
                    }
                    n_prime = (Mtmc_class_variable*)current->content;
                    if (!n_prime->variable_exists(names_of_objects[y]))
                    {
                        string err = "object '"+add_to(names_of_objects, y-1)+"' has not attribute '"+names_of_objects[y]+"'";
                        Error("execution", err);
                    }
                    current = n_prime->variable_get_value(names_of_objects[y]);
                }
                computes.push(current);
            }
            else
            {
                string err = "unkown variable or expression `" + names[i] + "`";
                Error("function execution", err);
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

vector<Mtmc_variable *> compute_listed_values_function(vector<Node *> listed_values, vector<Mtmc_variable *> &variable_function, int recursive)
{
    vector<Mtmc_variable *> ret;
    for (auto val : listed_values)
    {
        ret.push_back(compute_function(val, true, false, variable_function, recursive));
    }
    return ret;
}

//—————————————————————— Visitor ——————————————————————

Mtmc_variable *execute_function(vector<Node *> ast, vector<Mtmc_variable *> args, vector<Node *> normal_args, vector<Mtmc_variable *> variable_function, int recursive)
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

    Mtmc_variable *what_to_return = new_mtmc_variable_from_string("", "none");

    // Asigniation des arguments
    for (int i = 0; i < args.size(); ++i)
    {
        string name = args[i]->name;
        Mtmc_variable *var = new Mtmc_variable(*args[i]);
        var->name = normal_args[i]->children[0]->value;
        if (variable_exist_function(name, variable_function))
        {
            variable_override_function(var, variable_function);
        }
        variable_asignement_function(var, variable_function);
    }

    int index = 0;

    // Début du "scan" de la fonction
    while (index != ast.size())
    {
        Node *instruction = ast[index];
        if (instruction->value == "expr")
        {
            Mtmc_variable *result = compute_function(instruction->children[0], true, false, variable_function, recursive);
            if (instruction->children.size() > 1 and instruction->children[1]->value == "->")
            {
                if (instruction->children[2]->children.size() == 3) // pour les assigantions des listes genre '3 -> liste*3'
                {
                    // Normalement, sa se présente soit liste*index soit liste*[index]
                    // On crée donc un sous noeud qui contient une fois la liste, pour pouvoir utiliser la fonction de calcule
                    Node *n = new Node();
                    n->children.push_back(instruction->children[2]->children[0]);
                    Mtmc_variable *list_to_mod = compute_function(n, false, false, variable_function, recursive);

                    if (list_to_mod->type != "list")
                    {
                        string err = "the type of the list being modified must be 'list', not '" + list_to_mod->type + "'";
                        Error("execution", err);
                    }

                    // Pareil
                    n = new Node();
                    n->children.push_back(instruction->children[2]->children[2]);
                    Mtmc_variable *list_index = compute_function(n, false, false, variable_function, recursive);

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
                        r->name = instruction->children[2]->children[0]->value;
                        if (variable_exist_function(r->name, variable_function))
                        {
                            variable_override_function(r, variable_function);
                        }
                        else
                            variable_asignement_function(r, variable_function);
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
                        r->name = names_of_objects[1];
                        n_prime->variable_override(r);
                    }
                }
            }
        }
        else if (instruction->value == "keyword")
        {
            Node *args_s = instruction->children[1];
            vector<string> s_args = childs_value(args_s);
            compute_function(args_s, false, false, variable_function, recursive);

            if (instruction->children[0]->value == "push")
            {
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
                int cargs1 = *(int *)arg1->content;
                if (cargs1 == 1)
                {
                    empty_compute_stack();
                    push_display_pile(arg2);
                }
                else if (cargs1 == 2)
                {
                    empty_compute_stack();
                    computes.push(arg2);
                }
                else if (cargs1 == 3)
                {
                    if (!Mtmc_variable_type_check(arg2, "int"))
                    {
                        Error("execution", "arguments for 'push' in the system pile needs to be an integer");
                    }
                    system_p.push_back(*(int *)arg2->content);
                }
            }
            else if (instruction->children[0]->value == "type")
            {
                if (s_args.size() < 1 and computes.size() < 1)
                {
                    Error("execution", "'type' keywords need at least one arguments");
                }
                Mtmc_variable *arg = computes.top();
                computes.pop();
                empty_compute_stack();
                Mtmc_variable *type = new_mtmc_variable_from_string("", to_string(arg->type));
                computes.push(type);
            }
            else if (instruction->children[0]->value == "return")
            {
                if (s_args.size() >= 1)
                {
                    what_to_return = computes.top();
                    computes.pop();
                }
                break;
            }
            else if (instruction->children[0]->value == "length")
            {
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
            else if (instruction->children[0]->value == "pop")
            {
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
                Mtmc_variable *element = Mtmc_listed_get(carg1, list);
                computes.push(element);
                Mtmc_listed_destuctor(carg1, list);
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
                    Error("function execution", "global keyword needs one and only one argument"); // Eh oui ..
                if (variable_exist_function(name, variable_function))
                {
                    // On définis donc une variable global sur le nom et la valeur de celle qui existe dans la fonction
                    Mtmc_variable *value = variable_get_value_function(name, variable_function);
                    value->name = name;
                    if (variable_exist(name))
                    {
                        variable_override(value);
                    }
                    else
                        variable_asignement(value);
                }
                else
                {
                    string err = "unknown variable `" + name + "` for a global asignement";
                    Error("function execution", err);
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
            Mtmc_variable *condition_value = compute_function(condition, true, false, variable_function, recursive);
            if (condition_value->type != "bool")
            {
                Error("execution", "condition for an if-else statement must be a boolean");
            }
            if (*(string *)condition_value->content == "true")
            {
                vector<Node *> paterns = recognize_paternes(instruction->children[2], true);
                string ret = normal_code_execution(paterns, true, variable_function, recursive, false); // Qui correspond au {}
                if (ret == "return")
                {
                    what_to_return = computes.top();
                    computes.pop();
                    break;
                }
            }
            else if (instruction->children.size() > 3 and instruction->children[3]->value == "else")
            {
                vector<Node *> paterns = recognize_paternes(instruction->children[4], true);
                string ret = normal_code_execution(paterns, true, variable_function, 0, false); // Qui correspond au {} du else statementif (ret == "return")
                if (ret == "return")
                {
                    if (computes.size() >= 1)
                    {
                        what_to_return = computes.top();
                        computes.pop();
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
            string ret = while_loop(paterns, true, variable_function, recursive, get_listed_values(instruction->children[1]->children[0]));
       	    if (ret == "return")
            {
                what_to_return = computes.top();
            computes.pop();
            break;
	        }
	    }
        else if (instruction->value == "for_loop")
        {
            vector<Node *> paterns = recognize_paternes(instruction->children[2], true);
            string ret = for_loop(paterns, true, variable_function, recursive, get_listed_values(instruction->children[1]->children[0])); // Qui correspond au {} du else statementif (ret == "return")
            if (ret == "return")
            {
                what_to_return = computes.top();
                computes.pop();
                break;
            }
        }
        else if (instruction->value == "struct")
        {
            string err = "can't have a structure definition inside of a function";
            Error("function execution", err);
        }
        output_display_pile();
        manage_system_pile();
        index++;
    }

    return what_to_return;
}
