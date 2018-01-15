typedef union {
    int             ival;
    char            *charp;
    io_port *port ;
    module  *mod ;
    struct {
      int ub ;
      int lb ;
    } range ;
    name_list  *namel ;
} YYSTYPE;
#define	YYID	257
#define	YYINUMBER	258
#define	YYRNUMBER	259
#define	YYSTRING	260
#define	YYENDMODULE	261
#define	YYMODULE	262
#define	YYINOUT	263
#define	YYINPUT	264
#define	YYOUTPUT	265
#define	YYPRIMITIVE	266
#define	YYENDPRIMITIVE	267
#define	YYOTHER	268


extern YYSTYPE yylval;
