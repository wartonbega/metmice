#include <string>
#include <map>
#include <vector>
#include <iostream>
#include "../include/node.h"
#include "../include/variables.hpp"
#include "../include/usefull.h"
#include "../include/types.h"
#include "../include/types_check.h"
#include "../include/conversion.h"
#include "../include/code_execution.h"

void Mtmc_variable_init(Mtmc_variable *var)
{
    var->type = "none";
    var->content = nullptr;
}

void Mtmc_variable_edit(Mtmc_variable *var, string content)
{ // S'occupe de la convertion vers le pointers et de l'asignation du type
    string type = get_type(content);
    void *val = convert(content, type);
    var->type = type;
    var->content = val;
}

Mtmc_variable *new_mtmc_variable_from_string(string content)
{ // Rassemble le edit et init
    Mtmc_variable *n = new Mtmc_variable();
    Mtmc_variable_init(n);
    Mtmc_variable_edit(n, content);
    return n;
}

string Mtmc_variable_string(Mtmc_variable *var)
{
    if (var->type == "int")
    {
        return to_string(*(int *)var->content);
    }
    else if (var->type == "float")
    {
        return to_string(*(float *)var->content);
    }
    else if (var->type == "bool")
    {
        return *(string *)var->content;
    }
    else if (var->type == "string")
    {
        return *(string *)var->content;
    }
    else if (var->type == "list")
    {
        return Mtmc_listed_string((Mtmc_listed *)var->content);
    }
    else if (is_typed_non_predefined(var->type))
    { // Si c'est une type définis par l'utilisateur
        return "";
    }
    return "none";
}

bool Mtmc_variable_type_check(Mtmc_variable *var, string type)
{
    if (type == var->type)
    {
        return true;
    }
    return false;
}

void Mtmc_variable_delete_atributes(Mtmc_variable *var)
{
    free(var->content);
    var->type.clear();
}

void Mtmc_listed_init(Mtmc_listed *t)
{
    t->end = new Mtmc_listed_node();
    t->begin = new Mtmc_listed_node();
    t->end->prev = t->begin;
    t->begin->next = t->end;
}

void Mtmc_listed_push_back(Mtmc_variable *cont, Mtmc_listed *parent)
{
    // Ajout d'un nouvel élément à la fin de la liste chainée

    // Initialisation du nouveau noeud
    Mtmc_listed_node *var = new Mtmc_listed_node();

    // Asignation de la valeur dans le noeud.
    // Création du pointeur vers la variable pour éviter de la modifier
    var->content = new Mtmc_variable(*cont);

    // Dernier noeud de la liste
    Mtmc_listed_node *ancient = parent->end->prev;

    // Chainage de la fin avec le nouveau noeud
    parent->end->prev = var;

    // Chainage du nouveau noeud avec l'ancien
    var->prev = ancient;

    // Chainage du nouveau noeud avec la fin
    var->next = parent->end;

    // Chainage de l'ancien noeud avec le nouveau
    ancient->next = var;

    // Incrémentation du nombre de noeuds
    parent->size++;
}

Mtmc_variable *Mtmc_listed_get(int index, Mtmc_listed *parent)
{
    if (index > parent->size - 1)
    {
        Error("variable creation", "the index is bigger than the size of the listed variable");
    }
    if (index >= 0)
    {
        Mtmc_listed_node *current = parent->begin;
        int count = -1;
        while (index != count)
        {
            current = current->next;
            count++;
        }
        return new Mtmc_variable(*current->content);
    }
    else
    {
        Mtmc_listed_node *current = parent->end;
        int count = 0;
        while (index != count)
        {
            current = current->prev;
            count--;
        }
        return new Mtmc_variable(*current->content);
    }
}

void Mtmc_listed_assigne(int64_t index, Mtmc_listed *parent, Mtmc_variable *n)
{
    if (index > parent->size)
    {
        Error("variable creation", "the index is bigger than the size of the listed variable");
    }
    if (index >= 0)
    {
        Mtmc_listed_node *current = parent->begin;
        int64_t count = -1;
        while (index != count)
        {
            current = current->next;
            count++;
        }
        current->content = new Mtmc_variable(*n);
    }
    else
    {
        Mtmc_listed_node *current = parent->end;
        int64_t count = 0;
        while (index != count)
        {
            current = current->prev;
            count--;
        }

        current->content = new Mtmc_variable(*n);
    }
}

void Mtmc_listed_destuctor(int index, Mtmc_listed *parent)
{
    if (index > parent->size)
    {
        Error("variable creation", "the index is bigger than the size of the listed variable for deletion");
    }
    if (index >= 0)
    {
        Mtmc_listed_node *current = parent->begin;
        int count = -1;
        while (index != count)
        {
            current = current->next;
            count++;
        }
        current->prev->next = current->next;
        current->next->prev = current->prev;
        delete current;
    }
    else
    {
        Mtmc_listed_node *current = parent->end;
        int count = 0;
        while (index != count)
        {
            current = current->prev;
            count--;
        }
        current->prev->next = current->next;
        current->next->prev = current->prev;
        delete current;
    }
    parent->size--;
}

void Mtmc_listed_print(Mtmc_listed *parent)
{
    /*
        Envoie à la sortie standard la liste chainée sous forme de string
        Ne met pas de retour à la ligne à la fin
        Fonction récursive : si il y a une sous liste, elle l'affiche aussi
    */
    Mtmc_listed_node *current = parent->begin->next;
    cout << "[ ";
    while (current != parent->end)
    {
        if (current->content->type == "list")
        {
            Mtmc_listed_print((struct Mtmc_listed *)current->content->content);
            if (current->next != parent->end)
            {
                cout << ", ";
            }
        }
        else
        {
            if (current->next == parent->end)
            { // Avec virgule
                if (current->content->type == "int")
                {
                    cout << *(int64_t *)current->content->content;
                }
                else if (current->content->type == "string" or current->content->type == "bool")
                {
                    cout << *(string *)current->content->content;
                }
                else if (current->content->type == "float")
                {
                    cout << *(float *)current->content->content;
                }
                else if (current->content->type == "none")
                {
                    cout << "none";
                }
            }
            else
            { // Sans virgule
                if (current->content->type == "int")
                {
                    cout << *(int64_t *)current->content->content;
                }
                else if (current->content->type == "string" or current->content->type == "bool")
                {
                    cout << *(string *)current->content->content;
                }
                else if (current->content->type == "float")
                {
                    cout << *(float *)current->content->content;
                }
                else if (current->content->type == "none")
                {
                    cout << "none";
                }
                cout << ", ";
            }
        }
        current = current->next;
    }
    cout << " ]";
}

string Mtmc_listed_string(Mtmc_listed *parent)
{
    /*
    Convertis la liste chainée en string et la renvoie
    le fait récursivement : les sous listes sont comprises dedans
    */
    string content = "";
    Mtmc_listed_node *current = parent->begin->next;
    content += "[ ";
    while (current != parent->end)
    {
        if (current->content->type == "list")
        {
            content += Mtmc_listed_string((struct Mtmc_listed *)current->content->content);
            if (current->next != parent->end)
            {
                content += ", ";
            }
        }
        else
        {
            if (current->content->type == "int")
            {
                content += to_string(*(int64_t *)current->content->content);
            }
            else if (current->content->type == "list")
            {
                content += Mtmc_listed_string((Mtmc_listed *)current->content->content);
            }
            else if (current->content->type == "string" or current->content->type == "bool")
            {
                content += *(string *)current->content->content;
            }
            else if (current->content->type == "float")
            {
                content += to_string(*(float *)current->content->content);
            }
            else if (current->content->type == "none")
            {
                content += "none";
            }

            if (current->next != parent->end)
            {
                content += ", ";
            }
        }
        current = current->next;
    }
    content += " ]";
    return content;
}

Mtmc_listed *new_mtmc_listed_from_string(string content)
{
    // Parse le string et le transforme en liste
    int index = 1;
    Mtmc_listed *result = new Mtmc_listed();
    Mtmc_listed_init(result);
    string complete = "";
    while (index < content.size() - 1)
    {
        if (content[index] == ',')
        {
            Mtmc_variable *v = new_mtmc_variable_from_string(complete);
            Mtmc_listed_push_back(v, result);
            complete = "";
        }
        else
        {
            complete += content[index];
        }
        index++;
    }
    if (complete != "")
    {
        Mtmc_variable *v = new_mtmc_variable_from_string(complete);
        Mtmc_listed_push_back(v, result);
    }
    return result;
}

void *convert(string value, string type)
{ // Convertis la value en un void pointer, fonction utilisable que dans un seul sens
    if (type == "string" or type == "bool" or type == "none")
    {
        string *n = new string(value);
        return (void *)n;
    }
    else if (type == "int")
    {
        int64_t r = to_int(value);
        int64_t *rp = new int64_t(r);
        return (void *)rp;
    }
    else if (type == "float")
    {
        float r = stof(value);
        float *rp = new float(r);
        return (void *)rp;
    }
    else if (type == "list")
    {
        Mtmc_listed *n = new_mtmc_listed_from_string(value);
        return (void *)n;
    }

    return new void *;
}

string Mtmc_class_variable::convert_to_string(vector<string> reference)
{ // Fonction prédéfinis pour convertir la variable en string
    /*
    On regarde si il y a une fonction du parent 'to_string'
    */
    string result = "";

    if (this->parent_class->function_exists("!to_string"))
    {
        Mtmc_functions *f = this->parent_class->function("!to_string");
        vector<Mtmc_variable *> args; // On initie les arguments : il y en as un : 'self'
        Mtmc_variable *self = new Mtmc_variable();
        self->content = (void *)this;
        self->type = "type:"+this->name;
        args.push_back(self);

        Mtmc_variable *result = execute_function(f->value, args, f->args, map<string, Mtmc_variable *>(), 0, reference);

        if (result->type == "string")
        {
            return *(string *)result->content;
        }
        else
        {
            return "none";
        }
    }
    else
    {
        return this->normal_name();
    }
}

Mtmc_functions *Mtmc_class_template::function(string name)
{
    for (int i = 0; i < this->functions.size(); ++i)
    {
        if (this->functions[i]->name == name)
        {
            return this->functions[i];
        }
    }
    string err = "unexisting function '"+name+"'";
    Error("execution", err);
    return 0;
}