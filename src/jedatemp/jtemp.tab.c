
/*  A Bison parser, made from jtemp.y
    by GNU Bison version 1.28  */

#define YYBISON 1  /* Identify Bison output.  */

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

#line 28 "jtemp.y"


#include <stdio.h>
#include "jtemp.h"

module *top_module = NULL ;


#line 38 "jtemp.y"
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
#include <stdio.h>

#ifndef __cplusplus
#ifndef __STDC__
#define const
#endif
#endif



#define	YYFINAL		98
#define	YYFLAG		-32768
#define	YYNTBASE	29

#define YYTRANSLATE(x) ((unsigned)(x) <= 268 ? yytranslate[x] : 53)

static const char yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,    15,
    16,    19,    17,    22,    18,    28,    20,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,    27,    21,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
    25,     2,    26,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,    23,     2,    24,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     1,     3,     4,     5,     6,
     7,     8,     9,    10,    11,    12,    13,    14
};

#if YYDEBUG != 0
static const short yyprhs[] = {     0,
     0,     1,     4,     6,     8,    15,    16,    20,    22,    26,
    28,    32,    34,    38,    39,    43,    44,    48,    54,    55,
    57,    59,    62,    64,    66,    68,    70,    75,    80,    85,
    86,    88,    94,    96,   100,   104,   108,   112,   116,   118,
   122,   124,   128,   133,   135,   138,   140,   142,   144,   146,
   148,   150,   152,   154,   156,   158,   160,   162,   164,   166
};

static const short yyrhs[] = {    -1,
    29,    30,     0,    31,     0,    50,     0,     8,     3,    32,
    21,    39,     7,     0,     0,    15,    33,    16,     0,    34,
     0,    33,    22,    34,     0,    36,     0,    23,    35,    24,
     0,    36,     0,    35,    22,    36,     0,     0,     3,    37,
    38,     0,     0,    25,     4,    26,     0,    25,     4,    27,
     4,    26,     0,     0,    40,     0,    41,     0,    40,    41,
     0,    42,     0,    43,     0,    44,     0,    51,     0,    10,
    45,    48,    21,     0,    11,    45,    48,    21,     0,     9,
    45,    48,    21,     0,     0,    46,     0,    25,    47,    27,
    47,    26,     0,     4,     0,    47,    17,    47,     0,    47,
    18,    47,     0,    47,    19,    47,     0,    47,    20,    47,
     0,    15,    47,    16,     0,    49,     0,    48,    22,    49,
     0,     3,     0,    49,    28,     3,     0,    12,     3,    39,
    13,     0,    52,     0,    51,    52,     0,    14,     0,     3,
     0,     4,     0,     5,     0,     6,     0,    15,     0,    16,
     0,    25,     0,    26,     0,    23,     0,    24,     0,    28,
     0,    22,     0,    27,     0,    21,     0
};

#endif

#if YYDEBUG != 0
static const short yyrline[] = { 0,
    91,    92,   107,   108,   113,   125,   126,   130,   131,   135,
   136,   140,   141,   145,   146,   150,   152,   153,   157,   158,
   162,   163,   177,   178,   179,   180,   185,   211,   237,   263,
   265,   270,   278,   282,   286,   290,   294,   298,   305,   313,
   327,   328,   332,   339,   340,   343,   344,   345,   346,   347,
   348,   349,   350,   351,   352,   353,   354,   355,   356,   357
};
#endif


#if YYDEBUG != 0 || defined (YYERROR_VERBOSE)

static const char * const yytname[] = {   "$","error","$undefined.","YYID","YYINUMBER",
"YYRNUMBER","YYSTRING","YYENDMODULE","YYMODULE","YYINOUT","YYINPUT","YYOUTPUT",
"YYPRIMITIVE","YYENDPRIMITIVE","YYOTHER","'('","')'","'+'","'-'","'*'","'/'",
"';'","','","'{'","'}'","'['","']'","':'","'.'","source_text","description",
"module","port_list_opt","port_list","port","port_ref_list","port_reference",
"@1","port_reference_arg","opt_module_items","module_items","module_item","input_declaration",
"output_declaration","inout_declaration","range_opt","range","constant","variable_list",
"identifier","primitive","others","any", NULL
};
#endif

static const short yyr1[] = {     0,
    29,    29,    30,    30,    31,    32,    32,    33,    33,    34,
    34,    35,    35,    37,    36,    38,    38,    38,    39,    39,
    40,    40,    41,    41,    41,    41,    42,    43,    44,    45,
    45,    46,    47,    47,    47,    47,    47,    47,    48,    48,
    49,    49,    50,    51,    51,    52,    52,    52,    52,    52,
    52,    52,    52,    52,    52,    52,    52,    52,    52,    52
};

static const short yyr2[] = {     0,
     0,     2,     1,     1,     6,     0,     3,     1,     3,     1,
     3,     1,     3,     0,     3,     0,     3,     5,     0,     1,
     1,     2,     1,     1,     1,     1,     4,     4,     4,     0,
     1,     5,     1,     3,     3,     3,     3,     3,     1,     3,
     1,     3,     4,     1,     2,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1
};

static const short yydefact[] = {     1,
     0,     0,     0,     2,     3,     4,     6,    19,     0,     0,
    47,    48,    49,    50,    30,    30,    30,    46,    51,    52,
    60,    58,    55,    56,    53,    54,    59,    57,     0,    20,
    21,    23,    24,    25,    26,    44,    14,     0,     0,     8,
    10,    19,     0,     0,    31,     0,     0,    43,    22,    45,
    16,     0,    12,     7,     0,     0,    33,     0,     0,    41,
     0,    39,     0,     0,     0,    15,     0,    11,     9,     5,
     0,     0,     0,     0,     0,     0,    29,     0,     0,    27,
    28,     0,    13,    38,    34,    35,    36,    37,     0,    40,
    42,    17,     0,    32,     0,    18,     0,     0
};

static const short yydefgoto[] = {     1,
     4,     5,    10,    39,    40,    52,    41,    51,    66,    29,
    30,    31,    32,    33,    34,    44,    45,    59,    61,    62,
     6,    35,    36
};

static const short yypact[] = {-32768,
    28,     3,     7,-32768,-32768,-32768,    19,    -2,    12,    -3,
-32768,-32768,-32768,-32768,     2,     2,     2,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,    25,    -2,
-32768,-32768,-32768,-32768,    27,-32768,-32768,    44,    -5,-32768,
-32768,    -2,     1,    85,-32768,    85,    85,-32768,-32768,-32768,
    64,    15,-32768,-32768,    12,    83,-32768,     1,    46,-32768,
    24,    65,    35,    53,    87,-32768,    44,-32768,-32768,-32768,
    61,     1,     1,     1,     1,     1,-32768,    85,    89,-32768,
-32768,    45,-32768,-32768,    63,    63,-32768,-32768,    50,    65,
-32768,-32768,    90,-32768,    69,-32768,    96,-32768
};

static const short yypgoto[] = {-32768,
-32768,-32768,-32768,-32768,    42,-32768,   -38,-32768,-32768,    56,
-32768,    70,-32768,-32768,-32768,    68,-32768,   -14,    40,    21,
-32768,-32768,    66
};


#define	YYLAST		101


static const short yytable[] = {    53,
    11,    12,    13,    14,    57,     7,    15,    16,    17,     8,
    54,    18,    19,    20,    37,    58,    55,    42,    21,    22,
    23,    24,    25,    26,    27,    28,    43,    97,    83,    11,
    12,    13,    14,     9,    38,     2,    67,    48,    68,     3,
    18,    19,    20,    71,    77,    78,    37,    21,    22,    23,
    24,    25,    26,    27,    28,    80,    78,    85,    86,    87,
    88,    89,    72,    73,    74,    75,    72,    73,    74,    75,
    92,    93,    76,    81,    78,    94,    84,    72,    73,    74,
    75,    74,    75,    46,    47,    63,    64,    60,    65,    70,
    82,    91,    79,    95,    96,    98,    69,    56,    90,    49,
    50
};

static const short yycheck[] = {    38,
     3,     4,     5,     6,     4,     3,     9,    10,    11,     3,
    16,    14,    15,    16,     3,    15,    22,    21,    21,    22,
    23,    24,    25,    26,    27,    28,    25,     0,    67,     3,
     4,     5,     6,    15,    23,     8,    22,    13,    24,    12,
    14,    15,    16,    58,    21,    22,     3,    21,    22,    23,
    24,    25,    26,    27,    28,    21,    22,    72,    73,    74,
    75,    76,    17,    18,    19,    20,    17,    18,    19,    20,
    26,    27,    27,    21,    22,    26,    16,    17,    18,    19,
    20,    19,    20,    16,    17,    46,    47,     3,    25,     7,
     4,     3,    28,     4,    26,     0,    55,    42,    78,    30,
    35
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */
#line 3 "/usr/share/bison.simple"
/* This file comes from bison-1.28.  */

/* Skeleton output parser for bison,
   Copyright (C) 1984, 1989, 1990 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* This is the parser code that is written into each bison parser
  when the %semantic_parser declaration is not specified in the grammar.
  It was written by Richard Stallman by simplifying the hairy parser
  used when %semantic_parser is specified.  */

#ifndef YYSTACK_USE_ALLOCA
#ifdef alloca
#define YYSTACK_USE_ALLOCA
#else /* alloca not defined */
#ifdef __GNUC__
#define YYSTACK_USE_ALLOCA
#define alloca __builtin_alloca
#else /* not GNU C.  */
#if (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc) || defined (__sgi) || (defined (__sun) && defined (__i386))
#define YYSTACK_USE_ALLOCA
#include <alloca.h>
#else /* not sparc */
/* We think this test detects Watcom and Microsoft C.  */
/* This used to test MSDOS, but that is a bad idea
   since that symbol is in the user namespace.  */
#if (defined (_MSDOS) || defined (_MSDOS_)) && !defined (__TURBOC__)
#if 0 /* No need for malloc.h, which pollutes the namespace;
	 instead, just don't use alloca.  */
#include <malloc.h>
#endif
#else /* not MSDOS, or __TURBOC__ */
#if defined(_AIX)
/* I don't know what this was needed for, but it pollutes the namespace.
   So I turned it off.   rms, 2 May 1997.  */
/* #include <malloc.h>  */
 #pragma alloca
#define YYSTACK_USE_ALLOCA
#else /* not MSDOS, or __TURBOC__, or _AIX */
#if 0
#ifdef __hpux /* haible@ilog.fr says this works for HPUX 9.05 and up,
		 and on HPUX 10.  Eventually we can turn this on.  */
#define YYSTACK_USE_ALLOCA
#define alloca __builtin_alloca
#endif /* __hpux */
#endif
#endif /* not _AIX */
#endif /* not MSDOS, or __TURBOC__ */
#endif /* not sparc */
#endif /* not GNU C */
#endif /* alloca not defined */
#endif /* YYSTACK_USE_ALLOCA not defined */

#ifdef YYSTACK_USE_ALLOCA
#define YYSTACK_ALLOC alloca
#else
#define YYSTACK_ALLOC malloc
#endif

/* Note: there must be only one dollar sign in this file.
   It is replaced by the list of actions, each action
   as one case of the switch.  */

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	goto yyacceptlab
#define YYABORT 	goto yyabortlab
#define YYERROR		goto yyerrlab1
/* Like YYERROR except do call yyerror.
   This remains here temporarily to ease the
   transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL		goto yyerrlab
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(token, value) \
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    { yychar = (token), yylval = (value);			\
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { yyerror ("syntax error: cannot back up"); YYERROR; }	\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

#ifndef YYPURE
#define YYLEX		yylex()
#endif

#ifdef YYPURE
#ifdef YYLSP_NEEDED
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, &yylloc, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval, &yylloc)
#endif
#else /* not YYLSP_NEEDED */
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval)
#endif
#endif /* not YYLSP_NEEDED */
#endif

/* If nonreentrant, generate the variables here */

#ifndef YYPURE

int	yychar;			/*  the lookahead symbol		*/
YYSTYPE	yylval;			/*  the semantic value of the		*/
				/*  lookahead symbol			*/

#ifdef YYLSP_NEEDED
YYLTYPE yylloc;			/*  location data for the lookahead	*/
				/*  symbol				*/
#endif

int yynerrs;			/*  number of parse errors so far       */
#endif  /* not YYPURE */

#if YYDEBUG != 0
int yydebug;			/*  nonzero means print parse trace	*/
/* Since this is uninitialized, it does not stop multiple parsers
   from coexisting.  */
#endif

/*  YYINITDEPTH indicates the initial size of the parser's stacks	*/

#ifndef	YYINITDEPTH
#define YYINITDEPTH 200
#endif

/*  YYMAXDEPTH is the maximum size the stacks can grow to
    (effective only if the built-in stack extension method is used).  */

#if YYMAXDEPTH == 0
#undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
#define YYMAXDEPTH 10000
#endif

/* Define __yy_memcpy.  Note that the size argument
   should be passed with type unsigned int, because that is what the non-GCC
   definitions require.  With GCC, __builtin_memcpy takes an arg
   of type size_t, but it can handle unsigned int.  */

#if __GNUC__ > 1		/* GNU C and GNU C++ define this.  */
#define __yy_memcpy(TO,FROM,COUNT)	__builtin_memcpy(TO,FROM,COUNT)
#else				/* not GNU C or C++ */
#ifndef __cplusplus

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (to, from, count)
     char *to;
     char *from;
     unsigned int count;
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#else /* __cplusplus */

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (char *to, char *from, unsigned int count)
{
  register char *t = to;
  register char *f = from;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#endif
#endif

#line 217 "/usr/share/bison.simple"

/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
#ifdef __cplusplus
#define YYPARSE_PARAM_ARG void *YYPARSE_PARAM
#define YYPARSE_PARAM_DECL
#else /* not __cplusplus */
#define YYPARSE_PARAM_ARG YYPARSE_PARAM
#define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
#endif /* not __cplusplus */
#else /* not YYPARSE_PARAM */
#define YYPARSE_PARAM_ARG
#define YYPARSE_PARAM_DECL
#endif /* not YYPARSE_PARAM */

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
#ifdef YYPARSE_PARAM
int yyparse (void *);
#else
int yyparse (void);
#endif
#endif

int
yyparse(YYPARSE_PARAM_ARG)
     YYPARSE_PARAM_DECL
{
  register int yystate;
  register int yyn;
  register short *yyssp;
  register YYSTYPE *yyvsp;
  int yyerrstatus;	/*  number of tokens to shift before error messages enabled */
  int yychar1 = 0;		/*  lookahead token as an internal (translated) token number */

  short	yyssa[YYINITDEPTH];	/*  the state stack			*/
  YYSTYPE yyvsa[YYINITDEPTH];	/*  the semantic value stack		*/

  short *yyss = yyssa;		/*  refer to the stacks thru separate pointers */
  YYSTYPE *yyvs = yyvsa;	/*  to allow yyoverflow to reallocate them elsewhere */

#ifdef YYLSP_NEEDED
  YYLTYPE yylsa[YYINITDEPTH];	/*  the location stack			*/
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;

#define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
#define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  int yystacksize = YYINITDEPTH;
  int yyfree_stacks = 0;

#ifdef YYPURE
  int yychar;
  YYSTYPE yylval;
  int yynerrs;
#ifdef YYLSP_NEEDED
  YYLTYPE yylloc;
#endif
#endif

  YYSTYPE yyval;		/*  the variable used to return		*/
				/*  semantic values from the action	*/
				/*  routines				*/

  int yylen;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Starting parse\n");
#endif

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss - 1;
  yyvsp = yyvs;
#ifdef YYLSP_NEEDED
  yylsp = yyls;
#endif

/* Push a new state, which is found in  yystate  .  */
/* In all cases, when you get here, the value and location stacks
   have just been pushed. so pushing a state here evens the stacks.  */
yynewstate:

  *++yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Give user a chance to reallocate the stack */
      /* Use copies of these so that the &'s don't force the real ones into memory. */
      YYSTYPE *yyvs1 = yyvs;
      short *yyss1 = yyss;
#ifdef YYLSP_NEEDED
      YYLTYPE *yyls1 = yyls;
#endif

      /* Get the current used size of the three stacks, in elements.  */
      int size = yyssp - yyss + 1;

#ifdef yyoverflow
      /* Each stack pointer address is followed by the size of
	 the data in use in that stack, in bytes.  */
#ifdef YYLSP_NEEDED
      /* This used to be a conditional around just the two extra args,
	 but that might be undefined if yyoverflow is a macro.  */
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yyls1, size * sizeof (*yylsp),
		 &yystacksize);
#else
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yystacksize);
#endif

      yyss = yyss1; yyvs = yyvs1;
#ifdef YYLSP_NEEDED
      yyls = yyls1;
#endif
#else /* no yyoverflow */
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	{
	  yyerror("parser stack overflow");
	  if (yyfree_stacks)
	    {
	      free (yyss);
	      free (yyvs);
#ifdef YYLSP_NEEDED
	      free (yyls);
#endif
	    }
	  return 2;
	}
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;
#ifndef YYSTACK_USE_ALLOCA
      yyfree_stacks = 1;
#endif
      yyss = (short *) YYSTACK_ALLOC (yystacksize * sizeof (*yyssp));
      __yy_memcpy ((char *)yyss, (char *)yyss1,
		   size * (unsigned int) sizeof (*yyssp));
      yyvs = (YYSTYPE *) YYSTACK_ALLOC (yystacksize * sizeof (*yyvsp));
      __yy_memcpy ((char *)yyvs, (char *)yyvs1,
		   size * (unsigned int) sizeof (*yyvsp));
#ifdef YYLSP_NEEDED
      yyls = (YYLTYPE *) YYSTACK_ALLOC (yystacksize * sizeof (*yylsp));
      __yy_memcpy ((char *)yyls, (char *)yyls1,
		   size * (unsigned int) sizeof (*yylsp));
#endif
#endif /* no yyoverflow */

      yyssp = yyss + size - 1;
      yyvsp = yyvs + size - 1;
#ifdef YYLSP_NEEDED
      yylsp = yyls + size - 1;
#endif

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Stack size increased to %d\n", yystacksize);
#endif

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Entering state %d\n", yystate);
#endif

  goto yybackup;
 yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Reading a token: ");
#endif
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)		/* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more */

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Now at end of input.\n");
#endif
    }
  else
    {
      yychar1 = YYTRANSLATE(yychar);

#if YYDEBUG != 0
      if (yydebug)
	{
	  fprintf (stderr, "Next token is %d (%s", yychar, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise meaning
	     of a token, for further debugging info.  */
#ifdef YYPRINT
	  YYPRINT (stderr, yychar, yylval);
#endif
	  fprintf (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    goto yydefault;

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrlab;

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting token %d (%s), ", yychar, yytname[yychar1]);
#endif

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  /* count tokens shifted since error; after three, turn off error status.  */
  if (yyerrstatus) yyerrstatus--;

  yystate = yyn;
  goto yynewstate;

/* Do the default action for the current state.  */
yydefault:

  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;

/* Do a reduction.  yyn is the number of a rule to reduce with.  */
yyreduce:
  yylen = yyr2[yyn];
  if (yylen > 0)
    yyval = yyvsp[1-yylen]; /* implement default value of the action */

#if YYDEBUG != 0
  if (yydebug)
    {
      int i;

      fprintf (stderr, "Reducing via rule %d (line %d), ",
	       yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (i = yyprhs[yyn]; yyrhs[i] > 0; i++)
	fprintf (stderr, "%s ", yytname[yyrhs[i]]);
      fprintf (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif


  switch (yyn) {

case 1:
#line 91 "jtemp.y"
{ yyval.mod = NULL ; ;
    break;}
case 2:
#line 93 "jtemp.y"
{
          module *mp ;
          if( yyvsp[-1].mod ) {
            mp = yyvsp[-1].mod ;
            while( mp->next ) mp = mp->next ;
            mp->next = yyvsp[0].mod ;
            yyval.mod = yyvsp[-1].mod ;
          }
          else yyval.mod = yyvsp[0].mod ;
          top_module = yyval.mod ;
        ;
    break;}
case 3:
#line 107 "jtemp.y"
{ yyval.mod = yyvsp[0].mod ; ;
    break;}
case 4:
#line 108 "jtemp.y"
{ yyval.mod = yyvsp[0].mod ; ;
    break;}
case 5:
#line 117 "jtemp.y"
{
	  yyval.mod = (module *)calloc( 1, sizeof(module) ) ;
	  yyval.mod->name = (char *)strdup(yyvsp[-4].charp) ;
	  yyval.mod->port = yyvsp[-1].port ;
	;
    break;}
case 6:
#line 125 "jtemp.y"
{ yyval.port = 0 ; ;
    break;}
case 7:
#line 126 "jtemp.y"
{ yyval.port = NULL ; ;
    break;}
case 8:
#line 130 "jtemp.y"
{ yyval.port = NULL ; ;
    break;}
case 9:
#line 131 "jtemp.y"
{ yyval.port = NULL ; ;
    break;}
case 10:
#line 135 "jtemp.y"
{ yyval.port = NULL ; ;
    break;}
case 11:
#line 136 "jtemp.y"
{ yyval.port = NULL ; ;
    break;}
case 12:
#line 140 "jtemp.y"
{ yyval.charp = NULL ; ;
    break;}
case 13:
#line 141 "jtemp.y"
{ yyval.charp = NULL ; ;
    break;}
case 14:
#line 145 "jtemp.y"
{ yyval.port = NULL ; ;
    break;}
case 15:
#line 146 "jtemp.y"
{ yyval.port = NULL ; ;
    break;}
case 16:
#line 151 "jtemp.y"
{ yyval.charp = NULL ; ;
    break;}
case 17:
#line 152 "jtemp.y"
{ yyval.charp = NULL ; ;
    break;}
case 18:
#line 153 "jtemp.y"
{ yyval.charp = NULL ; ;
    break;}
case 19:
#line 157 "jtemp.y"
{ yyval.port = NULL ; ;
    break;}
case 20:
#line 158 "jtemp.y"
{ yyval.port = yyvsp[0].port ; ;
    break;}
case 21:
#line 162 "jtemp.y"
{ yyval.port = yyvsp[0].port ; ;
    break;}
case 22:
#line 164 "jtemp.y"
{
          io_port *pp ;
          if( yyvsp[-1].port ) {
            pp = yyvsp[-1].port ;
            while( pp->next ) pp = pp->next ;
            pp->next = yyvsp[0].port ;
            yyval.port = yyvsp[-1].port ;
          }
          else yyval.port = yyvsp[0].port ;
        ;
    break;}
case 23:
#line 177 "jtemp.y"
{ yyval.port = yyvsp[0].port ; ;
    break;}
case 24:
#line 178 "jtemp.y"
{ yyval.port = yyvsp[0].port ; ;
    break;}
case 25:
#line 179 "jtemp.y"
{ yyval.port = yyvsp[0].port ; ;
    break;}
case 26:
#line 180 "jtemp.y"
{ yyval.port = yyvsp[0].port ; ;
    break;}
case 27:
#line 186 "jtemp.y"
{ 
          name_list  *namel ;
          io_port *port, *i_port, *p_port ;
          
          i_port = NULL ;
          
          if( yyvsp[-1].namel ) {
            namel = yyvsp[-1].namel ;
            while( namel ) {
              port = (io_port *)calloc(1,sizeof(io_port)) ; 
              if( i_port == NULL ) i_port = port ;
              else p_port->next = port ;
              p_port = port ;
              port->dir = 0 ; /* 0:input 1:output 2: inout */
              port->name = namel->name ;
              port->ub = yyvsp[-2].range.ub ;
              port->lb = yyvsp[-2].range.lb ;
              namel = namel->next ;
            }
          }
          yyval.port = i_port ;
        ;
    break;}
case 28:
#line 212 "jtemp.y"
{ 
          name_list  *namel ;
          io_port *port, *i_port, *p_port ;
          
          i_port = NULL ;
          
          if( yyvsp[-1].namel ) {
            namel = yyvsp[-1].namel ;
            while( namel ) {
              port = (io_port *)calloc(1,sizeof(io_port)) ; 
              if( i_port == NULL ) i_port = port ;
              else p_port->next = port ;
              p_port = port ;
              port->dir = 1 ; /* 0:input 1:output 2: inout */
              port->name = namel->name ;
              port->ub = yyvsp[-2].range.ub ;
              port->lb = yyvsp[-2].range.lb ;
              namel = namel->next ;
            }
          }
          yyval.port = i_port ;
        ;
    break;}
case 29:
#line 238 "jtemp.y"
{ 
          name_list  *namel ;
          io_port *port, *i_port, *p_port ;
          
          i_port = NULL ;
          
          if( yyvsp[-1].namel ) {
            namel = yyvsp[-1].namel ;
            while( namel ) {
              port = (io_port *)calloc(1,sizeof(io_port)) ; 
              if( i_port == NULL ) i_port = port ;
              else p_port->next = port ;
              p_port = port ;
              port->dir = 2 ; /* 0:input 1:output 2: inout */
              port->name = namel->name ;
              port->ub = yyvsp[-2].range.ub ;
              port->lb = yyvsp[-2].range.lb ;
              namel = namel->next ;
            }
          }
          yyval.port = i_port ;
        ;
    break;}
case 30:
#line 264 "jtemp.y"
{ yyval.range.ub = yyval.range.lb = -1 ; ;
    break;}
case 31:
#line 266 "jtemp.y"
{ yyval.range = yyvsp[0].range ; ;
    break;}
case 32:
#line 271 "jtemp.y"
{
          yyval.range.ub = yyvsp[-3].ival ;
          yyval.range.lb = yyvsp[-1].ival ;
        ;
    break;}
case 33:
#line 279 "jtemp.y"
{
	  yyval.ival = yyvsp[0].ival ;
	;
    break;}
case 34:
#line 283 "jtemp.y"
{
	  yyval.ival = yyvsp[-2].ival + yyvsp[0].ival ;
	;
    break;}
case 35:
#line 287 "jtemp.y"
{
	  yyval.ival = yyvsp[-2].ival - yyvsp[0].ival ;
	;
    break;}
case 36:
#line 291 "jtemp.y"
{
	  yyval.ival = yyvsp[-2].ival * yyvsp[0].ival ;
	;
    break;}
case 37:
#line 295 "jtemp.y"
{
	  yyval.ival = yyvsp[-2].ival / yyvsp[0].ival ;
	;
    break;}
case 38:
#line 299 "jtemp.y"
{
	  yyval.ival = yyvsp[-1].ival ;
	;
    break;}
case 39:
#line 306 "jtemp.y"
{
          if( yyvsp[0].charp ) {
            yyval.namel = (name_list *)calloc( 1, sizeof(name_list) ) ;
            yyval.namel->name = (char *)strdup(yyvsp[0].charp) ;
          }
          else yyval.namel = NULL ;
        ;
    break;}
case 40:
#line 314 "jtemp.y"
{
          name_list  *namel ;
          yyval.namel = yyvsp[-2].namel ;
          if( yyvsp[0].charp ) {
            namel = yyval.namel ;
            while( namel->next ) namel = namel->next ;
            namel->next = (name_list *)calloc( 1, sizeof(name_list) ) ;
            namel->next->name = (char *)strdup(yyvsp[0].charp) ;
          }
        ;
    break;}
case 41:
#line 327 "jtemp.y"
{ yyval.charp = yyvsp[0].charp ;
    break;}
case 42:
#line 328 "jtemp.y"
{ yyval.charp = NULL ; ;
    break;}
case 43:
#line 335 "jtemp.y"
{ yyval.mod = NULL ; ;
    break;}
case 44:
#line 339 "jtemp.y"
{ yyval.port = NULL ; ;
    break;}
case 45:
#line 340 "jtemp.y"
{ yyval.port = NULL ; ;
    break;}
case 46:
#line 343 "jtemp.y"
{ yyval.charp = NULL ; ;
    break;}
case 47:
#line 344 "jtemp.y"
{ yyval.charp = NULL ; ;
    break;}
case 48:
#line 345 "jtemp.y"
{ yyval.charp = NULL ; ;
    break;}
case 49:
#line 346 "jtemp.y"
{ yyval.charp = NULL ; ;
    break;}
case 50:
#line 347 "jtemp.y"
{ yyval.charp = NULL ; ;
    break;}
case 51:
#line 348 "jtemp.y"
{ yyval.charp = NULL ; ;
    break;}
case 52:
#line 349 "jtemp.y"
{ yyval.charp = NULL ; ;
    break;}
case 53:
#line 350 "jtemp.y"
{ yyval.charp = NULL ; ;
    break;}
case 54:
#line 351 "jtemp.y"
{ yyval.charp = NULL ; ;
    break;}
case 55:
#line 352 "jtemp.y"
{ yyval.charp = NULL ; ;
    break;}
case 56:
#line 353 "jtemp.y"
{ yyval.charp = NULL ; ;
    break;}
case 57:
#line 354 "jtemp.y"
{ yyval.charp = NULL ; ;
    break;}
case 58:
#line 355 "jtemp.y"
{ yyval.charp = NULL ; ;
    break;}
case 59:
#line 356 "jtemp.y"
{ yyval.charp = NULL ; ;
    break;}
case 60:
#line 357 "jtemp.y"
{ yyval.charp = NULL ; ;
    break;}
}
   /* the action file gets copied in in place of this dollarsign */
#line 543 "/usr/share/bison.simple"

  yyvsp -= yylen;
  yyssp -= yylen;
#ifdef YYLSP_NEEDED
  yylsp -= yylen;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;

#ifdef YYLSP_NEEDED
  yylsp++;
  if (yylen == 0)
    {
      yylsp->first_line = yylloc.first_line;
      yylsp->first_column = yylloc.first_column;
      yylsp->last_line = (yylsp-1)->last_line;
      yylsp->last_column = (yylsp-1)->last_column;
      yylsp->text = 0;
    }
  else
    {
      yylsp->last_line = (yylsp+yylen-1)->last_line;
      yylsp->last_column = (yylsp+yylen-1)->last_column;
    }
#endif

  /* Now "shift" the result of the reduction.
     Determine what state that goes to,
     based on the state we popped back to
     and the rule number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;

yyerrlab:   /* here on detecting error */

  if (! yyerrstatus)
    /* If not already recovering from an error, report this error.  */
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  int size = 0;
	  char *msg;
	  int x, count;

	  count = 0;
	  /* Start X at -yyn if nec to avoid negative indexes in yycheck.  */
	  for (x = (yyn < 0 ? -yyn : 0);
	       x < (sizeof(yytname) / sizeof(char *)); x++)
	    if (yycheck[x + yyn] == x)
	      size += strlen(yytname[x]) + 15, count++;
	  msg = (char *) malloc(size + 15);
	  if (msg != 0)
	    {
	      strcpy(msg, "parse error");

	      if (count < 5)
		{
		  count = 0;
		  for (x = (yyn < 0 ? -yyn : 0);
		       x < (sizeof(yytname) / sizeof(char *)); x++)
		    if (yycheck[x + yyn] == x)
		      {
			strcat(msg, count == 0 ? ", expecting `" : " or `");
			strcat(msg, yytname[x]);
			strcat(msg, "'");
			count++;
		      }
		}
	      yyerror(msg);
	      free(msg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exceeded");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror("parse error");
    }

  goto yyerrlab1;
yyerrlab1:   /* here on error raised explicitly by an action */

  if (yyerrstatus == 3)
    {
      /* if just tried and failed to reuse lookahead token after an error, discard it.  */

      /* return failure if at end of input */
      if (yychar == YYEOF)
	YYABORT;

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Discarding token %d (%s).\n", yychar, yytname[yychar1]);
#endif

      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token
     after shifting the error token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;

yyerrdefault:  /* current state does not do anything special for the error token. */

#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */
  yyn = yydefact[yystate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/
  if (yyn) goto yydefault;
#endif

yyerrpop:   /* pop the current state because it cannot handle the error token */

  if (yyssp == yyss) YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#ifdef YYLSP_NEEDED
  yylsp--;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "Error: state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

yyerrhandle:

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yyerrdefault;

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    goto yyerrdefault;

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrpop;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrpop;

  if (yyn == YYFINAL)
    YYACCEPT;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting error token, ");
#endif

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  yystate = yyn;
  goto yynewstate;

 yyacceptlab:
  /* YYACCEPT comes here.  */
  if (yyfree_stacks)
    {
      free (yyss);
      free (yyvs);
#ifdef YYLSP_NEEDED
      free (yyls);
#endif
    }
  return 0;

 yyabortlab:
  /* YYABORT comes here.  */
  if (yyfree_stacks)
    {
      free (yyss);
      free (yyvs);
#ifdef YYLSP_NEEDED
      free (yyls);
#endif
    }
  return 1;
}
#line 361 "jtemp.y"


static void gen_port_inc( FILE *out, io_port *clock, module *mod ) {
  io_port *pt ;
  OUT0( "//////////////////////////////////////////////////////\n" ) ;
  OUT1( "// portset file for module %s \n", mod->name ) ;
  OUT0( "//   created by jedatemp\n" ) ;
  OUT0( "//////////////////////////////////////////////////////\n" ) ;
  OUT0( "#ifndef DRIVE_SPEC\n" ) ;
  OUT0( "#define DRIVE_SPEC drive posedge skew 1\n" ) ;
  OUT0( "#endif\n" ) ;
  OUT0( "#ifndef SAMPLE_SPEC\n" ) ;
  OUT0( "#define SAMPLE_SPEC sample posedge \n" ) ;
  OUT0( "#endif\n" ) ;  
  OUT1( "portset %s ", mod->name ) ;
  OUT0( "{\n" ) ;
  pt = mod->port ;
  while( pt ) {
    switch( pt->dir ) {  /* 0:input 1:output 2: inout -> to be reversed */
      case 0: OUT0( "  output  " ) ; break ;
      case 1: OUT0( "  input " ) ; break ;
      case 2: OUT0( "  inout  " ) ; break ;
    }
    if( pt->ub >= 0 && pt->lb >= 0 ) {
      OUT2( "[%d:%d] ", pt->ub, pt->lb ) ;
    }
    OUT1( "%s ", pt->name ) ;
    switch( pt->dir ) {  /* 0:input 1:output 2: inout */
      case 0: OUT0( "DRIVE_SPEC " ) ; break ;
      case 1: OUT0( "SAMPLE_SPEC " ) ; break ;
      case 2: OUT0( "DRIVE_SPEC SAMPLE_SPEC " ) ; break ;
    }
    if( clock && strcmp( clock->name, pt->name) ) 
      OUT2( " @ %s.%s ", mod->name, clock->name ) ;
    OUT0( " ;\n" ) ;
    pt = pt->next ;
  }
  OUT0( "}\n" ) ;
}

static void gen_jeda_temp( FILE *out, module *mod ) {
  io_port *pt ;
  OUT0( "//////////////////////////////////////////////////////\n" ) ;
  OUT1( "// Jeda template for module %s \n", mod->name ) ;
  OUT0( "//   created by jedatemp\n" ) ;
  OUT0( "//////////////////////////////////////////////////////\n" ) ;
  OUT1( "#include \"%s_port.jh\"\n", mod->name ) ;
  OUT0( "\n" ) ;
  OUT0( "main() {\n" ) ;
  OUT1( "  // begining of the program for module %s\n", mod->name ) ;
  OUT0( "}\n" ) ;
  OUT0( "\n" ) ;
}


static void gen_verilog( FILE *out, io_port *clock, module *mod, char *inc_file ) {
  io_port *pt ;
  int first ;
  OUT0( "//////////////////////////////////////////////////////\n" ) ;
  OUT1( "// verilog testbench for module %s \n", mod->name ) ;
  OUT0( "//   created by jedatemp\n" ) ;
  OUT0( "//////////////////////////////////////////////////////\n" ) ;
  OUT1( "module %s_test ; \n", mod->name ) ;
  OUT0( "  parameter simulation_cycle = 100 ;\n" ) ;
  OUT0( "  wire CLOCK ;\n" ) ;
  OUT0( "  reg CLOCK_reg ;\n" ) ;
  OUT0( "  assign CLOCK = CLOCK_reg ;\n" ) ;
  pt = mod->port ;
  while( pt ) {
    OUT0( "  wire  " ) ;
    if( pt->ub >= 0 && pt->lb >= 0 ) {
      OUT2( "[%d:%d] ", pt->ub, pt->lb ) ;
    }
    
    OUT2( " %s_%s ;\n", mod->name, pt->name ) ;
    pt = pt->next ;
  }
  if( clock )
    OUT2( "  assign %s_%s = CLOCK ;\n", mod->name, clock->name ) ;
    
  OUT0( "  // Jeda module \n" ) ;
  OUT0( "  jeda_module jeda (\n" ) ;
  OUT0( "    .CLOCK(CLOCK)" ) ;
  pt = mod->port ;
  while( pt ) {
    OUT0( ",\n" ) ;
    OUT4( "    .%s_%s(%s_%s)", mod->name, pt->name, mod->name, pt->name ) ;
    pt = pt->next ;
  }
  OUT0( "\n  ) ; // end of jeda_module\n" ) ;
  
  OUT1( "  // module %s under test\n", mod->name ) ;
  OUT1( "  %s dut (\n", mod->name ) ;
  pt = mod->port ;
  first = 1 ;
  while( pt ) {
    if( first ) first = 0 ;
    else OUT0( ",\n" ) ;
    OUT3( "    .%s(%s_%s)", pt->name, mod->name, pt->name ) ;
    pt = pt->next ;
  }
  OUT0( "\n  ) ;\n\n" ) ;
  
  OUT0( "  // CLOCK toggling\n" ) ;
  OUT0( "  initial begin\n" ) ;
  OUT0( "    CLOCK_reg = 0 ;\n" ) ;
  OUT0( "    forever begin\n" ) ;
  OUT0( "      #(simulation_cycle/2) CLOCK_reg = ~CLOCK_reg ;\n" ) ;
  OUT0( "    end\n" ) ;
  OUT0( "  end\n\n" ) ;
  
  if( inc_file ) { 
    OUT1( "`include \"%s\"\n\n", inc_file ) ;
  }
  
  OUT0( "endmodule\n" ) ;
    
}

void gen_output( char *name, char *cname, char *inc_fname ) {
  module *mod ;
  io_port *clock ;
  char *port_fname, *veri_fname, *jeda_fname ;
  FILE *pout, *vout, *jout ;
  
  mod = top_module ;
  clock = NULL ;
  
  if( name  ) {
    while( mod ) {
      if( !strcmp(mod->name, name) ) break ;
      mod = mod->next ;
    }
    if( !mod ) {
      fprintf( stderr, "Can't find module name \"%s\"\n", name ) ;
      exit(1) ;
    }
  }
  else name = mod->name ;
  
  if( cname ) {
    clock = mod->port ;
    while( clock ) {
      if( !strcmp(clock->name, cname) ) break ;
      clock = clock->next ;
    }
    if( !clock ) {
      fprintf( stderr, "Can't find clock name \"%s\"\n", cname ) ;
      exit(1) ;
    }
  }
  
  port_fname = (char *)calloc( strlen(name)+50, sizeof(char) ) ;
  strcpy( port_fname, name ) ;
  strcat( port_fname, "_port.jh" ) ;
  pout = fopen( port_fname, "w" ) ;
  if( pout == NULL ) {
    fprintf( stderr, "Can't open port header file \"%s\"\n", port_fname ) ;
    exit(1) ;
  }  
  veri_fname = (char *)calloc( strlen(name)+50, sizeof(char) ) ;
  strcpy( veri_fname, name ) ;
  strcat( veri_fname, "_test.v" ) ;
  vout = fopen( veri_fname, "w" ) ;
  if( vout == NULL ) {
    fprintf( stderr, "Can't open verilog file \"%s\"\n", veri_fname ) ;
    exit(1) ;
  }  
  jeda_fname = (char *)calloc( strlen(name)+50, sizeof(char) ) ;
  strcpy( jeda_fname, name ) ;
  strcat( jeda_fname, "_test_template.j" ) ;
  jout = fopen( jeda_fname, "w" ) ;
  if( jout == NULL ) {
    fprintf( stderr, "Can't open jeda template file \"%s\"\n", jeda_fname ) ;
    exit(1) ;
  }  
  
  gen_port_inc( pout, clock, mod ) ;
  close( pout ) ;
  printf( "  Port file \"%s\" created\n", port_fname ) ;
  gen_jeda_temp( jout, mod ) ;
  close( jout ) ;
  printf( "  Jeda template file \"%s\" created\n", jeda_fname ) ;
  gen_verilog( vout, clock, mod, inc_fname ) ;
  close( vout ) ;
  printf( "  Verilog testbench file \"%s\" created\n", veri_fname ) ;
}
