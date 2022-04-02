char escape_char(const char c)
{
	if (c == 't')
	{
		return '\t';
	}
	else if (c == 'n')
	{
		return '\n';
	}
	else if (c == 'r')
	{
		return '\r';
	}
	else if (c == '\\')
	{
		return '\\';
	}
	else if (c == '\"')
	{
		return '\"';
	}
	else
	{
		string err = "unkonwn escape caractere";
		Error("lexer", err);
	}
	return ' ';
}

// Il faudrais pouvoir renvoyer la liste des lexemes et celle des réferences.
vector<string> lex(const char *src, vector<string> &ref)
{
	const char *upper = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	const char *lower = "abcdefghijklmnopqrstuvwxyz!_:";
	const char *digit = "1234567890.";
	const char *opera = "-+=*^/%|<>";
	const char *complete = "{}[]()";
	int column = 0;
	int line = 0;
	int index = 0;
	int secon_index;
	string total[3]; // (Expression, ligne, colone)
	string word;
	vector<string> lexemes_stack;
	while (*(src + index) != '\0')
	{
		column++;
		if (*(src + index) == '/' and *(src + index + 1) == '/')
		{ // Les commentaires avec '//'
			if (word != "") // On peut garder ce qu'il y a avant
			{
				lexemes_stack.push_back(word);
				ref.push_back(to_string(line) + ":" + to_string(column));
				word = "";
			}
			// On initie le total à un car il y a un double-slash
			int anyway = 1;
			while (*(src + index + anyway) != '\n')
				anyway++;
			index += anyway;
		} 
		else if (*(src + index) == '/' and *(src+index+1) == '*')
		{ // Les commentaires avec /* ... */
			if (word != "") // On peut garder ce qu'il y a avant
			{
				lexemes_stack.push_back(word);
				ref.push_back(to_string(line) + ":" + to_string(column));
				word = "";
			}
			int anyway_ = 1;
			while ( true ) 
			{
				anyway_ ++;
				if (*(src + index + anyway_ - 1) == '*' and *(src + index + anyway_ ) == '/')
				{
					break;
				}
			}
			index += anyway_ + 1;
		}

		if (*(src + index) == '\n')
		{ // On ignorera les sauts de ligne, sauf pour tenir compte justement des lignes
			line += 1;
			column = 0;
		}

		if (*(src + index) == ' ' || *(src + index) == '\t' || *(src + index) == '\n')
		{
			if (word != "")
			{
				lexemes_stack.push_back(word);
				ref.push_back(to_string(line) + ":" + to_string(column));
				word = "";
			}
		}
		else if (in(*(src + index), lower) || in(*(src + index), upper) || in(*(src + index), digit))
		{
			word += *(src + index);
		}
		else if (*(src + index) == '-' and *(src + index + 1) == '>')
		{
			if (word != "")
			{
				lexemes_stack.push_back(word);
				ref.push_back(to_string(line) + ":" + to_string(column));
			}
			lexemes_stack.push_back("->");
			ref.push_back(to_string(line) + ":" + to_string(column));
			word = "";
			index++;
			column++;
		}
		else if (in(*(src + index), opera))
		{
			if (word != "")
			{
				lexemes_stack.push_back(word);
				ref.push_back(to_string(line) + ":" + to_string(column));
			}
			word = "";
			word += *(src + index);
			lexemes_stack.push_back(word);
			ref.push_back(to_string(line) + ":" + to_string(column));
			word = "";
		}
		else if (*(src + index) == '"')
		{
			if (word != "")
			{
				lexemes_stack.push_back(word);
				ref.push_back(to_string(line) + ":" + to_string(column));
			}
			word = "\"";
			secon_index = 1;
			ref.push_back(to_string(line) + ":" + to_string(column));

			while (*(src + secon_index + index) != *(src + index))
			{
				if (*(src + secon_index + index + 1) == '\0')
				{
					Error(to_string(line) + ":" + to_string(column), "Unmatched '\"' token");
					break;
				}
				else if (*(src + secon_index + index) == '\\')
				{
					word += escape_char(*(src + secon_index + index + 1));
					secon_index++;
				}
				else
				{
					word += *(src + secon_index + index);
				}

				secon_index++;
			}
			word += *(src + secon_index + index);
			lexemes_stack.push_back(word);
			word = "";
			index += secon_index;
			column += secon_index;
		}
		else if (*(src + index) == ';')
		{
			if (word != "")
			{
				lexemes_stack.push_back(word);
			}
			word = "";
			lexemes_stack.push_back(";");
		}
		else if (in(*(src + index), complete))
		{
			if (word != "")
			{
				lexemes_stack.push_back(word);
			}
			word = *(src + index);
			lexemes_stack.push_back(word);
			word = "";
		}
		else if (*(src + index) == ',')
		{
			if (word != "")
			{
				lexemes_stack.push_back(word);
			}
			word = "";
			lexemes_stack.push_back(",");
		}
		else
		{
			string error_mess = "Unknown token '";
			error_mess += *(src + index);
			error_mess += "'";
			Error(to_string(line) + ":" + to_string(column), error_mess);
		}
		index++;
	}
	return lexemes_stack;
}
