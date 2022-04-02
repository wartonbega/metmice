
void push_display_pile(Mtmc_variable *element)
{ // J'ai pas interet à avoir foiré mes typages ... :/
    if (element->type == "string")
    {
        display.push_back(del_string(*(string *)element->content));
    }
    else if (element->type == "int")
    {
        display.push_back(to_string(*(int *)element->content));
    }
    else if (element->type == "float")
    {
        display.push_back(to_string(*(float *)element->content));
    }
    else if (element->type == "bool")
    {
        display.push_back(*(string *)element->content);
    }
    else if (element->type == "list")
    {
        display.push_back(Mtmc_listed_string((Mtmc_listed *)element->content));
    }
    else if (element->type == "none")
    {
        display.push_back("none");
    }
    else if (is_typed_non_predefined(element->type))
    {
        // Representation en chaine de caractère de l'addresse de l'objet
        Mtmc_class_variable *t = (Mtmc_class_variable *)element->content;
        
        display.push_back(del_string(t->convert_to_string()));
    }
    else
    {
        string error = "\"Error : the type of the value beeing pushed on the screen does not match any known type. Type : '"+element->type+"'\"";
        display.push_back(error);
    }
}
