"&~"		{
  yylval.keyword.linenum = lex_line_num ;
  yylval.keyword.filename = input_file_name ;
  return JD_ANDTILDA ;
}
"~&"		{
  yylval.keyword.linenum = lex_line_num ;
  yylval.keyword.filename = input_file_name ;
  return JD_TILDAAND ;
}
"|~"		{
  yylval.keyword.linenum = lex_line_num ;
  yylval.keyword.filename = input_file_name ;
  return JD_ORTILDA ;
}
"~|"		{
  yylval.keyword.linenum = lex_line_num ;
  yylval.keyword.filename = input_file_name ;
  return JD_TILDAOR ;
}
"^~"		{
  yylval.keyword.linenum = lex_line_num ;
  yylval.keyword.filename = input_file_name ;
  return JD_EORTILDA ;
}
"~^"		{
  yylval.keyword.linenum = lex_line_num ;
  yylval.keyword.filename = input_file_name ;
  return JD_TILDAEOR ;
}
"&&"		{
  yylval.keyword.linenum = lex_line_num ;
  yylval.keyword.filename = input_file_name ;
  return JD_LAND ;
}
"||"		{
  yylval.keyword.linenum = lex_line_num ;
  yylval.keyword.filename = input_file_name ;
  return JD_LOR ;
}
">>"		{
  yylval.keyword.linenum = lex_line_num ;
  yylval.keyword.filename = input_file_name ;
  return JD_RSHIFT ;
}
">>>"		{
  yylval.keyword.linenum = lex_line_num ;
  yylval.keyword.filename = input_file_name ;
  return JD_URSHIFT ;
}
"<<"		{
  yylval.keyword.linenum = lex_line_num ;
  yylval.keyword.filename = input_file_name ;
  return JD_LSHIFT ;
}
"=="		{
  yylval.keyword.linenum = lex_line_num ;
  yylval.keyword.filename = input_file_name ;
  return JD_EQEQ ;
}
"<="		{
  yylval.keyword.linenum = lex_line_num ;
  yylval.keyword.filename = input_file_name ;
  return JD_LE ;
}
">="		{
  yylval.keyword.linenum = lex_line_num ;
  yylval.keyword.filename = input_file_name ;
  return JD_GE ;
}
"!="		{
  yylval.keyword.linenum = lex_line_num ;
  yylval.keyword.filename = input_file_name ;
  return JD_NE ;
}
"==="		{
  yylval.keyword.linenum = lex_line_num ;
  yylval.keyword.filename = input_file_name ;
  return JD_EQEQEQ ;
}
"!=="		{
  yylval.keyword.linenum = lex_line_num ;
  yylval.keyword.filename = input_file_name ;
  return JD_NEQEQ ;
}
"=?="		{
  yylval.keyword.linenum = lex_line_num ;
  yylval.keyword.filename = input_file_name ;
  return JD_Q_EQ ;
}
"!?="		{
  yylval.keyword.linenum = lex_line_num ;
  yylval.keyword.filename = input_file_name ;
  return JD_Q_NEQ ;
}
"?=="		{
  yylval.keyword.linenum = lex_line_num ;
  yylval.keyword.filename = input_file_name ;
  return JD_QEQEQ ;
}
"==?"		{
  yylval.keyword.linenum = lex_line_num ;
  yylval.keyword.filename = input_file_name ;
  return JD_EQEQQ ;
}
"=:="		{
  yylval.keyword.linenum = lex_line_num ;
  yylval.keyword.filename = input_file_name ;
  return JD_C_EQEQ ;
}
"++"		{
  yylval.keyword.linenum = lex_line_num ;
  yylval.keyword.filename = input_file_name ;
  return JD_D_PLUS ;
}
"--"		{
  yylval.keyword.linenum = lex_line_num ;
  yylval.keyword.filename = input_file_name ;
  return JD_D_MINUS ;
}
"@@"		{
  yylval.keyword.linenum = lex_line_num ;
  yylval.keyword.filename = input_file_name ;
  return JD_D_AT ;
}
":="		{
  yylval.keyword.linenum = lex_line_num ;
  yylval.keyword.filename = input_file_name ;
  return JD_C_EQ ;
}
"+="		{
  yylval.keyword.linenum = lex_line_num ;
  yylval.keyword.filename = input_file_name ;
  return JD_PLUS_EQ ;
}
"-="		{
  yylval.keyword.linenum = lex_line_num ;
  yylval.keyword.filename = input_file_name ;
  return JD_MINUS_EQ ;
}
"*="		{
  yylval.keyword.linenum = lex_line_num ;
  yylval.keyword.filename = input_file_name ;
  return JD_STAR_EQ ;
}
"/="		{
  yylval.keyword.linenum = lex_line_num ;
  yylval.keyword.filename = input_file_name ;
  return JD_SLSH_EQ ;
}
"%="		{
  yylval.keyword.linenum = lex_line_num ;
  yylval.keyword.filename = input_file_name ;
  return JD_MOD_EQ ;
}
"<<="		{
  yylval.keyword.linenum = lex_line_num ;
  yylval.keyword.filename = input_file_name ;
  return JD_LSHIFT_EQ ;
}
">>="		{
  yylval.keyword.linenum = lex_line_num ;
  yylval.keyword.filename = input_file_name ;
  return JD_RSHIFT_EQ ;
}
">>>="		{
  yylval.keyword.linenum = lex_line_num ;
  yylval.keyword.filename = input_file_name ;
  return JD_URSHIFT_EQ ;
}
"&="		{
  yylval.keyword.linenum = lex_line_num ;
  yylval.keyword.filename = input_file_name ;
  return JD_AND_EQ ;
}
"|="		{
  yylval.keyword.linenum = lex_line_num ;
  yylval.keyword.filename = input_file_name ;
  return JD_OR_EQ ;
}
"^="		{
  yylval.keyword.linenum = lex_line_num ;
  yylval.keyword.filename = input_file_name ;
  return JD_EOR_EQ ;
}
"~&="		{
  yylval.keyword.linenum = lex_line_num ;
  yylval.keyword.filename = input_file_name ;
  return JD_NAND_EQ ;
}
"&~="		{
  yylval.keyword.linenum = lex_line_num ;
  yylval.keyword.filename = input_file_name ;
  return JD_ANDN_EQ ;
}
"~|="		{
  yylval.keyword.linenum = lex_line_num ;
  yylval.keyword.filename = input_file_name ;
  return JD_NOR_EQ ;
}
"|~="		{
  yylval.keyword.linenum = lex_line_num ;
  yylval.keyword.filename = input_file_name ;
  return JD_ORN_EQ ;
}
"~^="		{
  yylval.keyword.linenum = lex_line_num ;
  yylval.keyword.filename = input_file_name ;
  return JD_NEOR_EQ ;
}
"^~="		{
  yylval.keyword.linenum = lex_line_num ;
  yylval.keyword.filename = input_file_name ;
  return JD_EORN_EQ ;
}
