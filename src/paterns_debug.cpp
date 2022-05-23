#include <string>
#include <vector>
using namespace std;
#include "../include/node.h"
#include "../include/variables.hpp"
#include "../include/usefull.h"
#include "../include/paterns_debug.h"
#include "../include/parser.h"

bool recognize_if_debug(vector<string> c_values, int index, vector<string> reference)
{
	if (c_values[index] == "if")
	{
		if (c_values[index + 1] == "bracket()")
		{
			if (c_values[index + 2] == "bracket{}")
			{
				if (c_values[index + 3] == ";")
				{
					return true;
				}
				else if (c_values[index + 3] == "else")
				{
					if (c_values[index + 4] == "bracket{}")
					{
						if (c_values[index + 5] == ";")
						{
							return true;
						}
						else
						{
							Error(reference[index + 4], "missing `;` token at the end of an 'if-else' statement");
						}
					}
					else
						Error(reference[index + 3], "'else' statement must be followed by some actions (`{}`)");
				}
				else
				{
					Error(reference[index + 2], "missing `;` token at the end of an 'if' statement");
				}
			}
			else
			{
				Error(reference[index+1], "'if' statement must be followed by its arguments (`()`) and somes actions (`{}`)");
			}
		}
		else
		{
			Error(reference[index], "'if' statement must be followed by its arguments");
		}
	}
	return false;
}

bool recognize_while_debug(vector<string> c_values, int index, vector<string> reference)
{
	if (c_values[index] == "while")
	{
		if (c_values[index + 1] == "bracket()")
		{
			if (c_values[index + 2] == "bracket{}")
			{
				if (c_values[index + 3] == ";")
				{
					return true;
				}
				else
				{
					Error(reference[index+2], "missing `;` token at the end of a 'while' loop");
				}
			}
			else
			{
				Error(reference[index+1], "'while' statement must be followed by its arguments and some actions");
			}
		}
		else
		{
			Error(reference[index], "'while' statement must be followed by its arguments");
		}
	}
	return false;
}

bool recognize_for_debug(vector<string> c_values, int index, vector<string> reference)
{
	if (c_values[index] == "for")
	{
		if (c_values[index + 1] == "bracket()")
		{
			if (c_values[index + 2] == "bracket{}")
			{
				if (c_values[index + 3] == ";")
				{
					return true;
				}
				else
				{
					Error(reference[index+2], "missing `;` token at the end of a 'for' loop");
				}
			}
			else
			{
				Error(reference[index+1], "'for' statement must be followed by its arguments and some actions");
			}
		}
		else
		{
			Error(reference[index], "'for' statement must be followed by its arguments");
		}
	}
	return false;
}

bool recognize_expr_debug(vector<string> c_values, int index, vector<string> reference)
{
	if (c_values[index] == "*")
	{
		if (c_values[index + 1] == "->" and c_values[index + 2] == "*")
		{
			if (c_values[index + 3] == ";")
			{
				return true;
			}
			else
			{
				Error(reference[index+2], "missing `;` at the end of an assignement");
			}
		}
		else if (c_values[index + 1] == ";")
		{
			return true;
		}
		else
		{
			Error(reference[index], "missing `;` at the end of an expression");
		}
	}
	return false;
}

bool recognize_func_debug(vector<string> c_values, int index, vector<string> reference)
{
	if (c_values[index] == "*")
	{
		if (c_values[index + 1] == "bracket()")
		{
			if (c_values[index + 2] == "bracket[]")
			{
				if (c_values[index + 3] == "bracket{}")
				{
					if (c_values[index + 4] == ";")
					{
						return true;
					}
					else
						Error(reference[index+3], "missing `;` token at the end of a function declaration");
				}
				else
					Error(reference[index+2], "function declaration must be followed by its actions (`{}`)");
			}
			else
				Error(reference[index+1], "function declaration must be followed by its anti-arguments (`[]`). \nMaybe it is a function call and forgot token '!'");
		}
	}
	return false;
}

bool is_keyword(string exp)
{
	if (tokenize(exp) == "keyword")
	{
		return true;
	}
	return false;
}

bool recognize_keyword_debug(vector<string> c_values, int index, vector<string> reference)
{
	if (is_keyword(c_values[index]))
	{
		if (c_values[index + 1] == "*")
		{
			if (c_values[index + 2] == ";")
			{
				return true;
			}
			else
				Error(reference[index+1], "missing `;` token at the end of a keyword call");
		}
		else
		{
			if (c_values[index + 1] == ";")
			{
				return true;
			}
			else
				Error(reference[index], "missing `;` token at the end of a keyword call");
		}
	}
	return false;
}

bool recognize_struct_debug(vector<string> c_values, int index, vector<string> reference)
{
	if (c_values[index] == "struct")
	{
		if (c_values[index + 1] == "*")
		{
			if (c_values[index + 2] == "bracket{}")
			{
				if (c_values[index + 3] == ";")
				{
					return true;
				}
				else
					Error(reference[index+2], "missing `;` token at the end of a struct declaration");
			}
			else
			{
				Error(reference[index+1], "missing curly brackets for a struct declaration");
			}
		}
		else
		{
			Error(reference[index], "missing name for the structure declaration");
		}
	}
	return false;
}
