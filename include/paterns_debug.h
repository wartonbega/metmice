bool recognize_if_debug(vector<string> c_values, int index)
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
							Error("parser", "missing `;` token at the end of an 'if-else' statement");
					}
					else
						Error("parser", "'else' statement must be followed by some actions (`{}`)");
				}
				else
				{
					Error("parser", "missing `;` token at the end of an 'if' statement");
				}
			}
			else
			{
				Error("parser", "'if' statement must be followed by its arguments (`()`) and somes actions (`{}`)");
			}
		}
		else
		{
			Error("parser", "'if' statement must be followed by its arguments");
		}
	}
	return false;
}

bool recognize_while_debug(vector<string> c_values, int index)
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
					Error("parser", "missing `;` token at the end of a 'while' loop");
				}
			}
			else
			{
				Error("parser", "'while' statement must be followed by its arguments and some actions");
			}
		}
		else
		{
			Error("parser", "'while' statement must be followed by its arguments");
		}
	}
	return false;
}

bool recognize_for_debug(vector<string> c_values, int index)
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
					Error("parser", "missing `;` token at the end of a 'for' loop");
				}
			}
			else
			{
				Error("parser", "'for' statement must be followed by its arguments and some actions");
			}
		}
		else
		{
			Error("parser", "'for' statement must be followed by its arguments");
		}
	}
	return false;
}

bool recognize_expr_debug(vector<string> c_values, int index)
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
				Error("parser", "missing `;` at the end of an expression");
			}
		}
		else if (c_values[index + 1] == ";")
		{
			return true;
		}
		else
		{
			Error("parser", "missing `;` at the end of an expression");
		}
	}
	return false;
}

bool recognize_func_debug(vector<string> c_values, int index)
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
						Error("parser", "missing `;` token at the end of a function declaration");
				}
				else
					Error("parser", "function declaration must be followed by its actions (`{}`)");
			}
			else
				Error("parser", "function declaration must be followed by its anti-arguments (`[]`)");
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

bool recognize_keyword_debug(vector<string> c_values, int index)
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
				Error("parser", "missing `;` token at the end of a keyword call");
		}
		else
		{
			if (c_values[index + 1] == ";")
			{
				return true;
			}
			else
				Error("parser", "missing `;` token at the end of a keyword call");
		}
	}
	return false;
}

bool recognize_struct_debug(vector<string> c_values, int index)
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
					Error("parser", "missing `;` token at the end of a struct declaration");
			}
			else
			{
				Error("parser", "missing brackets for a struct declaration");
			}
		}
		else
		{
			Error("parser", "missing name for the structure declaration");
		}
	}
	return false;
}