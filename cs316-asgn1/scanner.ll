%filenames="scanner"
%lex-source="scanner.cc"

%%
//ADD YOUR CODE HERE

int 						{ return Parser::INTEGER;}
void 						{ return Parser::VOID;}
return						{ return Parser::RETURN;}
=							{ return Parser::ASSIGN;}

[-]?[0-9]+					{
								ParserBase::STYPE__ *val = getSval();
	                			val->integer_value = atoi(matched().c_str());
	                			//cout << val->integer_value << endl;
								return Parser::INTEGER_NUMBER;
							}

[a-zA-Z_]([a-zA-Z_0-9])*	{
								ParserBase::STYPE__ *val = getSval();
	                			val->string_value = new string(matched());
	                			//cout << *(val->string_value) << endl;
								return Parser::NAME;
							}

[(){};] 					{ //cout << matched()[0] << endl;
							  return matched()[0];
							}

\n    		|
";;".*  	|
[ \t]*";;".*	|
[ \t]*"//".*	|
[ \t]		{
			if (command_options.is_show_tokens_selected())
				ignore_token();
		}

.		{
			string error_message;
			error_message =  "Illegal character `" + matched();
			error_message += "' on line " + lineNr();

			CHECK_INPUT(CONTROL_SHOULD_NOT_REACH, error_message, lineNr());
		}
