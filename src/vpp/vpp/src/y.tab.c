
/*  A Bison parser, made from vpp.yacc
    by GNU Bison version 1.28  */

#define YYBISON 1  /* Identify Bison output.  */

#define	TTIF	257
#define	TTIFDEF	258
#define	TTIFNDEF	259
#define	TTELSE	260
#define	TTENDIF	261
#define	TTFOR	262
#define	TTENDFOR	263
#define	TTLET	264
#define	TTWHILE	265
#define	TTENDWHILE	266
#define	TTSWITCH	267
#define	TTCASE	268
#define	TTDEFAULT	269
#define	TTBREAKSW	270
#define	TTENDSWITCH	271
#define	TTID	272
#define	TTNUM	273
#define	TTREALNUM	274
#define	TTQS	275
#define	TTNAME	276
#define	TTWHITE	277
#define	TTL_EQ	278
#define	TTL_NEQ	279
#define	TTL_AND	280
#define	TTL_OR	281
#define	TTL_LEQ	282
#define	TTL_SHIFTR	283
#define	TTL_SHIFTL	284
#define	TT_POWER	285
#define	TT_UNKNOWN	286
#define	TTLOG2	287
#define	TTROUND	288
#define	TTCEIL	289
#define	TTFLOOR	290
#define	TTEVEN	291
#define	TTODD	292
#define	TTMAX	293
#define	TTMIN	294
#define	TTABS	295
#define	TTSTOI	296
#define	TTITOS	297
#define	TTSUBSTR	298
#define	TTSYSTEM	299
#define	TTCOMMENT	300
#define	TTCOMMENT2	301
#define	TTL_GEQ	302
#define	TTL_XNOR	303
#define	TTL_NAND	304
#define	TTL_NOR	305
#define	TTL_XOR	306
#define	UNARY	307

#line 1 "vpp.yacc"

/* Copyright (C) 1996 Himanshu M. Thaker

This file is part of vpp.

Vpp is distributed in the hope that it will be useful,
but without any warranty.  No author or distributor
accepts responsibility to anyone for the consequences of using it
or for whether it serves any particular purpose or works at all,
unless he says so in writing.

Everyone is granted permission to copy, modify and redistribute
vpp, but only under the conditions described in the
document "vpp copying permission notice".   An exact copy
of the document is supposed to have been given to you along with
vpp so that you can know how you may redistribute it all.
It should be in a file named COPYING.  Among other things, the
copyright notice and this notice must be preserved on all copies.  */
/*
 * $Header: /tmp_mnt/home/hmthaker/Src/vpp/src/RCS/vpp.yacc,v 1.1 1996/07/18 02:53:03 hmthaker Exp hmthaker $
 * Program :
 * Author : Himanshu M. Thaker
 * Date : Apr. 18, 1995
 * Description :
 * $Log: vpp.yacc,v $
 * Revision 1.8  1996/07/18  02:53:03  hmthaker
 * Initial revision
 *
 * Revision 1.7  1995/04/23  17:18:12  hemi
 * All features except string handling.
 */

#include <stdio.h>
#include "yacc_stuff.h"
#include "proto.h"

char ttid[MAXNAME];
var_list *head_table = NULL;
var_values *head_variable_list = NULL;
gen_ll *cstack = NULL;
extern gen_ll_mgr *code_pointer;
int loop_struct_count = 0;

varp *vp1;



#line 49 "vpp.yacc"
typedef union {
    int   ival;			/* integer value */
    char  *cp;			/* character pointer */
    expr  *ep;			/* expression pointer */
    varp  *vp;			/* variable identifier pointer */
} YYSTYPE;
#include <stdio.h>

#ifndef __cplusplus
#ifndef __STDC__
#define const
#endif
#endif



#define	YYFINAL		246
#define	YYFLAG		-32768
#define	YYNTBASE	73

#define YYTRANSLATE(x) ((unsigned)(x) <= 307 ? yytranslate[x] : 114)

static const char yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,    68,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,    66,     2,     2,     2,    59,    61,     2,    63,
    64,    57,    55,    72,    56,     2,    58,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,    69,    71,    48,
    70,    49,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,    62,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,    60,     2,    65,     2,     2,     2,     2,
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
    47,    50,    51,    52,    53,    54,    67
};

#if YYDEBUG != 0
static const short yyprhs[] = {     0,
     0,     1,     4,     7,    10,    13,    16,    19,    22,    25,
    28,    31,    34,    37,    40,    43,    46,    49,    50,    54,
    56,    58,    60,    62,    63,    68,    69,    75,    76,    78,
    79,    84,    85,    89,    90,    94,    95,   100,   101,   102,
   118,   119,   123,   124,   128,   129,   134,   135,   140,   141,
   146,   147,   151,   152,   156,   157,   159,   161,   163,   165,
   166,   174,   175,   177,   179,   182,   185,   188,   191,   194,
   197,   200,   203,   206,   209,   213,   217,   221,   225,   229,
   233,   237,   241,   245,   249,   253,   257,   261,   265,   269,
   273,   277,   281,   285,   290,   295,   300,   305,   310,   315,
   322,   329,   334,   339,   346,   355,   360,   362,   364,   366,
   368,   372
};

static const short yyrhs[] = {    -1,
    96,    73,     0,    98,    73,     0,   100,    73,     0,   102,
    73,     0,   104,    73,     0,    89,    73,     0,    92,    73,
     0,    87,    73,     0,    94,    73,     0,    76,    73,     0,
    78,    73,     0,    83,    73,     0,    81,    73,     0,    85,
    73,     0,   108,    73,     0,    75,    73,     0,     0,     1,
    74,    73,     0,    22,     0,    19,     0,    20,     0,    23,
     0,     0,    13,    77,   111,    68,     0,     0,    14,    79,
   111,    80,    68,     0,     0,    69,     0,     0,    15,    82,
    80,   107,     0,     0,    16,    84,   107,     0,     0,    17,
    86,   106,     0,     0,    11,    88,   111,   107,     0,     0,
     0,     8,    90,    63,   113,    70,   111,    91,    71,   111,
    71,   113,    70,   111,    64,   107,     0,     0,     9,    93,
   106,     0,     0,    12,    95,   106,     0,     0,     3,    97,
   111,   107,     0,     0,     4,    99,   111,   107,     0,     0,
     5,   101,   111,   107,     0,     0,     6,   103,   107,     0,
     0,     7,   105,   106,     0,     0,   107,     0,    68,     0,
    46,     0,    47,     0,     0,    10,   109,   113,    70,   111,
   110,   106,     0,     0,    71,     0,   112,     0,    55,   112,
     0,    56,   112,     0,    66,   112,     0,    65,   112,     0,
    61,   112,     0,    52,   112,     0,    53,   112,     0,    60,
   112,     0,    62,   112,     0,    51,   112,     0,   111,    55,
   111,     0,   111,    56,   111,     0,   111,    57,   111,     0,
   111,    58,   111,     0,   111,    59,   111,     0,   111,    24,
   111,     0,   111,    25,   111,     0,   111,    26,   111,     0,
   111,    27,   111,     0,   111,    48,   111,     0,   111,    28,
   111,     0,   111,    49,   111,     0,   111,    50,   111,     0,
   111,    61,   111,     0,   111,    60,   111,     0,   111,    62,
   111,     0,   111,    29,   111,     0,   111,    30,   111,     0,
   111,    31,   111,     0,    33,    63,   111,    64,     0,    34,
    63,   111,    64,     0,    35,    63,   111,    64,     0,    36,
    63,   111,    64,     0,    37,    63,   111,    64,     0,    38,
    63,   111,    64,     0,    39,    63,   111,    72,   111,    64,
     0,    40,    63,   111,    72,   111,    64,     0,    41,    63,
   111,    64,     0,    42,    63,   111,    64,     0,    43,    63,
   111,    72,   111,    64,     0,    44,    63,   111,    72,   111,
    72,   111,    64,     0,    45,    63,   111,    64,     0,    19,
     0,    20,     0,    22,     0,    21,     0,    63,   111,    64,
     0,    22,     0
};

#endif

#if YYDEBUG != 0
static const short yyrline[] = { 0,
   124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
   134,   135,   136,   137,   138,   139,   140,   141,   146,   148,
   149,   150,   151,   154,   154,   174,   174,   194,   195,   198,
   198,   218,   219,   238,   239,   264,   264,   281,   282,   283,
   301,   302,   328,   329,   355,   355,   372,   372,   389,   389,
   406,   407,   423,   424,   449,   450,   453,   454,   455,   458,
   458,   477,   478,   481,   486,   487,   488,   489,   490,   491,
   492,   493,   494,   495,   500,   501,   502,   503,   504,   505,
   506,   507,   508,   509,   510,   511,   512,   513,   514,   515,
   516,   517,   518,   519,   520,   521,   522,   523,   524,   525,
   526,   527,   528,   529,   530,   531,   534,   536,   538,   540,
   543,   546
};
#endif


#if YYDEBUG != 0 || defined (YYERROR_VERBOSE)

static const char * const yytname[] = {   "$","error","$undefined.","TTIF","TTIFDEF",
"TTIFNDEF","TTELSE","TTENDIF","TTFOR","TTENDFOR","TTLET","TTWHILE","TTENDWHILE",
"TTSWITCH","TTCASE","TTDEFAULT","TTBREAKSW","TTENDSWITCH","TTID","TTNUM","TTREALNUM",
"TTQS","TTNAME","TTWHITE","TTL_EQ","TTL_NEQ","TTL_AND","TTL_OR","TTL_LEQ","TTL_SHIFTR",
"TTL_SHIFTL","TT_POWER","TT_UNKNOWN","TTLOG2","TTROUND","TTCEIL","TTFLOOR","TTEVEN",
"TTODD","TTMAX","TTMIN","TTABS","TTSTOI","TTITOS","TTSUBSTR","TTSYSTEM","TTCOMMENT",
"TTCOMMENT2","'<'","'>'","TTL_GEQ","TTL_XNOR","TTL_NAND","TTL_NOR","TTL_XOR",
"'+'","'-'","'*'","'/'","'%'","'|'","'&'","'^'","'('","')'","'~'","'!'","UNARY",
"'\\n'","':'","'='","';'","','","source_text","@1","other_stuff","switch_declaration",
"@2","case_declaration","@3","optional_colon","default_declaration","@4","breaksw_declaration",
"@5","endswitch_declaration","@6","while_declaration","@7","for_declaration",
"@8","@9","endfor_declaration","@10","endwhile_declaration","@11","if_declaration",
"@12","ifdef_declaration","@13","ifndef_declaration","@14","else_declaration",
"@15","endif_declaration","@16","vpp_line_termination_opt","vpp_line_termination",
"let_declaration","@17","optional_semi","expression","primary","identifier", NULL
};
#endif

static const short yyr1[] = {     0,
    73,    73,    73,    73,    73,    73,    73,    73,    73,    73,
    73,    73,    73,    73,    73,    73,    73,    74,    73,    75,
    75,    75,    75,    77,    76,    79,    78,    80,    80,    82,
    81,    84,    83,    86,    85,    88,    87,    90,    91,    89,
    93,    92,    95,    94,    97,    96,    99,    98,   101,   100,
   103,   102,   105,   104,   106,   106,   107,   107,   107,   109,
   108,   110,   110,   111,   111,   111,   111,   111,   111,   111,
   111,   111,   111,   111,   111,   111,   111,   111,   111,   111,
   111,   111,   111,   111,   111,   111,   111,   111,   111,   111,
   111,   111,   111,   111,   111,   111,   111,   111,   111,   111,
   111,   111,   111,   111,   111,   111,   112,   112,   112,   112,
   112,   113
};

static const short yyr2[] = {     0,
     0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     0,     3,     1,
     1,     1,     1,     0,     4,     0,     5,     0,     1,     0,
     4,     0,     3,     0,     3,     0,     4,     0,     0,    15,
     0,     3,     0,     3,     0,     4,     0,     4,     0,     4,
     0,     3,     0,     3,     0,     1,     1,     1,     1,     0,
     7,     0,     1,     1,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     3,     3,     3,     3,     3,     3,
     3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
     3,     3,     3,     4,     4,     4,     4,     4,     4,     6,
     6,     4,     4,     6,     8,     4,     1,     1,     1,     1,
     3,     1
};

static const short yydefact[] = {     0,
    18,    45,    47,    49,    51,    53,    38,    41,    60,    36,
    43,    24,    26,    30,    32,    34,    21,    22,    20,    23,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,    55,     0,    55,     0,     0,    55,     0,     0,    28,
     0,    55,    17,    11,    12,    14,    13,    15,     9,     7,
     8,    10,     2,     3,     4,     5,     6,    16,    19,   107,
   108,   110,   109,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,    64,     0,
     0,    58,    59,    57,    52,    54,    56,     0,    42,   112,
     0,     0,    44,     0,    28,    29,     0,    33,    35,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,    74,    70,    71,    65,    66,    72,    69,    73,
     0,    68,    67,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,    46,    48,    50,     0,     0,    37,    25,     0,
    31,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,   111,    80,    81,    82,    83,    85,
    91,    92,    93,    84,    86,    87,    75,    76,    77,    78,
    79,    89,    88,    90,     0,    62,    27,    94,    95,    96,
    97,    98,    99,     0,     0,   102,   103,     0,     0,   106,
    39,    63,    55,     0,     0,     0,     0,     0,    61,   100,
   101,   104,     0,     0,     0,     0,   105,     0,     0,     0,
     0,     0,    40,     0,     0,     0
};

static const short yydefgoto[] = {    53,
    37,    21,    22,    48,    23,    49,   117,    24,    50,    25,
    51,    26,    52,    27,    46,    28,    43,   228,    29,    44,
    30,    47,    31,    38,    32,    39,    33,    40,    34,    41,
    35,    42,   106,   107,    36,    45,   223,    98,    99,   111
};

static const short yypact[] = {  1223,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  1223,  1223,  1223,  1223,  1223,  1223,  1223,  1223,  1223,  1223,
  1223,  1223,  1223,  1223,  1223,  1223,  1223,   303,   303,   303,
    12,    12,   -51,    12,    -8,   303,    12,   303,   303,   -54,
    12,    12,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,   -34,   -16,    -6,    -2,     1,     3,    10,
    31,    32,    64,    66,    67,    68,     6,     6,     6,     6,
     6,     6,     6,     6,   303,     6,     6,   470,-32768,   470,
   470,-32768,-32768,-32768,-32768,-32768,-32768,    -8,-32768,-32768,
     8,   470,-32768,   515,   424,-32768,    12,-32768,-32768,   303,
   303,   303,   303,   303,   303,   303,   303,   303,   303,   303,
   303,   303,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
   530,-32768,-32768,   303,   303,   303,   303,   303,   303,   303,
   303,   303,   303,   303,   303,   303,   303,   303,   303,   303,
   303,   303,-32768,-32768,-32768,    34,   303,-32768,-32768,    65,
-32768,   571,   612,   653,   694,   735,   776,    -7,   112,   817,
   858,   181,   220,   899,-32768,    41,    41,   -25,   331,    41,
   -22,   -22,   121,  1145,  1145,    41,    95,    95,    15,    15,
    15,-32768,-32768,-32768,   303,   346,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,   303,   303,-32768,-32768,   303,   303,-32768,
  1160,-32768,    12,   940,   981,  1022,   259,    63,-32768,-32768,
-32768,-32768,   303,   303,  1063,   385,-32768,    -8,    74,   303,
  1104,    12,-32768,   135,   145,-32768
};

static const short yypgoto[] = {   191,
-32768,-32768,-32768,-32768,-32768,-32768,    33,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,   -36,   -38,-32768,-32768,-32768,   -39,   167,  -106
};


#define	YYLAST		1246


static const short yytable[] = {   100,
   101,   166,   105,   149,   150,   151,   112,   109,   114,   115,
   113,   108,   118,   110,   116,   119,   144,   145,   146,   147,
   148,   149,   150,   151,    70,    71,    72,    73,   120,   155,
   156,   157,   158,   159,   160,   161,   162,   160,   161,   162,
   152,   153,   154,   149,   150,   151,   121,   155,   156,   157,
   158,   159,   160,   161,   162,   141,   122,   102,   103,   163,
   123,   164,   165,   124,   214,   125,   146,   147,    95,   149,
   150,   151,   126,   168,   160,   161,   162,   167,   171,   104,
   172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
   182,   183,   184,   127,   128,   155,   156,   157,   158,   159,
   160,   161,   162,   205,   186,   187,   188,   189,   190,   191,
   192,   193,   194,   195,   196,   197,   198,   199,   200,   201,
   202,   203,   204,   149,   150,   151,   129,   206,   130,   131,
   132,   239,   207,   234,   245,   144,   145,   146,   147,   148,
   149,   150,   151,   240,   246,     0,     0,   170,     0,   149,
   150,   157,   158,   159,   160,   161,   162,     0,     0,   152,
   153,   154,     0,     0,     0,   221,   155,   156,   157,   158,
   159,   160,   161,   162,   224,   225,     0,     0,   226,   227,
   160,   161,   162,   215,     0,     0,   229,     0,     0,     0,
   244,     0,     0,   235,   236,     0,     0,     0,     0,     0,
   241,     0,     0,   243,   144,   145,   146,   147,   148,   149,
   150,   151,    54,    55,    56,    57,    58,    59,    60,    61,
    62,    63,    64,    65,    66,    67,    68,    69,   152,   153,
   154,     0,     0,     0,     0,   155,   156,   157,   158,   159,
   160,   161,   162,   144,   145,   146,   147,   148,   149,   150,
   151,     0,   218,   133,   134,   135,   136,   137,   138,   139,
   140,     0,   142,   143,     0,     0,     0,   152,   153,   154,
     0,     0,     0,     0,   155,   156,   157,   158,   159,   160,
   161,   162,   144,   145,   146,   147,   148,   149,   150,   151,
     0,   219,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,   152,   153,   154,     0,
     0,     0,     0,   155,   156,   157,   158,   159,   160,   161,
   162,    70,    71,    72,    73,     0,     0,     0,     0,     0,
   233,     0,     0,     0,     0,    74,    75,    76,    77,    78,
    79,    80,    81,    82,    83,    84,    85,    86,     0,     0,
     0,     0,     0,    87,    88,    89,   146,    90,    91,   149,
   150,   151,    92,    93,    94,    95,     0,    96,    97,   144,
   145,   146,   147,   148,   149,   150,   151,     0,     0,     0,
     0,     0,     0,     0,     0,   155,   156,   157,   158,   159,
   160,   161,   162,   152,   153,   154,     0,     0,     0,     0,
   155,   156,   157,   158,   159,   160,   161,   162,   144,   145,
   146,   147,   148,   149,   150,   151,   222,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,   152,   153,   154,     0,     0,     0,     0,   155,
   156,   157,   158,   159,   160,   161,   162,   144,   145,   146,
   147,   148,   149,   150,   151,   238,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,   152,   153,   154,     0,     0,     0,     0,   155,   156,
   157,   158,   159,   160,   161,   162,     0,     0,     0,     0,
     0,     0,   116,   144,   145,   146,   147,   148,   149,   150,
   151,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,   102,   103,   152,   153,   154,
     0,     0,     0,     0,   155,   156,   157,   158,   159,   160,
   161,   162,     0,     0,     0,     0,     0,   104,   144,   145,
   146,   147,   148,   149,   150,   151,     0,     0,     0,     0,
     0,     0,     0,   144,   145,   146,   147,   148,   149,   150,
   151,     0,   152,   153,   154,     0,     0,     0,     0,   155,
   156,   157,   158,   159,   160,   161,   162,   152,   153,   154,
     0,     0,   169,     0,   155,   156,   157,   158,   159,   160,
   161,   162,     0,   185,   144,   145,   146,   147,   148,   149,
   150,   151,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,   152,   153,
   154,     0,     0,     0,     0,   155,   156,   157,   158,   159,
   160,   161,   162,     0,   208,   144,   145,   146,   147,   148,
   149,   150,   151,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,   152,
   153,   154,     0,     0,     0,     0,   155,   156,   157,   158,
   159,   160,   161,   162,     0,   209,   144,   145,   146,   147,
   148,   149,   150,   151,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
   152,   153,   154,     0,     0,     0,     0,   155,   156,   157,
   158,   159,   160,   161,   162,     0,   210,   144,   145,   146,
   147,   148,   149,   150,   151,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,   152,   153,   154,     0,     0,     0,     0,   155,   156,
   157,   158,   159,   160,   161,   162,     0,   211,   144,   145,
   146,   147,   148,   149,   150,   151,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,   152,   153,   154,     0,     0,     0,     0,   155,
   156,   157,   158,   159,   160,   161,   162,     0,   212,   144,
   145,   146,   147,   148,   149,   150,   151,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,   152,   153,   154,     0,     0,     0,     0,
   155,   156,   157,   158,   159,   160,   161,   162,     0,   213,
   144,   145,   146,   147,   148,   149,   150,   151,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,   152,   153,   154,     0,     0,     0,
     0,   155,   156,   157,   158,   159,   160,   161,   162,     0,
   216,   144,   145,   146,   147,   148,   149,   150,   151,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,   152,   153,   154,     0,     0,
     0,     0,   155,   156,   157,   158,   159,   160,   161,   162,
     0,   217,   144,   145,   146,   147,   148,   149,   150,   151,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,   152,   153,   154,     0,
     0,     0,     0,   155,   156,   157,   158,   159,   160,   161,
   162,     0,   220,   144,   145,   146,   147,   148,   149,   150,
   151,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,   152,   153,   154,
     0,     0,     0,     0,   155,   156,   157,   158,   159,   160,
   161,   162,     0,   230,   144,   145,   146,   147,   148,   149,
   150,   151,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,   152,   153,
   154,     0,     0,     0,     0,   155,   156,   157,   158,   159,
   160,   161,   162,     0,   231,   144,   145,   146,   147,   148,
   149,   150,   151,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,   152,
   153,   154,     0,     0,     0,     0,   155,   156,   157,   158,
   159,   160,   161,   162,     0,   232,   144,   145,   146,   147,
   148,   149,   150,   151,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
   152,   153,   154,     0,     0,     0,     0,   155,   156,   157,
   158,   159,   160,   161,   162,     0,   237,   144,   145,   146,
   147,   148,   149,   150,   151,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,   152,   153,   154,     0,     0,     0,     0,   155,   156,
   157,   158,   159,   160,   161,   162,     0,   242,   144,   145,
   146,   147,   148,   149,   150,   151,     0,     0,     0,     0,
     0,     0,     0,   144,   145,   146,   147,   148,   149,   150,
   151,     0,-32768,-32768,   154,     0,     0,     0,     0,   155,
   156,   157,   158,   159,   160,   161,   162,   152,   153,   154,
     0,     0,     0,     0,   155,   156,   157,   158,   159,   160,
   161,   162,    -1,     1,     0,     2,     3,     4,     5,     6,
     7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
     0,    17,    18,     0,    19,    20
};

static const short yycheck[] = {    39,
    40,   108,    41,    29,    30,    31,    46,    44,    48,    49,
    47,    63,    51,    22,    69,    52,    24,    25,    26,    27,
    28,    29,    30,    31,    19,    20,    21,    22,    63,    55,
    56,    57,    58,    59,    60,    61,    62,    60,    61,    62,
    48,    49,    50,    29,    30,    31,    63,    55,    56,    57,
    58,    59,    60,    61,    62,    95,    63,    46,    47,    98,
    63,   100,   101,    63,    72,    63,    26,    27,    63,    29,
    30,    31,    63,   112,    60,    61,    62,    70,   117,    68,
   120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
   130,   131,   132,    63,    63,    55,    56,    57,    58,    59,
    60,    61,    62,    70,   144,   145,   146,   147,   148,   149,
   150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
   160,   161,   162,    29,    30,    31,    63,   167,    63,    63,
    63,   238,    68,    71,     0,    24,    25,    26,    27,    28,
    29,    30,    31,    70,     0,    -1,    -1,   115,    -1,    29,
    30,    57,    58,    59,    60,    61,    62,    -1,    -1,    48,
    49,    50,    -1,    -1,    -1,   205,    55,    56,    57,    58,
    59,    60,    61,    62,   214,   215,    -1,    -1,   218,   219,
    60,    61,    62,    72,    -1,    -1,   223,    -1,    -1,    -1,
     0,    -1,    -1,   233,   234,    -1,    -1,    -1,    -1,    -1,
   240,    -1,    -1,   242,    24,    25,    26,    27,    28,    29,
    30,    31,    22,    23,    24,    25,    26,    27,    28,    29,
    30,    31,    32,    33,    34,    35,    36,    37,    48,    49,
    50,    -1,    -1,    -1,    -1,    55,    56,    57,    58,    59,
    60,    61,    62,    24,    25,    26,    27,    28,    29,    30,
    31,    -1,    72,    87,    88,    89,    90,    91,    92,    93,
    94,    -1,    96,    97,    -1,    -1,    -1,    48,    49,    50,
    -1,    -1,    -1,    -1,    55,    56,    57,    58,    59,    60,
    61,    62,    24,    25,    26,    27,    28,    29,    30,    31,
    -1,    72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    48,    49,    50,    -1,
    -1,    -1,    -1,    55,    56,    57,    58,    59,    60,    61,
    62,    19,    20,    21,    22,    -1,    -1,    -1,    -1,    -1,
    72,    -1,    -1,    -1,    -1,    33,    34,    35,    36,    37,
    38,    39,    40,    41,    42,    43,    44,    45,    -1,    -1,
    -1,    -1,    -1,    51,    52,    53,    26,    55,    56,    29,
    30,    31,    60,    61,    62,    63,    -1,    65,    66,    24,
    25,    26,    27,    28,    29,    30,    31,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    55,    56,    57,    58,    59,
    60,    61,    62,    48,    49,    50,    -1,    -1,    -1,    -1,
    55,    56,    57,    58,    59,    60,    61,    62,    24,    25,
    26,    27,    28,    29,    30,    31,    71,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    48,    49,    50,    -1,    -1,    -1,    -1,    55,
    56,    57,    58,    59,    60,    61,    62,    24,    25,    26,
    27,    28,    29,    30,    31,    71,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    48,    49,    50,    -1,    -1,    -1,    -1,    55,    56,
    57,    58,    59,    60,    61,    62,    -1,    -1,    -1,    -1,
    -1,    -1,    69,    24,    25,    26,    27,    28,    29,    30,
    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    46,    47,    48,    49,    50,
    -1,    -1,    -1,    -1,    55,    56,    57,    58,    59,    60,
    61,    62,    -1,    -1,    -1,    -1,    -1,    68,    24,    25,
    26,    27,    28,    29,    30,    31,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    24,    25,    26,    27,    28,    29,    30,
    31,    -1,    48,    49,    50,    -1,    -1,    -1,    -1,    55,
    56,    57,    58,    59,    60,    61,    62,    48,    49,    50,
    -1,    -1,    68,    -1,    55,    56,    57,    58,    59,    60,
    61,    62,    -1,    64,    24,    25,    26,    27,    28,    29,
    30,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    48,    49,
    50,    -1,    -1,    -1,    -1,    55,    56,    57,    58,    59,
    60,    61,    62,    -1,    64,    24,    25,    26,    27,    28,
    29,    30,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    48,
    49,    50,    -1,    -1,    -1,    -1,    55,    56,    57,    58,
    59,    60,    61,    62,    -1,    64,    24,    25,    26,    27,
    28,    29,    30,    31,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    48,    49,    50,    -1,    -1,    -1,    -1,    55,    56,    57,
    58,    59,    60,    61,    62,    -1,    64,    24,    25,    26,
    27,    28,    29,    30,    31,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    48,    49,    50,    -1,    -1,    -1,    -1,    55,    56,
    57,    58,    59,    60,    61,    62,    -1,    64,    24,    25,
    26,    27,    28,    29,    30,    31,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    48,    49,    50,    -1,    -1,    -1,    -1,    55,
    56,    57,    58,    59,    60,    61,    62,    -1,    64,    24,
    25,    26,    27,    28,    29,    30,    31,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    48,    49,    50,    -1,    -1,    -1,    -1,
    55,    56,    57,    58,    59,    60,    61,    62,    -1,    64,
    24,    25,    26,    27,    28,    29,    30,    31,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    48,    49,    50,    -1,    -1,    -1,
    -1,    55,    56,    57,    58,    59,    60,    61,    62,    -1,
    64,    24,    25,    26,    27,    28,    29,    30,    31,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    48,    49,    50,    -1,    -1,
    -1,    -1,    55,    56,    57,    58,    59,    60,    61,    62,
    -1,    64,    24,    25,    26,    27,    28,    29,    30,    31,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    48,    49,    50,    -1,
    -1,    -1,    -1,    55,    56,    57,    58,    59,    60,    61,
    62,    -1,    64,    24,    25,    26,    27,    28,    29,    30,
    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    48,    49,    50,
    -1,    -1,    -1,    -1,    55,    56,    57,    58,    59,    60,
    61,    62,    -1,    64,    24,    25,    26,    27,    28,    29,
    30,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    48,    49,
    50,    -1,    -1,    -1,    -1,    55,    56,    57,    58,    59,
    60,    61,    62,    -1,    64,    24,    25,    26,    27,    28,
    29,    30,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    48,
    49,    50,    -1,    -1,    -1,    -1,    55,    56,    57,    58,
    59,    60,    61,    62,    -1,    64,    24,    25,    26,    27,
    28,    29,    30,    31,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    48,    49,    50,    -1,    -1,    -1,    -1,    55,    56,    57,
    58,    59,    60,    61,    62,    -1,    64,    24,    25,    26,
    27,    28,    29,    30,    31,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    48,    49,    50,    -1,    -1,    -1,    -1,    55,    56,
    57,    58,    59,    60,    61,    62,    -1,    64,    24,    25,
    26,    27,    28,    29,    30,    31,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    24,    25,    26,    27,    28,    29,    30,
    31,    -1,    48,    49,    50,    -1,    -1,    -1,    -1,    55,
    56,    57,    58,    59,    60,    61,    62,    48,    49,    50,
    -1,    -1,    -1,    -1,    55,    56,    57,    58,    59,    60,
    61,    62,     0,     1,    -1,     3,     4,     5,     6,     7,
     8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
    -1,    19,    20,    -1,    22,    23
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

case 18:
#line 141 "vpp.yacc"
{
	    set_to_last_mode();
	    yyclearin;
	    yyerrok;
	  ;
    break;}
case 20:
#line 148 "vpp.yacc"
{printf("%s", ttid);;
    break;}
case 21:
#line 149 "vpp.yacc"
{printf("%s", ttid);;
    break;}
case 22:
#line 150 "vpp.yacc"
{printf("%s", ttid);;
    break;}
case 23:
#line 151 "vpp.yacc"
{printf("%s", ttid);;
    break;}
case 24:
#line 154 "vpp.yacc"
{set_to_vpp_mode();;
    break;}
case 25:
#line 155 "vpp.yacc"
{
/*
printf("got a switch with expression "); traverse_expression ($3); printf("\n");
*/
		    if (code_pointer != NULL)
		      cstack = push(cstack, create_generic_ll(CODE_POINTER,NULL,
							      code_pointer)
				    );
		    code_pointer = NULL;
		    cstack = push(cstack,
				  create_generic_ll(SWITCH_STRUCTURE, NULL,
						    create_switch_structure(yyvsp[-1].ep)
						    )
				  );
		    loop_struct_count++;
		    set_to_copy();
		;
    break;}
case 26:
#line 174 "vpp.yacc"
{set_to_vpp_mode();;
    break;}
case 27:
#line 175 "vpp.yacc"
{
		    if (code_pointer != NULL)
		      cstack = push(cstack, create_generic_ll(CODE_POINTER,NULL,
							      code_pointer)
				    );
		    code_pointer = NULL;
		    if (cstack != NULL) {
		      cstack = push(cstack, create_generic_ll(CASE_STRUCTURE,NULL,
							    create_case_structure(yyvsp[-2].ep))
				  );
		      set_to_last_mode();
		    }
 		    else {
		      fprintf(stderr,"ERROR : got default without matching switch!!!\n");
		      set_to_initial_mode();
		    }
		;
    break;}
case 30:
#line 198 "vpp.yacc"
{set_to_vpp_mode();;
    break;}
case 31:
#line 199 "vpp.yacc"
{
		    if (code_pointer != NULL)
		      cstack = push(cstack, create_generic_ll(CODE_POINTER,NULL,
							      code_pointer)
				    );
		    code_pointer = NULL;
		    if (cstack != NULL) {
		      cstack = push(cstack, create_generic_ll(DEFAULT_STRUCTURE,NULL,
							    NULL)
				  );
		      set_to_last_mode();
		    }
 		    else {
		      fprintf(stderr,"ERROR : got default without matching switch!!!\n");
		      set_to_initial_mode();
		    }
		;
    break;}
case 32:
#line 218 "vpp.yacc"
{set_to_vpp_mode();;
    break;}
case 33:
#line 219 "vpp.yacc"
{
		    if (code_pointer != NULL)
		      cstack = push(cstack, create_generic_ll(CODE_POINTER,NULL,
							      code_pointer)
				    );
		    code_pointer = NULL;
		    if (cstack != NULL) {
		      cstack = push(cstack, create_generic_ll(BREAKSW_STRUCTURE,NULL,
							    NULL)
				  );
		      set_to_last_mode();
		    }
 		    else {
		      fprintf(stderr,"ERROR : got breaksw without matching switch!!!\n");
		      set_to_initial_mode();
		    }
		;
    break;}
case 34:
#line 238 "vpp.yacc"
{set_to_vpp_mode();;
    break;}
case 35:
#line 239 "vpp.yacc"
{
		    if (code_pointer != NULL)
		      cstack = push(cstack, create_generic_ll(CODE_POINTER,NULL,
							      code_pointer)
				    );
		    code_pointer = NULL;
		    if (cstack != NULL)
		      cstack = push(cstack, create_generic_ll(ENDSWITCH_STRUCTURE,NULL,
							    NULL)
				  );
 		    else
		      fprintf(stderr,"ERROR : got an endswitch without matching switch!!!\n");
		    if (loop_struct_count != 0) {
			loop_struct_count--;
			if (loop_struct_count == 0) {
			  eval_stack(cstack);
			  cstack = NULL;
			  set_to_initial_mode();
		        }
		    else set_to_last_mode();
		    }
		    if (cstack == NULL) set_to_initial_mode();
		;
    break;}
case 36:
#line 264 "vpp.yacc"
{set_to_vpp_mode();;
    break;}
case 37:
#line 265 "vpp.yacc"
{
		    if (code_pointer != NULL)
		      cstack = push(cstack, create_generic_ll(CODE_POINTER,NULL,
							      code_pointer)
				    );
		    code_pointer = NULL;
		    cstack = push(cstack,
				  create_generic_ll(WHILE_STRUCTURE, NULL,
						    create_while_structure(yyvsp[-1].ep)
						    )
				  );
		    loop_struct_count++;
		    set_to_copy();
		;
    break;}
case 38:
#line 281 "vpp.yacc"
{set_to_vpp_mode();;
    break;}
case 39:
#line 282 "vpp.yacc"
{vp1 = create_variable_ref(yyvsp[-2].cp,yyvsp[0].ep,TRUE);;
    break;}
case 40:
#line 284 "vpp.yacc"
{
		    if (code_pointer != NULL)
		      cstack = push(cstack, create_generic_ll(CODE_POINTER,NULL,
							      code_pointer)
				    );
		    code_pointer = NULL;

		    cstack = push(cstack,
				  create_generic_ll(FOR_STRUCTURE, NULL,
						    create_for_structure(vp1,yyvsp[-6].ep,yyvsp[-4].cp,yyvsp[-2].ep)
						    )
				  );
		    loop_struct_count++;
		    set_to_copy();
		;
    break;}
case 41:
#line 301 "vpp.yacc"
{set_to_vpp_mode();;
    break;}
case 42:
#line 302 "vpp.yacc"
{
		    if (code_pointer != NULL)
		      cstack = push(cstack, create_generic_ll(CODE_POINTER,NULL,
							      code_pointer)
				    );
		    code_pointer = NULL;
		    if (cstack != NULL)
		      cstack = push(cstack, create_generic_ll(ENDFOR_STRUCTURE,NULL,
							    NULL)
				  );
 		    else
		      fprintf(stderr,"ERROR : got an endfor without matching for!!!\n");

		    if (loop_struct_count != 0) {
			loop_struct_count--;
			if (loop_struct_count == 0) {
			  eval_stack(cstack);
			  cstack = NULL;
			  set_to_initial_mode();
		        }
		    else set_to_last_mode();
		    }
		    if (cstack == NULL) set_to_initial_mode();
		;
    break;}
case 43:
#line 328 "vpp.yacc"
{set_to_vpp_mode();;
    break;}
case 44:
#line 329 "vpp.yacc"
{
		    if (code_pointer != NULL)
		      cstack = push(cstack, create_generic_ll(CODE_POINTER,NULL,
							      code_pointer)
				    );
		    code_pointer = NULL;
		    if (cstack != NULL)
		      cstack = push(cstack, create_generic_ll(ENDWHILE_STRUCTURE,NULL,
							    NULL)
				  );
		    else
		      fprintf(stderr,"ERROR : got an endwhile without matching while!!!\n");

		    if (loop_struct_count != 0) {
			loop_struct_count--;
			if (loop_struct_count == 0) {
			  eval_stack(cstack);
			  cstack = NULL;
			  set_to_initial_mode();
		        }
		    else set_to_last_mode();
		    }
		    if (cstack == NULL) set_to_initial_mode();
		;
    break;}
case 45:
#line 355 "vpp.yacc"
{set_to_vpp_mode();;
    break;}
case 46:
#line 356 "vpp.yacc"
{
		    if (code_pointer != NULL)
		      cstack = push(cstack, create_generic_ll(CODE_POINTER,NULL,
							      code_pointer)
				    );
		    code_pointer = NULL;
		    cstack = push(cstack,
				  create_generic_ll(IF_STRUCTURE, NULL,
						    create_if_structure(yyvsp[-1].ep)
						    )
				  );
		    loop_struct_count++;
		    set_to_copy();
		;
    break;}
case 47:
#line 372 "vpp.yacc"
{set_to_vpp_mode();;
    break;}
case 48:
#line 373 "vpp.yacc"
{
		    if (code_pointer != NULL)
		      cstack = push(cstack, create_generic_ll(CODE_POINTER,NULL,
							      code_pointer)
				    );
		    code_pointer = NULL;
		    cstack = push(cstack,
				  create_generic_ll(IFDEF_STRUCTURE, NULL,
						    create_if_structure(yyvsp[-1].ep)
						    )
				  );
		    loop_struct_count++;
		    set_to_copy();
		;
    break;}
case 49:
#line 389 "vpp.yacc"
{set_to_vpp_mode();;
    break;}
case 50:
#line 390 "vpp.yacc"
{
		    if (code_pointer != NULL)
		      cstack = push(cstack, create_generic_ll(CODE_POINTER,NULL,
							      code_pointer)
				    );
		    code_pointer = NULL;
		    cstack = push(cstack,
				  create_generic_ll(IFNDEF_STRUCTURE, NULL,
						    create_if_structure(yyvsp[-1].ep)
						    )
				  );
		    loop_struct_count++;
		    set_to_copy();
		;
    break;}
case 51:
#line 406 "vpp.yacc"
{set_to_vpp_mode();;
    break;}
case 52:
#line 407 "vpp.yacc"
{
		    if (code_pointer != NULL)
		      cstack = push(cstack, create_generic_ll(CODE_POINTER,NULL,
							      code_pointer)
				    );
		    code_pointer = NULL;
		    if (cstack != NULL)
		      cstack = push(cstack, create_generic_ll(ELSE_STRUCTURE,NULL,
							    NULL)
				  );
		    else
		      fprintf(stderr,"ERROR : got an else without matching if!!!\n");
		    set_to_last_mode();
		;
    break;}
case 53:
#line 423 "vpp.yacc"
{set_to_vpp_mode();;
    break;}
case 54:
#line 424 "vpp.yacc"
{
		    if (code_pointer != NULL)
		      cstack = push(cstack, create_generic_ll(CODE_POINTER,NULL,
							      code_pointer)
				    );
		    code_pointer = NULL;
		    if (cstack != NULL)
		      cstack = push(cstack, create_generic_ll(ENDIF_STRUCTURE,NULL,
					    NULL)
				    );
 		    else
		      fprintf(stderr,"ERROR : got an endif without matching if!!!\n");
		    if (loop_struct_count != 0) {
			loop_struct_count--;
			if (loop_struct_count == 0) {
			  eval_stack(cstack);
			  cstack = NULL;
			  set_to_initial_mode();
		        }
		    else set_to_last_mode();
		    }
		    if (cstack == NULL) set_to_initial_mode();
		;
    break;}
case 58:
#line 454 "vpp.yacc"
{printf("//");;
    break;}
case 59:
#line 455 "vpp.yacc"
{printf("/*");;
    break;}
case 60:
#line 458 "vpp.yacc"
{set_to_vpp_mode();;
    break;}
case 61:
#line 459 "vpp.yacc"
{
		  if (loop_struct_count > 0) {
		    if (code_pointer != NULL)
		      cstack = push(cstack, create_generic_ll(CODE_POINTER,NULL,
			    		      code_pointer)
				   );
		    code_pointer = NULL;
		    cstack = push(cstack, create_generic_ll(LET_STRUCTURE,NULL,
							  create_variable_ref(yyvsp[-4].cp,yyvsp[-2].ep,FALSE))
			         );
		  }
		  else {
		    create_variable_ref(yyvsp[-4].cp,yyvsp[-2].ep, TRUE);
		  }
		  set_to_last_mode();
		;
    break;}
case 64:
#line 481 "vpp.yacc"
{yyval.ep = yyvsp[0].ep;;
    break;}
case 65:
#line 486 "vpp.yacc"
{yyval.ep = create_expression(UPLUS,yyvsp[0].ep,NULL,NULL);;
    break;}
case 66:
#line 487 "vpp.yacc"
{yyval.ep = create_expression(UMINUS,yyvsp[0].ep,NULL,NULL);;
    break;}
case 67:
#line 488 "vpp.yacc"
{yyval.ep = create_expression(UNOT,yyvsp[0].ep,NULL,NULL);;
    break;}
case 68:
#line 489 "vpp.yacc"
{yyval.ep = create_expression(UTILDA,yyvsp[0].ep,NULL,NULL);;
    break;}
case 69:
#line 490 "vpp.yacc"
{yyval.ep = create_expression(UAND,yyvsp[0].ep,NULL,NULL);;
    break;}
case 70:
#line 491 "vpp.yacc"
{yyval.ep = create_expression(UNAND,yyvsp[0].ep,NULL,NULL);;
    break;}
case 71:
#line 492 "vpp.yacc"
{yyval.ep = create_expression(UNOR,yyvsp[0].ep,NULL,NULL);;
    break;}
case 72:
#line 493 "vpp.yacc"
{yyval.ep = create_expression(UPIPE,yyvsp[0].ep,NULL,NULL);;
    break;}
case 73:
#line 494 "vpp.yacc"
{yyval.ep = create_expression(UCARAT,yyvsp[0].ep,NULL,NULL);;
    break;}
case 74:
#line 495 "vpp.yacc"
{yyval.ep = create_expression(UXNOR,yyvsp[0].ep,NULL,NULL);;
    break;}
case 75:
#line 500 "vpp.yacc"
{yyval.ep = create_expression(ADD,yyvsp[-2].ep,yyvsp[0].ep,NULL);;
    break;}
case 76:
#line 501 "vpp.yacc"
{yyval.ep = create_expression(SUB,yyvsp[-2].ep,yyvsp[0].ep,NULL);;
    break;}
case 77:
#line 502 "vpp.yacc"
{yyval.ep = create_expression(MULT,yyvsp[-2].ep,yyvsp[0].ep,NULL);;
    break;}
case 78:
#line 503 "vpp.yacc"
{yyval.ep = create_expression(DIV,yyvsp[-2].ep,yyvsp[0].ep,NULL);;
    break;}
case 79:
#line 504 "vpp.yacc"
{yyval.ep = create_expression(MOD,yyvsp[-2].ep,yyvsp[0].ep,NULL);;
    break;}
case 80:
#line 505 "vpp.yacc"
{yyval.ep = create_expression(EQ,yyvsp[-2].ep,yyvsp[0].ep,NULL);;
    break;}
case 81:
#line 506 "vpp.yacc"
{yyval.ep = create_expression(NEQ,yyvsp[-2].ep,yyvsp[0].ep,NULL);;
    break;}
case 82:
#line 507 "vpp.yacc"
{yyval.ep = create_expression(AND,yyvsp[-2].ep,yyvsp[0].ep,NULL);;
    break;}
case 83:
#line 508 "vpp.yacc"
{yyval.ep = create_expression(OR,yyvsp[-2].ep,yyvsp[0].ep,NULL);;
    break;}
case 84:
#line 509 "vpp.yacc"
{yyval.ep = create_expression(LT,yyvsp[-2].ep,yyvsp[0].ep,NULL);;
    break;}
case 85:
#line 510 "vpp.yacc"
{yyval.ep = create_expression(LEQ,yyvsp[-2].ep,yyvsp[0].ep,NULL);;
    break;}
case 86:
#line 511 "vpp.yacc"
{yyval.ep = create_expression(GT,yyvsp[-2].ep,yyvsp[0].ep,NULL);;
    break;}
case 87:
#line 512 "vpp.yacc"
{yyval.ep = create_expression(GEQ,yyvsp[-2].ep,yyvsp[0].ep,NULL);;
    break;}
case 88:
#line 513 "vpp.yacc"
{yyval.ep = create_expression(BITAND,yyvsp[-2].ep,yyvsp[0].ep,NULL);;
    break;}
case 89:
#line 514 "vpp.yacc"
{yyval.ep = create_expression(BITOR,yyvsp[-2].ep,yyvsp[0].ep,NULL);;
    break;}
case 90:
#line 515 "vpp.yacc"
{yyval.ep = create_expression(BITXOR,yyvsp[-2].ep,yyvsp[0].ep,NULL);;
    break;}
case 91:
#line 516 "vpp.yacc"
{yyval.ep = create_expression(SHIFTR,yyvsp[-2].ep,yyvsp[0].ep,NULL);;
    break;}
case 92:
#line 517 "vpp.yacc"
{yyval.ep = create_expression(SHIFTL,yyvsp[-2].ep,yyvsp[0].ep,NULL);;
    break;}
case 93:
#line 518 "vpp.yacc"
{yyval.ep = create_expression(POWER,yyvsp[-2].ep,yyvsp[0].ep,NULL);;
    break;}
case 94:
#line 519 "vpp.yacc"
{yyval.ep = create_expression(LOG2,yyvsp[-1].ep,NULL,NULL);;
    break;}
case 95:
#line 520 "vpp.yacc"
{yyval.ep = create_expression(ROUND,yyvsp[-1].ep,NULL,NULL);;
    break;}
case 96:
#line 521 "vpp.yacc"
{yyval.ep = create_expression(CEIL,yyvsp[-1].ep,NULL,NULL);;
    break;}
case 97:
#line 522 "vpp.yacc"
{yyval.ep = create_expression(FLOOR,yyvsp[-1].ep,NULL,NULL);;
    break;}
case 98:
#line 523 "vpp.yacc"
{yyval.ep = create_expression(EVEN,yyvsp[-1].ep,NULL,NULL);;
    break;}
case 99:
#line 524 "vpp.yacc"
{yyval.ep = create_expression(ODD,yyvsp[-1].ep,NULL,NULL);;
    break;}
case 100:
#line 525 "vpp.yacc"
{yyval.ep = create_expression(MAX,yyvsp[-3].ep,yyvsp[-1].ep,NULL);;
    break;}
case 101:
#line 526 "vpp.yacc"
{yyval.ep = create_expression(MIN,yyvsp[-3].ep,yyvsp[-1].ep,NULL);;
    break;}
case 102:
#line 527 "vpp.yacc"
{yyval.ep = create_expression(ABS,yyvsp[-1].ep,NULL,NULL);;
    break;}
case 103:
#line 528 "vpp.yacc"
{yyval.ep = create_expression(STOI,yyvsp[-1].ep,NULL,NULL);;
    break;}
case 104:
#line 529 "vpp.yacc"
{yyval.ep = create_expression(ITOS,yyvsp[-3].ep,yyvsp[-1].ep,NULL);;
    break;}
case 105:
#line 530 "vpp.yacc"
{yyval.ep = create_expression(SUBSTR,yyvsp[-5].ep,yyvsp[-3].ep,yyvsp[-1].ep);;
    break;}
case 106:
#line 531 "vpp.yacc"
{yyval.ep = create_expression(SYSTEM,yyvsp[-1].ep,NULL,NULL);;
    break;}
case 107:
#line 535 "vpp.yacc"
{yyval.ep = create_primary(NMBR,ttid,NULL,NULL,NULL);;
    break;}
case 108:
#line 537 "vpp.yacc"
{yyval.ep = create_primary(REAL,ttid,NULL,NULL,NULL);;
    break;}
case 109:
#line 539 "vpp.yacc"
{yyval.ep = create_primary(STRING,ttid,NULL,NULL,NULL);;
    break;}
case 110:
#line 540 "vpp.yacc"
{
	    	  yyval.ep = create_primary(QUOTED_STRING,ttid,NULL,NULL,NULL);
	       ;
    break;}
case 111:
#line 543 "vpp.yacc"
{yyval.ep = yyvsp[-1].ep;;
    break;}
case 112:
#line 546 "vpp.yacc"
{yyval.cp = (char *) strdup(ttid);;
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
#line 554 "vpp.yacc"


#include "lex.yy.c"


