
string normal_code_execution(vector<Node *> ast, bool function, vector<Mtmc_variable *> &variable_function, int recursive, bool loop)
{
    int index = 0;
    while (index != ast.size())
    {
        Node *instruction = ast[index];
        if (instruction->value == "expr")
        {
            Mtmc_variable *result;
            if (function)
            {                                                                                                  // Si on se trouve dans l'execution d'une fonction
                result = compute_function(instruction->children[0], true, true, variable_function, recursive); // Des calculs chiants
            }
            else
            {
                result = compute(instruction->children[0], true, false);
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
                        list_to_mod = compute_function(n, false, function, variable_function, recursive);
                    }
                    else
                    {
                        list_to_mod = compute(n, false, false);
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
                        list_index = compute_function(n, false, true, variable_function, recursive);
                    } else 
                    {
                        list_index = compute(n, false, false);
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
                    result->name = instruction->children[2]->children[0]->value; // Asignation du nom à la variable
                    if (function)
                    {
                        if (variable_exist_function(result->name, variable_function))
                        {
                            variable_override_function(result, variable_function);
                        }
                        else
                            variable_asignement_function(result, variable_function);
                    }
                    else if (variable_exist(result->name))
                    {
                        variable_override(result);
                    }
                    variable_asignement(result);
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
                    compute_function(args, false, false, variable_function, recursive);
                }
                else
                    compute(args, false, false);
            }

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
                int carg1 = *(int *)arg1->content;

                if (carg1 == 1)
                {
                    empty_compute_stack();
                    push_display_pile(arg2);
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
            else if (instruction->children[0]->value == "type")
            {
                if (s_args.size() < 1 or computes.size() < 1)
                {
                    Error("execution", "'type' keywords need at least one arguments");
                }
                Mtmc_variable *arg = computes.top();
                computes.pop();
                empty_compute_stack();
                Mtmc_variable *type = new_mtmc_variable_from_string("", arg->type);
                computes.push(type);
            }
            else if (instruction->children[0]->value == "return")
            {
                if (function)
                {
                    return "return";
                }
            }
            else if (instruction->children[0]->value == "break")
            {
                if (loop)
                {
                    return "break";
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
                Mtmc_variable *condition_value = compute(condition, true, function);
                string result;
                if (!Mtmc_variable_type_check(condition_value, "bool"))
                {
                    Error("execution", "condition of an if statement needs to be a boolean");
                }
                string cond = *(string *)condition_value->content;
                if (cond == "true")
                {
                    vector<Node *> paterns = recognize_paternes(instruction->children[2], true);
                    result = normal_code_execution(paterns, 0, variable_function, 0, loop); // Qui correspond au {}
                }
                else if (instruction->children.size() > 3 and instruction->children[3]->value == "else")
                {
                    vector<Node *> paterns = recognize_paternes(instruction->children[4], true);
                    result = normal_code_execution(paterns, 0, variable_function, 0, loop); // Qui correspond au {} du else statement
                }
                if (result == "return")
                {
                    return "return";
                }
                else if (result == "break")
                {
                    return "break";
                }
            }
            else
            {
                Mtmc_variable *condition_value = compute_function(condition, true, function, variable_function, recursive);
                string result;
                if (!Mtmc_variable_type_check(condition_value, "bool"))
                {
                    Error("execution", "condition of an if statement needs to be a boolean");
                }
                string cond = *(string *)condition_value->content;

                if (cond == "true")
                {
                    vector<Node *> paterns = recognize_paternes(instruction->children[2], true);
                    result = normal_code_execution(paterns, function, variable_function, 0, loop); // Qui correspond au {}
                }
                else if (instruction->children.size() > 3 and instruction->children[3]->value == "else")
                {
                    vector<Node *> paterns = recognize_paternes(instruction->children[4], true);
                    result = normal_code_execution(paterns, function, variable_function, 0, loop); // Qui correspond au {} du else statement
                }
                if (result == "return")
                {
                    return "return";
                }
                else if (result == "break")
                {
                    return "break";
                }
            }
        }
        else if (instruction->value == "for_loop")
        {
            vector<Node *> paterns = recognize_paternes(instruction->children[2], debug);
            vector<Node *> args_for_loop = get_listed_values(instruction->children[1]->children[0]);
            string ret = for_loop(paterns, function, variable_function, recursive, args_for_loop);
            if (ret == "return")
            {
                return "return";
            }
        }
        output_display_pile();
        manage_system_pile();
        index++;
    }
    return "";
}