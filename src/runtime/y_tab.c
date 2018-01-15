
/*  A Bison parser, made from jdb.y
    by GNU Bison version 1.28  */

#define YYBISON 1  /* Identify Bison output.  */

#define yyparse JDB_yyparse
#define yylex JDB_yylex
#define yyerror JDB_yyerror
#define yylval JDB_yylval
#define yychar JDB_yychar
#define yydebug JDB_yydebug
#define yynerrs JDB_yynerrs
#define	Identifier	257
#define	Real_num	258
#define	Real_or_Depth	259
#define	Decimal_num	260
#define	V_decimal_num	261
#define	C_decimal_num	262
#define	V_hex_num	263
#define	C_hex_num	264
#define	V_octal_num	265
#define	C_octal_num	266
#define	V_bin_num	267
#define	C_bin_num	268
#define	String_literal	269
#define	JD_WORD_NULL	270
#define	JD_WORD_SUPER	271
#define	JD_WORD_THIS	272
#define	JD_ANDTILDA	273
#define	JD_TILDAAND	274
#define	JD_ORTILDA	275
#define	JD_TILDAOR	276
#define	JD_EORTILDA	277
#define	JD_TILDAEOR	278
#define	JD_LAND	279
#define	JD_LOR	280
#define	JD_RSHIFT	281
#define	JD_URSHIFT	282
#define	JD_LSHIFT	283
#define	JD_EQEQ	284
#define	JD_LE	285
#define	JD_GE	286
#define	JD_NE	287
#define	JD_EQEQEQ	288
#define	JD_NEQEQ	289
#define	JD_Q_EQ	290
#define	JD_Q_NEQ	291
#define	JD_QEQEQ	292
#define	JD_EQEQQ	293
#define	JD_C_EQEQ	294
#define	JD_D_PLUS	295
#define	JD_D_MINUS	296
#define	JD_D_AT	297
#define	JD_C_EQ	298
#define	JD_PLUS_EQ	299
#define	JD_MINUS_EQ	300
#define	JD_STAR_EQ	301
#define	JD_SLSH_EQ	302
#define	JD_MOD_EQ	303
#define	JD_LSHIFT_EQ	304
#define	JD_RSHIFT_EQ	305
#define	JD_URSHIFT_EQ	306
#define	JD_AND_EQ	307
#define	JD_OR_EQ	308
#define	JD_EOR_EQ	309
#define	JD_NAND_EQ	310
#define	JD_ANDN_EQ	311
#define	JD_NOR_EQ	312
#define	JD_ORN_EQ	313
#define	JD_NEOR_EQ	314
#define	JD_EORN_EQ	315



/* 
   
   Copyright (C) 2001 Juniper Networks Inc.

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2, or (at your option) any
   later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307,
   USA.  
   
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "runtime.h"

#include "veriuser.h"
#include "acc_user.h"

#include "pli.h"

#include "debugger.h"
#include "debugger_error_msg.h"
#include "exec_macro.h"

int JD_debugger_assign_command = 0 ;

extern int JD_dbg_eval_error ;



typedef union {
  char *string_value ;
  JD_dbg_expression_node *expression ;
  int keyword ;
} YYSTYPE;
#include <stdio.h>

#ifndef __cplusplus
#ifndef __STDC__
#define const
#endif
#endif



#define	YYFINAL		157
#define	YYFLAG		-32768
#define	YYNTBASE	89

#define YYTRANSLATE(x) ((unsigned)(x) <= 315 ? yytranslate[x] : 103)

static const char yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,    64,     2,    79,    80,    65,    67,    87,    83,
    84,    74,    69,    88,    70,    76,    75,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,    63,     2,    71,
    73,    72,    62,    78,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
    85,     2,    86,    66,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,    81,    68,    82,    77,     2,     2,     2,     2,
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
     7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
    17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
    27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
    37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
    47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
    57,    58,    59,    60,    61
};

#if YYDEBUG != 0
static const short yyprhs[] = {     0,
     0,     2,     4,     6,    10,    12,    14,    16,    18,    22,
    26,    30,    34,    38,    42,    46,    50,    54,    58,    62,
    66,    70,    74,    78,    82,    86,    90,    94,    98,   102,
   106,   110,   114,   118,   122,   126,   130,   134,   138,   142,
   145,   148,   151,   154,   157,   160,   163,   166,   169,   172,
   175,   178,   184,   190,   194,   196,   203,   205,   211,   215,
   219,   221,   223,   224,   226,   227,   229,   231,   234,   236,
   242,   244,   247,   251,   253,   255,   257,   259,   261,   263,
   265,   267,   269,   271,   273,   277,   279
};

static const short yyrhs[] = {    90,
     0,    91,     0,     1,     0,    92,    73,    91,     0,   100,
     0,    15,     0,    92,     0,    16,     0,    91,    70,    91,
     0,    91,    69,    91,     0,    91,    74,    91,     0,    91,
    75,    91,     0,    91,    65,    91,     0,    91,    67,    91,
     0,    91,    68,    91,     0,    91,    66,    91,     0,    91,
    19,    91,     0,    91,    20,    91,     0,    91,    21,    91,
     0,    91,    22,    91,     0,    91,    23,    91,     0,    91,
    24,    91,     0,    91,    25,    91,     0,    91,    26,    91,
     0,    91,    27,    91,     0,    91,    28,    91,     0,    91,
    29,    91,     0,    91,    71,    91,     0,    91,    72,    91,
     0,    91,    30,    91,     0,    91,    31,    91,     0,    91,
    32,    91,     0,    91,    33,    91,     0,    91,    34,    91,
     0,    91,    35,    91,     0,    91,    36,    91,     0,    91,
    37,    91,     0,    91,    39,    91,     0,    91,    38,    91,
     0,    70,    91,     0,    77,    91,     0,    64,    91,     0,
    67,    91,     0,    68,    91,     0,    66,    91,     0,    20,
    91,     0,    19,    91,     0,    22,    91,     0,    21,    91,
     0,    23,    91,     0,    24,    91,     0,    91,    40,    91,
    63,    91,     0,    91,    62,    91,    63,    91,     0,    83,
    91,    84,     0,   101,     0,    81,   100,    81,    91,    82,
    82,     0,    93,     0,    92,    76,     3,    94,    95,     0,
    92,    76,    17,     0,     3,    94,    95,     0,    17,     0,
    18,     0,     0,     5,     0,     0,    96,     0,    98,     0,
    98,    97,     0,    97,     0,    85,    91,    63,    91,    86,
     0,    99,     0,    98,    99,     0,    85,    91,    86,     0,
     4,     0,     5,     0,     6,     0,     7,     0,     8,     0,
     9,     0,    10,     0,    11,     0,    12,     0,    13,     0,
    14,     0,    81,   102,    82,     0,    91,     0,   102,    88,
    91,     0
};

#endif

#if YYDEBUG != 0
static const short yyrline[] = { 0,
   183,   189,   202,   211,   234,   240,   249,   255,   264,   277,
   290,   303,   316,   329,   342,   355,   368,   381,   394,   407,
   420,   433,   446,   459,   472,   485,   498,   511,   524,   537,
   550,   563,   576,   589,   602,   615,   628,   641,   654,   667,
   678,   689,   700,   711,   722,   733,   744,   755,   766,   777,
   788,   799,   814,   829,   835,   841,   873,   879,   893,   903,
   948,   955,   965,   971,   983,   989,   997,  1003,  1012,  1020,
  1037,  1043,  1062,  1082,  1091,  1100,  1109,  1117,  1125,  1133,
  1141,  1149,  1157,  1165,  1180,  1260,  1266
};
#endif


#if YYDEBUG != 0 || defined (YYERROR_VERBOSE)

static const char * const yytname[] = {   "$","error","$undefined.","Identifier",
"Real_num","Real_or_Depth","Decimal_num","V_decimal_num","C_decimal_num","V_hex_num",
"C_hex_num","V_octal_num","C_octal_num","V_bin_num","C_bin_num","String_literal",
"JD_WORD_NULL","JD_WORD_SUPER","JD_WORD_THIS","JD_ANDTILDA","JD_TILDAAND","JD_ORTILDA",
"JD_TILDAOR","JD_EORTILDA","JD_TILDAEOR","JD_LAND","JD_LOR","JD_RSHIFT","JD_URSHIFT",
"JD_LSHIFT","JD_EQEQ","JD_LE","JD_GE","JD_NE","JD_EQEQEQ","JD_NEQEQ","JD_Q_EQ",
"JD_Q_NEQ","JD_QEQEQ","JD_EQEQQ","JD_C_EQEQ","JD_D_PLUS","JD_D_MINUS","JD_D_AT",
"JD_C_EQ","JD_PLUS_EQ","JD_MINUS_EQ","JD_STAR_EQ","JD_SLSH_EQ","JD_MOD_EQ","JD_LSHIFT_EQ",
"JD_RSHIFT_EQ","JD_URSHIFT_EQ","JD_AND_EQ","JD_OR_EQ","JD_EOR_EQ","JD_NAND_EQ",
"JD_ANDN_EQ","JD_NOR_EQ","JD_ORN_EQ","JD_NEOR_EQ","JD_EORN_EQ","'?'","':'","'!'",
"'%'","'^'","'&'","'|'","'+'","'-'","'<'","'>'","'='","'*'","'/'","'.'","'~'",
"'@'","'#'","'$'","'{'","'}'","'('","')'","'['","']'","'\\''","','","debug_expression",
"assign_expression","expression","varport_expression","a_ver_expression","opt_depth",
"opt_access_range","access_range","bit_access_range","array_access_ranges","array_access_range",
"number","concat_expression","expression_list", NULL
};
#endif

static const short yyr1[] = {     0,
    89,    89,    89,    90,    91,    91,    91,    91,    91,    91,
    91,    91,    91,    91,    91,    91,    91,    91,    91,    91,
    91,    91,    91,    91,    91,    91,    91,    91,    91,    91,
    91,    91,    91,    91,    91,    91,    91,    91,    91,    91,
    91,    91,    91,    91,    91,    91,    91,    91,    91,    91,
    91,    91,    91,    91,    91,    91,    92,    92,    92,    93,
    93,    93,    94,    94,    95,    95,    96,    96,    96,    97,
    98,    98,    99,   100,   100,   100,   100,   100,   100,   100,
   100,   100,   100,   100,   101,   102,   102
};

static const short yyr2[] = {     0,
     1,     1,     1,     3,     1,     1,     1,     1,     3,     3,
     3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
     3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
     3,     3,     3,     3,     3,     3,     3,     3,     3,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     5,     5,     3,     1,     6,     1,     5,     3,     3,
     1,     1,     0,     1,     0,     1,     1,     2,     1,     5,
     1,     2,     3,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     3,     1,     3
};

static const short yydefact[] = {     0,
     3,    63,    74,    75,    76,    77,    78,    79,    80,    81,
    82,    83,    84,     6,     8,    61,    62,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     1,     2,     7,    57,     5,    55,    64,    65,    47,
     7,    46,    49,    48,    50,    51,    42,    45,    43,    44,
    40,    41,    86,     5,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,    60,    66,    69,    67,    71,     0,    85,     0,
    54,    17,    18,    19,    20,    21,    22,    23,    24,    25,
    26,    27,    30,    31,    32,    33,    34,    35,    36,    37,
    39,    38,     0,     0,    13,    16,    14,    15,    10,     9,
    28,    29,    11,    12,     4,    63,    59,     0,    68,    72,
     0,    87,     0,     0,    65,     0,    73,     0,    52,    53,
    58,     0,    56,    70,     0,     0,     0
};

static const short yydefgoto[] = {   155,
    32,    33,    41,    35,    39,    93,    94,    95,    96,    97,
    36,    37,    55
};

static const short yypact[] = {    80,
-32768,    12,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,   161,   161,   161,
   161,   161,   161,   161,   161,   161,   161,   161,   161,   161,
   161,-32768,   563,   -57,-32768,-32768,-32768,-32768,   -65,-32768,
   -58,-32768,-32768,-32768,-32768,-32768,     3,   721,   778,   699,
   308,     3,   563,   -52,   -67,   319,   161,   161,   161,   161,
   161,   161,   161,   161,   161,   161,   161,   161,   161,   161,
   161,   161,   161,   161,   161,   161,   161,   161,   161,   161,
   161,   161,   161,   161,   161,   161,   161,   161,   161,   161,
    11,   161,-32768,-32768,-32768,   -65,-32768,   161,-32768,   161,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,   642,   620,   240,
   240,   240,   800,   817,   817,   800,   800,   800,   800,   800,
   800,   800,   449,   506,     3,   721,   778,   699,   308,   308,
   817,   817,     3,     3,   563,    12,-32768,   183,-32768,-32768,
   385,   563,   161,   161,   -65,   161,-32768,   -51,   563,   563,
-32768,   251,-32768,-32768,    30,    32,-32768
};

static const short yypgoto[] = {-32768,
-32768,   -18,    33,-32768,  -102,  -110,-32768,   -60,-32768,   -59,
     8,-32768,-32768
};


#define	YYLAST		892


static const short yytable[] = {    40,
    42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
    52,    53,    56,   136,    99,    90,    38,    91,    91,    92,
   100,    57,    58,    59,    60,    61,    62,   137,    98,   156,
   153,   157,    34,   145,   151,   139,   140,    54,   102,   103,
   104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
   114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
   124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
   134,   135,     0,   138,     0,     0,     0,     0,     0,   141,
     1,   142,     2,     3,     4,     5,     6,     7,     8,     9,
    10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
    20,    21,    22,    23,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,   149,   150,     0,   152,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,    24,     0,    25,    26,    27,     0,    28,
     0,     0,     0,     0,     0,     0,    29,     0,     0,     0,
    30,     0,    31,     2,     3,     4,     5,     6,     7,     8,
     9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
    19,    20,    21,    22,    23,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,    57,    58,    59,    60,    61,    62,    63,    64,    65,
    66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
    76,    77,    78,     0,    24,     0,    25,    26,    27,     0,
    28,     0,     0,     0,     0,     0,     0,    29,     0,     0,
     0,    30,     0,    31,    79,   146,     0,    80,    81,    82,
    83,    84,    85,    86,    87,     0,    88,    89,    57,    58,
    59,    60,    61,    62,     0,     0,     0,     0,   147,    57,
    58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
    68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
    78,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,    80,     0,     0,     0,    84,    85,
     0,     0,    79,    88,    89,    80,    81,    82,    83,    84,
    85,    86,    87,     0,    88,    89,    57,    58,    59,    60,
    61,    62,     0,     0,     0,     0,   154,    57,    58,    59,
    60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
    70,    71,    72,    73,    74,    75,    76,    77,    78,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,    80,     0,     0,     0,     0,     0,     0,     0,
    79,    88,    89,    80,    81,    82,    83,    84,    85,    86,
    87,     0,    88,    89,     0,     0,     0,     0,     0,     0,
     0,     0,   101,    57,    58,    59,    60,    61,    62,    63,
    64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
    74,    75,    76,    77,    78,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,    79,     0,     0,    80,
    81,    82,    83,    84,    85,    86,    87,     0,    88,    89,
     0,     0,     0,     0,     0,     0,   148,    57,    58,    59,
    60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
    70,    71,    72,    73,    74,    75,    76,    77,    78,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    79,   143,     0,    80,    81,    82,    83,    84,    85,    86,
    87,     0,    88,    89,    57,    58,    59,    60,    61,    62,
    63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
    73,    74,    75,    76,    77,    78,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,    79,   144,     0,
    80,    81,    82,    83,    84,    85,    86,    87,     0,    88,
    89,    57,    58,    59,    60,    61,    62,    63,    64,    65,
    66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
    76,    77,    78,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,    79,     0,     0,    80,    81,    82,
    83,    84,    85,    86,    87,     0,    88,    89,    57,    58,
    59,    60,    61,    62,    63,     0,    65,    66,    67,    68,
    69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
    57,    58,    59,    60,    61,    62,     0,     0,    65,    66,
    67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
    77,    78,     0,     0,    80,    81,    82,    83,    84,    85,
    86,    87,     0,    88,    89,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,    80,    81,    82,    83,
    84,    85,    86,    87,     0,    88,    89,    57,    58,    59,
    60,    61,    62,     0,     0,    65,    66,    67,    68,    69,
    70,    71,    72,    73,    74,    75,    76,    77,    78,    57,
    58,    59,    60,    61,    62,     0,     0,    65,    66,    67,
    68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
    78,     0,     0,    80,    81,    82,     0,    84,    85,    86,
    87,     0,    88,    89,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,    80,     0,    82,     0,    84,
    85,    86,    87,     0,    88,    89,    57,    58,    59,    60,
    61,    62,     0,     0,    65,    66,    67,    68,    69,    70,
    71,    72,    73,    74,    75,    76,    77,    78,    57,    58,
    59,    60,    61,    62,     0,     0,    65,    66,    67,     0,
    69,    70,     0,     0,     0,    57,    58,    59,    60,    61,
    62,     0,    80,    65,    66,    67,    84,    85,    86,    87,
     0,    88,    89,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,    80,     0,     0,     0,    84,    85,
    86,    87,     0,    88,    89,     0,     0,     0,     0,     0,
     0,    80,     0,     0,     0,    84,    85,     0,     0,     0,
    88,    89
};

static const short yycheck[] = {    18,
    19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
    29,    30,    31,     3,    82,    73,     5,    76,    76,    85,
    88,    19,    20,    21,    22,    23,    24,    17,    81,     0,
    82,     0,     0,   136,   145,    96,    96,    30,    57,    58,
    59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
    69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
    79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
    89,    90,    -1,    92,    -1,    -1,    -1,    -1,    -1,    98,
     1,   100,     3,     4,     5,     6,     7,     8,     9,    10,
    11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
    21,    22,    23,    24,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,   143,   144,    -1,   146,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    64,    -1,    66,    67,    68,    -1,    70,
    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,
    81,    -1,    83,     3,     4,     5,     6,     7,     8,     9,
    10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
    20,    21,    22,    23,    24,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    19,    20,    21,    22,    23,    24,    25,    26,    27,
    28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
    38,    39,    40,    -1,    64,    -1,    66,    67,    68,    -1,
    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    -1,
    -1,    81,    -1,    83,    62,    63,    -1,    65,    66,    67,
    68,    69,    70,    71,    72,    -1,    74,    75,    19,    20,
    21,    22,    23,    24,    -1,    -1,    -1,    -1,    86,    19,
    20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
    30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
    40,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    65,    -1,    -1,    -1,    69,    70,
    -1,    -1,    62,    74,    75,    65,    66,    67,    68,    69,
    70,    71,    72,    -1,    74,    75,    19,    20,    21,    22,
    23,    24,    -1,    -1,    -1,    -1,    86,    19,    20,    21,
    22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
    32,    33,    34,    35,    36,    37,    38,    39,    40,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    62,    74,    75,    65,    66,    67,    68,    69,    70,    71,
    72,    -1,    74,    75,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    84,    19,    20,    21,    22,    23,    24,    25,
    26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
    36,    37,    38,    39,    40,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    62,    -1,    -1,    65,
    66,    67,    68,    69,    70,    71,    72,    -1,    74,    75,
    -1,    -1,    -1,    -1,    -1,    -1,    82,    19,    20,    21,
    22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
    32,    33,    34,    35,    36,    37,    38,    39,    40,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    62,    63,    -1,    65,    66,    67,    68,    69,    70,    71,
    72,    -1,    74,    75,    19,    20,    21,    22,    23,    24,
    25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
    35,    36,    37,    38,    39,    40,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,    -1,
    65,    66,    67,    68,    69,    70,    71,    72,    -1,    74,
    75,    19,    20,    21,    22,    23,    24,    25,    26,    27,
    28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
    38,    39,    40,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    62,    -1,    -1,    65,    66,    67,
    68,    69,    70,    71,    72,    -1,    74,    75,    19,    20,
    21,    22,    23,    24,    25,    -1,    27,    28,    29,    30,
    31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
    19,    20,    21,    22,    23,    24,    -1,    -1,    27,    28,
    29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
    39,    40,    -1,    -1,    65,    66,    67,    68,    69,    70,
    71,    72,    -1,    74,    75,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    65,    66,    67,    68,
    69,    70,    71,    72,    -1,    74,    75,    19,    20,    21,
    22,    23,    24,    -1,    -1,    27,    28,    29,    30,    31,
    32,    33,    34,    35,    36,    37,    38,    39,    40,    19,
    20,    21,    22,    23,    24,    -1,    -1,    27,    28,    29,
    30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
    40,    -1,    -1,    65,    66,    67,    -1,    69,    70,    71,
    72,    -1,    74,    75,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    65,    -1,    67,    -1,    69,
    70,    71,    72,    -1,    74,    75,    19,    20,    21,    22,
    23,    24,    -1,    -1,    27,    28,    29,    30,    31,    32,
    33,    34,    35,    36,    37,    38,    39,    40,    19,    20,
    21,    22,    23,    24,    -1,    -1,    27,    28,    29,    -1,
    31,    32,    -1,    -1,    -1,    19,    20,    21,    22,    23,
    24,    -1,    65,    27,    28,    29,    69,    70,    71,    72,
    -1,    74,    75,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    65,    -1,    -1,    -1,    69,    70,
    71,    72,    -1,    74,    75,    -1,    -1,    -1,    -1,    -1,
    -1,    65,    -1,    -1,    -1,    69,    70,    -1,    -1,    -1,
    74,    75
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */

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

{
    /* top_assign_expression */
    yyval.expression = NULL ;
  ;
    break;}
case 2:

{
    /* top_expression */
    SKIP_ON_ERROR ;
    if( JD_debugger_assign_command ) {
      GBG_ERROR_ILLEGAL_EXPRESSION_FOR_ASSIGN_COMMAND ;
    }
    else {
      JD_dbg_print_expression( yyvsp[0].expression, 0 ) ; /* (exp, indent) */
    }
    yyval.expression = NULL ;
  ;
    break;}
case 3:

{
    /* error */
    JD_debugger_clear_word() ;
    yyval.expression = NULL ;
  ;
    break;}
case 4:

{
    /* assign_expression */
    SKIP_ON_ERROR ;
    if( JD_debugger_assign_command ) {
      if( yyvsp[-2].expression->type != JD_D_dt ) {
        GBG_ERROR_ILLEGAL_LEFT_EXPRESSION_FOR_ASSIGN_COMMAND ;
      }
      else {
        JD_dbg_assign_expression( yyvsp[-2].expression, yyvsp[0].expression ) ;
      }
    }
    else {
      GBG_ERROR_ILLEGAL_ASSIGN_EXPRESSION_FOR_PRINT_COMMAND ;
    }
    yyval.expression = NULL ;
  ;
    break;}
case 5:

{
    /* DDeqD1 */
    yyval.expression = yyvsp[0].expression ; 
  ;
    break;}
case 6:

{
    /* string_expression */
    SKIP_ON_ERROR ;
    yyval.expression = (void *)JD_dbg_alloc_exp() ;
    yyval.expression->type = JD_D_string ;
    yyval.expression->string = yyvsp[0].string_value ;
  ;
    break;}
case 7:

{
    /* DDeqD1 */
    yyval.expression = yyvsp[0].expression ; 
  ;
    break;}
case 8:

{
    /* null_expression */
    SKIP_ON_ERROR ;
    yyval.expression = (void *)JD_dbg_alloc_exp() ;
    yyval.expression->type = JD_D_null ;
    yyval.expression->dt = NULL ;
  ;
    break;}
case 9:

{
    /* exp_minus_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp(yyvsp[-2].expression) ;
    JD_dbg_eval_exp(yyvsp[0].expression) ;
    SKIP_ON_ERROR ;
    yyval.expression = (void *)JD_dbg_alloc_exp() ;
    JD_eval_bin_ops( JD_dbg_b_minus_op, yyvsp[-2].expression, yyvsp[0].expression, yyval.expression ) ;
    JD_dbg_free_exp(yyvsp[-2].expression) ;
    JD_dbg_free_exp(yyvsp[0].expression) ;
  ;
    break;}
case 10:

{
    /* exp_plus_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp(yyvsp[-2].expression) ;
    JD_dbg_eval_exp(yyvsp[0].expression) ;
    SKIP_ON_ERROR ;
    yyval.expression = (void *)JD_dbg_alloc_exp() ;
    JD_eval_bin_ops( JD_dbg_b_plus_op, yyvsp[-2].expression, yyvsp[0].expression, yyval.expression ) ;
    JD_dbg_free_exp(yyvsp[-2].expression) ;
    JD_dbg_free_exp(yyvsp[0].expression) ;
  ;
    break;}
case 11:

{
    /* exp_times_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp(yyvsp[-2].expression) ;
    JD_dbg_eval_exp(yyvsp[0].expression) ;
    SKIP_ON_ERROR ;
    yyval.expression = (void *)JD_dbg_alloc_exp() ;
    JD_eval_bin_ops( JD_dbg_b_times_op, yyvsp[-2].expression, yyvsp[0].expression, yyval.expression ) ;
    JD_dbg_free_exp(yyvsp[-2].expression) ;
    JD_dbg_free_exp(yyvsp[0].expression) ;
  ;
    break;}
case 12:

{
    /* exp_div_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp(yyvsp[-2].expression) ;
    JD_dbg_eval_exp(yyvsp[0].expression) ;
    SKIP_ON_ERROR ;
    yyval.expression = (void *)JD_dbg_alloc_exp() ;
    JD_eval_bin_ops( JD_dbg_b_div_op, yyvsp[-2].expression, yyvsp[0].expression, yyval.expression ) ;
    JD_dbg_free_exp(yyvsp[-2].expression) ;
    JD_dbg_free_exp(yyvsp[0].expression) ;
  ;
    break;}
case 13:

{
    /* exp_mod_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp(yyvsp[-2].expression) ;
    JD_dbg_eval_exp(yyvsp[0].expression) ;
    SKIP_ON_ERROR ;
    yyval.expression = (void *)JD_dbg_alloc_exp() ;
    JD_eval_bin_ops( JD_dbg_b_mod_op, yyvsp[-2].expression, yyvsp[0].expression, yyval.expression ) ;
    JD_dbg_free_exp(yyvsp[-2].expression) ;
    JD_dbg_free_exp(yyvsp[0].expression) ;
  ;
    break;}
case 14:

{
    /* exp_and_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp(yyvsp[-2].expression) ;
    JD_dbg_eval_exp(yyvsp[0].expression) ;
    SKIP_ON_ERROR ;
    yyval.expression = (void *)JD_dbg_alloc_exp() ;
    JD_eval_bin_ops( JD_dbg_b_and_op, yyvsp[-2].expression, yyvsp[0].expression, yyval.expression ) ;
    JD_dbg_free_exp(yyvsp[-2].expression) ;
    JD_dbg_free_exp(yyvsp[0].expression) ;
  ;
    break;}
case 15:

{
    /* exp_or_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp(yyvsp[-2].expression) ;
    JD_dbg_eval_exp(yyvsp[0].expression) ;
    SKIP_ON_ERROR ;
    yyval.expression = (void *)JD_dbg_alloc_exp() ;
    JD_eval_bin_ops( JD_dbg_b_or_op, yyvsp[-2].expression, yyvsp[0].expression, yyval.expression ) ;
    JD_dbg_free_exp(yyvsp[-2].expression) ;
    JD_dbg_free_exp(yyvsp[0].expression) ;
  ;
    break;}
case 16:

{
    /* exp_eor_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp(yyvsp[-2].expression) ;
    JD_dbg_eval_exp(yyvsp[0].expression) ;
    SKIP_ON_ERROR ;
    yyval.expression = (void *)JD_dbg_alloc_exp() ;
    JD_eval_bin_ops( JD_dbg_b_eor_op, yyvsp[-2].expression, yyvsp[0].expression, yyval.expression ) ;
    JD_dbg_free_exp(yyvsp[-2].expression) ;
    JD_dbg_free_exp(yyvsp[0].expression) ;
  ;
    break;}
case 17:

{
    /* exp_nand_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp(yyvsp[-2].expression) ;
    JD_dbg_eval_exp(yyvsp[0].expression) ;
    SKIP_ON_ERROR ;
    yyval.expression = (void *)JD_dbg_alloc_exp() ;
    JD_eval_bin_ops( JD_dbg_b_nand_op, yyvsp[-2].expression, yyvsp[0].expression, yyval.expression ) ;
    JD_dbg_free_exp(yyvsp[-2].expression) ;
    JD_dbg_free_exp(yyvsp[0].expression) ;
  ;
    break;}
case 18:

{
    /* exp_nand_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp(yyvsp[-2].expression) ;
    JD_dbg_eval_exp(yyvsp[0].expression) ;
    SKIP_ON_ERROR ;
    yyval.expression = (void *)JD_dbg_alloc_exp() ;
    JD_eval_bin_ops( JD_dbg_b_nand_op, yyvsp[-2].expression, yyvsp[0].expression, yyval.expression ) ;
    JD_dbg_free_exp(yyvsp[-2].expression) ;
    JD_dbg_free_exp(yyvsp[0].expression) ;
  ;
    break;}
case 19:

{
    /* exp_nor_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp(yyvsp[-2].expression) ;
    JD_dbg_eval_exp(yyvsp[0].expression) ;
    SKIP_ON_ERROR ;
    yyval.expression = (void *)JD_dbg_alloc_exp() ;
    JD_eval_bin_ops( JD_dbg_b_nor_op, yyvsp[-2].expression, yyvsp[0].expression, yyval.expression ) ;
    JD_dbg_free_exp(yyvsp[-2].expression) ;
    JD_dbg_free_exp(yyvsp[0].expression) ;
  ;
    break;}
case 20:

{
    /* exp_nor_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp(yyvsp[-2].expression) ;
    JD_dbg_eval_exp(yyvsp[0].expression) ;
    SKIP_ON_ERROR ;
    yyval.expression = (void *)JD_dbg_alloc_exp() ;
    JD_eval_bin_ops( JD_dbg_b_nor_op, yyvsp[-2].expression, yyvsp[0].expression, yyval.expression ) ;
    JD_dbg_free_exp(yyvsp[-2].expression) ;
    JD_dbg_free_exp(yyvsp[0].expression) ;
  ;
    break;}
case 21:

{
    /* exp_neor_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp(yyvsp[-2].expression) ;
    JD_dbg_eval_exp(yyvsp[0].expression) ;
    SKIP_ON_ERROR ;
    yyval.expression = (void *)JD_dbg_alloc_exp() ;
    JD_eval_bin_ops( JD_dbg_b_neor_op, yyvsp[-2].expression, yyvsp[0].expression, yyval.expression ) ;
    JD_dbg_free_exp(yyvsp[-2].expression) ;
    JD_dbg_free_exp(yyvsp[0].expression) ;
  ;
    break;}
case 22:

{
    /* exp_neor_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp(yyvsp[-2].expression) ;
    JD_dbg_eval_exp(yyvsp[0].expression) ;
    SKIP_ON_ERROR ;
    yyval.expression = (void *)JD_dbg_alloc_exp() ;
    JD_eval_bin_ops( JD_dbg_b_neor_op, yyvsp[-2].expression, yyvsp[0].expression, yyval.expression ) ;
    JD_dbg_free_exp(yyvsp[-2].expression) ;
    JD_dbg_free_exp(yyvsp[0].expression) ;
  ;
    break;}
case 23:

{
    /* exp_land_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp(yyvsp[-2].expression) ;
    JD_dbg_eval_exp(yyvsp[0].expression) ;
    SKIP_ON_ERROR ;
    yyval.expression = (void *)JD_dbg_alloc_exp() ;
    JD_eval_bin_ops( JD_dbg_b_land_op, yyvsp[-2].expression, yyvsp[0].expression, yyval.expression ) ;
    JD_dbg_free_exp(yyvsp[-2].expression) ;
    JD_dbg_free_exp(yyvsp[0].expression) ;
  ;
    break;}
case 24:

{
    /* exp_lor_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp(yyvsp[-2].expression) ;
    JD_dbg_eval_exp(yyvsp[0].expression) ;
    SKIP_ON_ERROR ;
    yyval.expression = (void *)JD_dbg_alloc_exp() ;
    JD_eval_bin_ops( JD_dbg_b_lor_op, yyvsp[-2].expression, yyvsp[0].expression, yyval.expression ) ;
    JD_dbg_free_exp(yyvsp[-2].expression) ;
    JD_dbg_free_exp(yyvsp[0].expression) ;
  ;
    break;}
case 25:

{
    /* exp_rshift_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp(yyvsp[-2].expression) ;
    JD_dbg_eval_exp(yyvsp[0].expression) ;
    SKIP_ON_ERROR ;
    yyval.expression = (void *)JD_dbg_alloc_exp() ;
    JD_eval_bin_ops( JD_dbg_b_rshift_op, yyvsp[-2].expression, yyvsp[0].expression, yyval.expression ) ;
    JD_dbg_free_exp(yyvsp[-2].expression) ;
    JD_dbg_free_exp(yyvsp[0].expression) ;
  ;
    break;}
case 26:

{
    /* exp_urshift_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp(yyvsp[-2].expression) ;
    JD_dbg_eval_exp(yyvsp[0].expression) ;
    SKIP_ON_ERROR ;
    yyval.expression = (void *)JD_dbg_alloc_exp() ;
    JD_eval_bin_ops( JD_dbg_b_urshift_op, yyvsp[-2].expression, yyvsp[0].expression, yyval.expression ) ;
    JD_dbg_free_exp(yyvsp[-2].expression) ;
    JD_dbg_free_exp(yyvsp[0].expression) ;
  ;
    break;}
case 27:

{
    /* exp_lshift_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp(yyvsp[-2].expression) ;
    JD_dbg_eval_exp(yyvsp[0].expression) ;
    SKIP_ON_ERROR ;
    yyval.expression = (void *)JD_dbg_alloc_exp() ;
    JD_eval_bin_ops( JD_dbg_b_lshift_op, yyvsp[-2].expression, yyvsp[0].expression, yyval.expression ) ;
    JD_dbg_free_exp(yyvsp[-2].expression) ;
    JD_dbg_free_exp(yyvsp[0].expression) ;
  ;
    break;}
case 28:

{
    /* exp_lt_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp(yyvsp[-2].expression) ;
    JD_dbg_eval_exp(yyvsp[0].expression) ;
    SKIP_ON_ERROR ;
    yyval.expression = (void *)JD_dbg_alloc_exp() ;
    JD_eval_bin_ops( JD_dbg_b_lt_op, yyvsp[-2].expression, yyvsp[0].expression, yyval.expression ) ;
    JD_dbg_free_exp(yyvsp[-2].expression) ;
    JD_dbg_free_exp(yyvsp[0].expression) ;
  ;
    break;}
case 29:

{
    /* exp_gt_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp(yyvsp[-2].expression) ;
    JD_dbg_eval_exp(yyvsp[0].expression) ;
    SKIP_ON_ERROR ;
    yyval.expression = (void *)JD_dbg_alloc_exp() ;
    JD_eval_bin_ops( JD_dbg_b_gt_op, yyvsp[-2].expression, yyvsp[0].expression, yyval.expression ) ;
    JD_dbg_free_exp(yyvsp[-2].expression) ;
    JD_dbg_free_exp(yyvsp[0].expression) ;
  ;
    break;}
case 30:

{
    /* exp_eqeq_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp(yyvsp[-2].expression) ;
    JD_dbg_eval_exp(yyvsp[0].expression) ;
    SKIP_ON_ERROR ;
    yyval.expression = (void *)JD_dbg_alloc_exp() ;
    JD_eval_bin_ops( JD_dbg_b_eqeq_op, yyvsp[-2].expression, yyvsp[0].expression, yyval.expression ) ;
    JD_dbg_free_exp(yyvsp[-2].expression) ;
    JD_dbg_free_exp(yyvsp[0].expression) ;
  ;
    break;}
case 31:

{
    /* exp_le_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp(yyvsp[-2].expression) ;
    JD_dbg_eval_exp(yyvsp[0].expression) ;
    SKIP_ON_ERROR ;
    yyval.expression = (void *)JD_dbg_alloc_exp() ;
    JD_eval_bin_ops( JD_dbg_b_le_op, yyvsp[-2].expression, yyvsp[0].expression, yyval.expression ) ;
    JD_dbg_free_exp(yyvsp[-2].expression) ;
    JD_dbg_free_exp(yyvsp[0].expression) ;
  ;
    break;}
case 32:

{
    /* exp_ge_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp(yyvsp[-2].expression) ;
    JD_dbg_eval_exp(yyvsp[0].expression) ;
    SKIP_ON_ERROR ;
    yyval.expression = (void *)JD_dbg_alloc_exp() ;
    JD_eval_bin_ops( JD_dbg_b_ge_op, yyvsp[-2].expression, yyvsp[0].expression, yyval.expression ) ;
    JD_dbg_free_exp(yyvsp[-2].expression) ;
    JD_dbg_free_exp(yyvsp[0].expression) ;
  ;
    break;}
case 33:

{
    /* exp_ne_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp(yyvsp[-2].expression) ;
    JD_dbg_eval_exp(yyvsp[0].expression) ;
    SKIP_ON_ERROR ;
    yyval.expression = (void *)JD_dbg_alloc_exp() ;
    JD_eval_bin_ops( JD_dbg_b_ne_op, yyvsp[-2].expression, yyvsp[0].expression, yyval.expression ) ;
    JD_dbg_free_exp(yyvsp[-2].expression) ;
    JD_dbg_free_exp(yyvsp[0].expression) ;
  ;
    break;}
case 34:

{
    /* exp_eqeqeq_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp(yyvsp[-2].expression) ;
    JD_dbg_eval_exp(yyvsp[0].expression) ;
    SKIP_ON_ERROR ;
    yyval.expression = (void *)JD_dbg_alloc_exp() ;
    JD_eval_bin_ops( JD_dbg_b_eqeqeq_op, yyvsp[-2].expression, yyvsp[0].expression, yyval.expression ) ;
    JD_dbg_free_exp(yyvsp[-2].expression) ;
    JD_dbg_free_exp(yyvsp[0].expression) ;
  ;
    break;}
case 35:

{
    /* exp_neqeq_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp(yyvsp[-2].expression) ;
    JD_dbg_eval_exp(yyvsp[0].expression) ;
    SKIP_ON_ERROR ;
    yyval.expression = (void *)JD_dbg_alloc_exp() ;
    JD_eval_bin_ops( JD_dbg_b_neqeq_op, yyvsp[-2].expression, yyvsp[0].expression, yyval.expression ) ;
    JD_dbg_free_exp(yyvsp[-2].expression) ;
    JD_dbg_free_exp(yyvsp[0].expression) ;
  ;
    break;}
case 36:

{
    /* exp_eqQeq_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp(yyvsp[-2].expression) ;
    JD_dbg_eval_exp(yyvsp[0].expression) ;
    SKIP_ON_ERROR ;
    yyval.expression = (void *)JD_dbg_alloc_exp() ;
    JD_eval_bin_ops( JD_dbg_b_eqQeq_op, yyvsp[-2].expression, yyvsp[0].expression, yyval.expression ) ;
    JD_dbg_free_exp(yyvsp[-2].expression) ;
    JD_dbg_free_exp(yyvsp[0].expression) ;
  ;
    break;}
case 37:

{
    /* exp_Qne_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp(yyvsp[-2].expression) ;
    JD_dbg_eval_exp(yyvsp[0].expression) ;
    SKIP_ON_ERROR ;
    yyval.expression = (void *)JD_dbg_alloc_exp() ;
    JD_eval_bin_ops( JD_dbg_b_Qne_op, yyvsp[-2].expression, yyvsp[0].expression, yyval.expression ) ;
    JD_dbg_free_exp(yyvsp[-2].expression) ;
    JD_dbg_free_exp(yyvsp[0].expression) ;
  ;
    break;}
case 38:

{
    /* exp_eqeqQ_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp(yyvsp[-2].expression) ;
    JD_dbg_eval_exp(yyvsp[0].expression) ;
    SKIP_ON_ERROR ;
    yyval.expression = (void *)JD_dbg_alloc_exp() ;
    JD_eval_bin_ops( JD_dbg_b_eqeqQ_op, yyvsp[-2].expression, yyvsp[0].expression, yyval.expression ) ;
    JD_dbg_free_exp(yyvsp[-2].expression) ;
    JD_dbg_free_exp(yyvsp[0].expression) ;
  ;
    break;}
case 39:

{
    /* exp_Qeqeq_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp(yyvsp[-2].expression) ;
    JD_dbg_eval_exp(yyvsp[0].expression) ;
    SKIP_ON_ERROR ;
    yyval.expression = (void *)JD_dbg_alloc_exp() ;
    JD_eval_bin_ops( JD_dbg_b_Qeqeq_op, yyvsp[-2].expression, yyvsp[0].expression, yyval.expression ) ;
    JD_dbg_free_exp(yyvsp[-2].expression) ;
    JD_dbg_free_exp(yyvsp[0].expression) ;
  ;
    break;}
case 40:

{
    /* u_minus_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp(yyvsp[0].expression) ;
    SKIP_ON_ERROR ;
    yyval.expression = (void *)JD_dbg_alloc_exp() ;
    JD_eval_u_minus( yyvsp[0].expression, yyval.expression ) ;
    JD_dbg_free_exp(yyvsp[0].expression) ;
  ;
    break;}
case 41:

{
    /* u_tilde_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp(yyvsp[0].expression) ;
    SKIP_ON_ERROR ;
    yyval.expression = (void *)JD_dbg_alloc_exp() ;
    JD_eval_u_tilde( yyvsp[0].expression, yyval.expression ) ;
    JD_dbg_free_exp(yyvsp[0].expression) ;
  ;
    break;}
case 42:

{
    /* u_not_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp(yyvsp[0].expression) ;
    SKIP_ON_ERROR ;
    yyval.expression = (void *)JD_dbg_alloc_exp() ;
    JD_eval_u_not( yyvsp[0].expression, yyval.expression ) ;
    JD_dbg_free_exp(yyvsp[0].expression) ;
  ;
    break;}
case 43:

{
    /* u_and_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp(yyvsp[0].expression) ;
    SKIP_ON_ERROR ;
    yyval.expression = (void *)JD_dbg_alloc_exp() ;
    JD_eval_u_and( yyvsp[0].expression, yyval.expression ) ;
    JD_dbg_free_exp(yyvsp[0].expression) ;
  ;
    break;}
case 44:

{
    /* u_or_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp(yyvsp[0].expression) ;
    SKIP_ON_ERROR ;
    yyval.expression = (void *)JD_dbg_alloc_exp() ;
    JD_eval_u_or( yyvsp[0].expression, yyval.expression ) ;
    JD_dbg_free_exp(yyvsp[0].expression) ;
  ;
    break;}
case 45:

{
    /* u_eor_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp(yyvsp[0].expression) ;
    SKIP_ON_ERROR ;
    yyval.expression = (void *)JD_dbg_alloc_exp() ;
    JD_eval_u_eor( yyvsp[0].expression, yyval.expression ) ;
    JD_dbg_free_exp(yyvsp[0].expression) ;
  ;
    break;}
case 46:

{
    /* u_nand_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp(yyvsp[0].expression) ;
    SKIP_ON_ERROR ;
    yyval.expression = (void *)JD_dbg_alloc_exp() ;
    JD_eval_u_nand( yyvsp[0].expression, yyval.expression ) ;
    JD_dbg_free_exp(yyvsp[0].expression) ;
  ;
    break;}
case 47:

{
    /* u_nand_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp(yyvsp[0].expression) ;
    SKIP_ON_ERROR ;
    yyval.expression = (void *)JD_dbg_alloc_exp() ;
    JD_eval_u_nand( yyvsp[0].expression, yyval.expression ) ;
    JD_dbg_free_exp(yyvsp[0].expression) ;
  ;
    break;}
case 48:

{
    /* u_nor_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp(yyvsp[0].expression) ;
    SKIP_ON_ERROR ;
    yyval.expression = (void *)JD_dbg_alloc_exp() ;
    JD_eval_u_nor( yyvsp[0].expression, yyval.expression ) ;
    JD_dbg_free_exp(yyvsp[0].expression) ;
  ;
    break;}
case 49:

{
    /* u_nor_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp(yyvsp[0].expression) ;
    SKIP_ON_ERROR ;
    yyval.expression = (void *)JD_dbg_alloc_exp() ;
    JD_eval_u_nor( yyvsp[0].expression, yyval.expression ) ;
    JD_dbg_free_exp(yyvsp[0].expression) ;
  ;
    break;}
case 50:

{
    /* u_neor_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp(yyvsp[0].expression) ;
    SKIP_ON_ERROR ;
    yyval.expression = (void *)JD_dbg_alloc_exp() ;
    JD_eval_u_neor( yyvsp[0].expression, yyval.expression ) ;
    JD_dbg_free_exp(yyvsp[0].expression) ;
  ;
    break;}
case 51:

{
    /* u_neor_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp(yyvsp[0].expression) ;
    SKIP_ON_ERROR ;
    yyval.expression = (void *)JD_dbg_alloc_exp() ;
    JD_eval_u_neor( yyvsp[0].expression, yyval.expression ) ;
    JD_dbg_free_exp(yyvsp[0].expression) ;
  ;
    break;}
case 52:

{
    /* u_eqCeq_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp(yyvsp[-4].expression) ;
    JD_dbg_eval_exp(yyvsp[-2].expression) ;
    JD_dbg_eval_exp(yyvsp[0].expression) ;
    SKIP_ON_ERROR ;
    yyval.expression = (void *)JD_dbg_alloc_exp() ;
    JD_eval_eqCeq_exp( yyvsp[-4].expression, yyvsp[-2].expression, yyvsp[0].expression, yyval.expression ) ; 
    JD_dbg_free_exp(yyvsp[-4].expression) ;
    JD_dbg_free_exp(yyvsp[-2].expression) ;
    JD_dbg_free_exp(yyvsp[0].expression) ;
  ;
    break;}
case 53:

{
    /* conditional_exp */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp(yyvsp[-4].expression) ;
    JD_dbg_eval_exp(yyvsp[-2].expression) ;
    JD_dbg_eval_exp(yyvsp[0].expression) ;
    SKIP_ON_ERROR ;
    yyval.expression = (void *)JD_dbg_alloc_exp() ;
    JD_eval_cond_exp( yyvsp[-4].expression, yyvsp[-2].expression, yyvsp[0].expression, yyval.expression ) ; 
    JD_dbg_free_exp(yyvsp[-4].expression) ;
    JD_dbg_free_exp(yyvsp[-2].expression) ;
    JD_dbg_free_exp(yyvsp[0].expression) ;
  ;
    break;}
case 54:

{
    /* DDeqD2 */
    yyval.expression = yyvsp[-1].expression ; 
  ;
    break;}
case 55:

{
    /* DDeqD1 */
    yyval.expression = yyvsp[0].expression ; 
  ;
    break;}
case 56:

{
    /* duplicate_string */
    int len = 0 ;
    int i, n ;
    
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp(yyvsp[-2].expression) ;
    if( yyvsp[-2].expression->type != JD_D_string ) {
      DBG_ERROR_ILLEGAL_DATA_TYPE_FOR_STRING_DUPLICATION ;
      JD_dbg_free_exp(yyvsp[-2].expression) ;
      break ;
    }
    if( yyvsp[-2].expression && yyvsp[-2].expression->string ) {
      len = strlen(yyvsp[-2].expression->string) ;
      if( yyvsp[-4].expression->type == JD_D_int ) {
        n = yyvsp[-4].expression->int_value ;
        len *= n ;
      }
      else n = 0 ; 
    }
    yyval.expression = (void *)JD_dbg_alloc_exp() ;
    yyval.expression->type = JD_D_string ;
    yyval.expression->string = calloc(len+1,sizeof(char)) ;
    for( i = 0 ; i < n ; i++ ) {
      strcat(yyval.expression->string, yyvsp[-2].expression->string) ;
    }
    JD_dbg_free_exp(yyvsp[-2].expression) ;
  ;
    break;}
case 57:

{
    /* DDeqD1 */
    yyval.expression = yyvsp[0].expression ; 
  ;
    break;}
case 58:

{
    /* var_dot_var */
    SKIP_ON_ERROR ;
    /* working here */
    JD_dbg_eval_exp(yyvsp[-4].expression) ;
    yyval.expression = (void *)JD_debugger_get_member( yyvsp[-4].expression, yyvsp[-2].string_value ) ;
    SKIP_ON_ERROR ;
    yyval.expression->depth = yyvsp[-1].expression ;
    if( yyvsp[0].expression ) {
      JD_debugger_resolve_array_access( yyval.expression, yyvsp[0].expression ) ;
    }
  ;
    break;}
case 59:

{
    /* var_dot_super */
    SKIP_ON_ERROR ;
    yyval.expression =  (void *)JD_dbg_get_super_of( yyvsp[-2].expression ) ;
    
  ;
    break;}
case 60:

{
    /* var_expression */
    JD_dbg_expression_node *exp ;
    SKIP_ON_ERROR ;
    
    exp = (void *)JD_debugger_get_data( yyvsp[-2].string_value ) ;
    if( exp && yyvsp[0].expression ) {
      if( exp->type == JD_D_dt ) {
        exp =  (void *)JD_debugger_resolve_array_access( exp, yyvsp[0].expression ) ;
      }
      else {
        DBG_ERROR_ILLEGAL_NAME_TYPE_FOR_ARRAY_ACCESS ;
      }
    }
    yyval.expression = exp ;
    SKIP_ON_ERROR ;
    if( exp ) {
      if( yyvsp[-1].expression ) {
        if( exp->dt == NULL || exp->dt->type != JD_D_signal ||
            exp->dt->sub_type != JD_SD_single
        ) {
          DBG_ERROR_ILLEGAL_DEPTH_USAGE_ON_NON_SIGNAL_EXP ;
          break ;
        }
        exp->depth = yyvsp[-1].expression ;
        if( yyvsp[-1].expression->int_value > 0 ) {
          JD_pli_object *obj ;
          obj = (JD_pli_object *)exp->dt->data.signal.port ;
          if( obj == NULL ) {
            DBG_ERROR_EMPTY_SIGNAL_ACCESS ;
            break ;
          }
          if( obj->is.port.depth < yyvsp[-1].expression->int_value ) {
            DBG_ERROR_SIGNAL_DEPTH_TOO_DEEP ;
            break ;
          }
        }
      }
    }
    else {
      DBG_ERROR_NAME_NOT_FOUND( yyvsp[-2].string_value ) ;
    }
  ;
    break;}
case 61:

{
    /* super */
    SKIP_ON_ERROR ;
    yyval.expression =  (void *)JD_dbg_get_super_exp() ;
  ;
    break;}
case 62:

{
    /* this */
    SKIP_ON_ERROR ;
    yyval.expression =  (void *)JD_dbg_get_this_exp() ;
  ;
    break;}
case 63:

{
    /* DDeqNULL */
    yyval.expression = NULL ;
  ;
    break;}
case 64:

{
    /* dot_depth */
    SKIP_ON_ERROR ;
    yyval.expression =  (void *)JD_dbg_alloc_exp() ;
    yyval.expression->type = JD_D_int ;
    yyval.expression->int_value = atoi(&yyvsp[0].string_value[1]) ;
    free(yyvsp[0].string_value) ;
  ;
    break;}
case 65:

{
    /* DDeqNULL */
    yyval.expression = NULL ;
  ;
    break;}
case 66:

{
    /* DDeqD1 */
    yyval.expression = yyvsp[0].expression ; 
  ;
    break;}
case 67:

{
    /* DDeqD1 */
    yyval.expression = yyvsp[0].expression ; 
  ;
    break;}
case 68:

{
    /* array_access_range_bit_access_range */
    SKIP_ON_ERROR ;
    /* array_access_range_bit_access_range */
    yyval.expression = yyvsp[-1].expression ;
    if( yyval.expression ) yyval.expression->next_level = yyvsp[0].expression ;
  ;
    break;}
case 69:

{
    /* DDeqD1 */
    yyval.expression = yyvsp[0].expression ; 
  ;
    break;}
case 70:

{
    /* bit_access_range */
    SKIP_ON_ERROR ;
    /* bit_access_range */
    JD_dbg_eval_exp_to_int(yyvsp[-3].expression) ;
    JD_dbg_eval_exp_to_int(yyvsp[-1].expression) ;
    SKIP_ON_ERROR ;
    yyval.expression = yyvsp[-3].expression ;
    if( yyval.expression ) { 
      yyval.expression->next = yyvsp[-1].expression ;
      yyval.expression->etype = JD_dbg_exp_bit_range ;
    }
  ;
    break;}
case 71:

{
    /* DDeqD1 */
    yyval.expression = yyvsp[0].expression ; 
  ;
    break;}
case 72:

{
    /* Expression_D1_NEXTeqD2 */
    SKIP_ON_ERROR ;
    {
      JD_dbg_expression_node *d1, *d2 ;
      d1 = yyvsp[-1].expression ;
      d2 = yyvsp[0].expression ;
      
      if(d1) {
        while(d1->next) d1 = d1->next ;
        d1->next = d2 ;
      }
    }
    yyval.expression = yyvsp[-1].expression ;
  ;
    break;}
case 73:

{
    /* array_access_range */
    SKIP_ON_ERROR ;
    JD_dbg_eval_exp(yyvsp[-1].expression) ;
    SKIP_ON_ERROR ;
    yyval.expression = yyvsp[-1].expression ;
    if( yyval.expression ) {
      if( yyval.expression->type == JD_D_string )
        yyval.expression->etype = JD_dbg_exp_string_array_range ;
      else {
        JD_dbg_eval_exp_to_int(yyval.expression) ;
        SKIP_ON_ERROR ;
        yyval.expression->etype = JD_dbg_exp_array_range ;
      }
    }
  ;
    break;}
case 74:

{
    /* real_num */
    SKIP_ON_ERROR ;
    yyval.expression = (void *)JD_dbg_alloc_exp() ;
    yyval.expression->type = JD_D_double ;
    yyval.expression->real_value = atof(yyvsp[0].string_value) ;
  ;
    break;}
case 75:

{
    /* real_num */
    SKIP_ON_ERROR ;
    yyval.expression = (void *)JD_dbg_alloc_exp() ;
    yyval.expression->type = JD_D_double ;
    yyval.expression->real_value = atof(yyvsp[0].string_value) ;
  ;
    break;}
case 76:

{
    /* decimal_num */
    SKIP_ON_ERROR ;
    yyval.expression = (void *)JD_dbg_alloc_exp() ;
    yyval.expression->type = JD_D_int ;
    yyval.expression->int_value = atoi(yyvsp[0].string_value) ;
  ;
    break;}
case 77:

{
    /* v_decimal_num */
    SKIP_ON_ERROR ;
    yyval.expression = (void *)JD_dbg_alloc_exp() ;
    JD_scan_v_decimal( yyvsp[0].string_value, yyval.expression ) ;
  ;
    break;}
case 78:

{
    /* c_decimal_num */
    SKIP_ON_ERROR ;
    yyval.expression = (void *)JD_dbg_alloc_exp() ;
    JD_scan_c_decimal( yyvsp[0].string_value, yyval.expression ) ;
  ;
    break;}
case 79:

{
    /* v_hex_num */
    SKIP_ON_ERROR ;
    yyval.expression = (void *)JD_dbg_alloc_exp() ;
    JD_scan_v_hex( yyvsp[0].string_value, yyval.expression ) ;
  ;
    break;}
case 80:

{
    /* c_hex_num */
    SKIP_ON_ERROR ;
    yyval.expression = (void *)JD_dbg_alloc_exp() ;
    JD_scan_c_hex( yyvsp[0].string_value, yyval.expression ) ;
  ;
    break;}
case 81:

{
    /* v_octal_num */
    SKIP_ON_ERROR ;
    yyval.expression = (void *)JD_dbg_alloc_exp() ;
    JD_scan_v_octal( yyvsp[0].string_value, yyval.expression ) ;
  ;
    break;}
case 82:

{
    /* c_octal_num */
    SKIP_ON_ERROR ;
    yyval.expression = (void *)JD_dbg_alloc_exp() ;
    JD_scan_c_octal( yyvsp[0].string_value, yyval.expression ) ;
  ;
    break;}
case 83:

{
    /* v_bin_num */
    SKIP_ON_ERROR ;
    yyval.expression = (void *)JD_dbg_alloc_exp() ;
    JD_scan_v_binary( yyvsp[0].string_value, yyval.expression ) ;
  ;
    break;}
case 84:

{
    /* c_bin_num */
    SKIP_ON_ERROR ;
    yyval.expression = (void *)JD_dbg_alloc_exp() ;
    JD_scan_c_binary( yyvsp[0].string_value, yyval.expression ) ;
  ;
    break;}
case 85:

{
    /* concat_expression */
    char *str ;
    int len ;
    JD_dbg_expression_node *d ;
    
    SKIP_ON_ERROR ;
    d = yyvsp[-1].expression ;
    JD_dbg_eval_exp(d) ;
    if( d->type == JD_D_string ) {
      len = 0 ;
      while( d ) {
        if( d->string )
          len += strlen(d->string) ;
        d = d->next ;
        if( d ) {
          JD_dbg_eval_exp(d) ;
          if( d->type != JD_D_string ) {
            DBG_ERROR_ILLEGAL_DATA_TYPE_FOR_STRING_CONCATENATION ;
            JD_dbg_free_exp(yyvsp[-1].expression) ;
            break ;
          }
        }
      }
      SKIP_ON_ERROR ;
      yyval.expression = (void *)JD_dbg_alloc_exp() ;
      yyval.expression->type = JD_D_string ;
      yyval.expression->string = calloc(len+1,sizeof(char)) ;
      d = yyvsp[-1].expression ;
      while( d ) {
        if( d->string )
          strcat(yyval.expression->string, d->string) ;
        d = d->next ;
      }
    }
    else {
      len = 0 ;
      while( d ) {
        if( d->type == JD_D_bit )
          len += d->size ;
        else if( d->type == JD_D_int ) 
          len += 32 ;
        else {
          DBG_ERROR_ILLEGAL_DATA_TYPE_FOR_CONCATENATION ;
          JD_dbg_free_exp(yyvsp[-1].expression) ;
          break ;
        }  
        d = d->next ;
        if( d ) JD_dbg_eval_exp(d) ;
      }
      SKIP_ON_ERROR ;
      yyval.expression = (void *)JD_dbg_alloc_exp() ;
      yyval.expression->type = JD_D_bit ;
      yyval.expression->size = len ;
      yyval.expression->bit_a = calloc(NWORD(len),sizeof(int)) ;
      yyval.expression->bit_b = calloc(NWORD(len),sizeof(int)) ;
      d = yyvsp[-1].expression ;
      while( d ) {
        if( d->type == JD_D_bit ) {
          JD_dbg_concat_bit( 
            yyval.expression->bit_a, yyval.expression->bit_b, yyval.expression->size, 
            d->bit_a, d->bit_b, d->size
          ) ;
        }
        else if( d->type == JD_D_int ) {
          len = 0 ;
          JD_dbg_concat_bit( 
            yyval.expression->bit_a, yyval.expression->bit_b, yyval.expression->size, 
            &d->int_value, &len, 32
          ) ;
        }
        d = d->next ;
      }
    }
    JD_dbg_free_exp(yyvsp[-1].expression) ;
  ;
    break;}
case 86:

{
    /* DDeqD1 */
    yyval.expression = yyvsp[0].expression ; 
  ;
    break;}
case 87:

{
    /* Expression_D1_NEXTeqD3 */
    SKIP_ON_ERROR ;
    {
      JD_dbg_expression_node *d1, *d3 ;
      d1 = yyvsp[-2].expression ;
      d3 = yyvsp[0].expression ;
      
      if(d1) {
        while(d1->next) d1 = d1->next ;
        d1->next = d3 ;
      }
    }
    yyval.expression = yyvsp[-2].expression ;
  ;
    break;}
}
   /* the action file gets copied in in place of this dollarsign */


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



int JDB_yyerror() {
  PRINTF( "Syntax error detected in expression\n" ) ;
}


