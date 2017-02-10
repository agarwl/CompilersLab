%filenames="scanner"
%lex-source="scanner.cc"

digit [0-9]
operators [-+/\*]
letters [a-zA-Z_]
meta_chars [)({};]
%%
//ADD YOUR CODE HERE

float                       {
                              store_token_name("FLOAT");
                              return Parser::FLOAT;
                            }

int                         {
                              store_token_name("INTEGER");
                              return Parser::INTEGER;
                            }

void                        {
                              store_token_name("VOID");
                              return Parser::VOID;
                            }

=                           {
                              store_token_name("ASSIGN_OP");
                              return Parser::ASSIGN;
                            }

{operators}                 {
                              store_token_name("ARITHOP");
                              return matched()[0];
                            }

{meta_chars}                {
                              store_token_name("META CHAR");
                              return matched()[0];
                            }

{digit}+                    {
                                store_token_name("NUM");
                                ParserBase::STYPE__ *val = getSval();
                                val->integer_value = atoi(matched().c_str());
                                return Parser::INTEGER_NUMBER;
                            }

({digit}*\.{digit}+|{digit}+[\.]?{digit}*)([eE][+-]?{digit}+)?     {
                              store_token_name("FNUM");
                              ParserBase::STYPE__ *val = getSval();
                              val->double_value = atof(matched().c_str());
                              return Parser::DOUBLE_NUMBER;
                            }

{letters}({letters}|{digit})*    {
                                    store_token_name("NAME");
                                    ParserBase::STYPE__ *val = getSval();
                                    val->string_value = new string(matched());
                                    return Parser::NAME;
                                 }


\n          |
";;".*      |
[ \t]*";;".*    |
[ \t]*"//".*    |
[ \t]       {
            if (command_options.is_show_tokens_selected())
                ignore_token();
        }

.       {
            string error_message;
            error_message =  "Illegal character `" + matched();
            error_message += "' on line " + lineNr();

            CHECK_INPUT(CONTROL_SHOULD_NOT_REACH, error_message, lineNr());
        }
