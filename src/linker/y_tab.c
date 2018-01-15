
/*  A Bison parser, made from vinst.y
    by GNU Bison version 1.28  */

#define YYBISON 1  /* Identify Bison output.  */

#define	Identifier	257
#define	Name	258
#define	Real_num	259
#define	Decimal_num	260
#define	V_decimal_num	261
#define	C_decimal_num	262
#define	V_hex_num	263
#define	C_hex_num	264
#define	V_octal_num	265
#define	C_octal_num	266
#define	V_bin_num	267
#define	C_bin_num	268
#define	String_lit	269
#define	DebugInfo	270
#define	EOL	271
#define	VI_Qeqeq	272
#define	VI_abstract	273
#define	VI_aspect	274
#define	VI_advice	275
#define	VI_advice_end	276
#define	VI_afunc	277
#define	VI_alloc	278
#define	VI_alloc_func	279
#define	VI_alloc_static	280
#define	VI_alu	281
#define	VI_and	282
#define	VI_arg	283
#define	VI_array	284
#define	VI_assoc	285
#define	VI_bit	286
#define	VI_bothedge	287
#define	VI_breakpoint	288
#define	VI_call	289
#define	VI_call_m	290
#define	VI_call_a	291
#define	VI_call_advice	292
#define	VI_cancel	293
#define	VI_chk_overwrite	294
#define	VI_class	295
#define	VI_class_alloc	296
#define	VI_class_alloc_end	297
#define	VI_class_initial_begin	298
#define	VI_class_initial_end	299
#define	VI_class_type	300
#define	VI_clk_of_port	301
#define	VI_clk_of_pset_port	302
#define	VI_clk_of_signal	303
#define	VI_clone	304
#define	VI_cmp	305
#define	VI_cmpx	306
#define	VI_cmpz	307
#define	VI_concat	308
#define	VI_connection	309
#define	VI_copy	310
#define	VI_copy_obj	311
#define	VI_convert	312
#define	VI_dec	313
#define	VI_depth	314
#define	VI_div	315
#define	VI_double	316
#define	VI_drive	317
#define	VI_dup	318
#define	VI_duplicate	319
#define	VI_enter_advice	320
#define	VI_enum	321
#define	VI_eor	322
#define	VI_eqQeq	323
#define	VI_eqeq	324
#define	VI_eqeqQ	325
#define	VI_eqeqeq	326
#define	VI_eqCeq	327
#define	VI_Qne	328
#define	VI_error	329
#define	VI_exec	330
#define	VI_exit	331
#define	VI_export	332
#define	VI_extends	333
#define	VI_extern	334
#define	VI_final	335
#define	VI_float	336
#define	VI_fork	337
#define	VI_formal	338
#define	VI_func	339
#define	VI_function	340
#define	VI_func_end	341
#define	VI_ge	342
#define	VI_gen_child	343
#define	VI_gen_frame	344
#define	VI_gen_m_frame	345
#define	VI_gen_a_frame	346
#define	VI_get_this	347
#define	VI_get_sig_subbit	348
#define	VI_get_subbit	349
#define	VI_get_super	350
#define	VI_get_child	351
#define	VI_get_return	352
#define	VI_global	353
#define	VI_gt	354
#define	VI_i_endian	355
#define	VI_inc	356
#define	VI_input	357
#define	VI_inout	358
#define	VI_initial_begin	359
#define	VI_initial_end	360
#define	VI_initial_init	361
#define	VI_int	362
#define	VI_jedacmp	363
#define	VI_jmp	364
#define	VI_jnz	365
#define	VI_join	366
#define	VI_jz	367
#define	VI_knife	368
#define	VI_le	369
#define	VI_leaf	370
#define	VI_list	371
#define	VI_load_const	372
#define	VI_load_one	373
#define	VI_load_port	374
#define	VI_load_port_ptr	375
#define	VI_load_pset_port	376
#define	VI_load_pset_port_ptr	377
#define	VI_load_sig_pt	378
#define	VI_load_static	379
#define	VI_load_super	380
#define	VI_load_this	381
#define	VI_load_thread	382
#define	VI_load_zero	383
#define	VI_loadg	384
#define	VI_loadl	385
#define	VI_loadm	386
#define	VI_local	387
#define	VI_long	388
#define	VI_lshift	389
#define	VI_lt	390
#define	VI_lvar	391
#define	VI_marray	392
#define	VI_member_signal	393
#define	VI_mfunc	394
#define	VI_minus	395
#define	VI_mod	396
#define	VI_nand	397
#define	VI_ne	398
#define	VI_negedge	399
#define	VI_neor	400
#define	VI_neqeq	401
#define	VI_new	402
#define	VI_new_obj	403
#define	VI_noedge	404
#define	VI_nor	405
#define	VI_normal	406
#define	VI_null	407
#define	VI_nz_lsb	408
#define	VI_obj	409
#define	VI_or	410
#define	VI_output	411
#define	VI_overwrite	412
#define	VI_p_and	413
#define	VI_p_or	414
#define	VI_path	415
#define	VI_plus	416
#define	VI_pop	417
#define	VI_pop_this	418
#define	VI_port	419
#define	VI_portset	420
#define	VI_posedge	421
#define	VI_protected	422
#define	VI_pset_port	423
#define	VI_ptset	424
#define	VI_push	425
#define	VI_restore_acc	426
#define	VI_return	427
#define	VI_return_advice	428
#define	VI_return_l	429
#define	VI_return_n	430
#define	VI_rshift	431
#define	VI_sample	432
#define	VI_save_acc	433
#define	VI_signal	434
#define	VI_single	435
#define	VI_skew	436
#define	VI_spoon	437
#define	VI_stat_var	438
#define	VI_static	439
#define	VI_static_init	440
#define	VI_static_init_end	441
#define	VI_store_adjust	442
#define	VI_store_obj	443
#define	VI_store_port	444
#define	VI_store_pset_port	445
#define	VI_store_sig_g	446
#define	VI_store_sig_l	447
#define	VI_store_sig_m	448
#define	VI_store_sig_static	449
#define	VI_store_static	450
#define	VI_storeg	451
#define	VI_storel	452
#define	VI_storem	453
#define	VI_stridx	454
#define	VI_string	455
#define	VI_strong	456
#define	VI_subbit	457
#define	VI_sync	458
#define	VI_sys_class	459
#define	VI_terminate	460
#define	VI_times	461
#define	VI_to_int	462
#define	VI_u_and	463
#define	VI_u_eor	464
#define	VI_u_minus	465
#define	VI_u_nand	466
#define	VI_u_neor	467
#define	VI_u_nor	468
#define	VI_u_not	469
#define	VI_u_or	470
#define	VI_u_tilde	471
#define	VI_urshift	472
#define	VI_var	473
#define	VI_variable	474
#define	VI_veri_call	475
#define	VI_verilog_task	476
#define	VI_void	477
#define	VI_weak	478



/* 
   
   Copyright (C) 1999, 2000, 2001 Juniper Networks Inc.
   Copyright (C) 2002, 2003 Jeda Technologies, Inc.

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

#include "../include/compiler_macro.h"
#include "../include/link_error_msg.h"
#include "../include/linker.h"

extern char *input_file_name ;

extern int error_count ;
extern int error_flag ;

extern int parse_sys_utils ;

extern unsigned lex_line_num ;

static char *filename ;
static int linenum ;

/* counting variables to allocate global space
   index 0 is prealocated for CLOCK port
 */
int num_global_var = 1 ;

int yyerrflag ;

int inst_num = 0 ;

vcode_info *Current_Class = NULL ;


vcode_info  *JD_top_inst ;
vcode_info  *JD_first_inst = NULL ;

int main_defined = 0 ;



typedef union {
  int  integer_value ;
  char *string ;
  struct {
    char *name ;
    int  linenum ;
    char *filename ;
    int  sys_flag ;
  } string_value ;
  int keyword ;
  vcode_info *inst ;
  vinst_info *code ;
  expression_node *exp ;
  JD_name *name ;
  /* instruction_node  *inst ; */
} YYSTYPE;
#include <stdio.h>

#ifndef __cplusplus
#ifndef __STDC__
#define const
#endif
#endif



#define	YYFINAL		997
#define	YYFLAG		-32768
#define	YYNTBASE	240

#define YYTRANSLATE(x) ((unsigned)(x) <= 478 ? yytranslate[x] : 323)

static const short yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,   234,
   235,   236,   238,   239,   230,   229,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,   226,     2,     2,
   233,     2,   237,   228,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
   225,     2,   227,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,   231,     2,   232,     2,     2,     2,     2,     2,
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
    57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
    67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
    77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
    87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
    97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
   107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
   117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
   127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
   137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
   147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
   157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
   167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
   177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
   187,   188,   189,   190,   191,   192,   193,   194,   195,   196,
   197,   198,   199,   200,   201,   202,   203,   204,   205,   206,
   207,   208,   209,   210,   211,   212,   213,   214,   215,   216,
   217,   218,   219,   220,   221,   222,   223,   224
};

#if YYDEBUG != 0
static const short yyprhs[] = {     0,
     0,     2,     5,     8,    13,    15,    18,    20,    22,    24,
    26,    28,    30,    32,    34,    36,    38,    40,    42,    44,
    46,    48,    50,    52,    54,    56,    58,    60,    63,    74,
    76,    78,    80,    81,    83,    85,    88,    93,    98,   101,
   104,   107,   112,   114,   117,   125,   127,   130,   137,   145,
   153,   155,   157,   159,   161,   163,   170,   173,   176,   178,
   180,   182,   185,   187,   190,   196,   198,   200,   202,   204,
   207,   208,   210,   212,   215,   217,   219,   221,   223,   225,
   227,   229,   234,   236,   239,   252,   253,   255,   257,   260,
   262,   264,   266,   268,   270,   272,   274,   276,   277,   279,
   281,   284,   290,   294,   301,   304,   311,   317,   318,   320,
   322,   325,   331,   337,   343,   345,   348,   350,   352,   367,
   368,   381,   382,   393,   402,   403,   406,   407,   409,   411,
   414,   416,   418,   420,   422,   423,   425,   427,   430,   432,
   434,   435,   437,   439,   442,   444,   446,   448,   457,   463,
   469,   476,   483,   491,   498,   506,   514,   515,   517,   519,
   522,   524,   526,   528,   532,   535,   543,   554,   557,   558,
   563,   566,   569,   577,   587,   597,   600,   603,   610,   615,
   620,   624,   628,   632,   636,   640,   644,   648,   652,   656,
   660,   664,   668,   672,   676,   680,   684,   688,   692,   696,
   700,   704,   708,   712,   716,   720,   724,   728,   732,   736,
   740,   744,   748,   752,   756,   760,   764,   768,   772,   776,
   780,   784,   787,   790,   795,   799,   803,   807,   810,   815,
   820,   824,   828,   833,   837,   841,   845,   849,   853,   858,
   861,   864,   867,   870,   874,   879,   884,   887,   890,   893,
   898,   904,   910,   915,   918,   922,   925,   928,   932,   935,
   938,   942,   946,   949,   953,   960,   966,   971,   978,   984,
   989,   996,  1002,  1010,  1014,  1019,  1023,  1028,  1033,  1039,
  1044,  1050,  1053,  1057,  1060,  1067,  1074,  1083,  1090,  1099,
  1107,  1117,  1123,  1131,  1138,  1147,  1155,  1163,  1172,  1182,
  1192,  1203,  1211,  1220,  1230,  1241,  1247,  1251,  1255,  1260,
  1264,  1268,  1272,  1275,  1278,  1281,  1284,  1286,  1288,  1291,
  1294,  1295,  1299,  1303,  1309,  1317,  1318,  1320,  1322,  1325,
  1328,  1330,  1332,  1334,  1336,  1338,  1340,  1342,  1344,  1346,
  1348,  1350,  1352,  1354,  1356,  1358,  1360,  1362,  1366,  1368,
  1370,  1372,  1374,  1376,  1377,  1380,  1386,  1388,  1390,  1392,
  1393,  1395,  1396,  1403,  1410,  1412,  1414,  1416,  1418,  1420,
  1422,  1424,  1426,  1428,  1430,  1432,  1434,  1436
};

static const short yyrhs[] = {   241,
     0,   240,   241,     0,   242,   243,     0,   109,     6,     6,
    17,     0,   244,     0,   243,   244,     0,   245,     0,   246,
     0,   247,     0,   253,     0,   255,     0,   256,     0,   263,
     0,   265,     0,   278,     0,   280,     0,   282,     0,   293,
     0,   294,     0,   295,     0,   296,     0,   297,     0,   300,
     0,   301,     0,   298,     0,   299,     0,    17,     0,    16,
    17,     0,   165,   248,   225,     6,   226,     6,   227,   322,
   249,    17,     0,   103,     0,   157,     0,   104,     0,     0,
   250,     0,   251,     0,   250,   251,     0,   178,   316,   182,
   252,     0,    63,   316,   182,   252,     0,    60,     6,     0,
   161,    15,     0,   228,   322,     0,   228,   322,   229,   322,
     0,     6,     0,   230,     6,     0,   166,   322,   231,    17,
   254,   232,    17,     0,   247,     0,   254,   247,     0,    55,
   165,   322,   233,    15,    17,     0,    55,   166,   322,   322,
   233,    15,    17,     0,   220,     6,   257,   322,   258,   260,
    17,     0,   223,     0,   108,     0,   134,     0,    82,     0,
    62,     0,    32,   225,     6,   226,     6,   227,     0,    41,
   322,     0,   205,   322,     0,   201,     0,   180,     0,    46,
     0,    67,   322,     0,   181,     0,    30,     6,     0,   138,
     6,   234,   259,   235,     0,   200,     0,    31,     0,   117,
     0,     6,     0,   259,     6,     0,     0,   261,     0,   262,
     0,   261,   262,     0,   185,     0,    50,     0,    80,     0,
   133,     0,    99,     0,   168,     0,    81,     0,    67,   322,
   264,    17,     0,   322,     0,   264,   322,     0,    86,     6,
   266,   257,   322,     6,    15,   234,    17,   269,   235,    17,
     0,     0,   267,     0,   268,     0,   267,   268,     0,    19,
     0,   133,     0,   168,     0,    81,     0,    78,     0,   116,
     0,   158,     0,    80,     0,     0,   270,     0,   271,     0,
   270,   271,     0,   272,   257,   322,   258,    17,     0,    84,
   257,    17,     0,   234,    17,   271,   235,   236,    17,     0,
   237,    17,     0,   234,    17,   271,   235,   238,    17,     0,
   225,    17,   271,   227,    17,     0,     0,   219,     0,   274,
     0,   273,   274,     0,    35,   234,   275,   235,    17,     0,
   173,   234,   275,   235,    17,     0,   158,   234,   275,   235,
    17,     0,   276,     0,   275,   276,     0,   322,     0,    15,
     0,    21,     6,   266,   257,   322,     6,    15,    17,   273,
   234,    17,   269,   235,    17,     0,     0,    41,   322,   283,
   284,     6,    15,   231,    17,   279,   287,   232,    17,     0,
     0,    20,   322,     6,    15,   231,    17,   281,   290,   232,
    17,     0,   222,     4,   234,    17,   269,   235,    15,    17,
     0,     0,    79,   322,     0,     0,   285,     0,   286,     0,
   285,   286,     0,    19,     0,   133,     0,    81,     0,    80,
     0,     0,   288,     0,   289,     0,   288,   289,     0,   256,
     0,   265,     0,     0,   291,     0,   292,     0,   291,   292,
     0,   256,     0,   265,     0,   277,     0,   184,   322,   322,
   257,   322,   258,   260,    17,     0,   186,    17,   302,   187,
    17,     0,   105,    17,   302,   106,    17,     0,    44,   322,
    17,   302,    45,    17,     0,    42,   322,    17,   302,    43,
    17,     0,   140,   322,   322,    17,   302,    87,    17,     0,
    85,   322,    17,   302,    87,    17,     0,    23,   322,   322,
    17,   302,    87,    17,     0,    21,   322,   322,    17,   302,
    22,    17,     0,     0,   303,     0,   304,     0,   303,   304,
     0,   263,     0,   245,     0,   246,     0,     3,   226,    17,
     0,   153,    17,     0,   137,     6,   257,   322,   258,   312,
    17,     0,   137,   185,     6,   257,   322,   322,   322,   258,
   312,    17,     0,   148,    17,     0,     0,   149,   322,    17,
   305,     0,    57,    17,     0,    89,    17,     0,    24,     6,
   257,   322,   258,   312,    17,     0,    26,   322,   322,     6,
   257,   322,   258,   312,    17,     0,   272,    29,     6,     6,
   257,   322,   258,   312,    17,     0,   223,    17,     0,   208,
    17,     0,    58,    32,     6,     6,   309,    17,     0,    58,
   201,   309,    17,     0,    58,    82,   309,    17,     0,   102,
   309,    17,     0,    59,   309,    17,     0,   141,   310,    17,
     0,   162,   310,    17,     0,   207,   310,    17,     0,    61,
   310,    17,     0,   142,   310,    17,     0,    28,   310,    17,
     0,   156,   310,    17,     0,    68,   310,    17,     0,   143,
   310,    17,     0,   151,   310,    17,     0,   146,   310,    17,
     0,   177,   309,    17,     0,   218,   309,    17,     0,   135,
   309,    17,     0,   136,   310,    17,     0,   100,   310,    17,
     0,    70,   310,    17,     0,   115,   310,    17,     0,    88,
   310,    17,     0,   144,   310,    17,     0,    72,   310,    17,
     0,   147,   310,    17,     0,    69,   310,    17,     0,    71,
   310,    17,     0,    18,   310,    17,     0,    74,   310,    17,
     0,    73,   311,    17,     0,    51,   310,    17,     0,    53,
   310,    17,     0,    52,   310,    17,     0,   211,   309,    17,
     0,   217,   309,    17,     0,   215,   309,    17,     0,   209,
   309,    17,     0,   216,   309,    17,     0,   210,   309,    17,
     0,   212,   309,    17,     0,   214,   309,    17,     0,   213,
   309,    17,     0,   129,    17,     0,   119,    17,     0,   118,
   309,   314,    17,     0,   127,   272,    17,     0,   126,   272,
    17,     0,   128,   315,    17,     0,    93,    17,     0,    96,
   272,     6,    17,     0,    97,   272,     6,    17,     0,    54,
   310,    17,     0,    65,   309,    17,     0,   117,   309,     6,
    17,     0,    95,   319,    17,     0,   163,     6,    17,     0,
    64,     6,    17,     0,   163,    27,    17,     0,   171,    27,
    17,     0,    56,    27,     6,    17,     0,    90,    17,     0,
    91,    17,     0,    92,    17,     0,   164,    17,     0,   110,
   315,    17,     0,   113,   309,   315,    17,     0,   111,   309,
   315,    17,     0,   175,    17,     0,   173,    17,     0,   176,
    17,     0,    35,   322,     6,    17,     0,    36,   322,   322,
     6,    17,     0,    37,   322,   322,     6,    17,     0,   221,
   322,     6,    17,     0,    83,    17,     0,    76,   315,    17,
     0,   112,    17,     0,   183,    17,     0,   183,    39,    17,
     0,   114,    17,     0,    77,    17,     0,    77,   159,    17,
     0,    77,   160,    17,     0,   206,    17,     0,    34,    15,
    17,     0,   204,    48,   316,   322,   322,    17,     0,   204,
    47,   316,   322,    17,     0,   204,    49,   316,    17,     0,
   204,   169,   316,   322,   322,    17,     0,   204,   165,   316,
   322,    17,     0,   204,   180,   316,    17,     0,   131,   317,
   318,   272,     6,    17,     0,   130,   317,   272,   322,    17,
     0,   132,   317,   318,   272,   322,   322,    17,     0,   120,
   322,    17,     0,   122,   322,   322,    17,     0,   121,   322,
    17,     0,   123,   322,   322,    17,     0,   120,   203,   322,
    17,     0,   122,   203,   322,   322,    17,     0,   121,   203,
   322,    17,     0,   123,   203,   322,   322,    17,     0,   124,
    17,     0,   124,   203,    17,     0,    94,    17,     0,   125,
   317,   322,   322,   322,    17,     0,   198,   317,   318,   310,
     6,    17,     0,   198,   317,   318,   203,   319,   310,     6,
    17,     0,   197,   317,   318,   310,   322,    17,     0,   197,
   317,   318,   203,   319,   310,   322,    17,     0,   199,   317,
   318,   310,   322,   322,    17,     0,   199,   317,   318,   203,
   319,   310,   322,   322,    17,     0,   190,   310,   321,   322,
    17,     0,   190,   203,   319,   310,   321,   322,    17,     0,
   191,   310,   321,   322,   322,    17,     0,   191,   203,   319,
   310,   321,   322,   322,    17,     0,   193,   317,   318,   310,
   321,     6,    17,     0,   192,   317,   318,   310,   321,   322,
    17,     0,   194,   317,   318,   310,   321,   322,   322,    17,
     0,   193,   317,   318,   203,   319,   310,   321,     6,    17,
     0,   192,   317,   318,   203,   319,   310,   321,   322,    17,
     0,   194,   317,   318,   203,   319,   310,   321,   322,   322,
    17,     0,   196,   317,   310,   322,   322,   322,    17,     0,
   195,   317,   310,   321,   322,   322,   322,    17,     0,   196,
   317,   203,   319,   310,   322,   322,   322,    17,     0,   195,
   317,   203,   319,   310,   321,   322,   322,   322,    17,     0,
   189,   310,   322,   322,    17,     0,   188,   309,    17,     0,
    25,     6,   322,     0,   158,   322,   322,   322,     0,    40,
     6,   322,     0,    38,   306,    17,     0,   174,   306,    17,
     0,   179,    17,     0,   172,    17,     0,    66,    17,     0,
    98,    17,     0,    75,     0,   307,     0,   306,   307,     0,
   322,   308,     0,     0,   234,   322,   235,     0,   234,   313,
   235,     0,   234,   313,   239,   313,   235,     0,   234,   313,
   239,   313,   239,   313,   235,     0,     0,   185,     0,    50,
     0,   185,    50,     0,    50,   185,     0,   201,     0,   223,
     0,   153,     0,   108,     0,    32,     0,    82,     0,   170,
     0,   180,     0,    67,     0,   155,     0,    30,     0,   138,
     0,    31,     0,   200,     0,   117,     0,   320,     0,    15,
     0,   322,   322,     6,     0,     3,     0,   167,     0,   145,
     0,    33,     0,   150,     0,     0,    30,     6,     0,   138,
     6,   234,   259,   235,     0,   200,     0,    31,     0,   117,
     0,     0,   185,     0,     0,   101,   225,     6,   226,     6,
   227,     0,   154,   225,     6,   226,     6,   227,     0,     5,
     0,     6,     0,     7,     0,     8,     0,     9,     0,    10,
     0,    11,     0,    12,     0,    13,     0,    14,     0,   152,
     0,   224,     0,   202,     0,     4,     0
};

#endif

#if YYDEBUG != 0
static const short yyrline[] = { 0,
   355,   361,   375,   383,   401,   406,   428,   433,   438,   461,
   492,   497,   532,   564,   595,   635,   667,   693,   698,   703,
   708,   713,   718,   723,   728,   733,   740,   747,   980,   994,
   999,  1004,  1011,  1016,  1023,  1028,  1050,  1060,  1070,  1078,
  1086,  1094,  1106,  1112,  1120,  1133,  1138,  1161,  1170,  1182,
  1197,  1204,  1211,  1218,  1225,  1232,  1241,  1250,  1259,  1266,
  1273,  1280,  1291,  1298,  1306,  1315,  1322,  1329,  1338,  1346,
  1360,  1365,  1372,  1414,  1458,  1463,  1468,  1473,  1478,  1483,
  1488,  1495,  1506,  1514,  1528,  1550,  1556,  1563,  1605,  1649,
  1654,  1659,  1664,  1669,  1674,  1679,  1684,  1691,  1696,  1703,
  1708,  1730,  1740,  1746,  1754,  1760,  1768,  1778,  1783,  1790,
  1795,  1817,  1825,  1833,  1844,  1849,  1864,  1873,  1884,  1908,
  1923,  1932,  1944,  1953,  1967,  1972,  1980,  1985,  1992,  2034,
  2078,  2083,  2088,  2093,  2100,  2105,  2112,  2117,  2139,  2144,
  2151,  2156,  2163,  2168,  2190,  2195,  2200,  2207,  2255,  2268,
  2281,  2295,  2328,  2351,  2372,  2395,  2423,  2428,  2435,  2444,
  2464,  2472,  2477,  2485,  2495,  2504,  2518,  2550,  2559,  2569,
  2577,  2586,  2595,  2609,  2641,  2658,  2667,  2676,  2689,  2701,
  2713,  2723,  2733,  2743,  2753,  2763,  2773,  2783,  2793,  2803,
  2813,  2823,  2833,  2843,  2853,  2863,  2873,  2883,  2893,  2903,
  2913,  2923,  2933,  2943,  2953,  2963,  2973,  2983,  2993,  3003,
  3013,  3023,  3033,  3043,  3053,  3063,  3073,  3083,  3093,  3103,
  3113,  3123,  3132,  3141,  3152,  3162,  3172,  3182,  3191,  3202,
  3213,  3223,  3233,  3245,  3255,  3266,  3277,  3287,  3297,  3308,
  3317,  3326,  3335,  3344,  3354,  3365,  3376,  3385,  3394,  3403,
  3425,  3460,  3490,  3509,  3518,  3528,  3537,  3546,  3556,  3565,
  3574,  3584,  3594,  3603,  3614,  3637,  3655,  3666,  3689,  3707,
  3718,  3731,  3752,  3778,  3794,  3815,  3831,  3852,  3869,  3891,
  3908,  3930,  3939,  3949,  3958,  3988,  4001,  4017,  4039,  4063,
  4088,  4116,  4134,  4154,  4177,  4202,  4216,  4239,  4265,  4282,
  4308,  4337,  4368,  4400,  4434,  4469,  4492,  4502,  4513,  4548,
  4559,  4569,  4579,  4588,  4597,  4606,  4615,  4626,  4631,  4647,
  4657,  4662,  4671,  4680,  4690,  4701,  4706,  4713,  4720,  4728,
  4738,  4743,  4748,  4753,  4758,  4763,  4768,  4773,  4778,  4783,
  4788,  4793,  4798,  4803,  4808,  4815,  4820,  4828,  4840,  4847,
  4852,  4857,  4862,  4870,  4875,  4883,  4892,  4899,  4906,  4915,
  4920,  4927,  4932,  4941,  4952,  4961,  4970,  4978,  4986,  4994,
  5002,  5010,  5018,  5026,  5036,  5041,  5046,  5053
};
#endif


#if YYDEBUG != 0 || defined (YYERROR_VERBOSE)

static const char * const yytname[] = {   "$","error","$undefined.","Identifier",
"Name","Real_num","Decimal_num","V_decimal_num","C_decimal_num","V_hex_num",
"C_hex_num","V_octal_num","C_octal_num","V_bin_num","C_bin_num","String_lit",
"DebugInfo","EOL","VI_Qeqeq","VI_abstract","VI_aspect","VI_advice","VI_advice_end",
"VI_afunc","VI_alloc","VI_alloc_func","VI_alloc_static","VI_alu","VI_and","VI_arg",
"VI_array","VI_assoc","VI_bit","VI_bothedge","VI_breakpoint","VI_call","VI_call_m",
"VI_call_a","VI_call_advice","VI_cancel","VI_chk_overwrite","VI_class","VI_class_alloc",
"VI_class_alloc_end","VI_class_initial_begin","VI_class_initial_end","VI_class_type",
"VI_clk_of_port","VI_clk_of_pset_port","VI_clk_of_signal","VI_clone","VI_cmp",
"VI_cmpx","VI_cmpz","VI_concat","VI_connection","VI_copy","VI_copy_obj","VI_convert",
"VI_dec","VI_depth","VI_div","VI_double","VI_drive","VI_dup","VI_duplicate",
"VI_enter_advice","VI_enum","VI_eor","VI_eqQeq","VI_eqeq","VI_eqeqQ","VI_eqeqeq",
"VI_eqCeq","VI_Qne","VI_error","VI_exec","VI_exit","VI_export","VI_extends",
"VI_extern","VI_final","VI_float","VI_fork","VI_formal","VI_func","VI_function",
"VI_func_end","VI_ge","VI_gen_child","VI_gen_frame","VI_gen_m_frame","VI_gen_a_frame",
"VI_get_this","VI_get_sig_subbit","VI_get_subbit","VI_get_super","VI_get_child",
"VI_get_return","VI_global","VI_gt","VI_i_endian","VI_inc","VI_input","VI_inout",
"VI_initial_begin","VI_initial_end","VI_initial_init","VI_int","VI_jedacmp",
"VI_jmp","VI_jnz","VI_join","VI_jz","VI_knife","VI_le","VI_leaf","VI_list","VI_load_const",
"VI_load_one","VI_load_port","VI_load_port_ptr","VI_load_pset_port","VI_load_pset_port_ptr",
"VI_load_sig_pt","VI_load_static","VI_load_super","VI_load_this","VI_load_thread",
"VI_load_zero","VI_loadg","VI_loadl","VI_loadm","VI_local","VI_long","VI_lshift",
"VI_lt","VI_lvar","VI_marray","VI_member_signal","VI_mfunc","VI_minus","VI_mod",
"VI_nand","VI_ne","VI_negedge","VI_neor","VI_neqeq","VI_new","VI_new_obj","VI_noedge",
"VI_nor","VI_normal","VI_null","VI_nz_lsb","VI_obj","VI_or","VI_output","VI_overwrite",
"VI_p_and","VI_p_or","VI_path","VI_plus","VI_pop","VI_pop_this","VI_port","VI_portset",
"VI_posedge","VI_protected","VI_pset_port","VI_ptset","VI_push","VI_restore_acc",
"VI_return","VI_return_advice","VI_return_l","VI_return_n","VI_rshift","VI_sample",
"VI_save_acc","VI_signal","VI_single","VI_skew","VI_spoon","VI_stat_var","VI_static",
"VI_static_init","VI_static_init_end","VI_store_adjust","VI_store_obj","VI_store_port",
"VI_store_pset_port","VI_store_sig_g","VI_store_sig_l","VI_store_sig_m","VI_store_sig_static",
"VI_store_static","VI_storeg","VI_storel","VI_storem","VI_stridx","VI_string",
"VI_strong","VI_subbit","VI_sync","VI_sys_class","VI_terminate","VI_times","VI_to_int",
"VI_u_and","VI_u_eor","VI_u_minus","VI_u_nand","VI_u_neor","VI_u_nor","VI_u_not",
"VI_u_or","VI_u_tilde","VI_urshift","VI_var","VI_variable","VI_veri_call","VI_verilog_task",
"VI_void","VI_weak","'['","':'","']'","'@'","'.'","'-'","'{'","'}'","'='","'('",
"')'","'*'","'?'","'+'","','","object_files","virtual_code","inital_header",
"vinst_units","vinst_unit","empty_line","debug_information","port_info","direction",
"opt_port_attrib","port_attribs","port_attrib","int_value","portset_info","port_infos",
"connection_info","variable_dcl","data_type","array_range","range_list","opt_var_attribs",
"var_attribs","var_attrib","enum_dcl","enum_members","function_info","opt_func_options",
"func_options","func_option","opt_arguments","arguments","argument","opt_var",
"pointcuts","pointcut","pointcut_names","pointcut_name","advice_info","class_info",
"@1","aspect_info","@2","verilog_task_info","opt_extention","opt_class_attribs",
"class_attribs","class_attrib","opt_class_members","class_members","class_member",
"opt_aspect_members","aspect_members","aspect_member","static_var_dcl","static_init_code",
"init_code","class_init_code","class_alloc_code","mfunc_code","func_code","afunc_code",
"advice_code","opt_vinsts","vinsts","vinst","@3","pointcut_specs","pointcut_spec",
"opt_class_pointcut","stype","dtype","ttype","opt_flags","type_code","const_data",
"label","edge","opt_array_access","opt_static","opt_bit_type","number","strength",
"name", NULL
};
#endif

static const short yyr1[] = {     0,
   240,   240,   241,   242,   243,   243,   244,   244,   244,   244,
   244,   244,   244,   244,   244,   244,   244,   244,   244,   244,
   244,   244,   244,   244,   244,   244,   245,   246,   247,   248,
   248,   248,   249,   249,   250,   250,   251,   251,   251,   251,
   251,   251,   252,   252,   253,   254,   254,   255,   255,   256,
   257,   257,   257,   257,   257,   257,   257,   257,   257,   257,
   257,   257,   258,   258,   258,   258,   258,   258,   259,   259,
   260,   260,   261,   261,   262,   262,   262,   262,   262,   262,
   262,   263,   264,   264,   265,   266,   266,   267,   267,   268,
   268,   268,   268,   268,   268,   268,   268,   269,   269,   270,
   270,   271,   271,   271,   271,   271,   271,   272,   272,   273,
   273,   274,   274,   274,   275,   275,   276,   276,   277,   279,
   278,   281,   280,   282,   283,   283,   284,   284,   285,   285,
   286,   286,   286,   286,   287,   287,   288,   288,   289,   289,
   290,   290,   291,   291,   292,   292,   292,   293,   294,   295,
   296,   297,   298,   299,   300,   301,   302,   302,   303,   303,
   304,   304,   304,   304,   304,   304,   304,   304,   305,   304,
   304,   304,   304,   304,   304,   304,   304,   304,   304,   304,
   304,   304,   304,   304,   304,   304,   304,   304,   304,   304,
   304,   304,   304,   304,   304,   304,   304,   304,   304,   304,
   304,   304,   304,   304,   304,   304,   304,   304,   304,   304,
   304,   304,   304,   304,   304,   304,   304,   304,   304,   304,
   304,   304,   304,   304,   304,   304,   304,   304,   304,   304,
   304,   304,   304,   304,   304,   304,   304,   304,   304,   304,
   304,   304,   304,   304,   304,   304,   304,   304,   304,   304,
   304,   304,   304,   304,   304,   304,   304,   304,   304,   304,
   304,   304,   304,   304,   304,   304,   304,   304,   304,   304,
   304,   304,   304,   304,   304,   304,   304,   304,   304,   304,
   304,   304,   304,   304,   304,   304,   304,   304,   304,   304,
   304,   304,   304,   304,   304,   304,   304,   304,   304,   304,
   304,   304,   304,   304,   304,   304,   304,   304,   304,   304,
   304,   304,   304,   304,   304,   304,   304,   306,   306,   307,
   308,   308,   309,   310,   311,   312,   312,   312,   312,   312,
   313,   313,   313,   313,   313,   313,   313,   313,   313,   313,
   313,   313,   313,   313,   313,   314,   314,   314,   315,   316,
   316,   316,   316,   317,   317,   317,   317,   317,   317,   318,
   318,   319,   319,   319,   320,   320,   320,   320,   320,   320,
   320,   320,   320,   320,   321,   321,   321,   322
};

static const short yyr2[] = {     0,
     1,     2,     2,     4,     1,     2,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     2,    10,     1,
     1,     1,     0,     1,     1,     2,     4,     4,     2,     2,
     2,     4,     1,     2,     7,     1,     2,     6,     7,     7,
     1,     1,     1,     1,     1,     6,     2,     2,     1,     1,
     1,     2,     1,     2,     5,     1,     1,     1,     1,     2,
     0,     1,     1,     2,     1,     1,     1,     1,     1,     1,
     1,     4,     1,     2,    12,     0,     1,     1,     2,     1,
     1,     1,     1,     1,     1,     1,     1,     0,     1,     1,
     2,     5,     3,     6,     2,     6,     5,     0,     1,     1,
     2,     5,     5,     5,     1,     2,     1,     1,    14,     0,
    12,     0,    10,     8,     0,     2,     0,     1,     1,     2,
     1,     1,     1,     1,     0,     1,     1,     2,     1,     1,
     0,     1,     1,     2,     1,     1,     1,     8,     5,     5,
     6,     6,     7,     6,     7,     7,     0,     1,     1,     2,
     1,     1,     1,     3,     2,     7,    10,     2,     0,     4,
     2,     2,     7,     9,     9,     2,     2,     6,     4,     4,
     3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
     3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
     3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
     3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
     3,     2,     2,     4,     3,     3,     3,     2,     4,     4,
     3,     3,     4,     3,     3,     3,     3,     3,     4,     2,
     2,     2,     2,     3,     4,     4,     2,     2,     2,     4,
     5,     5,     4,     2,     3,     2,     2,     3,     2,     2,
     3,     3,     2,     3,     6,     5,     4,     6,     5,     4,
     6,     5,     7,     3,     4,     3,     4,     4,     5,     4,
     5,     2,     3,     2,     6,     6,     8,     6,     8,     7,
     9,     5,     7,     6,     8,     7,     7,     8,     9,     9,
    10,     7,     8,     9,    10,     5,     3,     3,     4,     3,
     3,     3,     2,     2,     2,     2,     1,     1,     2,     2,
     0,     3,     3,     5,     7,     0,     1,     1,     2,     2,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     3,     1,     1,
     1,     1,     1,     0,     2,     5,     1,     1,     1,     0,
     1,     0,     6,     6,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1
};

static const short yydefact[] = {     0,
     0,     0,     1,     0,     0,     2,     0,    27,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     3,     5,     7,     8,
     9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
    19,    20,    21,    22,    25,    26,    23,    24,     0,    28,
   378,     0,     0,     0,   125,     0,     0,     0,     0,     0,
     0,    86,   108,     0,    30,    32,    31,     0,     0,     0,
   108,     0,     0,     6,     4,     0,     0,     0,     0,   127,
   108,   108,     0,     0,     0,    83,   108,    90,    94,    97,
    93,    95,    91,    96,    92,     0,    87,    88,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,   317,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
   362,   108,   108,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
   354,   108,   108,     0,     0,   354,   354,   354,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,   354,
   354,   354,   354,   354,   354,   354,   354,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,   109,     0,     0,   162,   163,   161,     0,     0,   158,
   159,     0,     0,     0,     0,     0,     0,     0,    61,    55,
     0,    54,    52,    53,    60,    59,     0,    51,     0,     0,
     0,   108,   108,   126,   131,   134,   133,   132,     0,   128,
   129,     0,     0,     0,     0,    82,    84,     0,     0,    89,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,   318,   321,     0,     0,     0,     0,     0,     0,
   171,     0,     0,     0,     0,     0,     0,     0,     0,   315,
     0,     0,     0,     0,     0,     0,     0,     0,   349,     0,
   260,     0,     0,   254,     0,   172,   240,   241,   242,   228,
   284,     0,     0,     0,     0,     0,   316,     0,     0,     0,
     0,   256,     0,   259,     0,     0,     0,   223,     0,     0,
     0,     0,     0,     0,     0,     0,   282,     0,     0,   358,
   359,     0,   357,     0,     0,     0,     0,   222,   108,   360,
   360,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,   168,     0,     0,   165,     0,     0,     0,     0,     0,
   243,     0,   314,   248,     0,   247,   249,     0,   313,   257,
     0,     0,     0,   362,     0,   362,     0,   360,   360,   360,
     0,     0,   360,   360,   360,     0,     0,     0,     0,     0,
     0,   263,     0,   177,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,   176,     0,     0,   160,   108,
     0,     0,     0,     0,     0,    57,    62,    58,     0,   108,
     0,     0,     0,     0,   130,     0,     0,     0,     0,     0,
     0,   164,   341,   343,   335,   339,   336,   334,   345,   342,
   333,   340,   337,   338,   344,   331,   332,     0,   207,     0,
   308,     0,   188,   264,     0,     0,     0,   311,   319,     0,
   320,   310,   210,   212,   211,   231,     0,     0,     0,     0,
     0,   182,   186,   236,   232,   190,   205,   199,   206,   203,
     0,   209,   208,   255,   261,   262,   201,     0,     0,   234,
     0,     0,   198,   181,   244,     0,     0,   200,     0,   365,
   366,   367,   368,   369,   370,   371,   372,   373,   374,   347,
     0,   346,     0,     0,   274,     0,   276,     0,     0,     0,
     0,   283,   355,     0,     0,   226,   225,   227,     0,   361,
   108,   108,   196,   197,     0,     0,   183,   187,   191,   202,
   193,   204,   169,   192,   189,     0,   184,   235,   237,   238,
   312,   194,   258,   307,     0,     0,   375,   377,   376,     0,
     0,     0,     0,     0,     0,   362,     0,   362,     0,     0,
     0,     0,   352,   351,   353,   350,     0,     0,     0,     0,
     0,     0,   185,   216,   218,   213,   219,   221,   220,   215,
   217,   214,   195,     0,     0,   150,     0,     0,    46,     0,
     0,   149,     0,     0,    67,    68,     0,    63,    66,    71,
     0,     0,     0,     0,     0,   108,   100,     0,   122,     0,
     0,     0,   152,   151,    48,     0,   154,     0,     0,     0,
     0,   250,     0,     0,     0,   239,     0,   180,   179,   323,
     0,     0,     0,   229,   230,   246,   245,   233,   224,     0,
   278,   280,     0,   275,     0,   277,     0,     0,     0,     0,
     0,     0,     0,   170,   309,     0,     0,     0,     0,     0,
   362,     0,   362,     0,   362,     0,     0,     0,     0,     0,
   362,     0,   362,     0,   362,     0,     0,     0,   267,     0,
     0,   270,   253,     0,     0,     0,     0,    47,    71,     0,
    64,     0,    76,    77,    81,    79,    78,    80,    75,     0,
    72,    73,     0,   108,   108,   105,     0,   101,     0,   141,
   156,   155,     0,    49,     0,     0,   326,     0,   251,   252,
   322,     0,     0,     0,     0,   348,   279,   281,    69,     0,
     0,   272,     0,     0,   326,     0,   306,     0,   292,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,   266,     0,   269,
     0,     0,   153,     0,    45,     0,     0,     0,    50,    74,
   103,     0,     0,     0,     0,     0,   145,   146,   147,     0,
   142,   143,   120,     0,   324,   328,   327,     0,     0,   178,
     0,     0,     0,    70,   356,   285,   271,     0,     0,     0,
     0,     0,   294,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,   288,     0,   286,     0,     0,   265,
   268,     0,    33,   148,    56,     0,     0,     0,   124,     0,
    86,     0,   144,   135,   108,   330,   329,   173,   326,     0,
   363,   364,   273,   166,     0,   293,     0,     0,   297,     0,
   296,     0,     0,     0,     0,     0,   302,     0,     0,     0,
   290,   326,     0,     0,     0,     0,     0,     0,    34,    35,
    65,   107,     0,     0,   102,     0,   123,   139,   140,     0,
   136,   137,     0,     0,   325,   326,   295,     0,     0,     0,
   298,     0,   303,     0,   289,   287,     0,     0,    39,     0,
    40,     0,    41,    29,    36,   104,   106,     0,     0,   138,
     0,   174,     0,   300,   299,     0,     0,   304,   291,   175,
     0,     0,     0,     0,   121,    85,   167,   301,   305,    43,
     0,    38,    37,    42,     0,    44,     0,     0,     0,     0,
     0,     0,   110,     0,     0,     0,     0,   111,   118,     0,
   115,   117,     0,     0,   108,     0,   116,     0,     0,     0,
   112,   114,   113,     0,   119,     0,     0
};

static const short yydefgoto[] = {     2,
     3,     4,    27,    28,   225,   226,    31,    68,   898,   899,
   900,   962,    32,   620,    33,    34,   249,   630,   760,   730,
   731,   732,   227,    85,    36,    96,    97,    98,   635,   636,
   637,   228,   972,   973,   980,   981,   809,    37,   864,    38,
   740,    39,    80,   259,   260,   261,   910,   911,   912,   810,
   811,   812,    40,    41,    42,    43,    44,    45,    46,    47,
    48,   229,   230,   231,   684,   282,   283,   481,   296,   273,
   307,   818,   468,   531,   310,   597,   354,   551,   324,   532,
   580,   982
};

static const short yypact[] = {     7,
   119,   110,-32768,   428,   163,-32768,   187,-32768,   217,   217,
   217,   217,   217,   217,    84,   217,   217,   225,   228,   217,
    25,   217,   217,   240,   271,   286,   428,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,   297,-32768,
-32768,   296,   217,   217,   248,   314,   316,   217,   217,   217,
   324,   223,   881,   217,-32768,-32768,-32768,   118,   115,   217,
  1098,   146,   121,-32768,-32768,   334,   340,   341,   217,   281,
  1315,  1532,   127,   217,   123,-32768,  1749,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,   146,   223,-32768,   137,   130,
   361,   367,   217,   130,   363,   217,   217,   217,   217,   374,
   130,   130,   130,   130,   355,   368,    69,   154,   130,   381,
   154,   372,   130,   130,   130,   130,   130,   159,   130,-32768,
   391,   109,   382,   130,   386,   390,   392,   393,   394,   395,
    14,   179,   179,   396,   130,   154,   391,   154,   399,   154,
   403,   130,   154,   154,   404,    36,    58,    59,    61,    40,
    82,   179,   179,   391,   405,    82,    82,    82,   154,   130,
    68,   130,   130,   130,   130,   130,   130,   406,   217,   130,
   408,   130,   217,   130,   285,   409,   400,   411,   412,   217,
   413,   414,   154,   416,   278,   154,   130,   -62,   -61,    82,
    82,    82,    82,    82,    82,    82,    82,   116,   417,   130,
   418,   154,   154,   154,   154,   154,   154,   154,   154,   154,
   154,-32768,   217,   420,-32768,-32768,-32768,   410,   332,  1966,
-32768,   423,   437,   429,   146,   260,   227,   217,-32768,-32768,
   217,-32768,-32768,-32768,-32768,-32768,   217,-32768,   217,   433,
   229,  2183,  1749,-32768,-32768,-32768,-32768,-32768,   448,   281,
-32768,   421,   422,   441,   226,-32768,-32768,   378,   217,-32768,
   451,   262,   459,   146,   217,   217,   460,   464,   476,   217,
   217,   150,-32768,   252,   217,   470,   471,   473,   477,   490,
-32768,   491,   154,   154,   262,   484,   485,   487,   488,-32768,
   489,   492,   493,   494,   495,   262,   501,   502,-32768,   503,
-32768,   504,   505,-32768,   506,-32768,-32768,-32768,-32768,-32768,
-32768,   283,   300,   509,   521,   522,-32768,   513,   515,   517,
   391,-32768,   391,-32768,   518,   531,   621,-32768,   217,   525,
   217,   527,   217,   217,   217,   217,-32768,   528,   532,-32768,
-32768,   533,-32768,   217,   530,   541,   542,-32768,   179,   322,
   322,   543,   545,   146,   557,   547,   548,   550,   552,   553,
   555,-32768,   559,   560,-32768,   561,   217,   562,   564,   565,
-32768,   566,-32768,-32768,   301,-32768,-32768,   567,-32768,-32768,
   568,   569,   217,    14,   -49,    14,   -49,   322,   322,   322,
   -57,   -51,   322,   322,   322,    60,    60,    60,    60,    60,
    60,-32768,   570,-32768,   573,   578,   579,   580,   581,   582,
   583,   586,   587,   592,   604,-32768,   605,   596,-32768,  1749,
   348,   450,   217,   599,   611,-32768,-32768,-32768,    78,   140,
   601,   597,   534,   607,-32768,   603,   606,   620,   623,   622,
   634,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,   402,-32768,   217,
-32768,   636,-32768,-32768,   626,   638,   639,-32768,-32768,   217,
-32768,-32768,-32768,-32768,-32768,-32768,   629,   641,   632,   635,
   419,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
   424,-32768,-32768,-32768,-32768,-32768,-32768,   645,   647,-32768,
   640,   642,-32768,-32768,-32768,   643,   644,-32768,   648,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
   649,-32768,   217,   650,-32768,   651,-32768,   217,   653,   217,
   654,-32768,-32768,   430,   217,-32768,-32768,-32768,   217,-32768,
   179,   179,-32768,-32768,   217,   146,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,   217,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,   217,   130,-32768,-32768,-32768,   217,
   130,   217,   -48,   -45,   -44,    14,   -49,    14,   217,   -43,
   -41,     0,-32768,-32768,-32768,-32768,   217,   217,   656,   217,
   217,   658,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,   659,   652,-32768,   575,   671,-32768,   -76,
    78,-32768,   452,   673,-32768,-32768,   677,-32768,-32768,   273,
   146,   668,   669,   670,   453,   167,-32768,   146,-32768,   678,
   679,   425,-32768,-32768,-32768,   681,-32768,   685,   262,    78,
   146,-32768,   684,   686,   469,-32768,   154,-32768,-32768,-32768,
   262,   479,   480,-32768,-32768,-32768,-32768,-32768,-32768,   701,
-32768,-32768,   691,-32768,   692,-32768,   704,   217,   694,   706,
   217,    78,   217,-32768,-32768,   696,   -49,   697,   -49,   217,
    14,   -49,    14,   -49,    14,   -49,   130,   217,   130,   217,
    14,   217,    14,   709,    14,   217,   699,   217,-32768,   700,
   217,-32768,-32768,   146,   702,   496,   703,-32768,   273,   712,
-32768,   497,-32768,-32768,-32768,-32768,-32768,-32768,-32768,   705,
   273,-32768,   707,   171,   171,-32768,   710,-32768,   217,    38,
-32768,-32768,   711,-32768,   499,   486,    45,   217,-32768,-32768,
-32768,   717,   498,   720,   721,-32768,-32768,-32768,-32768,    12,
   718,-32768,   719,   217,    45,   217,-32768,   217,-32768,   217,
   723,   130,   217,   130,   732,   130,   217,   -49,   217,   217,
   217,   130,   724,   130,   725,   130,   217,-32768,   726,-32768,
   727,   217,-32768,   217,-32768,   728,   519,   704,-32768,-32768,
-32768,   520,   514,   731,    78,   744,-32768,-32768,-32768,   524,
    38,-32768,-32768,   734,-32768,   544,   708,   735,    78,-32768,
   262,   526,   535,-32768,-32768,-32768,-32768,   737,   743,   217,
   746,   217,-32768,   -49,   748,   -49,   749,   -49,   217,   217,
   217,   217,   750,   217,-32768,   763,-32768,   217,   756,-32768,
-32768,    78,   138,-32768,-32768,    17,   757,   -42,-32768,   758,
   223,   759,-32768,     5,   140,-32768,-32768,-32768,    45,   546,
-32768,-32768,-32768,-32768,    78,-32768,   760,   217,-32768,   773,
-32768,   217,   765,   217,   767,   217,-32768,   769,   770,   217,
-32768,    45,   774,    60,   775,    60,   217,   771,   138,-32768,
-32768,-32768,   772,   776,-32768,   146,-32768,-32768,-32768,   563,
     5,-32768,   556,   777,-32768,    45,-32768,   779,   780,   217,
-32768,   217,-32768,   781,-32768,-32768,   782,   783,-32768,   610,
-32768,   619,   426,-32768,-32768,-32768,-32768,   217,   785,-32768,
   786,-32768,   787,-32768,-32768,   788,   789,-32768,-32768,-32768,
    18,    18,   217,   801,-32768,-32768,-32768,-32768,-32768,-32768,
   802,-32768,-32768,-32768,   794,-32768,   793,    42,   577,   584,
   585,     3,-32768,   282,   282,   282,   795,-32768,-32768,    11,
-32768,-32768,    52,    54,   140,   796,-32768,   797,   798,   589,
-32768,-32768,-32768,   800,-32768,   820,-32768
};

static const short yypgoto[] = {-32768,
   823,-32768,-32768,   799,   139,   153,  -384,-32768,-32768,-32768,
   -77,  -125,-32768,-32768,-32768,  -667,   -90,  -596,    30,   117,
-32768,    98,   175,-32768,  -623,   -27,-32768,   740,  -763,-32768,
  -451,  -133,-32768,  -134,  -656,  -659,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,   588,-32768,-32768,   -71,-32768,
-32768,    31,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,   -35,-32768,   613,-32768,   655,  -178,-32768,   -82,   -92,
-32768,  -594,  -278,-32768,   -68,  -288,   349,    75,  -353,-32768,
  -381,    -9
};


#define	YYLAST		2406


static const short yytable[] = {    52,
    53,    54,    55,    56,    57,   269,    60,    61,   325,   326,
    64,   277,    69,    70,    51,   582,   491,   824,   286,   287,
   288,   289,   824,   960,   719,   979,   297,   501,   355,   356,
   301,   302,   303,   304,   305,   236,   308,   969,   299,    51,
   576,   315,   581,    77,    78,   262,   263,   619,    83,    84,
    86,   268,   328,   747,   232,    51,   347,    51,   806,   335,
   235,    51,    51,   329,    51,   331,   979,   333,   979,   254,
   336,   337,   807,   364,   265,   267,   969,   363,   330,   366,
   367,   368,   369,   370,   371,   765,   362,   374,    21,   376,
    18,   378,   593,   276,   816,   357,   279,   280,   281,   284,
   292,   913,   577,   479,   393,   395,   397,   624,   625,   996,
   388,   349,   350,   392,   322,     1,   808,   413,   598,   599,
   600,   601,   602,    18,     5,   311,    51,    65,    66,   415,
   416,   417,   418,   419,   420,   421,   422,   423,   424,   266,
   394,   396,    29,   807,   433,   586,   340,   342,   344,   346,
   293,   588,   578,    51,   691,   717,    30,   693,   695,   701,
   970,   703,   406,   407,   408,    29,   478,   323,    49,   373,
   829,   272,   272,   377,   579,   971,   272,   237,    35,    30,
   284,    67,   272,   470,   738,   272,   238,   808,   272,   272,
   272,   239,   272,   903,   626,   904,   908,   893,   351,   970,
   894,    35,   705,    50,   594,   698,   479,   240,   860,   595,
   489,   490,   241,   425,   971,   627,   442,   443,     1,   352,
    51,   990,   869,   631,    25,   549,   596,   242,   436,   817,
    62,   437,   697,   272,   699,   718,   977,   438,   339,   439,
   909,    88,   348,   908,    63,   986,   825,   961,    58,    59,
   631,   901,   365,   243,   631,   892,    71,    25,   628,   451,
   341,   343,   516,   345,   517,   471,   472,   312,   313,   294,
   476,   477,   284,   555,   914,   482,    72,   629,   916,   244,
   409,   353,   802,   803,   410,    51,   988,   909,   989,    73,
   379,   453,   454,   455,   390,   411,   979,   928,   895,   255,
    89,    76,    90,    91,    51,   768,   638,   770,   587,   589,
   773,   380,   775,    75,   777,   896,   391,   571,   983,   984,
   987,   943,   723,   987,   987,   245,    79,   533,   456,   534,
    81,   536,    82,   538,   539,   540,   541,   772,    92,   774,
    87,   776,   233,   457,   545,   234,   246,   782,   251,   784,
   247,   786,   724,   725,   250,    93,   252,   253,   222,   264,
   256,   257,   271,   272,   632,   897,   274,   566,   248,   458,
   746,   726,   275,   633,   -98,   284,   634,   278,   459,   285,
    94,   290,   753,   575,   291,   222,   298,   295,   300,   222,
    95,   632,   306,   309,   617,   632,   840,   222,   314,   460,
   633,   -99,   316,   634,   633,   727,   317,   634,   318,   319,
   320,   321,   327,   258,   461,   332,   462,   680,   681,   334,
   338,   358,   372,   621,   375,   381,   382,   383,   384,   386,
   387,   463,   389,   412,   414,   552,   426,   428,   427,   430,
   728,   464,   431,     7,     8,   432,   434,     9,    10,   440,
    11,   435,   878,   444,   880,   448,   882,   729,   449,   441,
   650,   465,   466,   446,   450,   683,   447,   452,    12,    13,
   655,    14,   583,   584,   585,   469,   473,   590,   591,   592,
   474,   475,    15,   687,   467,   480,   483,   484,   689,   485,
   692,   694,   696,   486,    16,   487,   488,   702,   704,   706,
   492,   493,   638,   494,   495,   496,   550,   508,   497,   498,
   499,   500,    17,    18,   359,   360,   361,   502,   503,   504,
   505,   506,   507,   670,   509,   510,   511,   512,   673,   513,
   675,   514,    19,   515,   518,   678,   519,   543,   544,   679,
   733,   535,   870,   537,   542,   682,   546,   739,   398,   399,
   400,   401,   402,   403,   404,   405,   685,   547,   548,   553,
   748,   554,   556,   557,   558,   686,   559,    20,   560,   561,
   688,   562,   690,   618,   752,   563,   564,   565,   567,   700,
   568,   569,   570,   572,   573,   574,   603,   707,   708,   604,
   710,   711,    21,    22,   605,   606,   607,   608,   609,   610,
   638,   638,   611,   612,   778,   930,   780,   932,   613,   614,
   615,    23,   616,    24,    21,   622,   623,   639,   640,   643,
   641,   642,   644,   792,    51,   520,   521,   522,   523,   524,
   525,   526,   527,   528,   529,   530,   645,   646,   647,   648,
   649,   651,   652,   653,   654,   656,   657,    25,   658,    26,
   662,   659,   663,   660,   953,   743,   664,   714,   665,   666,
   667,   715,   661,   677,   668,   669,   671,   672,   761,   674,
   676,   764,   709,   766,   712,   713,   716,   720,   721,   834,
   771,   836,   722,   838,   734,   735,   736,   737,   779,   844,
   781,   846,   783,   848,   741,   742,   787,   744,   789,   745,
   749,   791,   750,   751,   754,   755,   756,   757,   758,   759,
   762,   763,   767,   769,   785,   788,   790,   797,   793,   795,
   815,   799,   794,   801,   804,   822,   823,   813,   866,   805,
   798,   638,   814,   820,   826,   827,   821,   837,   819,   833,
   845,   847,   850,   851,   854,   855,   857,   859,   858,   861,
   865,   868,   871,   873,   828,   862,   830,   867,   831,   874,
   832,   872,   876,   835,   879,   881,   887,   839,   889,   841,
   842,   843,   891,   902,   905,   907,   917,   849,   919,   929,
   915,   921,   852,   923,   853,   925,   926,   934,   936,   931,
   941,   951,   937,   942,   939,   944,   945,   948,   949,   950,
   952,   955,   956,   957,   958,   959,   965,   966,   967,   968,
   974,   985,   991,   992,   993,   938,   995,   975,   976,   997,
   875,   935,   877,   994,     6,    74,   963,   856,   800,   883,
   884,   885,   886,   906,   888,   796,   270,   978,   890,   940,
     0,   863,   429,     0,   385,     0,     0,   445,     0,     0,
     0,   638,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,   918,     0,
     0,     0,   920,     0,   922,     0,   924,     0,     0,     0,
   927,     0,     0,    99,     0,     0,     0,   933,     0,     0,
     0,     0,     0,     0,     0,     0,     7,     8,   100,     0,
     0,     0,     0,     0,   101,   102,   103,     0,   104,     0,
   946,     0,   947,     0,   105,   106,   107,   108,   109,     0,
   110,     0,     0,     0,     0,     0,     0,     0,   954,     0,
     0,   111,   112,   113,   114,     0,   115,   116,   117,   118,
     0,   119,     0,   964,   120,   121,   122,    16,   123,   124,
   125,   126,   127,   128,   129,   130,   131,   132,     0,     0,
     0,     0,     0,   133,     0,     0,     0,     0,   134,   135,
   136,   137,   138,   139,   140,   141,   142,   143,   144,     0,
   145,     0,   146,     0,     0,     0,  -157,     0,     0,     0,
   147,   148,   149,   150,   151,   152,     0,   153,   154,   155,
   156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
   166,   167,   168,     0,     0,   169,   170,   171,     0,     0,
     0,   172,   173,   174,   175,     0,   176,   177,   178,   179,
     0,   180,     0,   181,     0,     0,   182,     0,   183,     0,
     0,     0,   184,   185,   186,     0,     0,     0,     0,     0,
     0,   187,   188,   189,   190,   191,   192,   193,     0,   194,
     0,     0,     0,   195,     0,     0,     0,     0,   196,   197,
   198,   199,   200,   201,   202,   203,   204,   205,   206,   207,
     0,     0,     0,     0,   208,     0,   209,   210,   211,   212,
   213,   214,   215,   216,   217,   218,   219,   220,   221,   222,
    99,   223,     0,   224,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     7,     8,   100,     0,     0,     0,     0,
     0,   101,   102,   103,     0,   104,     0,     0,     0,     0,
     0,   105,   106,   107,   108,   109,     0,   110,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,   111,   112,
   113,   114,     0,   115,   116,   117,   118,     0,   119,     0,
     0,   120,   121,   122,    16,   123,   124,   125,   126,   127,
   128,   129,   130,   131,   132,     0,     0,     0,     0,     0,
   133,     0,     0,     0,     0,   134,   135,   136,   137,   138,
   139,   140,   141,   142,   143,   144,     0,   145,     0,   146,
     0,     0,     0,     0,     0,     0,     0,   147,   148,   149,
   150,   151,   152,     0,   153,   154,   155,   156,   157,   158,
   159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     0,     0,   169,   170,   171,     0,     0,     0,   172,   173,
   174,   175,     0,   176,   177,   178,   179,     0,   180,     0,
   181,     0,     0,   182,     0,   183,     0,     0,     0,   184,
   185,   186,     0,     0,     0,     0,     0,     0,   187,   188,
   189,   190,   191,   192,   193,     0,   194,     0,     0,     0,
   195,     0,     0,     0,  -157,   196,   197,   198,   199,   200,
   201,   202,   203,   204,   205,   206,   207,     0,     0,     0,
     0,   208,     0,   209,   210,   211,   212,   213,   214,   215,
   216,   217,   218,   219,   220,   221,   222,    99,   223,     0,
   224,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     7,     8,   100,     0,     0,     0,     0,     0,   101,   102,
   103,     0,   104,     0,     0,     0,     0,     0,   105,   106,
   107,   108,   109,     0,   110,     0,     0,  -157,     0,     0,
     0,     0,     0,     0,     0,   111,   112,   113,   114,     0,
   115,   116,   117,   118,     0,   119,     0,     0,   120,   121,
   122,    16,   123,   124,   125,   126,   127,   128,   129,   130,
   131,   132,     0,     0,     0,     0,     0,   133,     0,     0,
     0,     0,   134,   135,   136,   137,   138,   139,   140,   141,
   142,   143,   144,     0,   145,     0,   146,     0,     0,     0,
     0,     0,     0,     0,   147,   148,   149,   150,   151,   152,
     0,   153,   154,   155,   156,   157,   158,   159,   160,   161,
   162,   163,   164,   165,   166,   167,   168,     0,     0,   169,
   170,   171,     0,     0,     0,   172,   173,   174,   175,     0,
   176,   177,   178,   179,     0,   180,     0,   181,     0,     0,
   182,     0,   183,     0,     0,     0,   184,   185,   186,     0,
     0,     0,     0,     0,     0,   187,   188,   189,   190,   191,
   192,   193,     0,   194,     0,     0,     0,   195,     0,     0,
     0,     0,   196,   197,   198,   199,   200,   201,   202,   203,
   204,   205,   206,   207,     0,     0,     0,     0,   208,     0,
   209,   210,   211,   212,   213,   214,   215,   216,   217,   218,
   219,   220,   221,   222,    99,   223,     0,   224,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     7,     8,   100,
     0,     0,     0,     0,     0,   101,   102,   103,     0,   104,
     0,     0,     0,     0,     0,   105,   106,   107,   108,   109,
     0,   110,     0,     0,     0,     0,  -157,     0,     0,     0,
     0,     0,   111,   112,   113,   114,     0,   115,   116,   117,
   118,     0,   119,     0,     0,   120,   121,   122,    16,   123,
   124,   125,   126,   127,   128,   129,   130,   131,   132,     0,
     0,     0,     0,     0,   133,     0,     0,     0,     0,   134,
   135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     0,   145,     0,   146,     0,     0,     0,     0,     0,     0,
     0,   147,   148,   149,   150,   151,   152,     0,   153,   154,
   155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
   165,   166,   167,   168,     0,     0,   169,   170,   171,     0,
     0,     0,   172,   173,   174,   175,     0,   176,   177,   178,
   179,     0,   180,     0,   181,     0,     0,   182,     0,   183,
     0,     0,     0,   184,   185,   186,     0,     0,     0,     0,
     0,     0,   187,   188,   189,   190,   191,   192,   193,     0,
   194,     0,     0,     0,   195,     0,     0,     0,     0,   196,
   197,   198,   199,   200,   201,   202,   203,   204,   205,   206,
   207,     0,     0,     0,     0,   208,     0,   209,   210,   211,
   212,   213,   214,   215,   216,   217,   218,   219,   220,   221,
   222,    99,   223,     0,   224,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     7,     8,   100,     0,     0,     0,
     0,     0,   101,   102,   103,     0,   104,     0,     0,     0,
     0,     0,   105,   106,   107,   108,   109,     0,   110,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,   111,
   112,   113,   114,     0,   115,   116,   117,   118,     0,   119,
     0,     0,   120,   121,   122,    16,   123,   124,   125,   126,
   127,   128,   129,   130,   131,   132,     0,     0,     0,     0,
     0,   133,     0,     0,     0,  -157,   134,   135,   136,   137,
   138,   139,   140,   141,   142,   143,   144,     0,   145,     0,
   146,     0,     0,     0,     0,     0,     0,     0,   147,   148,
   149,   150,   151,   152,     0,   153,   154,   155,   156,   157,
   158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
   168,     0,     0,   169,   170,   171,     0,     0,     0,   172,
   173,   174,   175,     0,   176,   177,   178,   179,     0,   180,
     0,   181,     0,     0,   182,     0,   183,     0,     0,     0,
   184,   185,   186,     0,     0,     0,     0,     0,     0,   187,
   188,   189,   190,   191,   192,   193,     0,   194,     0,     0,
     0,   195,     0,     0,     0,     0,   196,   197,   198,   199,
   200,   201,   202,   203,   204,   205,   206,   207,     0,     0,
     0,     0,   208,     0,   209,   210,   211,   212,   213,   214,
   215,   216,   217,   218,   219,   220,   221,   222,    99,   223,
     0,   224,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     7,     8,   100,     0,     0,     0,     0,     0,   101,
   102,   103,     0,   104,  -108,     0,     0,     0,     0,   105,
   106,   107,   108,   109,     0,   110,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,   111,   112,   113,   114,
     0,   115,   116,   117,   118,     0,   119,     0,     0,   120,
   121,   122,    16,   123,   124,   125,   126,   127,   128,   129,
   130,   131,   132,     0,     0,     0,     0,     0,   133,     0,
     0,     0,     0,   134,   135,   136,   137,   138,   139,   140,
   141,   142,   143,   144,     0,   145,     0,   146,     0,     0,
     0,     0,     0,     0,     0,   147,   148,   149,   150,   151,
   152,     0,   153,   154,   155,   156,   157,   158,   159,   160,
   161,   162,   163,   164,   165,   166,   167,   168,     0,     0,
   169,   170,   171,     0,     0,     0,   172,   173,   174,   175,
     0,   176,   177,   178,   179,     0,   180,     0,   181,     0,
     0,   182,     0,   183,     0,     0,     0,   184,   185,   186,
     0,     0,     0,     0,     0,     0,   187,   188,   189,   190,
   191,   192,   193,     0,   194,     0,     0,     0,   195,     0,
     0,     0,     0,   196,   197,   198,   199,   200,   201,   202,
   203,   204,   205,   206,   207,     0,     0,     0,     0,   208,
     0,   209,   210,   211,   212,   213,   214,   215,   216,   217,
   218,   219,   220,   221,   222,    99,   223,     0,   224,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     7,     8,
   100,     0,     0,     0,  -157,     0,   101,   102,   103,     0,
   104,     0,     0,     0,     0,     0,   105,   106,   107,   108,
   109,     0,   110,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,   111,   112,   113,   114,     0,   115,   116,
   117,   118,     0,   119,     0,     0,   120,   121,   122,    16,
   123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     0,     0,     0,     0,     0,   133,     0,     0,     0,     0,
   134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
   144,     0,   145,     0,   146,     0,     0,     0,     0,     0,
     0,     0,   147,   148,   149,   150,   151,   152,     0,   153,
   154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
   164,   165,   166,   167,   168,     0,     0,   169,   170,   171,
     0,     0,     0,   172,   173,   174,   175,     0,   176,   177,
   178,   179,     0,   180,     0,   181,     0,     0,   182,     0,
   183,     0,     0,     0,   184,   185,   186,     0,     0,     0,
     0,     0,     0,   187,   188,   189,   190,   191,   192,   193,
     0,   194,     0,     0,     0,   195,     0,     0,     0,     0,
   196,   197,   198,   199,   200,   201,   202,   203,   204,   205,
   206,   207,     0,     0,     0,     0,   208,     0,   209,   210,
   211,   212,   213,   214,   215,   216,   217,   218,   219,   220,
   221,   222,     0,   223,     0,   224
};

static const short yycheck[] = {     9,
    10,    11,    12,    13,    14,    96,    16,    17,   142,   143,
    20,   104,    22,    23,     4,   397,   295,     6,   111,   112,
   113,   114,     6,     6,   621,    15,   119,   306,   162,   163,
   123,   124,   125,   126,   127,    71,   129,    35,   121,     4,
   394,   134,   396,    53,    54,    81,    82,   432,    58,    59,
    60,    87,   145,   650,    64,     4,    17,     4,    21,   152,
    70,     4,     4,   146,     4,   148,    15,   150,    15,    79,
   153,   154,   740,     6,    84,    85,    35,   170,   147,   172,
   173,   174,   175,   176,   177,   682,   169,   180,   165,   182,
    86,   184,    33,   103,    50,   164,   106,   107,   108,   109,
    32,   865,   152,   282,   197,   198,   199,    30,    31,     0,
   193,    30,    31,   196,   101,   109,   740,   210,   407,   408,
   409,   410,   411,    86,     6,    17,     4,   103,   104,   212,
   213,   214,   215,   216,   217,   218,   219,   220,   221,    17,
   203,   203,     4,   811,   235,   203,   156,   157,   158,   159,
    82,   203,   202,     4,   203,   232,     4,   203,   203,   203,
   158,   203,    47,    48,    49,    27,    17,   154,     6,   179,
   765,   234,   234,   183,   224,   173,   234,    32,     4,    27,
   190,   157,   234,   274,   636,   234,    41,   811,   234,   234,
   234,    46,   234,   236,   117,   238,   864,    60,   117,   158,
    63,    27,   203,    17,   145,   587,   385,    62,   805,   150,
   293,   294,    67,   223,   173,   138,   252,   253,   109,   138,
     4,   985,   819,    84,   220,   359,   167,    82,   238,   185,
     6,   241,   586,   234,   588,   620,   234,   247,   203,   249,
   864,    19,   203,   911,    17,   235,   235,   230,   165,   166,
    84,   235,   185,   108,    84,   852,    17,   220,   181,   269,
   203,   203,   331,   203,   333,   275,   276,   159,   160,   201,
   280,   281,   282,   364,   869,   285,     6,   200,   875,   134,
   165,   200,   734,   735,   169,     4,   235,   911,   235,     4,
     6,    30,    31,    32,    17,   180,    15,   892,   161,    19,
    78,     6,    80,    81,     4,   687,   440,   689,   401,   402,
   692,    27,   694,    17,   696,   178,    39,    17,   975,   976,
   980,   916,    50,   983,   984,   180,    79,   337,    67,   339,
    17,   341,    17,   343,   344,   345,   346,   691,   116,   693,
    17,   695,   225,    82,   354,   231,   201,   701,    15,   703,
   205,   705,    80,    81,   234,   133,    17,    17,   219,   233,
    80,    81,   226,   234,   225,   228,     6,   377,   223,   108,
   649,    99,     6,   234,   235,   385,   237,    15,   117,     6,
   158,    27,   661,   393,    17,   219,     6,   234,    17,   219,
   168,   225,   234,     3,   430,   225,   778,   219,    17,   138,
   234,   235,    17,   237,   234,   133,    17,   237,    17,    17,
    17,    17,    17,   133,   153,    17,   155,   551,   552,    17,
    17,    17,    17,   433,    17,    17,    27,    17,    17,    17,
    17,   170,    17,    17,    17,   361,    17,   106,    29,    17,
   168,   180,     6,    16,    17,    17,   187,    20,    21,    17,
    23,   225,   834,     6,   836,    15,   838,   185,   233,   231,
   470,   200,   201,    43,    87,   556,    45,    17,    41,    42,
   480,    44,   398,   399,   400,    17,    17,   403,   404,   405,
    17,     6,    55,   576,   223,   234,    17,    17,   581,    17,
   583,   584,   585,    17,    67,     6,     6,   590,   591,   592,
    17,    17,   636,    17,    17,    17,   185,   225,    17,    17,
    17,    17,    85,    86,   166,   167,   168,    17,    17,    17,
    17,    17,    17,   533,   225,    17,     6,     6,   538,    17,
   540,    17,   105,    17,    17,   545,     6,     6,     6,   549,
   631,    17,   821,    17,    17,   555,    17,   638,   200,   201,
   202,   203,   204,   205,   206,   207,   566,    17,    17,    17,
   651,    17,     6,    17,    17,   575,    17,   140,    17,    17,
   580,    17,   582,   226,   657,    17,    17,    17,    17,   589,
    17,    17,    17,    17,    17,    17,    17,   597,   598,    17,
   600,   601,   165,   166,    17,    17,    17,    17,    17,    17,
   734,   735,    17,    17,   697,   894,   699,   896,    17,     6,
     6,   184,    17,   186,   165,    17,     6,    17,    22,    17,
    87,    15,    17,   714,     4,     5,     6,     7,     8,     9,
    10,    11,    12,    13,    14,    15,    17,    15,    17,     6,
   239,     6,    17,     6,     6,    17,     6,   220,    17,   222,
     6,    17,     6,   235,   229,   231,    17,     6,    17,    17,
    17,    87,   239,   234,    17,    17,    17,    17,   678,    17,
    17,   681,    17,   683,    17,    17,     6,   226,     6,   772,
   690,   774,     6,   776,    17,    17,    17,   235,   698,   782,
   700,   784,   702,   786,    17,    17,   706,    17,   708,    15,
    17,   711,    17,   235,   226,   226,     6,    17,    17,     6,
    17,     6,    17,    17,     6,    17,    17,     6,    17,    17,
   235,    17,   227,    17,    15,     6,     6,    17,   185,   739,
   234,   865,   234,    17,    17,    17,   239,     6,   748,    17,
    17,    17,    17,    17,    17,   227,   227,    17,   235,     6,
    17,    17,   227,    17,   764,   232,   766,    50,   768,    17,
   770,   227,    17,   773,    17,    17,    17,   777,     6,   779,
   780,   781,    17,    17,    17,    17,    17,   787,     6,     6,
   235,    17,   792,    17,   794,    17,    17,    17,    17,    15,
   235,   182,    17,    17,   232,    17,    17,    17,    17,    17,
   182,    17,    17,    17,    17,    17,     6,     6,    15,    17,
   234,    17,    17,    17,    17,   906,    17,   234,   234,     0,
   830,   899,   832,   235,     2,    27,   952,   798,   731,   839,
   840,   841,   842,   861,   844,   719,    97,   972,   848,   911,
    -1,   811,   230,    -1,   190,    -1,    -1,   260,    -1,    -1,
    -1,   985,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   878,    -1,
    -1,    -1,   882,    -1,   884,    -1,   886,    -1,    -1,    -1,
   890,    -1,    -1,     3,    -1,    -1,    -1,   897,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    16,    17,    18,    -1,
    -1,    -1,    -1,    -1,    24,    25,    26,    -1,    28,    -1,
   920,    -1,   922,    -1,    34,    35,    36,    37,    38,    -1,
    40,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   938,    -1,
    -1,    51,    52,    53,    54,    -1,    56,    57,    58,    59,
    -1,    61,    -1,   953,    64,    65,    66,    67,    68,    69,
    70,    71,    72,    73,    74,    75,    76,    77,    -1,    -1,
    -1,    -1,    -1,    83,    -1,    -1,    -1,    -1,    88,    89,
    90,    91,    92,    93,    94,    95,    96,    97,    98,    -1,
   100,    -1,   102,    -1,    -1,    -1,   106,    -1,    -1,    -1,
   110,   111,   112,   113,   114,   115,    -1,   117,   118,   119,
   120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
   130,   131,   132,    -1,    -1,   135,   136,   137,    -1,    -1,
    -1,   141,   142,   143,   144,    -1,   146,   147,   148,   149,
    -1,   151,    -1,   153,    -1,    -1,   156,    -1,   158,    -1,
    -1,    -1,   162,   163,   164,    -1,    -1,    -1,    -1,    -1,
    -1,   171,   172,   173,   174,   175,   176,   177,    -1,   179,
    -1,    -1,    -1,   183,    -1,    -1,    -1,    -1,   188,   189,
   190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
    -1,    -1,    -1,    -1,   204,    -1,   206,   207,   208,   209,
   210,   211,   212,   213,   214,   215,   216,   217,   218,   219,
     3,   221,    -1,   223,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    16,    17,    18,    -1,    -1,    -1,    -1,
    -1,    24,    25,    26,    -1,    28,    -1,    -1,    -1,    -1,
    -1,    34,    35,    36,    37,    38,    -1,    40,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    51,    52,
    53,    54,    -1,    56,    57,    58,    59,    -1,    61,    -1,
    -1,    64,    65,    66,    67,    68,    69,    70,    71,    72,
    73,    74,    75,    76,    77,    -1,    -1,    -1,    -1,    -1,
    83,    -1,    -1,    -1,    -1,    88,    89,    90,    91,    92,
    93,    94,    95,    96,    97,    98,    -1,   100,    -1,   102,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,   111,   112,
   113,   114,   115,    -1,   117,   118,   119,   120,   121,   122,
   123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
    -1,    -1,   135,   136,   137,    -1,    -1,    -1,   141,   142,
   143,   144,    -1,   146,   147,   148,   149,    -1,   151,    -1,
   153,    -1,    -1,   156,    -1,   158,    -1,    -1,    -1,   162,
   163,   164,    -1,    -1,    -1,    -1,    -1,    -1,   171,   172,
   173,   174,   175,   176,   177,    -1,   179,    -1,    -1,    -1,
   183,    -1,    -1,    -1,   187,   188,   189,   190,   191,   192,
   193,   194,   195,   196,   197,   198,   199,    -1,    -1,    -1,
    -1,   204,    -1,   206,   207,   208,   209,   210,   211,   212,
   213,   214,   215,   216,   217,   218,   219,     3,   221,    -1,
   223,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    16,    17,    18,    -1,    -1,    -1,    -1,    -1,    24,    25,
    26,    -1,    28,    -1,    -1,    -1,    -1,    -1,    34,    35,
    36,    37,    38,    -1,    40,    -1,    -1,    43,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    51,    52,    53,    54,    -1,
    56,    57,    58,    59,    -1,    61,    -1,    -1,    64,    65,
    66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
    76,    77,    -1,    -1,    -1,    -1,    -1,    83,    -1,    -1,
    -1,    -1,    88,    89,    90,    91,    92,    93,    94,    95,
    96,    97,    98,    -1,   100,    -1,   102,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,   110,   111,   112,   113,   114,   115,
    -1,   117,   118,   119,   120,   121,   122,   123,   124,   125,
   126,   127,   128,   129,   130,   131,   132,    -1,    -1,   135,
   136,   137,    -1,    -1,    -1,   141,   142,   143,   144,    -1,
   146,   147,   148,   149,    -1,   151,    -1,   153,    -1,    -1,
   156,    -1,   158,    -1,    -1,    -1,   162,   163,   164,    -1,
    -1,    -1,    -1,    -1,    -1,   171,   172,   173,   174,   175,
   176,   177,    -1,   179,    -1,    -1,    -1,   183,    -1,    -1,
    -1,    -1,   188,   189,   190,   191,   192,   193,   194,   195,
   196,   197,   198,   199,    -1,    -1,    -1,    -1,   204,    -1,
   206,   207,   208,   209,   210,   211,   212,   213,   214,   215,
   216,   217,   218,   219,     3,   221,    -1,   223,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    16,    17,    18,
    -1,    -1,    -1,    -1,    -1,    24,    25,    26,    -1,    28,
    -1,    -1,    -1,    -1,    -1,    34,    35,    36,    37,    38,
    -1,    40,    -1,    -1,    -1,    -1,    45,    -1,    -1,    -1,
    -1,    -1,    51,    52,    53,    54,    -1,    56,    57,    58,
    59,    -1,    61,    -1,    -1,    64,    65,    66,    67,    68,
    69,    70,    71,    72,    73,    74,    75,    76,    77,    -1,
    -1,    -1,    -1,    -1,    83,    -1,    -1,    -1,    -1,    88,
    89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
    -1,   100,    -1,   102,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,   110,   111,   112,   113,   114,   115,    -1,   117,   118,
   119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
   129,   130,   131,   132,    -1,    -1,   135,   136,   137,    -1,
    -1,    -1,   141,   142,   143,   144,    -1,   146,   147,   148,
   149,    -1,   151,    -1,   153,    -1,    -1,   156,    -1,   158,
    -1,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,    -1,
    -1,    -1,   171,   172,   173,   174,   175,   176,   177,    -1,
   179,    -1,    -1,    -1,   183,    -1,    -1,    -1,    -1,   188,
   189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
   199,    -1,    -1,    -1,    -1,   204,    -1,   206,   207,   208,
   209,   210,   211,   212,   213,   214,   215,   216,   217,   218,
   219,     3,   221,    -1,   223,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    16,    17,    18,    -1,    -1,    -1,
    -1,    -1,    24,    25,    26,    -1,    28,    -1,    -1,    -1,
    -1,    -1,    34,    35,    36,    37,    38,    -1,    40,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    51,
    52,    53,    54,    -1,    56,    57,    58,    59,    -1,    61,
    -1,    -1,    64,    65,    66,    67,    68,    69,    70,    71,
    72,    73,    74,    75,    76,    77,    -1,    -1,    -1,    -1,
    -1,    83,    -1,    -1,    -1,    87,    88,    89,    90,    91,
    92,    93,    94,    95,    96,    97,    98,    -1,   100,    -1,
   102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,   111,
   112,   113,   114,   115,    -1,   117,   118,   119,   120,   121,
   122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
   132,    -1,    -1,   135,   136,   137,    -1,    -1,    -1,   141,
   142,   143,   144,    -1,   146,   147,   148,   149,    -1,   151,
    -1,   153,    -1,    -1,   156,    -1,   158,    -1,    -1,    -1,
   162,   163,   164,    -1,    -1,    -1,    -1,    -1,    -1,   171,
   172,   173,   174,   175,   176,   177,    -1,   179,    -1,    -1,
    -1,   183,    -1,    -1,    -1,    -1,   188,   189,   190,   191,
   192,   193,   194,   195,   196,   197,   198,   199,    -1,    -1,
    -1,    -1,   204,    -1,   206,   207,   208,   209,   210,   211,
   212,   213,   214,   215,   216,   217,   218,   219,     3,   221,
    -1,   223,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,    24,
    25,    26,    -1,    28,    29,    -1,    -1,    -1,    -1,    34,
    35,    36,    37,    38,    -1,    40,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    51,    52,    53,    54,
    -1,    56,    57,    58,    59,    -1,    61,    -1,    -1,    64,
    65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
    75,    76,    77,    -1,    -1,    -1,    -1,    -1,    83,    -1,
    -1,    -1,    -1,    88,    89,    90,    91,    92,    93,    94,
    95,    96,    97,    98,    -1,   100,    -1,   102,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,   110,   111,   112,   113,   114,
   115,    -1,   117,   118,   119,   120,   121,   122,   123,   124,
   125,   126,   127,   128,   129,   130,   131,   132,    -1,    -1,
   135,   136,   137,    -1,    -1,    -1,   141,   142,   143,   144,
    -1,   146,   147,   148,   149,    -1,   151,    -1,   153,    -1,
    -1,   156,    -1,   158,    -1,    -1,    -1,   162,   163,   164,
    -1,    -1,    -1,    -1,    -1,    -1,   171,   172,   173,   174,
   175,   176,   177,    -1,   179,    -1,    -1,    -1,   183,    -1,
    -1,    -1,    -1,   188,   189,   190,   191,   192,   193,   194,
   195,   196,   197,   198,   199,    -1,    -1,    -1,    -1,   204,
    -1,   206,   207,   208,   209,   210,   211,   212,   213,   214,
   215,   216,   217,   218,   219,     3,   221,    -1,   223,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    16,    17,
    18,    -1,    -1,    -1,    22,    -1,    24,    25,    26,    -1,
    28,    -1,    -1,    -1,    -1,    -1,    34,    35,    36,    37,
    38,    -1,    40,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    51,    52,    53,    54,    -1,    56,    57,
    58,    59,    -1,    61,    -1,    -1,    64,    65,    66,    67,
    68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
    -1,    -1,    -1,    -1,    -1,    83,    -1,    -1,    -1,    -1,
    88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
    98,    -1,   100,    -1,   102,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,   110,   111,   112,   113,   114,   115,    -1,   117,
   118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
   128,   129,   130,   131,   132,    -1,    -1,   135,   136,   137,
    -1,    -1,    -1,   141,   142,   143,   144,    -1,   146,   147,
   148,   149,    -1,   151,    -1,   153,    -1,    -1,   156,    -1,
   158,    -1,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,
    -1,    -1,    -1,   171,   172,   173,   174,   175,   176,   177,
    -1,   179,    -1,    -1,    -1,   183,    -1,    -1,    -1,    -1,
   188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
   198,   199,    -1,    -1,    -1,    -1,   204,    -1,   206,   207,
   208,   209,   210,   211,   212,   213,   214,   215,   216,   217,
   218,   219,    -1,   221,    -1,   223
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
int yydebug = 1;
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
    /* object_file */
    JD_top_inst = yyval.inst = yyvsp[0].inst ;
  ;
    break;}
case 2:

{
    /* ObjectChain */
    vcode_info *last = yyvsp[-1].inst ;
    if( yyvsp[-1].inst ) {
      while(last->next) last = last->next ;
      last->next = yyvsp[0].inst ;
      yyval.inst = yyvsp[-1].inst ;
    }
    else JD_top_inst = yyval.inst = yyvsp[0].inst ;
  ;
    break;}
case 3:

{
    yyval.inst = yyvsp[0].inst ; 
  ;
    break;}
case 4:

{
    /* inital_header */
    int version, revision ;
    
    version = atoi(yyvsp[-2].string_value.name) ;
    revision = atoi(yyvsp[-1].string_value.name) ;
    
    if( version > 0 && revision > 0 ) {
      /* do the version & revision consistency check */
      /* TBI */ 
    
    }
    
  ;
    break;}
case 5:

{
    yyval.inst = yyvsp[0].inst ; 
  ;
    break;}
case 6:

{
    /* VcodeChain */
    int *pp = NULL ;
    vcode_info *last = yyvsp[-1].inst ;
    if( JD_first_inst == NULL ) JD_first_inst = yyvsp[-1].inst ;
    if( yyvsp[-1].inst ) {
      while(last->next) {
        if( last == yyvsp[0].inst ) {
          printf( "VcodeChain loop detected\n" ) ;
          *pp = 0 ;
        }
        last = last->next ;
      }
      last->next = yyvsp[0].inst ;
      yyval.inst = yyvsp[-1].inst ;
    }
    else yyval.inst = yyvsp[0].inst ;
  ;
    break;}
case 7:

{
    yyval.inst = NULL ;
  ;
    break;}
case 8:

{
    yyval.inst = yyvsp[0].inst ; 
  ;
    break;}
case 9:

{
    /* top_port_info */
    JD_name *name ;
    yyval.inst = yyvsp[0].inst ; 
    name = find_global_name( yyval.inst->is.port.name ) ;
    if( !name ) {
      name = add_global_name( yyval.inst->is.port.name ) ;
      name->info = yyval.inst ;
      name->type = JD_port_name ;
      yyval.inst->code_name = name ;
    }
    else {
      if( name->type != JD_port_name ) {
        ERROR_PORT_NAME_TYPE_MISMATCH( yyval.inst->is.port.name ) ;
      }
      else if( !comp_info( name->info, yyval.inst ) ) {
        ERROR_PORT_NAME_ATTRIB_MISMATCH( yyval.inst->is.port.name ) ;
      }
      yyval.inst = NULL ;
    }
  ;
    break;}
case 10:

{
    /* top_portset_info */
    JD_name *name, *ptname ;
    vcode_info *pt ;
    int lflag = 1 ;
    yyval.inst = yyvsp[0].inst ; 
    name = find_global_name( yyval.inst->is.portset.name ) ;
    if( !name ) {
      name = add_global_name( yyval.inst->is.portset.name ) ;
      name->info = yyval.inst ;
      name->type = JD_portset_name ;
      pt = yyval.inst->is.portset.port_infos ;
      while( pt ) {
        ptname = add_name_to_scope( pt->is.port.name, &name->child ) ;
        ptname->type = JD_port_name ;
        ptname->info = pt ;
        pt->is.port.portset = yyval.inst ;
        pt->code_name = ptname ;
        pt = pt->next ;
      }
    }
    else {
      if( name->type != JD_portset_name ) {
        ERROR_PORTSET_NAME_TYPE_MISMATCH( yyval.inst->is.portset.name ) ;
      }
      comp_portset_info( yyval.inst, name ) ;
      yyval.inst = NULL ;
    }
  ;
    break;}
case 11:

{
    yyval.inst = yyvsp[0].inst ; 
  ;
    break;}
case 12:

{
    /* top_variable_dcl */
    JD_name *name ;
    vcode_info *attrib ;
    
    yyval.inst = yyvsp[0].inst ; 
    attrib = yyvsp[0].inst->is.variable_dcl.var_attrib ;
    if( attrib && attrib->is.attrib.is_local )
      name = find_local_name( yyval.inst->is.variable_dcl.name ) ;
    else
      name = find_global_name( yyval.inst->is.variable_dcl.name ) ;
    
    if( name ) {
      if( name->type != JD_variable_name ) {
        ERROR_VARIABLE_NAME_TYPE_MISMATCH( yyval.inst->is.variable_dcl.name ) ;
      }
      else if( !comp_info( name->info, yyval.inst ) ) {
        ERROR_VARIABLE_NAME_ATTRIB_MISMATCH( yyval.inst->is.variable_dcl.name ) ;
      }
      yyval.inst = NULL ;
    }
    else {
      if( attrib && attrib->is.attrib.is_local ) {
        name = add_local_name( yyval.inst->is.variable_dcl.name ) ;
      }
      else {
        name = add_global_name( yyval.inst->is.variable_dcl.name ) ;
      }
      name->type = JD_variable_name ;
      name->info = yyval.inst ;
      yyval.inst->code_name = name ;
    }
  ;
    break;}
case 13:

{
    /* top_enum_dcl */
    JD_name *name, *mnname ;
    vcode_info *mn ;
    int  index ;
    yyval.inst = yyvsp[0].inst ;
    name = find_global_name( yyvsp[0].inst->is.enum_dcl.name ) ;
    if( !name ) {
      name = add_global_name( yyvsp[0].inst->is.enum_dcl.name ) ;
      name->info = yyvsp[0].inst ;
      name->type = JD_enum_name ;
      mn = yyvsp[0].inst->is.enum_dcl.members ;
      index = 0 ;
      while( mn ) {
        mnname = add_name_to_scope( mn->is.string, &name->child ) ;
        mnname->info = mn ;
        mnname->type = JD_enum_member_name ;
        mnname->index = index++ ;
        mn = mn->next ;
      }
    }
    else {
      if( name->type != JD_enum_name ) {
        ERROR_ENUM_NAME_TYPE_MISMATCH( yyvsp[0].inst->is.enum_dcl.name ) ;
      }
      comp_enum_info( yyvsp[0].inst, name ) ;
      yyval.inst = NULL ;
    }
  
  ;
    break;}
case 14:

{
    /* top_function_info */
    JD_name *name ;
    vcode_info *mn, *attrib ;
    
    yyval.inst = yyvsp[0].inst ;
    attrib = yyvsp[0].inst->is.function_info.func_options ;
    if( attrib && attrib->is.attrib.is_local )
      name = find_local_name( yyval.inst->is.function_info.name ) ;
    else
      name = find_global_name( yyval.inst->is.function_info.name ) ;
    if( !name ) {
      if( attrib && attrib->is.attrib.is_local )
        name = add_local_name( yyvsp[0].inst->is.function_info.name ) ;
      else
        name = add_global_name( yyvsp[0].inst->is.function_info.name ) ;
      name->info = yyvsp[0].inst ;
      if( yyvsp[0].inst->is.function_info.sys_flag ) name->type = JD_sys_func_name ;
      else name->type = JD_func_name ;
    }
    else {
      if( name->type != JD_func_name ) {
        ERROR_FUNCTION_NAME_TYPE_MISMATCH( yyvsp[0].inst->is.function_info.name ) ;
      }
      comp_func_info( NULL, yyvsp[0].inst, name->info ) ;
      yyval.inst = NULL ;
    }
  
  ;
    break;}
case 15:

{
    /* top_class_info */
    JD_name *name, *mname ;
    vcode_info *mn, *attrib ;
  
    yyval.inst = yyvsp[0].inst ;
    attrib = yyvsp[0].inst->is.class.attribs ;
    if( attrib && attrib->is.attrib.is_local )
      name = find_local_name( yyvsp[0].inst->is.class.name ) ;
    else
      name = find_global_name( yyvsp[0].inst->is.class.name ) ;
    if( !name ) {
      if( attrib && attrib->is.attrib.is_local ) 
        name = add_local_name( yyvsp[0].inst->is.class.name ) ;
      else 
        name = add_global_name( yyvsp[0].inst->is.class.name ) ;
      name->info = yyvsp[0].inst ;
      if( yyvsp[0].inst->is.class.sys_flag ) name->type = JD_sys_class_name ;
      else name->type = JD_class_name ;
      /* add member name to the class scope */
      mn = yyvsp[0].inst->is.class.members ;
      while( mn ) {
        mname = add_name_to_scope( get_info_name( mn ), &name->child ) ;
        mname->type = get_info_type( mn ) ;
        mname->info = mn ;
        mname->index = mn->index ;
        mn = mn->next ;
      }
    }
    else {
      if( name->type != JD_class_name ) {
        ERROR_CLASS_NAME_TYPE_MISMATCH( yyvsp[0].inst->is.class.name ) ;
      }
      (void)comp_class( yyvsp[0].inst, name ) ;
      yyval.inst = NULL ;
    }
  
  ;
    break;}
case 16:

{
    /* top_aspect_info */
    JD_name *name, *mname ;
    vcode_info *mn, *attrib ;
  
    yyval.inst = yyvsp[0].inst ;
    name = find_global_name( yyvsp[0].inst->is.class.name ) ;
    if( !name ) {
      name = add_global_name( yyvsp[0].inst->is.class.name ) ;
      name->info = yyvsp[0].inst ;
      name->type = JD_aspect_name ;
      /* add member name to the class scope */
      mn = yyvsp[0].inst->is.class.members ;
      while( mn ) {
        mname = add_name_to_scope( get_info_name( mn ), &name->child ) ;
        mname->type = get_info_type( mn ) ;
        mname->info = mn ;
        mname->index = mn->index ;
        mn = mn->next ;
      }
    }
    else {
      if( name->type != JD_aspect_name ) {
        ERROR_ASPECT_NAME_TYPE_MISMATCH( yyvsp[0].inst->is.function_info.name ) ;
      }
      (void)comp_aspect( yyvsp[0].inst, name ) ;
      yyval.inst = NULL ;
    }
  
  ;
    break;}
case 17:

{
    /* top_verilog_task_info */
    JD_name *name ;
    vcode_info *mn ;
    
    yyval.inst = yyvsp[0].inst ;
    /* verilog task needs its own name scope */
    name = find_verilog_name( yyvsp[0].inst->is.function_info.name ) ;
    if( !name )
    {
      name = add_verilog_name( yyvsp[0].inst->is.function_info.name ) ;
      name->info = yyvsp[0].inst ;
      name->type = JD_verilog_task_name ;
      yyval.inst->code_name = name ;
    }
    else {
      if( name->type != JD_verilog_task_name ) {
        ERROR_FUNCTION_NAME_TYPE_MISMATCH( yyvsp[0].inst->is.function_info.name ) ;
      }
      comp_veri_task_info( yyvsp[0].inst, name->info ) ;
      yyval.inst = NULL ;
    }
  
  ;
    break;}
case 18:

{
    yyval.inst = yyvsp[0].inst ; 
  ;
    break;}
case 19:

{
    yyval.inst = yyvsp[0].inst ; 
  ;
    break;}
case 20:

{
    yyval.inst = yyvsp[0].inst ; 
  ;
    break;}
case 21:

{
    yyval.inst = yyvsp[0].inst ; 
  ;
    break;}
case 22:

{
    yyval.inst = yyvsp[0].inst ; 
  ;
    break;}
case 23:

{
    yyval.inst = yyvsp[0].inst ; 
  ;
    break;}
case 24:

{
    yyval.inst = yyvsp[0].inst ; 
  ;
    break;}
case 25:

{
    yyval.inst = yyvsp[0].inst ; 
  ;
    break;}
case 26:

{
    yyval.inst = yyvsp[0].inst ; 
  ;
    break;}
case 27:

{
    yyval.inst = NULL ;
  ;
    break;}
case 28:

{
    /* debug_information */
    char *cp, *c ;
    yyval.inst = ALLOC(vinst_info) ;
    yyval.inst->type = JD_debug_info ;
    yyval.inst->is.debug_info.string = c = cp = yyvsp[-1].string_value.name ;
    if( !strncmp( cp, "#statement", 10 ) ) {
      yyval.inst->type = JD_debug_statement_info ;
      yyval.inst->is.debug_info.string = c ;
      while( !isspace(*c) ) c++ ;
      while( isspace(*c) ) c++ ;
      yyval.inst->is.debug_info.statementnum = atoi(c) ;
      while( !isspace(*c) ) c++ ;
      while( isspace(*c) ) c++ ;
      cp = c ;
      while( !isspace(*c) ) c++ ;
      *c = '\0' ;
      yyval.inst->is.debug_info.filename = cp ;
      c++ ;
      while( isspace(*c) ) c++ ;
      yyval.inst->is.debug_info.linenum = atoi(c) ;
    }
    else if( !strncmp( cp, "#cwd", 4 ) ) {
      yyval.inst->type = JD_debug_cwd_info ;
      while( !isspace(*c) ) c++ ;
      while( isspace(*c) ) c++ ;
      yyval.inst->is.debug_cwd_info.dir = c ;
      while( !isspace(*c) && ( *c != '\0') ) c++ ;
      if( *c != '\0' ) *c = '\0' ;
    }
    else if( !strncmp( cp, "#file", 4 ) ) {
      yyval.inst->type = JD_debug_file_info ;
      while( !isspace(*c) ) c++ ;
      while( isspace(*c) ) c++ ;
      yyval.inst->is.debug_file_info.file = c ;
      while( !isspace(*c) && ( *c != '\0') ) c++ ;
      if( *c != '\0' ) {
        *c = '\0' ;
        c++ ;
        while( isspace(*c) && *c != '\0' ) c++ ;
        if( *c != '\0' ) {
          yyval.inst->is.debug_file_info.dir = c ;
          while( !isspace(*c) && ( *c != '\0') ) c++ ;
          if( *c != '\0' ) *c = '\0' ;
        }
        else yyval.inst->is.debug_file_info.dir = "" ;
      }
      else {
        yyval.inst->is.debug_file_info.dir = "" ;
      }
    }
    else if( !strncmp( cp, "#gen_func_frame", 15 ) ) {
      yyval.inst->type = JD_debug_gen_func_frame_info ;
      while( !isspace(*c) ) c++ ;
      while( isspace(*c) ) c++ ;
      yyval.inst->is.debug_gen_func_frame_info.func = c ;
      while( !isspace(*c) && ( *c != '\0') ) c++ ;
      if( *c != '\0' ) *c = '\0' ;
    }
    else if( !strncmp( cp, "#gen_mfunc_frame", 16 ) ) {
      yyval.inst->type = JD_debug_gen_mfunc_frame_info ;
      while( !isspace(*c) ) c++ ;
      while( isspace(*c) ) c++ ;
      yyval.inst->is.debug_gen_mfunc_frame_info.mfunc = c ;
      while( !isspace(*c) && ( *c != '\0') ) c++ ;
      if( *c != '\0' ) *c = '\0' ;
    }
    else if( !strncmp( cp, "#gen_afunc_frame", 16 ) ) {
      yyval.inst->type = JD_debug_gen_afunc_frame_info ;
      while( !isspace(*c) ) c++ ;
      while( isspace(*c) ) c++ ;
      yyval.inst->is.debug_gen_mfunc_frame_info.mfunc = c ;
      while( !isspace(*c) && ( *c != '\0') ) c++ ;
      if( *c != '\0' ) *c = '\0' ;
    }
    else if( !strncmp( cp, "#gen_advice_frame", 17 ) ) {
      yyval.inst->type = JD_debug_gen_advice_frame_info ;
      while( !isspace(*c) ) c++ ;
      while( isspace(*c) ) c++ ;
      yyval.inst->is.debug_gen_mfunc_frame_info.mfunc = c ;
      while( !isspace(*c) && ( *c != '\0') ) c++ ;
      if( *c != '\0' ) *c = '\0' ;
    }
    else if( !strncmp( cp, "#argument", 9 ) ) {
      yyval.inst->type = JD_debug_argument_info ;
      while( !isspace(*c) ) c++ ;
      while( isspace(*c) ) c++ ;
      cp = c ;
      while( !isspace(*c) ) c++ ;
      *c = '\0' ;
      yyval.inst->is.debug_var_argument_info.name = cp ;
      c++ ;
      while( isspace(*c) ) c++ ;
      yyval.inst->is.debug_var_argument_info.level = atoi(c) ;
      while( !isspace(*c) ) c++ ;
      while( isspace(*c) ) c++ ;
      cp = c ;
      while( !isspace(*c) && ( *c != '\0') ) c++ ;
      if( *c != '\0' ) *c = '\0' ;
      yyval.inst->is.debug_var_argument_info.type = cp ;
      c++ ;
      while( isspace(*c) && ( *c != '\0') ) c++ ;
      yyval.inst->is.debug_var_argument_info.ub = atoi(c) ;
      while( !isspace(*c) && ( *c != '\0') ) c++ ;
      while( isspace(*c) && ( *c != '\0') ) c++ ;
      yyval.inst->is.debug_var_argument_info.lb = atoi(c) ;
    }
    else if( !strncmp( cp, "#variable", 9 ) ) {
      yyval.inst->type = JD_debug_variable_info ;
      while( !isspace(*c) ) c++ ;
      while( isspace(*c) ) c++ ;
      cp = c ;
      while( !isspace(*c) && ( *c != '\0') ) c++ ;
      *c = '\0' ;
      yyval.inst->is.debug_var_argument_info.name = cp ;
      c++ ;
      while( isspace(*c) ) c++ ;
      yyval.inst->is.debug_var_argument_info.level = atoi(c) ;
      while( !isspace(*c) ) c++ ;
      while( isspace(*c) ) c++ ;
      cp = c ;
      while( !isspace(*c) && ( *c != '\0') ) c++ ;
      if( *c != '\0' ) *c = '\0' ;
      yyval.inst->is.debug_var_argument_info.type = cp ;
      c++ ;
      while( isspace(*c) && ( *c != '\0') ) c++ ;
      yyval.inst->is.debug_var_argument_info.ub = atoi(c) ;
      while( !isspace(*c) && ( *c != '\0') ) c++ ;
      while( isspace(*c) && ( *c != '\0') ) c++ ;
      yyval.inst->is.debug_var_argument_info.lb = atoi(c) ;
    }
    else if( !strncmp( cp, "#enum", 5 ) ) {
      int n ;
      yyval.inst->type = JD_debug_enum_info ;
      while( !isspace(*c) ) c++ ;
      while( isspace(*c) ) c++ ;
      cp = c ;
      while( !isspace(*c) ) c++ ;
      *c = '\0' ;
      yyval.inst->is.debug_enum_info.name = cp ;
      c++ ;
      while( isspace(*c) && (*c != '\n') ) c++ ;
      cp = c ;
      n = 0 ;
      while( (*c != '\n') && (*c != '\0') ) {
        n++ ;
        while( !isspace(*c) ) c++ ;
        while( isspace(*c) && (*c != '\n') && (*c != '\0') ) c++ ;
      }
      yyval.inst->is.debug_enum_info.num_member = n ;
      if( n ) {
        int i ;
        yyval.inst->is.debug_enum_info.member = 
          (char **)calloc( n, sizeof(char *) ) ;
        c = cp ;
        for( i = 0 ; i < n ; i++ ) {
          cp = c ;
          while( !isspace(*c) && ( *c != '\0') ) c++ ;
          *c = '\0' ; c++ ;
          yyval.inst->is.debug_enum_info.member[i] = cp ;
          if( i < n-1 ) while( isspace(*c) ) c++ ;
          cp = c ;
        }
      }
    }
    else if( !strncmp( cp, "#block", 6 ) ) {
      yyval.inst->type = JD_debug_block_info ;
      while( !isspace(*c) ) c++ ;
      while( isspace(*c) ) c++ ;
      yyval.inst->is.debug_block_info.level = atoi(c) ;
    }
    else if( !strncmp( cp, "#endblock", 9 ) ) {
      yyval.inst->type = JD_debug_endblock_info ;
      while( !isspace(*c) ) c++ ;
      while( isspace(*c) ) c++ ;
      yyval.inst->is.debug_endblock_info.level = atoi(c) ;
    }
    else if( !strncmp( cp, "#member_variable", 16 ) ) {
      yyval.inst->type = JD_debug_member_variable_info ;
      while( !isspace(*c) ) c++ ;
      while( isspace(*c) ) c++ ;
      cp = c ;
      while( !isspace(*c) ) c++ ;
      *c = '\0' ;
      yyval.inst->is.debug_member_variable_info.class = cp ;
      c++ ;
      while( isspace(*c) ) c++ ;
      cp = c ;
      while( !isspace(*c) ) c++ ;
      *c = '\0' ;
      yyval.inst->is.debug_member_variable_info.name = cp ;
      c++ ;
      while( isspace(*c) ) c++ ;
      yyval.inst->is.debug_member_variable_info.index = atoi(c) ;
      while( !isspace(*c) && ( *c != '\0') ) c++ ;
      while( isspace(*c) && ( *c != '\0') ) c++ ;
      yyval.inst->is.debug_member_variable_info.level = atoi(c) ;
    }
    else if( !strncmp( cp, "#aspect_variable", 16 ) ) {
      JD_name *cls, *var ;
      yyval.inst->type = JD_debug_aspect_variable_info ;
      while( !isspace(*c) ) c++ ;
      while( isspace(*c) ) c++ ;
      cp = c ;
      while( !isspace(*c) ) c++ ;
      *c = '\0' ;
      yyval.inst->is.debug_member_variable_info.class = cp ;
      cls = find_local_name( cp ) ;
      if( !cls ) {
        cls = find_global_name( cp ) ;
      }
      c++ ;
      while( isspace(*c) ) c++ ;
      cp = c ;
      while( !isspace(*c) ) c++ ;
      *c = '\0' ;
      yyval.inst->is.debug_member_variable_info.name = cp ;
      if( cls ) {
        var = find_name_in_scope( cp, cls->child ) ;
        if( var && var->static_name )
          yyval.inst->is.debug_member_variable_info.static_name = var->static_name ;
      }
      c++ ;
      while( isspace(*c) ) c++ ;
      while( !isspace(*c) && ( *c != '\0') ) c++ ;
      while( isspace(*c) && ( *c != '\0') ) c++ ;
      yyval.inst->is.debug_member_variable_info.level = atoi(c) ;
    }
  ;
    break;}
case 29:

{
    /* port_info */
    yyval.inst = ALLOC(vcode_info) ;
    yyval.inst->type = JD_port_info ;
    yyval.inst->is.port.direction = yyvsp[-8].integer_value ;
    yyval.inst->is.port.ub = atoi(yyvsp[-6].string_value.name) ;
    yyval.inst->is.port.lb = atoi(yyvsp[-4].string_value.name) ;
    yyval.inst->is.port.name = yyvsp[-2].string_value.name ;
    yyval.inst->is.port.attribs = yyvsp[-1].inst ;
  ;
    break;}
case 30:

{
    yyval.integer_value = yyvsp[0].keyword ; 
  ;
    break;}
case 31:

{
    yyval.integer_value = yyvsp[0].keyword ; 
  ;
    break;}
case 32:

{
    yyval.integer_value = yyvsp[0].keyword ; 
  ;
    break;}
case 33:

{
    yyval.inst = NULL ;
  ;
    break;}
case 34:

{
    yyval.inst = yyvsp[0].inst ; 
  ;
    break;}
case 35:

{
    yyval.inst = yyvsp[0].inst ; 
  ;
    break;}
case 36:

{
    /* VcodeChain */
    int *pp = NULL ;
    vcode_info *last = yyvsp[-1].inst ;
    if( JD_first_inst == NULL ) JD_first_inst = yyvsp[-1].inst ;
    if( yyvsp[-1].inst ) {
      while(last->next) {
        if( last == yyvsp[0].inst ) {
          printf( "VcodeChain loop detected\n" ) ;
          *pp = 0 ;
        }
        last = last->next ;
      }
      last->next = yyvsp[0].inst ;
      yyval.inst = yyvsp[-1].inst ;
    }
    else yyval.inst = yyvsp[0].inst ;
  ;
    break;}
case 37:

{
    /* port_access_attrib */
    yyval.inst = ALLOC(vcode_info) ;
    yyval.inst->type = JD_port_access_attrib ;
    yyval.inst->is.port_access_attrib.drive_sample = yyvsp[-3].keyword ;
    yyval.inst->is.port_access_attrib.edge = yyvsp[-2].integer_value ;
    yyval.inst->is.port_access_attrib.skew = yyvsp[0].integer_value ;
  ;
    break;}
case 38:

{
    /* port_access_attrib */
    yyval.inst = ALLOC(vcode_info) ;
    yyval.inst->type = JD_port_access_attrib ;
    yyval.inst->is.port_access_attrib.drive_sample = yyvsp[-3].keyword ;
    yyval.inst->is.port_access_attrib.edge = yyvsp[-2].integer_value ;
    yyval.inst->is.port_access_attrib.skew = yyvsp[0].integer_value ;
  ;
    break;}
case 39:

{
    /* depth */
    yyval.inst = ALLOC(vcode_info) ;
    yyval.inst->type = JD_port_depth_attrib ;
    yyval.inst->is.int_value = atoi(yyvsp[0].string_value.name) ;
  ;
    break;}
case 40:

{
    /* path */
    yyval.inst = ALLOC(vcode_info) ;
    yyval.inst->type = JD_port_path_attrib ;
    yyval.inst->is.string = yyvsp[0].string_value.name ;
  ;
    break;}
case 41:

{
    /* port_clock_attirb */
    yyval.inst = ALLOC(vcode_info) ;
    yyval.inst->type = JD_port_clock_attrib ;
    yyval.inst->is.port_clock_attrib.port = yyvsp[0].string_value.name ;
  ;
    break;}
case 42:

{
    /* portset_clock_attirb */
    yyval.inst = ALLOC(vcode_info) ;
    yyval.inst->type = JD_port_clock_attrib ;
    yyval.inst->is.port_clock_attrib.pset = yyvsp[-2].string_value.name ;
    yyval.inst->is.port_clock_attrib.port = yyvsp[0].string_value.name ;
    /* link to symbol table: TBI */
  ;
    break;}
case 43:

{
    /* int_value */
    yyval.integer_value = atoi(yyvsp[0].string_value.name) ;
  ;
    break;}
case 44:

{
    /* minus_int_value */
    yyval.integer_value = -atoi(yyvsp[0].string_value.name) ;
  ;
    break;}
case 45:

{
    /* portset_info */
    yyval.inst = ALLOC(vcode_info) ;
    yyval.inst->type = JD_portset_info ;
    yyval.inst->is.portset.name = yyvsp[-5].string_value.name ;
    yyval.inst->is.portset.port_infos = yyvsp[-2].inst ;
  ;
    break;}
case 46:

{
    yyval.inst = yyvsp[0].inst ; 
  ;
    break;}
case 47:

{
    /* VcodeChain */
    int *pp = NULL ;
    vcode_info *last = yyvsp[-1].inst ;
    if( JD_first_inst == NULL ) JD_first_inst = yyvsp[-1].inst ;
    if( yyvsp[-1].inst ) {
      while(last->next) {
        if( last == yyvsp[0].inst ) {
          printf( "VcodeChain loop detected\n" ) ;
          *pp = 0 ;
        }
        last = last->next ;
      }
      last->next = yyvsp[0].inst ;
      yyval.inst = yyvsp[-1].inst ;
    }
    else yyval.inst = yyvsp[0].inst ;
  ;
    break;}
case 48:

{
    /* port_connection_info */
    yyval.inst = ALLOC(vcode_info) ;
    yyval.inst->type = JD_connection_info ;
    yyval.inst->is.connection_info.port = yyvsp[-3].string_value.name ;
    yyval.inst->is.connection_info.path = yyvsp[-1].string_value.name ;
  ;
    break;}
case 49:

{
    /* portset_connection_info */
    yyval.inst = ALLOC(vcode_info) ;
    yyval.inst->type = JD_connection_info ;
    yyval.inst->is.connection_info.portset = yyvsp[-4].string_value.name ;
    yyval.inst->is.connection_info.port = yyvsp[-3].string_value.name ;
    yyval.inst->is.connection_info.path = yyvsp[-1].string_value.name ;
  ;
    break;}
case 50:

{
    /* variable_dcl */
    yyval.inst = ALLOC(vcode_info) ;
    yyval.inst->type = JD_variable_dcl ;
    yyval.inst->index = yyval.inst->is.variable_dcl.index = atoi(yyvsp[-5].string_value.name) ;
    yyval.inst->is.variable_dcl.data_type = yyvsp[-4].inst ;
    yyval.inst->is.variable_dcl.name = yyvsp[-3].string_value.name ;
    yyval.inst->is.variable_dcl.array_range = yyvsp[-2].inst ;
    yyval.inst->is.variable_dcl.var_attrib = yyvsp[-1].inst ;
    yyval.inst->is.variable_dcl.class = Current_Class ;
  ;
    break;}
case 51:

{
    /* data_type */
    yyval.inst = ALLOC(vcode_info) ;
    yyval.inst->is.data_type.type = yyvsp[0].keyword ;
  ;
    break;}
case 52:

{
    /* data_type */
    yyval.inst = ALLOC(vcode_info) ;
    yyval.inst->is.data_type.type = yyvsp[0].keyword ;
  ;
    break;}
case 53:

{
    /* data_type */
    yyval.inst = ALLOC(vcode_info) ;
    yyval.inst->is.data_type.type = yyvsp[0].keyword ;
  ;
    break;}
case 54:

{
    /* data_type */
    yyval.inst = ALLOC(vcode_info) ;
    yyval.inst->is.data_type.type = yyvsp[0].keyword ;
  ;
    break;}
case 55:

{
    /* data_type */
    yyval.inst = ALLOC(vcode_info) ;
    yyval.inst->is.data_type.type = yyvsp[0].keyword ;
  ;
    break;}
case 56:

{
    /* bit_data_type */
    yyval.inst = ALLOC(vcode_info) ;
    yyval.inst->is.data_type.type = yyvsp[-5].keyword ;
    yyval.inst->is.data_type.ub = atoi(yyvsp[-3].string_value.name) ;
    yyval.inst->is.data_type.lb = atoi(yyvsp[-1].string_value.name) ;
  ;
    break;}
case 57:

{
    /* named_data_type */
    yyval.inst = ALLOC(vcode_info) ;
    yyval.inst->is.data_type.type = yyvsp[-1].keyword ;
    yyval.inst->is.data_type.name = yyvsp[0].string_value.name ;
    /* link to synbol table: TBI */
  ;
    break;}
case 58:

{
    /* named_data_type */
    yyval.inst = ALLOC(vcode_info) ;
    yyval.inst->is.data_type.type = yyvsp[-1].keyword ;
    yyval.inst->is.data_type.name = yyvsp[0].string_value.name ;
    /* link to synbol table: TBI */
  ;
    break;}
case 59:

{
    /* data_type */
    yyval.inst = ALLOC(vcode_info) ;
    yyval.inst->is.data_type.type = yyvsp[0].keyword ;
  ;
    break;}
case 60:

{
    /* data_type */
    yyval.inst = ALLOC(vcode_info) ;
    yyval.inst->is.data_type.type = yyvsp[0].keyword ;
  ;
    break;}
case 61:

{
    /* data_type */
    yyval.inst = ALLOC(vcode_info) ;
    yyval.inst->is.data_type.type = yyvsp[0].keyword ;
  ;
    break;}
case 62:

{
    /* named_data_type */
    yyval.inst = ALLOC(vcode_info) ;
    yyval.inst->is.data_type.type = yyvsp[-1].keyword ;
    yyval.inst->is.data_type.name = yyvsp[0].string_value.name ;
    /* link to synbol table: TBI */
  ;
    break;}
case 63:

{
    /* array_range */
    yyval.inst = ALLOC(vcode_info) ;
    yyval.inst->is.array_range.type = yyvsp[0].keyword ;
  ;
    break;}
case 64:

{
    /* array_array_range */
    yyval.inst = ALLOC(vcode_info) ;
    yyval.inst->is.array_range.type = yyvsp[-1].keyword ;
    yyval.inst->is.array_range.size = atoi(yyvsp[0].string_value.name) ;
  ;
    break;}
case 65:

{
    /* marray_array_range */
    yyval.inst = ALLOC(vcode_info) ;
    yyval.inst->is.array_range.type = yyvsp[-4].keyword ;
    yyval.inst->is.array_range.size = atoi(yyvsp[-3].string_value.name) ;
    yyval.inst->is.array_range.range_list = yyvsp[-1].inst ;
  ;
    break;}
case 66:

{
    /* array_range */
    yyval.inst = ALLOC(vcode_info) ;
    yyval.inst->is.array_range.type = yyvsp[0].keyword ;
  ;
    break;}
case 67:

{
    /* array_range */
    yyval.inst = ALLOC(vcode_info) ;
    yyval.inst->is.array_range.type = yyvsp[0].keyword ;
  ;
    break;}
case 68:

{
    /* array_range */
    yyval.inst = ALLOC(vcode_info) ;
    yyval.inst->is.array_range.type = yyvsp[0].keyword ;
  ;
    break;}
case 69:

{
    /* a_range */
    yyval.inst = ALLOC(vcode_info) ;
    yyval.inst->is.int_value = atoi(yyvsp[0].string_value.name) ;
  
  ;
    break;}
case 70:

{
    /* range_list */
    vcode_info *new, *pre ;
    yyval.inst = yyvsp[-1].inst ;
    new = ALLOC(vcode_info) ;
    new->is.int_value = atoi(yyvsp[0].string_value.name) ;
    pre = yyval.inst ;
    while( pre->next ) pre = pre->next ;
    pre->next = new ;
  ;
    break;}
case 71:

{
    yyval.inst = NULL ;
  ;
    break;}
case 72:

{
    yyval.inst = yyvsp[0].inst ; 
  ;
    break;}
case 73:

{
    /* attrib */
    yyval.inst = ALLOC(vcode_info) ;
    switch( yyvsp[0].integer_value ) {
      case VI_static:
        yyval.inst->is.attrib.is_static = 1 ;
        break ;
      case VI_clone:
        yyval.inst->is.attrib.is_clone = 1 ;
        break ;
      case VI_extern:
        yyval.inst->is.attrib.is_extern = 1 ;
        break ;
      case VI_local:
        yyval.inst->is.attrib.is_local = 1 ;
        break ;
      case VI_global:
        yyval.inst->is.attrib.is_global = 1 ;
        break ;
      case VI_protected:
        yyval.inst->is.attrib.is_protected = 1 ;
        break ;
      case VI_final:
        yyval.inst->is.attrib.is_final = 1 ;
        break ;
      case VI_abstract:
        yyval.inst->is.attrib.is_abstract = 1 ;
        break ;
      case VI_export:
        yyval.inst->is.attrib.is_export = 1 ;
        break ;
      case VI_leaf:
        yyval.inst->is.attrib.is_leaf = 1 ;
        break ;
      case VI_overwrite:
        yyval.inst->is.attrib.is_overwrite = 1 ;
        break ;
    }
  
  ;
    break;}
case 74:

{
    /* var_attribs */
    yyval.inst = yyvsp[-1].inst ;
    switch( yyvsp[0].integer_value ) {
      case VI_static:
        yyval.inst->is.attrib.is_static = 1 ;
        break ;
      case VI_clone:
        yyval.inst->is.attrib.is_clone = 1 ;
        break ;
      case VI_extern:
        yyval.inst->is.attrib.is_extern = 1 ;
        break ;
      case VI_local:
        yyval.inst->is.attrib.is_local = 1 ;
        break ;
      case VI_global:
        yyval.inst->is.attrib.is_global = 1 ;
        break ;
      case VI_protected:
        yyval.inst->is.attrib.is_protected = 1 ;
        break ;
      case VI_final:
        yyval.inst->is.attrib.is_final = 1 ;
        break ;
      case VI_abstract:
        yyval.inst->is.attrib.is_abstract = 1 ;
        break ;
      case VI_export:
        yyval.inst->is.attrib.is_export = 1 ;
        break ;
      case VI_leaf:
        yyval.inst->is.attrib.is_leaf = 1 ;
        break ;
      case VI_overwrite:
        yyval.inst->is.attrib.is_overwrite = 1 ;
        break ;
    }
  
  ;
    break;}
case 75:

{
    yyval.integer_value = yyvsp[0].keyword ; 
  ;
    break;}
case 76:

{
    yyval.integer_value = yyvsp[0].keyword ; 
  ;
    break;}
case 77:

{
    yyval.integer_value = yyvsp[0].keyword ; 
  ;
    break;}
case 78:

{
    yyval.integer_value = yyvsp[0].keyword ; 
  ;
    break;}
case 79:

{
    yyval.integer_value = yyvsp[0].keyword ; 
  ;
    break;}
case 80:

{
    yyval.integer_value = yyvsp[0].keyword ; 
  ;
    break;}
case 81:

{
    yyval.integer_value = yyvsp[0].keyword ; 
  ;
    break;}
case 82:

{
    /* enum_dcl */
    yyval.inst = ALLOC(vcode_info) ;
    yyval.inst->type = JD_enum_dcl ;
    yyval.inst->is.enum_dcl.name = yyvsp[-2].string_value.name ;
    yyval.inst->is.enum_dcl.members = yyvsp[-1].inst ;
  ;
    break;}
case 83:

{
    /* a_string */
    yyval.inst = ALLOC(vcode_info) ;
    yyval.inst->is.string = yyvsp[0].string_value.name ;
  
  ;
    break;}
case 84:

{
    /* string_list */
    vcode_info *new, *pre ;
    yyval.inst = yyvsp[-1].inst ;
    new = ALLOC(vcode_info) ;
    new->is.string = yyvsp[0].string_value.name ;
    pre = yyval.inst ;
    while( pre->next ) pre = pre->next ;
    pre->next = new ;
  ;
    break;}
case 85:

{
    /* function_info */
    yyval.inst = ALLOC(vcode_info) ;
    yyval.inst->type = JD_function_info ;
    yyval.inst->index = atoi(yyvsp[-10].string_value.name) ;
    yyval.inst->is.function_info.func_options = yyvsp[-9].inst ;
    yyval.inst->is.function_info.data_type = yyvsp[-8].inst ;
    yyval.inst->is.function_info.name = yyvsp[-7].string_value.name ;
    yyval.inst->is.function_info.sys_flag = yyvsp[-7].string_value.sys_flag ;
    yyval.inst->srclinenum = atoi(yyvsp[-6].string_value.name) ;
    yyval.inst->srcfilename = yyvsp[-5].string_value.name ;
    yyval.inst->is.function_info.arguments = yyvsp[-2].inst ;
    yyval.inst->is.function_info.class = Current_Class ;
    
  ;
    break;}
case 86:

{
    /* null_attrib */
    yyval.inst = ALLOC(vcode_info) ;
  ;
    break;}
case 87:

{
    yyval.inst = yyvsp[0].inst ; 
  ;
    break;}
case 88:

{
    /* attrib */
    yyval.inst = ALLOC(vcode_info) ;
    switch( yyvsp[0].integer_value ) {
      case VI_static:
        yyval.inst->is.attrib.is_static = 1 ;
        break ;
      case VI_clone:
        yyval.inst->is.attrib.is_clone = 1 ;
        break ;
      case VI_extern:
        yyval.inst->is.attrib.is_extern = 1 ;
        break ;
      case VI_local:
        yyval.inst->is.attrib.is_local = 1 ;
        break ;
      case VI_global:
        yyval.inst->is.attrib.is_global = 1 ;
        break ;
      case VI_protected:
        yyval.inst->is.attrib.is_protected = 1 ;
        break ;
      case VI_final:
        yyval.inst->is.attrib.is_final = 1 ;
        break ;
      case VI_abstract:
        yyval.inst->is.attrib.is_abstract = 1 ;
        break ;
      case VI_export:
        yyval.inst->is.attrib.is_export = 1 ;
        break ;
      case VI_leaf:
        yyval.inst->is.attrib.is_leaf = 1 ;
        break ;
      case VI_overwrite:
        yyval.inst->is.attrib.is_overwrite = 1 ;
        break ;
    }
  
  ;
    break;}
case 89:

{
    /* var_attribs */
    yyval.inst = yyvsp[-1].inst ;
    switch( yyvsp[0].integer_value ) {
      case VI_static:
        yyval.inst->is.attrib.is_static = 1 ;
        break ;
      case VI_clone:
        yyval.inst->is.attrib.is_clone = 1 ;
        break ;
      case VI_extern:
        yyval.inst->is.attrib.is_extern = 1 ;
        break ;
      case VI_local:
        yyval.inst->is.attrib.is_local = 1 ;
        break ;
      case VI_global:
        yyval.inst->is.attrib.is_global = 1 ;
        break ;
      case VI_protected:
        yyval.inst->is.attrib.is_protected = 1 ;
        break ;
      case VI_final:
        yyval.inst->is.attrib.is_final = 1 ;
        break ;
      case VI_abstract:
        yyval.inst->is.attrib.is_abstract = 1 ;
        break ;
      case VI_export:
        yyval.inst->is.attrib.is_export = 1 ;
        break ;
      case VI_leaf:
        yyval.inst->is.attrib.is_leaf = 1 ;
        break ;
      case VI_overwrite:
        yyval.inst->is.attrib.is_overwrite = 1 ;
        break ;
    }
  
  ;
    break;}
case 90:

{
    yyval.integer_value = yyvsp[0].keyword ; 
  ;
    break;}
case 91:

{
    yyval.integer_value = yyvsp[0].keyword ; 
  ;
    break;}
case 92:

{
    yyval.integer_value = yyvsp[0].keyword ; 
  ;
    break;}
case 93:

{
    yyval.integer_value = yyvsp[0].keyword ; 
  ;
    break;}
case 94:

{
    yyval.integer_value = yyvsp[0].keyword ; 
  ;
    break;}
case 95:

{
    yyval.integer_value = yyvsp[0].keyword ; 
  ;
    break;}
case 96:

{
    yyval.integer_value = yyvsp[0].keyword ; 
  ;
    break;}
case 97:

{
    yyval.integer_value = yyvsp[0].keyword ; 
  ;
    break;}
case 98:

{
    yyval.inst = NULL ;
  ;
    break;}
case 99:

{
    yyval.inst = yyvsp[0].inst ; 
  ;
    break;}
case 100:

{
    yyval.inst = yyvsp[0].inst ; 
  ;
    break;}
case 101:

{
    /* VcodeChain */
    int *pp = NULL ;
    vcode_info *last = yyvsp[-1].inst ;
    if( JD_first_inst == NULL ) JD_first_inst = yyvsp[-1].inst ;
    if( yyvsp[-1].inst ) {
      while(last->next) {
        if( last == yyvsp[0].inst ) {
          printf( "VcodeChain loop detected\n" ) ;
          *pp = 0 ;
        }
        last = last->next ;
      }
      last->next = yyvsp[0].inst ;
      yyval.inst = yyvsp[-1].inst ;
    }
    else yyval.inst = yyvsp[0].inst ;
  ;
    break;}
case 102:

{
    /* argument */
    yyval.inst = ALLOC(vcode_info) ;
    yyval.inst->is.argument.var = yyvsp[-4].integer_value ;
    yyval.inst->is.argument.data_type = yyvsp[-3].inst ;
    yyval.inst->is.argument.name = yyvsp[-2].string_value.name ;
    yyval.inst->is.argument.array_range = yyvsp[-1].inst ;
  ;
    break;}
case 103:

{
    /* formal_argument */
    yyval.inst = NULL ;
  ;
    break;}
case 104:

{
    /* formal_argument */
    yyval.inst = NULL ;
  ;
    break;}
case 105:

{
    /* formal_argument */
    yyval.inst = NULL ;
  ;
    break;}
case 106:

{
    /* formal_argument */
    yyval.inst = NULL ;
  ;
    break;}
case 107:

{
    /* formal_argument */
    yyval.inst = NULL ;
  ;
    break;}
case 108:

{
    yyval.integer_value = 0 ;
  ;
    break;}
case 109:

{
    yyval.integer_value = 1 ;
  ;
    break;}
case 110:

{
    yyval.inst = yyvsp[0].inst ; 
  ;
    break;}
case 111:

{
    /* VcodeChain */
    int *pp = NULL ;
    vcode_info *last = yyvsp[-1].inst ;
    if( JD_first_inst == NULL ) JD_first_inst = yyvsp[-1].inst ;
    if( yyvsp[-1].inst ) {
      while(last->next) {
        if( last == yyvsp[0].inst ) {
          printf( "VcodeChain loop detected\n" ) ;
          *pp = 0 ;
        }
        last = last->next ;
      }
      last->next = yyvsp[0].inst ;
      yyval.inst = yyvsp[-1].inst ;
    }
    else yyval.inst = yyvsp[0].inst ;
  ;
    break;}
case 112:

{
    /* call_pointcut */
    yyval.inst = ALLOC(vcode_info) ;
    yyval.inst->type = JD_call_pointcut ;
    yyval.inst->is.pointcut_info.next = yyvsp[-2].inst ;
  ;
    break;}
case 113:

{
    /* return_pointcut */
    yyval.inst = ALLOC(vcode_info) ;
    yyval.inst->type = JD_return_pointcut ;
    yyval.inst->is.pointcut_info.next = yyvsp[-2].inst ;
  ;
    break;}
case 114:

{
    /* call_pointcut */
    yyval.inst = ALLOC(vcode_info) ;
    yyval.inst->type = JD_overwrite_pointcut ;
    yyval.inst->is.pointcut_info.next = yyvsp[-2].inst ;
  ;
    break;}
case 115:

{
    yyval.inst = yyvsp[0].inst ; 
  ;
    break;}
case 116:

{
    /* pointcut_chain */
    if( yyvsp[-1].inst ) {
      vcode_info *a ;
      a = yyvsp[-1].inst ;
      while( a->is.pointcut_info.next ) 
        a = a->is.pointcut_info.next ;
      a->is.pointcut_info.next = yyvsp[0].inst ;
    }
    yyval.inst = yyvsp[-1].inst ;
  ;
    break;}
case 117:

{
    /* pointcut_name */
    yyval.inst = ALLOC(vcode_info) ;
    yyval.inst->type = JD_pointcut_name ;
    yyval.inst->is.pointcut_info.name = yyvsp[0].string_value.name ;
  
  ;
    break;}
case 118:

{
    /* pointcut_name */
    yyval.inst = ALLOC(vcode_info) ;
    yyval.inst->type = JD_pointcut_string ;
    yyval.inst->is.pointcut_info.name = yyvsp[0].string_value.name ;
  
  ;
    break;}
case 119:

{
    /* advice_info */
    yyval.inst = ALLOC(vcode_info) ;
    yyval.inst->type = JD_advice_info ;
    yyval.inst->index = atoi(yyvsp[-12].string_value.name) ;
    yyval.inst->is.function_info.func_options = yyvsp[-11].inst ;
    yyval.inst->is.function_info.data_type = yyvsp[-10].inst ;
    yyval.inst->is.function_info.name = yyvsp[-9].string_value.name ;
    yyval.inst->srclinenum = atoi(yyvsp[-8].string_value.name) ;
    yyval.inst->srcfilename = yyvsp[-7].string_value.name ;
    yyval.inst->is.function_info.pointcut = yyvsp[-5].inst ;
    yyval.inst->is.function_info.arguments = yyvsp[-2].inst ;
    yyval.inst->is.function_info.class = Current_Class ;
    
  ;
    break;}
case 120:

{
    /* class_info_begin */
    yyval.inst = ALLOC(vcode_info) ;
    yyval.inst->type = JD_class_info ;
    yyval.inst->is.class.name = yyvsp[-6].string_value.name ;
    yyval.inst->is.class.sys_flag = yyvsp[-6].string_value.sys_flag ;
    yyval.inst->is.class.parent = yyvsp[-5].string ;
    yyval.inst->is.class.attribs = yyvsp[-4].inst ;
    yyval.inst->srclinenum = atoi(yyvsp[-3].string_value.name) ;
    yyval.inst->srcfilename = yyvsp[-2].string_value.name ;
    Current_Class = yyval.inst ;
  ;
    break;}
case 121:

{
    /* class_info_end */
    yyval.inst = Current_Class ;
    Current_Class = NULL ;
    yyval.inst->is.class.members = yyvsp[-2].inst ;
  ;
    break;}
case 122:

{
    /* aspect_info_begin */
    yyval.inst = ALLOC(vcode_info) ;
    yyval.inst->type = JD_aspect_info ;
    yyval.inst->is.class.name = yyvsp[-4].string_value.name ;
    yyval.inst->srclinenum = atoi(yyvsp[-3].string_value.name) ;
    yyval.inst->srcfilename = yyvsp[-2].string_value.name ;
    Current_Class = yyval.inst ;
  ;
    break;}
case 123:

{
    /* aspect_info_end */
    yyval.inst = Current_Class ;
    Current_Class = NULL ;
    yyval.inst->is.class.members = yyvsp[-2].inst ;
  ;
    break;}
case 124:

{
    /* verilog_task_info */
    yyval.inst = ALLOC(vcode_info) ;
    yyval.inst->type = JD_veri_task_info ;
    yyval.inst->is.function_info.name = yyvsp[-6].string_value.name ;
    yyval.inst->is.function_info.arguments = yyvsp[-3].inst ;
    yyval.inst->is.function_info.path = yyvsp[-1].string_value.name ;
  ;
    break;}
case 125:

{
    yyval.string = NULL ;
  ;
    break;}
case 126:

{
    /* extention */
    yyval.string = yyvsp[0].string_value.name ;
  ;
    break;}
case 127:

{
    yyval.inst = NULL ;
  ;
    break;}
case 128:

{
    yyval.inst = yyvsp[0].inst ; 
  ;
    break;}
case 129:

{
    /* attrib */
    yyval.inst = ALLOC(vcode_info) ;
    switch( yyvsp[0].integer_value ) {
      case VI_static:
        yyval.inst->is.attrib.is_static = 1 ;
        break ;
      case VI_clone:
        yyval.inst->is.attrib.is_clone = 1 ;
        break ;
      case VI_extern:
        yyval.inst->is.attrib.is_extern = 1 ;
        break ;
      case VI_local:
        yyval.inst->is.attrib.is_local = 1 ;
        break ;
      case VI_global:
        yyval.inst->is.attrib.is_global = 1 ;
        break ;
      case VI_protected:
        yyval.inst->is.attrib.is_protected = 1 ;
        break ;
      case VI_final:
        yyval.inst->is.attrib.is_final = 1 ;
        break ;
      case VI_abstract:
        yyval.inst->is.attrib.is_abstract = 1 ;
        break ;
      case VI_export:
        yyval.inst->is.attrib.is_export = 1 ;
        break ;
      case VI_leaf:
        yyval.inst->is.attrib.is_leaf = 1 ;
        break ;
      case VI_overwrite:
        yyval.inst->is.attrib.is_overwrite = 1 ;
        break ;
    }
  
  ;
    break;}
case 130:

{
    /* var_attribs */
    yyval.inst = yyvsp[-1].inst ;
    switch( yyvsp[0].integer_value ) {
      case VI_static:
        yyval.inst->is.attrib.is_static = 1 ;
        break ;
      case VI_clone:
        yyval.inst->is.attrib.is_clone = 1 ;
        break ;
      case VI_extern:
        yyval.inst->is.attrib.is_extern = 1 ;
        break ;
      case VI_local:
        yyval.inst->is.attrib.is_local = 1 ;
        break ;
      case VI_global:
        yyval.inst->is.attrib.is_global = 1 ;
        break ;
      case VI_protected:
        yyval.inst->is.attrib.is_protected = 1 ;
        break ;
      case VI_final:
        yyval.inst->is.attrib.is_final = 1 ;
        break ;
      case VI_abstract:
        yyval.inst->is.attrib.is_abstract = 1 ;
        break ;
      case VI_export:
        yyval.inst->is.attrib.is_export = 1 ;
        break ;
      case VI_leaf:
        yyval.inst->is.attrib.is_leaf = 1 ;
        break ;
      case VI_overwrite:
        yyval.inst->is.attrib.is_overwrite = 1 ;
        break ;
    }
  
  ;
    break;}
case 131:

{
    yyval.integer_value = yyvsp[0].keyword ; 
  ;
    break;}
case 132:

{
    yyval.integer_value = yyvsp[0].keyword ; 
  ;
    break;}
case 133:

{
    yyval.integer_value = yyvsp[0].keyword ; 
  ;
    break;}
case 134:

{
    yyval.integer_value = yyvsp[0].keyword ; 
  ;
    break;}
case 135:

{
    yyval.inst = NULL ;
  ;
    break;}
case 136:

{
    yyval.inst = yyvsp[0].inst ; 
  ;
    break;}
case 137:

{
    yyval.inst = yyvsp[0].inst ; 
  ;
    break;}
case 138:

{
    /* VcodeChain */
    int *pp = NULL ;
    vcode_info *last = yyvsp[-1].inst ;
    if( JD_first_inst == NULL ) JD_first_inst = yyvsp[-1].inst ;
    if( yyvsp[-1].inst ) {
      while(last->next) {
        if( last == yyvsp[0].inst ) {
          printf( "VcodeChain loop detected\n" ) ;
          *pp = 0 ;
        }
        last = last->next ;
      }
      last->next = yyvsp[0].inst ;
      yyval.inst = yyvsp[-1].inst ;
    }
    else yyval.inst = yyvsp[0].inst ;
  ;
    break;}
case 139:

{
    yyval.inst = yyvsp[0].inst ; 
  ;
    break;}
case 140:

{
    yyval.inst = yyvsp[0].inst ; 
  ;
    break;}
case 141:

{
    yyval.inst = NULL ;
  ;
    break;}
case 142:

{
    yyval.inst = yyvsp[0].inst ; 
  ;
    break;}
case 143:

{
    yyval.inst = yyvsp[0].inst ; 
  ;
    break;}
case 144:

{
    /* VcodeChain */
    int *pp = NULL ;
    vcode_info *last = yyvsp[-1].inst ;
    if( JD_first_inst == NULL ) JD_first_inst = yyvsp[-1].inst ;
    if( yyvsp[-1].inst ) {
      while(last->next) {
        if( last == yyvsp[0].inst ) {
          printf( "VcodeChain loop detected\n" ) ;
          *pp = 0 ;
        }
        last = last->next ;
      }
      last->next = yyvsp[0].inst ;
      yyval.inst = yyvsp[-1].inst ;
    }
    else yyval.inst = yyvsp[0].inst ;
  ;
    break;}
case 145:

{
    yyval.inst = yyvsp[0].inst ; 
  ;
    break;}
case 146:

{
    yyval.inst = yyvsp[0].inst ; 
  ;
    break;}
case 147:

{
    yyval.inst = yyvsp[0].inst ; 
  ;
    break;}
case 148:

{
    /* static_var_dcl */
    JD_name *name, *cls, *var ;
    yyval.inst = ALLOC(vcode_info) ;
    yyval.inst->type = JD_static_var_info ;
    yyval.inst->is.static_var_dcl.class = yyvsp[-6].string_value.name ;
    yyval.inst->is.static_var_dcl.func = yyvsp[-5].string_value.name ;
    yyval.inst->is.static_var_dcl.data_type = yyvsp[-4].inst ;
    yyval.inst->is.static_var_dcl.name = yyvsp[-3].string_value.name ;
    yyval.inst->is.static_var_dcl.array_range = yyvsp[-2].inst ;
    yyval.inst->is.static_var_dcl.attribs = yyvsp[-1].inst ;
    
    if( yyvsp[-1].inst && yyvsp[-1].inst->is.attrib.is_local ) {
      name = find_local_static_name( yyvsp[-6].string_value.name, yyvsp[-5].string_value.name, yyvsp[-3].string_value.name ) ;
      cls = find_local_name( yyvsp[-6].string_value.name ) ;
    }
    else {
      name = find_static_name( yyvsp[-6].string_value.name, yyvsp[-5].string_value.name, yyvsp[-3].string_value.name ) ;
      cls = find_global_name( yyvsp[-6].string_value.name ) ;
    }
    if( cls ) {
      var = find_name_in_scope( yyvsp[-3].string_value.name, cls->child ) ;
    }
    else var = NULL ;
    if( name ) {
      if( name->type != JD_variable_name ) {
        ERROR_VARIABLE_NAME_TYPE_MISMATCH( yyval.inst->is.variable_dcl.name ) ;
      }
      else if( !comp_info( name->info, yyval.inst ) ) {
        ERROR_VARIABLE_NAME_ATTRIB_MISMATCH( yyval.inst->is.variable_dcl.name ) ;
      }
      yyval.inst = NULL ; 
    }
    else {
      if( yyvsp[-1].inst && yyvsp[-1].inst->is.attrib.is_local )
        name = add_local_static_name( yyvsp[-6].string_value.name, yyvsp[-5].string_value.name, yyvsp[-3].string_value.name ) ;
      else
        name = add_static_name( yyvsp[-6].string_value.name, yyvsp[-5].string_value.name, yyvsp[-3].string_value.name ) ;
      name->type = JD_variable_name ;
      name->info = yyval.inst ;
      yyval.inst->code_name = name ;
      if( var ) var->static_name = name ;
    }
  ;
    break;}
case 149:

{
    /* static_init_code */
    yyval.inst = ALLOC(vcode_info) ;
    yyval.inst->type = JD_stat_init_code ;
    yyval.inst->is.init_code.code = yyvsp[-2].code ;
  
  ;
    break;}
case 150:

{
    /* init_code */
    yyval.inst = ALLOC(vcode_info) ;
    yyval.inst->type = JD_init_code ;
    yyval.inst->is.init_code.code = yyvsp[-2].code ;
    
  ;
    break;}
case 151:

{
    /* class_init_code */
    yyval.inst = ALLOC(vcode_info) ;
    yyval.inst->type = JD_class_init_code ;
    yyval.inst->is.init_code.name = yyvsp[-4].string_value.name ;
    yyval.inst->is.init_code.code = yyvsp[-2].code ;
  
  ;
    break;}
case 152:

{
    JD_name *cls, *var ;
    vinst_info *code ;
    /* class_alloc_code */
    yyval.inst = ALLOC(vcode_info) ;
    yyval.inst->type = JD_class_alloc_code ;
    yyval.inst->is.named_code.class = yyvsp[-4].string_value.name ;
    yyval.inst->is.named_code.code = yyvsp[-2].code ;
    
    cls = find_name( yyvsp[-4].string_value.name ) ;
    cls->defined = 1 ;
    code = yyvsp[-2].code ;
    while( code ) {
      if( code->type == VVI_inst ) {
        if( code->itype == VI_alloc || code->itype == VI_alloc_static ) {
          var = find_name_in_scope( code->name, cls->child ) ;
          if( var->defined ) {
            ERROR_DUPLICATED_DEFINITION_OF_CLASS_MEMBER_VAR( 
              cls->name, var->name 
            ) ;
          }
          var->defined = 1 ;
        }
      }
      code = code->next ;
    }
  ;
    break;}
case 153:

{
    /* mfunc_code */
    JD_name *cls, *func ;
    yyval.inst = ALLOC(vcode_info) ;
    yyval.inst->type = JD_mfunc_code ;
    yyval.inst->is.named_code.class = yyvsp[-5].string_value.name ;
    yyval.inst->is.named_code.func = yyvsp[-4].string_value.name ;
    yyval.inst->is.named_code.code = yyvsp[-2].code ;
  
    cls = find_name( yyvsp[-5].string_value.name ) ;
    func = find_name_in_scope( yyvsp[-4].string_value.name, cls->child ) ;
    if( func->defined ) {
      ERROR_DUPLICATED_DEFINITION_OF_CLASS_MEMBER_FUNC( cls->name, func->name ) ;
    }
    yyval.inst->fname = func ;
    func->defined = 1 ;  
  ;
    break;}
case 154:

{
    /* func_code */
    JD_name *func ;
    yyval.inst = ALLOC(vcode_info) ;
    yyval.inst->type = JD_func_code ;
    yyval.inst->is.named_code.func = yyvsp[-4].string_value.name ;
    yyval.inst->is.named_code.code = yyvsp[-2].code ;
    func = find_name( yyvsp[-4].string_value.name ) ;
    if( func->defined ) {
      ERROR_DUPLICATED_DEFINITION_OF_FUNC( func->name ) ;
    }
    yyval.inst->fname = func ;
    func->defined = 1 ;  
    if( !strcmp(yyvsp[-4].string_value.name, "main") ) main_defined = 1 ;
  ;
    break;}
case 155:

{
    /* afunc_code */
    JD_name *cls, *func ;
    yyval.inst = ALLOC(vcode_info) ;
    yyval.inst->type = JD_afunc_code ;
    yyval.inst->is.named_code.class = yyvsp[-5].string_value.name ;
    yyval.inst->is.named_code.func = yyvsp[-4].string_value.name ;
    yyval.inst->is.named_code.code = yyvsp[-2].code ;
  
    cls = find_name( yyvsp[-5].string_value.name ) ;
    func = find_name_in_scope( yyvsp[-4].string_value.name, cls->child ) ;
    if( func->defined ) {
      ERROR_DUPLICATED_DEFINITION_OF_ASPECT_MEMBER_FUNC( cls->name, func->name ) ;
    }
    yyval.inst->fname = func ;
    func->defined = 1 ;  
  ;
    break;}
case 156:

{
    /* advice_code */
    JD_name *cls, *func ;
    yyval.inst = ALLOC(vcode_info) ;
    yyval.inst->type = JD_advice_code ;
    yyval.inst->is.named_code.class = yyvsp[-5].string_value.name ;
    yyval.inst->is.named_code.func = yyvsp[-4].string_value.name ;
    yyval.inst->is.named_code.code = yyvsp[-2].code ;
  
    cls = find_name( yyvsp[-5].string_value.name ) ;
    func = find_name_in_scope( yyvsp[-4].string_value.name, cls->child ) ;
    if( func->defined ) {
      ERROR_DUPLICATED_DEFINITION_OF_ASPECT_MEMBER_ADVICE(
        cls->name, func->name 
      ) ;
    }
    yyval.inst->fname = func ;
    func->defined = 1 ;  
  ;
    break;}
case 157:

{
    yyval.code = NULL ;
  ;
    break;}
case 158:

{
    yyval.code = yyvsp[0].code ; 
  ;
    break;}
case 159:

{
    yyval.code = yyvsp[0].code ;
    if( yyval.code ) {
      yyval.code->filename = (char *)input_file_name ;
      yyval.code->linenum = lex_line_num-1 ;
    }
  ;
    break;}
case 160:

{
    /* VinstChain */
    vinst_info *last = yyvsp[-1].code ;
    if( yyvsp[-1].code ) {
      while(last->next) {
        last = last->next ;
      }
      last->next = yyvsp[0].code ;
      yyval.code = yyvsp[-1].code ;
    }
    else yyval.code = yyvsp[0].code ;
    if( yyvsp[0].code ) {
      yyvsp[0].code->filename = (char *)input_file_name ;
      yyvsp[0].code->linenum = lex_line_num-1 ;
    }
  ;
    break;}
case 161:

{
    /* vinst_enum_dcl */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_enum_dcl ;
    yyval.code->vinfo = yyvsp[0].inst ;
  ;
    break;}
case 162:

{
    yyval.code = NULL ;
  ;
    break;}
case 163:

{
    /* debug_info_vinst */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_debug_info ;
    yyval.code->vinfo = yyvsp[0].inst ;
  ;
    break;}
case 164:

{
    /* label */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_label ;
    yyval.code->name = yyvsp[-2].string_value.name ;
    if( yyvsp[-2].string_value.name[0] == 'f' ) yyval.code->itype = 1 ;
    else yyval.code->itype = 0 ;
  ;
    break;}
case 165:

{
    /* simple */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-1].keyword ;  
  ;
    break;}
case 166:

{
    /* var_aloc */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-6].keyword ;
    yyval.code->index = atoi(yyvsp[-5].string_value.name) ;
    yyval.code->data_type = yyvsp[-4].inst ;
    yyval.code->name = yyvsp[-3].string_value.name ;
    yyval.code->array_range = yyvsp[-2].inst ;
    yyval.code->opt_flags = yyvsp[-1].inst ; 
  ;
    break;}
case 167:

{
    /* var_aloc */
    JD_name *name ;
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-8].keyword ; /* use 'static' as itype */
    yyval.code->data_type = yyvsp[-6].inst ;
    yyval.code->class = yyvsp[-5].string_value.name ;
    yyval.code->func = yyvsp[-4].string_value.name ;
    yyval.code->name = yyvsp[-3].string_value.name ;
    yyval.code->array_range = yyvsp[-2].inst ;
    yyval.code->opt_flags = yyvsp[-1].inst ; 
    name = find_local_static_name( yyvsp[-5].string_value.name, yyvsp[-4].string_value.name, yyvsp[-3].string_value.name ) ;
    if( !name ) name = find_static_name( yyvsp[-5].string_value.name, yyvsp[-4].string_value.name, yyvsp[-3].string_value.name ) ;
    if( !name ) {
      int len ;
      char *full_name ;
      len = strlen(yyvsp[-5].string_value.name)+strlen(yyvsp[-4].string_value.name)+strlen(yyvsp[-3].string_value.name)+5 ;
      full_name = (char *)calloc( len, sizeof(char) ) ;
      strcat( full_name, yyvsp[-5].string_value.name ) ;
      strcat( full_name, "." ) ;
      strcat( full_name, yyvsp[-4].string_value.name ) ;
      strcat( full_name, "." ) ;
      strcat( full_name, yyvsp[-3].string_value.name ) ;
      ERROR_VARIABLE_NAME_NOT_FOUND( full_name ) ;  
      exit(1) ;
    }
    yyval.code->code_name = name ;
  ;
    break;}
case 168:

{
    /* simple */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-1].keyword ;  
  ;
    break;}
case 169:

{
    /* new_obj */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-2].keyword ;
    yyval.code->name = yyvsp[-1].string_value.name ;
  ;
    break;}
case 170:

{
    /* new_obj */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-3].keyword ; 
    yyval.code->name = yyvsp[-2].string_value.name ;
  ;
    break;}
case 171:

{
    /* simple */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-1].keyword ;  
  ;
    break;}
case 172:

{
    /* simple */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-1].keyword ;  
  ;
    break;}
case 173:

{
    /* var_aloc */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-6].keyword ;
    yyval.code->index = atoi(yyvsp[-5].string_value.name) ;
    yyval.code->data_type = yyvsp[-4].inst ;
    yyval.code->name = yyvsp[-3].string_value.name ;
    yyval.code->array_range = yyvsp[-2].inst ;
    yyval.code->opt_flags = yyvsp[-1].inst ; 
  ;
    break;}
case 174:

{
    /* alloc_static */
    JD_name *name ;
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-8].keyword ;
    yyval.code->class = yyvsp[-7].string_value.name ;
    yyval.code->func = yyvsp[-6].string_value.name ;
    yyval.code->data_type = yyvsp[-4].inst ;
    yyval.code->name = yyvsp[-3].string_value.name ;
    yyval.code->array_range = yyvsp[-2].inst ;
    yyval.code->opt_flags = yyvsp[-1].inst ; 
    name = find_local_static_name( yyvsp[-7].string_value.name, yyvsp[-6].string_value.name, yyvsp[-3].string_value.name ) ;
    if( !name ) name = find_static_name( yyvsp[-7].string_value.name, yyvsp[-6].string_value.name, yyvsp[-3].string_value.name ) ;
    if( !name ) {
      int len ;
      char *full_name ;
      len = strlen(yyvsp[-7].string_value.name)+strlen(yyvsp[-6].string_value.name)+strlen(yyvsp[-3].string_value.name)+5 ;
      full_name = (char *)calloc( len, sizeof(char) ) ;
      strcat( full_name, yyvsp[-7].string_value.name ) ;
      strcat( full_name, "." ) ;
      strcat( full_name, yyvsp[-6].string_value.name ) ;
      strcat( full_name, "." ) ;
      strcat( full_name, yyvsp[-3].string_value.name ) ;
      ERROR_VARIABLE_NAME_NOT_FOUND( full_name ) ;  
      exit(1) ;
    }
    yyval.code->code_name = name ;
  ;
    break;}
case 175:

{
    /*  */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->is_var = yyvsp[-8].integer_value ;
    yyval.code->itype = yyvsp[-7].keyword ;  
    yyval.code->index = atoi(yyvsp[-6].string_value.name) ;
    yyval.code->offset = atoi(yyvsp[-5].string_value.name) ;
    yyval.code->data_type = yyvsp[-4].inst ;
    yyval.code->name = yyvsp[-3].string_value.name ;
    yyval.code->array_range = yyvsp[-2].inst ;
    yyval.code->opt_flags = yyvsp[-1].inst ; 
    
  ;
    break;}
case 176:

{
    /* simple */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-1].keyword ;  
  ;
    break;}
case 177:

{
    /* simple */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-1].keyword ;  
  ;
    break;}
case 178:

{
    /* convert_bit */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-5].keyword ;  
    yyval.code->isubtype = yyvsp[-4].keyword ;
    yyval.code->ub = atoi(yyvsp[-3].string_value.name) ;
    yyval.code->lb = atoi(yyvsp[-2].string_value.name) ;
    yyval.code->sd_type = yyvsp[-1].inst ;
  ;
    break;}
case 179:

{
    /* convert */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-3].keyword ;  
    yyval.code->isubtype = yyvsp[-2].keyword ;
    yyval.code->sd_type = yyvsp[-1].inst ;
    
  ;
    break;}
case 180:

{
    /* convert */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-3].keyword ;  
    yyval.code->isubtype = yyvsp[-2].keyword ;
    yyval.code->sd_type = yyvsp[-1].inst ;
    
  ;
    break;}
case 181:

{
    /* aluops */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-2].keyword ;  
    yyval.code->sd_type = yyvsp[-1].inst ;
  ;
    break;}
case 182:

{
    /* aluops */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-2].keyword ;  
    yyval.code->sd_type = yyvsp[-1].inst ;
  ;
    break;}
case 183:

{
    /* aluops */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-2].keyword ;  
    yyval.code->sd_type = yyvsp[-1].inst ;
  ;
    break;}
case 184:

{
    /* aluops */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-2].keyword ;  
    yyval.code->sd_type = yyvsp[-1].inst ;
  ;
    break;}
case 185:

{
    /* aluops */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-2].keyword ;  
    yyval.code->sd_type = yyvsp[-1].inst ;
  ;
    break;}
case 186:

{
    /* aluops */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-2].keyword ;  
    yyval.code->sd_type = yyvsp[-1].inst ;
  ;
    break;}
case 187:

{
    /* aluops */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-2].keyword ;  
    yyval.code->sd_type = yyvsp[-1].inst ;
  ;
    break;}
case 188:

{
    /* aluops */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-2].keyword ;  
    yyval.code->sd_type = yyvsp[-1].inst ;
  ;
    break;}
case 189:

{
    /* aluops */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-2].keyword ;  
    yyval.code->sd_type = yyvsp[-1].inst ;
  ;
    break;}
case 190:

{
    /* aluops */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-2].keyword ;  
    yyval.code->sd_type = yyvsp[-1].inst ;
  ;
    break;}
case 191:

{
    /* aluops */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-2].keyword ;  
    yyval.code->sd_type = yyvsp[-1].inst ;
  ;
    break;}
case 192:

{
    /* aluops */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-2].keyword ;  
    yyval.code->sd_type = yyvsp[-1].inst ;
  ;
    break;}
case 193:

{
    /* aluops */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-2].keyword ;  
    yyval.code->sd_type = yyvsp[-1].inst ;
  ;
    break;}
case 194:

{
    /* aluops */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-2].keyword ;  
    yyval.code->sd_type = yyvsp[-1].inst ;
  ;
    break;}
case 195:

{
    /* aluops */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-2].keyword ;  
    yyval.code->sd_type = yyvsp[-1].inst ;
  ;
    break;}
case 196:

{
    /* aluops */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-2].keyword ;  
    yyval.code->sd_type = yyvsp[-1].inst ;
  ;
    break;}
case 197:

{
    /* aluops */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-2].keyword ;  
    yyval.code->sd_type = yyvsp[-1].inst ;
  ;
    break;}
case 198:

{
    /* aluops */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-2].keyword ;  
    yyval.code->sd_type = yyvsp[-1].inst ;
  ;
    break;}
case 199:

{
    /* aluops */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-2].keyword ;  
    yyval.code->sd_type = yyvsp[-1].inst ;
  ;
    break;}
case 200:

{
    /* aluops */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-2].keyword ;  
    yyval.code->sd_type = yyvsp[-1].inst ;
  ;
    break;}
case 201:

{
    /* aluops */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-2].keyword ;  
    yyval.code->sd_type = yyvsp[-1].inst ;
  ;
    break;}
case 202:

{
    /* aluops */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-2].keyword ;  
    yyval.code->sd_type = yyvsp[-1].inst ;
  ;
    break;}
case 203:

{
    /* aluops */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-2].keyword ;  
    yyval.code->sd_type = yyvsp[-1].inst ;
  ;
    break;}
case 204:

{
    /* aluops */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-2].keyword ;  
    yyval.code->sd_type = yyvsp[-1].inst ;
  ;
    break;}
case 205:

{
    /* aluops */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-2].keyword ;  
    yyval.code->sd_type = yyvsp[-1].inst ;
  ;
    break;}
case 206:

{
    /* aluops */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-2].keyword ;  
    yyval.code->sd_type = yyvsp[-1].inst ;
  ;
    break;}
case 207:

{
    /* aluops */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-2].keyword ;  
    yyval.code->sd_type = yyvsp[-1].inst ;
  ;
    break;}
case 208:

{
    /* aluops */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-2].keyword ;  
    yyval.code->sd_type = yyvsp[-1].inst ;
  ;
    break;}
case 209:

{
    /* aluops */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-2].keyword ;  
    yyval.code->sd_type = yyvsp[-1].inst ;
  ;
    break;}
case 210:

{
    /* aluops */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-2].keyword ;  
    yyval.code->sd_type = yyvsp[-1].inst ;
  ;
    break;}
case 211:

{
    /* aluops */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-2].keyword ;  
    yyval.code->sd_type = yyvsp[-1].inst ;
  ;
    break;}
case 212:

{
    /* aluops */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-2].keyword ;  
    yyval.code->sd_type = yyvsp[-1].inst ;
  ;
    break;}
case 213:

{
    /* aluops */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-2].keyword ;  
    yyval.code->sd_type = yyvsp[-1].inst ;
  ;
    break;}
case 214:

{
    /* aluops */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-2].keyword ;  
    yyval.code->sd_type = yyvsp[-1].inst ;
  ;
    break;}
case 215:

{
    /* aluops */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-2].keyword ;  
    yyval.code->sd_type = yyvsp[-1].inst ;
  ;
    break;}
case 216:

{
    /* aluops */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-2].keyword ;  
    yyval.code->sd_type = yyvsp[-1].inst ;
  ;
    break;}
case 217:

{
    /* aluops */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-2].keyword ;  
    yyval.code->sd_type = yyvsp[-1].inst ;
  ;
    break;}
case 218:

{
    /* aluops */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-2].keyword ;  
    yyval.code->sd_type = yyvsp[-1].inst ;
  ;
    break;}
case 219:

{
    /* aluops */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-2].keyword ;  
    yyval.code->sd_type = yyvsp[-1].inst ;
  ;
    break;}
case 220:

{
    /* aluops */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-2].keyword ;  
    yyval.code->sd_type = yyvsp[-1].inst ;
  ;
    break;}
case 221:

{
    /* aluops */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-2].keyword ;  
    yyval.code->sd_type = yyvsp[-1].inst ;
  ;
    break;}
case 222:

{
    /* simple */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-1].keyword ;  
  ;
    break;}
case 223:

{
    /* simple */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-1].keyword ;  
  ;
    break;}
case 224:

{
    /* load_const */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-3].keyword ;  
    yyval.code->sd_type = yyvsp[-2].inst ;
    yyval.code->const_data = yyvsp[-1].exp ;
  ;
    break;}
case 225:

{
    /* op_optvar */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-2].keyword ;  
    yyval.code->is_var = yyvsp[-1].integer_value ;
  ;
    break;}
case 226:

{
    /* op_optvar */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-2].keyword ;  
    yyval.code->is_var = yyvsp[-1].integer_value ;
  ;
    break;}
case 227:

{
    /* op_label */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-2].keyword ;  
    yyval.code->name = yyvsp[-1].string_value.name ;
  ;
    break;}
case 228:

{
    /* simple */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-1].keyword ;  
  ;
    break;}
case 229:

{
    /* op_optvar_index */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-3].keyword ;  
    yyval.code->is_var = yyvsp[-2].integer_value ;
    yyval.code->index = atoi(yyvsp[-1].string_value.name) ;
  ;
    break;}
case 230:

{
    /* op_optvar_index */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-3].keyword ;  
    yyval.code->is_var = yyvsp[-2].integer_value ;
    yyval.code->index = atoi(yyvsp[-1].string_value.name) ;
  ;
    break;}
case 231:

{
    /* aluops */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-2].keyword ;  
    yyval.code->sd_type = yyvsp[-1].inst ;
  ;
    break;}
case 232:

{
    /* aluops */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-2].keyword ;  
    yyval.code->sd_type = yyvsp[-1].inst ;
  ;
    break;}
case 233:

{
    /*  */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-3].keyword ;  
    yyval.code->sd_type = yyvsp[-2].inst ;
    yyval.code->index = atoi(yyvsp[-1].string_value.name) ;
    
  ;
    break;}
case 234:

{
    /*  */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-2].keyword ;  
    yyval.code->bit_type = yyvsp[-1].inst ;
  ;
    break;}
case 235:

{
    /* op_index */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-2].keyword ; 
    yyval.code->index = atoi(yyvsp[-1].string_value.name) ; 
    
  ;
    break;}
case 236:

{
    /* op_index */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-2].keyword ; 
    yyval.code->index = atoi(yyvsp[-1].string_value.name) ; 
    
  ;
    break;}
case 237:

{
    /* op_subop */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-2].keyword ;  
    yyval.code->isubtype = yyvsp[-1].keyword ;
  ;
    break;}
case 238:

{
    /* op_subop */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-2].keyword ;  
    yyval.code->isubtype = yyvsp[-1].keyword ;
  ;
    break;}
case 239:

{
    /* op_subop_index */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-3].keyword ;  
    yyval.code->isubtype = yyvsp[-2].keyword ;
    yyval.code->index = atoi(yyvsp[-1].string_value.name) ; 
  ;
    break;}
case 240:

{
    /* simple */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-1].keyword ;  
  ;
    break;}
case 241:

{
    /* simple */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-1].keyword ;  
  ;
    break;}
case 242:

{
    /* simple */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-1].keyword ;  
  ;
    break;}
case 243:

{
    /* simple */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-1].keyword ;  
  ;
    break;}
case 244:

{
    /* op_label */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-2].keyword ;  
    yyval.code->name = yyvsp[-1].string_value.name ;
  ;
    break;}
case 245:

{
    /* op_label */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-3].keyword ;  
    yyval.code->sd_type = yyvsp[-2].inst ;
    yyval.code->name = yyvsp[-1].string_value.name ;
  ;
    break;}
case 246:

{
    /* op_label */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-3].keyword ;  
    yyval.code->sd_type = yyvsp[-2].inst ;
    yyval.code->name = yyvsp[-1].string_value.name ;
  ;
    break;}
case 247:

{
    /* simple */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-1].keyword ;  
  ;
    break;}
case 248:

{
    /* simple */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-1].keyword ;  
  ;
    break;}
case 249:

{
    /* simple */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-1].keyword ;  
  ;
    break;}
case 250:

{
    /* call */
    JD_name *func ;
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-3].keyword ;  
    yyval.code->func = yyvsp[-2].string_value.name ;
    yyval.code->index = atoi(yyvsp[-1].string_value.name) ;
    func = find_name( yyvsp[-2].string_value.name ) ;
    if( func == NULL || 
        ( func->type != JD_func_name && func->type != JD_mfunc_name && 
          func->type != JD_sys_func_name && func->type != JD_sys_mfunc_name ) 
      ) 
    {
      ERROR_FUNC_NAME_NOT_FOUND( yyvsp[-2].string_value.name ) ;
    }
    yyval.code->fname = func ;
    yyval.code->func_type = func->type ;  
  ;
    break;}
case 251:

{
    /* call_m */
    JD_name *cls, *func ;
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-4].keyword ;  
    yyval.code->class = yyvsp[-3].string_value.name ;
    yyval.code->func = yyvsp[-2].string_value.name ;
    yyval.code->index = atoi(yyvsp[-1].string_value.name) ;
    cls = find_name( yyvsp[-3].string_value.name ) ;
    if( cls == NULL )
    {
      ERROR_CLASS_NAME_NOT_FOUND( yyvsp[-3].string_value.name ) ;
    }
    else {
      func = find_name_in_scope( yyvsp[-2].string_value.name, cls->child ) ;
      if( func == NULL && !strcmp( yyvsp[-2].string_value.name, "new" ) ) {
        yyval.code->type = VVI_dummy_new ;
      }
      else {
        if( func == NULL || 
            ( func->type != JD_func_name && func->type != JD_mfunc_name && 
            func->type != JD_sys_func_name && func->type != JD_sys_mfunc_name ) 
          ) 
        {
          ERROR_MEMBER_FUNCTION_NOT_FOUND( yyvsp[-3].string_value.name, yyvsp[-2].string_value.name ) ;
        }
        yyval.code->fname = func ;
        yyval.code->func_type = func->type ;
      } 
    } 
  ;
    break;}
case 252:

{
    /* call_a */
    JD_name *cls, *func ;
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-4].keyword ;  
    yyval.code->class = yyvsp[-3].string_value.name ;
    yyval.code->func = yyvsp[-2].string_value.name ;
    yyval.code->index = atoi(yyvsp[-1].string_value.name) ;
    cls = find_name( yyvsp[-3].string_value.name ) ;
    if( cls == NULL )
    {
      ERROR_ASPECT_NAME_NOT_FOUND( yyvsp[-3].string_value.name ) ;
    }
    else {
      func = find_name_in_scope( yyvsp[-2].string_value.name, cls->child ) ;
      if( func == NULL || 
          ( func->type != JD_func_name && func->type != JD_mfunc_name && 
          func->type != JD_sys_func_name && func->type != JD_sys_mfunc_name ) 
        ) 
      {
        ERROR_MEMBER_FUNCTION_NOT_FOUND( yyvsp[-3].string_value.name, yyvsp[-2].string_value.name ) ;
      }
      yyval.code->fname = func ;
      yyval.code->func_type = func->type ;
    }
  ;
    break;}
case 253:

{
    /* call */
    JD_name *func ;
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-3].keyword ;  
    yyval.code->func = yyvsp[-2].string_value.name ;
    yyval.code->index = atoi(yyvsp[-1].string_value.name) ;
    func = find_verilog_name( yyvsp[-2].string_value.name ) ;
    if( func == NULL || func->type != JD_verilog_task_name  ) 
    {
      ERROR_VERILOG_TASK_NAME_NOT_FOUND( yyvsp[-2].string_value.name ) ;
    }
    yyval.code->is_leaf = 1 ;
    yyval.code->func_type = func->type ;  
  ;
    break;}
case 254:

{
    /* simple */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-1].keyword ;  
  ;
    break;}
case 255:

{
    /* op_label */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-2].keyword ;  
    yyval.code->name = yyvsp[-1].string_value.name ;
  ;
    break;}
case 256:

{
    /* simple */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-1].keyword ;  
  ;
    break;}
case 257:

{
    /* simple */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-1].keyword ;  
  ;
    break;}
case 258:

{
    /* op_subop */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-2].keyword ;  
    yyval.code->isubtype = yyvsp[-1].keyword ;
  ;
    break;}
case 259:

{
    /* simple */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-1].keyword ;  
  ;
    break;}
case 260:

{
    /* simple */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-1].keyword ;  
  ;
    break;}
case 261:

{
    /* op_subop */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-2].keyword ;  
    yyval.code->isubtype = yyvsp[-1].keyword ;
  ;
    break;}
case 262:

{
    /* op_subop */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-2].keyword ;  
    yyval.code->isubtype = yyvsp[-1].keyword ;
  ;
    break;}
case 263:

{
    /* simple */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-1].keyword ;  
  ;
    break;}
case 264:

{
    /* breakpoint */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-2].keyword ;
    yyval.code->name = yyvsp[-1].string_value.name ;
    
  ;
    break;}
case 265:

{
    /* sync_pset */
    JD_name *pset, *name ;
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-5].keyword ;  
    yyval.code->isubtype = yyvsp[-4].keyword ;
    yyval.code->edge = yyvsp[-3].integer_value ;
    yyval.code->portset = yyvsp[-2].string_value.name ;
    yyval.code->port = yyvsp[-1].string_value.name ;
    pset = find_global_name( yyvsp[-2].string_value.name ) ;
    if( !pset ) {
      ERROR_PORT_SET_NAME_NOT_FOUND( yyvsp[-2].string_value.name ) ;
    }
    name = find_name_in_scope( yyvsp[-1].string_value.name, pset->child ) ;
    if( !name ) {
      ERROR_PORT_NAME_NOT_FOUND_IN_PORT_SET( yyvsp[-2].string_value.name, yyvsp[-1].string_value.name ) ;
    }
    yyval.code->code_name = name ;
  ;
    break;}
case 266:

{
    /* sync_port */
    JD_name *name ;
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-4].keyword ;  
    yyval.code->isubtype = yyvsp[-3].keyword ;
    yyval.code->edge = yyvsp[-2].integer_value ;
    yyval.code->port = yyvsp[-1].string_value.name ;
    name = find_global_name( yyvsp[-1].string_value.name ) ;
    if( !name ) {
      ERROR_PORT_NAME_NOT_FOUND( yyvsp[-1].string_value.name ) ;
    }
    yyval.code->code_name = name ;
  ;
    break;}
case 267:

{
    /* sync_sig */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-3].keyword ;  
    yyval.code->isubtype = yyvsp[-2].keyword ;
    yyval.code->edge = yyvsp[-1].integer_value ;
  ;
    break;}
case 268:

{
    /* sync_pset */
    JD_name *pset, *name ;
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-5].keyword ;  
    yyval.code->isubtype = yyvsp[-4].keyword ;
    yyval.code->edge = yyvsp[-3].integer_value ;
    yyval.code->portset = yyvsp[-2].string_value.name ;
    yyval.code->port = yyvsp[-1].string_value.name ;
    pset = find_global_name( yyvsp[-2].string_value.name ) ;
    if( !pset ) {
      ERROR_PORT_SET_NAME_NOT_FOUND( yyvsp[-2].string_value.name ) ;
    }
    name = find_name_in_scope( yyvsp[-1].string_value.name, pset->child ) ;
    if( !name ) {
      ERROR_PORT_NAME_NOT_FOUND_IN_PORT_SET( yyvsp[-2].string_value.name, yyvsp[-1].string_value.name ) ;
    }
    yyval.code->code_name = name ;
  ;
    break;}
case 269:

{
    /* sync_port */
    JD_name *name ;
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-4].keyword ;  
    yyval.code->isubtype = yyvsp[-3].keyword ;
    yyval.code->edge = yyvsp[-2].integer_value ;
    yyval.code->port = yyvsp[-1].string_value.name ;
    name = find_global_name( yyvsp[-1].string_value.name ) ;
    if( !name ) {
      ERROR_PORT_NAME_NOT_FOUND( yyvsp[-1].string_value.name ) ;
    }
    yyval.code->code_name = name ;
  ;
    break;}
case 270:

{
    /* sync_sig */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-3].keyword ;  
    yyval.code->isubtype = yyvsp[-2].keyword ;
    yyval.code->edge = yyvsp[-1].integer_value ;
  ;
    break;}
case 271:

{
    /* loadl */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-5].keyword ;  
    yyval.code->array_access = yyvsp[-4].inst ;
    yyval.code->is_static = yyvsp[-3].integer_value ;
    yyval.code->is_var = yyvsp[-2].integer_value ;
    yyval.code->index = atoi(yyvsp[-1].string_value.name) ;
  ;
    break;}
case 272:

{
    /* loadg */
    JD_name *name ;
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-4].keyword ;  
    yyval.code->array_access = yyvsp[-3].inst ;
    yyval.code->is_var = yyvsp[-2].integer_value ;
    yyval.code->name = yyvsp[-1].string_value.name ;
    name = find_local_name( yyvsp[-1].string_value.name ) ;
    if( !name ) {
      name = find_global_name( yyvsp[-1].string_value.name ) ;
    }
    if( !name ) {
      ERROR_VARIABLE_NAME_NOT_FOUND( yyvsp[-1].string_value.name ) ;
    }
    yyval.code->code_name = name ;
  ;
    break;}
case 273:

{
    /* loadm */
    JD_name *cls, *vr ;
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-6].keyword ;  
    yyval.code->array_access = yyvsp[-5].inst ;
    yyval.code->is_static = yyvsp[-4].integer_value ;
    yyval.code->is_var = yyvsp[-3].integer_value ;
    yyval.code->class = yyvsp[-2].string_value.name ;
    yyval.code->name = yyvsp[-1].string_value.name ;
    cls = find_name( yyvsp[-2].string_value.name ) ;
    if( cls == NULL )
    {
      ERROR_CLASS_NAME_NOT_FOUND( yyvsp[-2].string_value.name ) ;
    }
    vr = find_name_in_scope( yyvsp[-1].string_value.name, cls->child ) ; 
    if( vr == NULL || vr->type != JD_variable_name ) {
      ERROR_MEMBER_NAME_NOT_FOUND( yyvsp[-2].string_value.name, yyvsp[-1].string_value.name ) ;
      exit(1) ;
    }
    yyval.code->index = vr->info->is.variable_dcl.index ;
  ;
    break;}
case 274:

{
    /* op_port */
    JD_name *name ;
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-2].keyword ;  
    yyval.code->port = yyvsp[-1].string_value.name ;
    name = find_global_name( yyvsp[-1].string_value.name ) ;
    if( !name ) {
      ERROR_PORT_NAME_NOT_FOUND( yyvsp[-1].string_value.name ) ;
    }
    yyval.code->code_name = name ;
  ;
    break;}
case 275:

{
    /* op_pset */
    JD_name *pset, *name ;
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-3].keyword ;  
    yyval.code->portset = yyvsp[-2].string_value.name ;
    yyval.code->port = yyvsp[-1].string_value.name ;
    pset = find_global_name( yyvsp[-2].string_value.name ) ;
    if( !pset ) {
      ERROR_PORT_SET_NAME_NOT_FOUND( yyvsp[-2].string_value.name ) ;
    }
    name = find_name_in_scope( yyvsp[-1].string_value.name, pset->child ) ;
    if( !name ) {
      ERROR_PORT_NAME_NOT_FOUND_IN_PORT_SET( yyvsp[-2].string_value.name, yyvsp[-1].string_value.name ) ;
    }
    yyval.code->code_name = name ;
  ;
    break;}
case 276:

{
    /* op_port */
    JD_name *name ;
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-2].keyword ;  
    yyval.code->port = yyvsp[-1].string_value.name ;
    name = find_global_name( yyvsp[-1].string_value.name ) ;
    if( !name ) {
      ERROR_PORT_NAME_NOT_FOUND( yyvsp[-1].string_value.name ) ;
    }
    yyval.code->code_name = name ;
  ;
    break;}
case 277:

{
    /* op_pset */
    JD_name *pset, *name ;
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-3].keyword ;  
    yyval.code->portset = yyvsp[-2].string_value.name ;
    yyval.code->port = yyvsp[-1].string_value.name ;
    pset = find_global_name( yyvsp[-2].string_value.name ) ;
    if( !pset ) {
      ERROR_PORT_SET_NAME_NOT_FOUND( yyvsp[-2].string_value.name ) ;
    }
    name = find_name_in_scope( yyvsp[-1].string_value.name, pset->child ) ;
    if( !name ) {
      ERROR_PORT_NAME_NOT_FOUND_IN_PORT_SET( yyvsp[-2].string_value.name, yyvsp[-1].string_value.name ) ;
    }
    yyval.code->code_name = name ;
  ;
    break;}
case 278:

{
    /* load_port_sub */
    JD_name *name ;
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-3].keyword ;  
    yyval.code->isubtype = yyvsp[-2].keyword ;
    yyval.code->port = yyvsp[-1].string_value.name ;
    name = find_global_name( yyvsp[-1].string_value.name ) ;
    if( !name ) {
      ERROR_PORT_NAME_NOT_FOUND( yyvsp[-1].string_value.name ) ;
    }
    yyval.code->code_name = name ;
  ;
    break;}
case 279:

{
    /* op_pset_sub */
    JD_name *pset, *name ;
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-4].keyword ;  
    yyval.code->isubtype = yyvsp[-3].keyword ;
    yyval.code->portset = yyvsp[-2].string_value.name ;
    yyval.code->port = yyvsp[-1].string_value.name ;
    pset = find_global_name( yyvsp[-2].string_value.name ) ;
    if( !pset ) {
      ERROR_PORT_SET_NAME_NOT_FOUND( yyvsp[-2].string_value.name ) ;
    }
    name = find_name_in_scope( yyvsp[-1].string_value.name, pset->child ) ;
    if( !name ) {
      ERROR_PORT_NAME_NOT_FOUND_IN_PORT_SET( yyvsp[-2].string_value.name, yyvsp[-1].string_value.name ) ;
    }
    yyval.code->code_name = name ;
  ;
    break;}
case 280:

{
    /* load_port_sub */
    JD_name *name ;
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-3].keyword ;  
    yyval.code->isubtype = yyvsp[-2].keyword ;
    yyval.code->port = yyvsp[-1].string_value.name ;
    name = find_global_name( yyvsp[-1].string_value.name ) ;
    if( !name ) {
      ERROR_PORT_NAME_NOT_FOUND( yyvsp[-1].string_value.name ) ;
    }
    yyval.code->code_name = name ;
  ;
    break;}
case 281:

{
    /* op_pset_sub */
    JD_name *pset, *name ;
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-4].keyword ;  
    yyval.code->isubtype = yyvsp[-3].keyword ;
    yyval.code->portset = yyvsp[-2].string_value.name ;
    yyval.code->port = yyvsp[-1].string_value.name ;
    pset = find_global_name( yyvsp[-2].string_value.name ) ;
    if( !pset ) {
      ERROR_PORT_SET_NAME_NOT_FOUND( yyvsp[-2].string_value.name ) ;
    }
    name = find_name_in_scope( yyvsp[-1].string_value.name, pset->child ) ;
    if( !name ) {
      ERROR_PORT_NAME_NOT_FOUND_IN_PORT_SET( yyvsp[-2].string_value.name, yyvsp[-1].string_value.name ) ;
    }
    yyval.code->code_name = name ;
  ;
    break;}
case 282:

{
    /* simple */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-1].keyword ;  
  ;
    break;}
case 283:

{
    /* op_subop */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-2].keyword ;  
    yyval.code->isubtype = yyvsp[-1].keyword ;
  ;
    break;}
case 284:

{
    /* simple */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-1].keyword ;  
  ;
    break;}
case 285:

{
    /* load_static */
    JD_name *name ;
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-5].keyword ;  
    yyval.code->array_access = yyvsp[-4].inst ;
    yyval.code->class = yyvsp[-3].string_value.name ;
    yyval.code->func = yyvsp[-2].string_value.name ;
    yyval.code->name = yyvsp[-1].string_value.name ;
    name = find_local_static_name( yyvsp[-3].string_value.name, yyvsp[-2].string_value.name, yyvsp[-1].string_value.name ) ;
    if( !name ) name = find_static_name( yyvsp[-3].string_value.name, yyvsp[-2].string_value.name, yyvsp[-1].string_value.name ) ;
    if( !name ) {
      int len ;
      char *full_name ;
      len = strlen(yyvsp[-3].string_value.name)+strlen(yyvsp[-2].string_value.name)+strlen(yyvsp[-1].string_value.name)+5 ;
      full_name = (char *)calloc( len, sizeof(char) ) ;
      strcat( full_name, yyvsp[-3].string_value.name ) ;
      strcat( full_name, "." ) ;
      strcat( full_name, yyvsp[-2].string_value.name ) ;
      strcat( full_name, "." ) ;
      strcat( full_name, yyvsp[-1].string_value.name ) ;
      ERROR_VARIABLE_NAME_NOT_FOUND( full_name ) ;  
      exit(1) ;
    }
    yyval.code->code_name = name ;
  ;
    break;}
case 286:

{
    /* storel */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-5].keyword ;  
    yyval.code->array_access = yyvsp[-4].inst ;
    yyval.code->is_static = yyvsp[-3].integer_value ;
    yyval.code->sd_type = yyvsp[-2].inst ;
    yyval.code->index = atoi(yyvsp[-1].string_value.name) ;
  ;
    break;}
case 287:

{
    /* storel_sub */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-7].keyword ;  
    yyval.code->array_access = yyvsp[-6].inst ;
    yyval.code->is_static = yyvsp[-5].integer_value ;
    yyval.code->isubtype = yyvsp[-4].keyword ;
    yyval.code->bit_type = yyvsp[-3].inst ;
    yyval.code->sd_type = yyvsp[-2].inst ;
    yyval.code->index = atoi(yyvsp[-1].string_value.name) ;
  ;
    break;}
case 288:

{
    /* storeg */
    JD_name *name ;
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-5].keyword ;  
    yyval.code->array_access = yyvsp[-4].inst ;
    yyval.code->is_static = yyvsp[-3].integer_value ;
    yyval.code->sd_type = yyvsp[-2].inst ;
    yyval.code->name = yyvsp[-1].string_value.name ;
    name = find_local_name( yyvsp[-1].string_value.name ) ;
    if( !name ) {
      name = find_global_name( yyvsp[-1].string_value.name ) ;
    }
    if( !name ) {
      ERROR_VARIABLE_NAME_NOT_FOUND( yyvsp[-1].string_value.name ) ;
    }
    yyval.code->code_name = name ;
  ;
    break;}
case 289:

{
    /* storeg_sub */
    JD_name *name ;
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-7].keyword ;  
    yyval.code->array_access = yyvsp[-6].inst ;
    yyval.code->is_static = yyvsp[-5].integer_value ;
    yyval.code->isubtype = yyvsp[-4].keyword ;
    yyval.code->bit_type = yyvsp[-3].inst ;
    yyval.code->sd_type = yyvsp[-2].inst ;
    yyval.code->name = yyvsp[-1].string_value.name ;
    name = find_local_name( yyvsp[-1].string_value.name ) ;
    if( !name ) {
      name = find_global_name( yyvsp[-1].string_value.name ) ;
    }
    if( !name ) {
      ERROR_VARIABLE_NAME_NOT_FOUND( yyvsp[-1].string_value.name ) ;
    }
    yyval.code->code_name = name ;
  ;
    break;}
case 290:

{
    /* storem */
    JD_name *cls, *vr ;
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-6].keyword ;  
    yyval.code->array_access = yyvsp[-5].inst ;
    yyval.code->is_static = yyvsp[-4].integer_value ;
    yyval.code->sd_type = yyvsp[-3].inst ;
    yyval.code->class = yyvsp[-2].string_value.name ;
    yyval.code->name = yyvsp[-1].string_value.name ;
    cls = find_name( yyvsp[-2].string_value.name ) ;
    if( cls == NULL )
    {
      ERROR_CLASS_NAME_NOT_FOUND( yyvsp[-2].string_value.name ) ;
    }
    vr = find_name_in_scope( yyvsp[-1].string_value.name, cls->child ) ; ;
    if( vr == NULL || vr->type != JD_variable_name ) {
      ERROR_MEMBER_NAME_NOT_FOUND( yyvsp[-2].string_value.name, yyvsp[-1].string_value.name ) ;
    }
    yyval.code->index = vr->info->is.variable_dcl.index ;
  ;
    break;}
case 291:

{
    /* storem_sub */
    JD_name *cls, *vr ;
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-8].keyword ;  
    yyval.code->array_access = yyvsp[-7].inst ;
    yyval.code->is_static = yyvsp[-6].integer_value ;
    yyval.code->isubtype = yyvsp[-5].keyword ;
    yyval.code->bit_type = yyvsp[-4].inst ;
    yyval.code->sd_type = yyvsp[-3].inst ;
    yyval.code->class = yyvsp[-2].string_value.name ;
    yyval.code->name = yyvsp[-1].string_value.name ;
    cls = find_name( yyvsp[-2].string_value.name ) ;
    if( cls == NULL )
    {
      ERROR_CLASS_NAME_NOT_FOUND( yyvsp[-2].string_value.name ) ;
    }
    vr = find_name_in_scope( yyvsp[-1].string_value.name, cls->child ) ; ;
    if( vr == NULL || vr->type != JD_variable_name ) {
      ERROR_MEMBER_NAME_NOT_FOUND( yyvsp[-2].string_value.name, yyvsp[-1].string_value.name ) ;
      exit(1) ;
    }
    yyval.code->index = vr->info->is.variable_dcl.index ;
  ;
    break;}
case 292:

{
    /* store_port */
    JD_name *name ;
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-4].keyword ;  
    yyval.code->sd_type = yyvsp[-3].inst ;
    yyval.code->strength = yyvsp[-2].integer_value ;
    yyval.code->port = yyvsp[-1].string_value.name ;
    name = find_global_name( yyvsp[-1].string_value.name ) ;
    if( !name ) {
      ERROR_PORT_NAME_NOT_FOUND( yyvsp[-1].string_value.name ) ;
    }
    yyval.code->code_name = name ;
  ;
    break;}
case 293:

{
    /* store_port_sub */
    JD_name *name ;
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-6].keyword ;  
    yyval.code->isubtype = yyvsp[-5].keyword ;  
    yyval.code->bit_type = yyvsp[-4].inst ;
    yyval.code->sd_type = yyvsp[-3].inst ;
    yyval.code->strength = yyvsp[-2].integer_value ;
    yyval.code->port = yyvsp[-1].string_value.name ;
    name = find_global_name( yyvsp[-1].string_value.name ) ;
    if( !name ) {
      ERROR_PORT_NAME_NOT_FOUND( yyvsp[-1].string_value.name ) ;
    }
    yyval.code->code_name = name ;
  ;
    break;}
case 294:

{
    /* store_pset */
    JD_name *pset, *port ;
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-5].keyword ;  
    yyval.code->sd_type = yyvsp[-4].inst ;
    yyval.code->strength = yyvsp[-3].integer_value ;
    yyval.code->portset = yyvsp[-2].string_value.name ;
    yyval.code->port = yyvsp[-1].string_value.name ;
    pset = find_global_name( yyvsp[-2].string_value.name ) ;
    if( !pset ) {
      ERROR_PORT_SET_NAME_NOT_FOUND( yyvsp[-2].string_value.name ) ;
    }
    port = find_name_in_scope( yyvsp[-1].string_value.name, pset->child ) ; 
    if( !port ) {
      ERROR_PORT_NAME_NOT_FOUND_IN_PORT_SET( yyvsp[-2].string_value.name, yyvsp[-1].string_value.name ) ;
    }
    yyval.code->code_name = port ;
  ;
    break;}
case 295:

{
    /* store_pset_sub */
    JD_name *pset, *port ;
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-7].keyword ;  
    yyval.code->isubtype = yyvsp[-6].keyword ;  
    yyval.code->bit_type = yyvsp[-5].inst ;
    yyval.code->sd_type = yyvsp[-4].inst ;
    yyval.code->strength = yyvsp[-3].integer_value ;
    yyval.code->portset = yyvsp[-2].string_value.name ;
    yyval.code->port = yyvsp[-1].string_value.name ;
    pset = find_global_name( yyvsp[-2].string_value.name ) ;
    if( !pset ) {
      ERROR_PORT_SET_NAME_NOT_FOUND( yyvsp[-2].string_value.name ) ;
    }
    port = find_name_in_scope( yyvsp[-1].string_value.name, pset->child ) ; 
    if( !port ) {
      ERROR_PORT_NAME_NOT_FOUND_IN_PORT_SET( yyvsp[-1].string_value.name, yyvsp[-2].string_value.name ) ;
    }
    yyval.code->code_name = port ;
  ;
    break;}
case 296:

{
    /* store_sig_l */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-6].keyword ;  
    yyval.code->array_access = yyvsp[-5].inst ;
    yyval.code->is_static = yyvsp[-4].integer_value ;
    yyval.code->sd_type = yyvsp[-3].inst ;
    yyval.code->strength = yyvsp[-2].integer_value ;
    yyval.code->index = atoi(yyvsp[-1].string_value.name) ;  
  ;
    break;}
case 297:

{
    /* store_sig_g */
    JD_name *name ;
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-6].keyword ;  
    yyval.code->array_access = yyvsp[-5].inst ;
    yyval.code->is_static = yyvsp[-4].integer_value ;
    yyval.code->sd_type = yyvsp[-3].inst ;
    yyval.code->strength = yyvsp[-2].integer_value ;
    yyval.code->name = yyvsp[-1].string_value.name ;
    name = find_local_name( yyvsp[-1].string_value.name ) ;
    if( !name ) {
      name = find_global_name( yyvsp[-1].string_value.name ) ;
    }
    if( !name ) {
      ERROR_VARIABLE_NAME_NOT_FOUND( yyvsp[-1].string_value.name ) ;
    }
    yyval.code->code_name = name ;
  ;
    break;}
case 298:

{
    /* store_sig_m */
    JD_name *cls, *vr ;
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-7].keyword ;  
    yyval.code->array_access = yyvsp[-6].inst ;
    yyval.code->is_static = yyvsp[-5].integer_value ;
    yyval.code->sd_type = yyvsp[-4].inst ;
    yyval.code->strength = yyvsp[-3].integer_value ;
    yyval.code->class = yyvsp[-2].string_value.name ;
    yyval.code->name = yyvsp[-1].string_value.name ;
    cls = find_name( yyvsp[-2].string_value.name ) ;
    if( cls == NULL )
    {
      ERROR_CLASS_NAME_NOT_FOUND( yyvsp[-2].string_value.name ) ;
    }
    vr = find_name_in_scope( yyvsp[-1].string_value.name, cls->child ) ; ;
    if( vr == NULL || vr->type != JD_variable_name ) {
      ERROR_MEMBER_NAME_NOT_FOUND( yyvsp[-2].string_value.name, yyvsp[-1].string_value.name ) ;
    }
    yyval.code->index = vr->info->is.variable_dcl.index ;
  ;
    break;}
case 299:

{
    /* store_sig_l_sub */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-8].keyword ;  
    yyval.code->array_access = yyvsp[-7].inst ;
    yyval.code->is_static = yyvsp[-6].integer_value ;
    yyval.code->isubtype = yyvsp[-5].keyword ;
    yyval.code->bit_type = yyvsp[-4].inst ;
    yyval.code->sd_type = yyvsp[-3].inst ;
    yyval.code->strength = yyvsp[-2].integer_value ;
    yyval.code->index = atoi(yyvsp[-1].string_value.name) ;  
  ;
    break;}
case 300:

{
    /* store_sig_g_sub */
    JD_name *name ;
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-8].keyword ;  
    yyval.code->array_access = yyvsp[-7].inst ;
    yyval.code->is_static = yyvsp[-6].integer_value ;
    yyval.code->isubtype = yyvsp[-5].keyword ;
    yyval.code->bit_type = yyvsp[-4].inst ;
    yyval.code->sd_type = yyvsp[-3].inst ;
    yyval.code->strength = yyvsp[-2].integer_value ;
    yyval.code->name = yyvsp[-1].string_value.name ;  
    name = find_local_name( yyvsp[-1].string_value.name ) ;
    if( !name ) {
      name = find_global_name( yyvsp[-1].string_value.name ) ;
    }
    if( !name ) {
      ERROR_VARIABLE_NAME_NOT_FOUND( yyvsp[-1].string_value.name ) ;
    }
    yyval.code->code_name = name ;
  ;
    break;}
case 301:

{
    /* store_sig_m_sub */
    JD_name *cls, *vr ;
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-9].keyword ;  
    yyval.code->array_access = yyvsp[-8].inst ;
    yyval.code->is_static = yyvsp[-7].integer_value ;
    yyval.code->isubtype = yyvsp[-6].keyword ;
    yyval.code->bit_type = yyvsp[-5].inst ;
    yyval.code->sd_type = yyvsp[-4].inst ;
    yyval.code->strength = yyvsp[-3].integer_value ;
    yyval.code->class = yyvsp[-2].string_value.name ;  
    yyval.code->name = yyvsp[-1].string_value.name ;  
    cls = find_name( yyvsp[-2].string_value.name ) ;
    if( cls == NULL )
    {
      ERROR_CLASS_NAME_NOT_FOUND( yyvsp[-2].string_value.name ) ;
    }
    vr = find_name_in_scope( yyvsp[-1].string_value.name, cls->child ) ; ;
    if( vr == NULL || vr->type != JD_variable_name ) {
      ERROR_MEMBER_NAME_NOT_FOUND( yyvsp[-2].string_value.name, yyvsp[-1].string_value.name ) ;
    }
    yyval.code->index = vr->info->is.variable_dcl.index ;
  ;
    break;}
case 302:

{
    /* store_static */
    JD_name *name ;
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-6].keyword ;  
    yyval.code->array_access = yyvsp[-5].inst ;
    yyval.code->sd_type = yyvsp[-4].inst ;
    yyval.code->class = yyvsp[-3].string_value.name ;
    yyval.code->func = yyvsp[-2].string_value.name ;
    yyval.code->name = yyvsp[-1].string_value.name ;
    name = find_local_static_name( yyvsp[-3].string_value.name, yyvsp[-2].string_value.name, yyvsp[-1].string_value.name ) ;
    if( !name ) name = find_static_name( yyvsp[-3].string_value.name, yyvsp[-2].string_value.name, yyvsp[-1].string_value.name ) ;
    if( !name ) {
      int len ;
      char *full_name ;
      len = strlen(yyvsp[-3].string_value.name)+strlen(yyvsp[-2].string_value.name)+strlen(yyvsp[-1].string_value.name)+5 ;
      full_name = (char *)calloc( len, sizeof(char) ) ;
      strcat( full_name, yyvsp[-3].string_value.name ) ;
      strcat( full_name, "." ) ;
      strcat( full_name, yyvsp[-2].string_value.name ) ;
      strcat( full_name, "." ) ;
      strcat( full_name, yyvsp[-1].string_value.name ) ;
      ERROR_VARIABLE_NAME_NOT_FOUND( full_name ) ;  
      exit(1) ;
    }
    yyval.code->code_name = name ;
  ;
    break;}
case 303:

{
    /* store_sig_static */
    JD_name *name ;
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-7].keyword ;  
    yyval.code->array_access = yyvsp[-6].inst ;
    yyval.code->sd_type = yyvsp[-5].inst ;
    yyval.code->strength = yyvsp[-4].integer_value ;
    yyval.code->class = yyvsp[-3].string_value.name ;
    yyval.code->func = yyvsp[-2].string_value.name ;
    yyval.code->name = yyvsp[-1].string_value.name ;
    name = find_local_static_name( yyvsp[-3].string_value.name, yyvsp[-2].string_value.name, yyvsp[-1].string_value.name ) ;
    if( !name ) name = find_static_name( yyvsp[-3].string_value.name, yyvsp[-2].string_value.name, yyvsp[-1].string_value.name ) ;
    if( !name ) {
      int len ;
      char *full_name ;
      len = strlen(yyvsp[-3].string_value.name)+strlen(yyvsp[-2].string_value.name)+strlen(yyvsp[-1].string_value.name)+5 ;
      full_name = (char *)calloc( len, sizeof(char) ) ;
      strcat( full_name, yyvsp[-3].string_value.name ) ;
      strcat( full_name, "." ) ;
      strcat( full_name, yyvsp[-2].string_value.name ) ;
      strcat( full_name, "." ) ;
      strcat( full_name, yyvsp[-1].string_value.name ) ;
      ERROR_VARIABLE_NAME_NOT_FOUND( full_name ) ;  
      exit(1) ;
    }
    yyval.code->code_name = name ;
  ;
    break;}
case 304:

{
    /* store_static_sub */
    JD_name *name ;
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-8].keyword ;  
    yyval.code->array_access = yyvsp[-7].inst ;
    yyval.code->isubtype = yyvsp[-6].keyword ;
    yyval.code->bit_type = yyvsp[-5].inst ;
    yyval.code->sd_type = yyvsp[-4].inst ;
    yyval.code->class = yyvsp[-3].string_value.name ;
    yyval.code->func = yyvsp[-2].string_value.name ;
    yyval.code->name = yyvsp[-1].string_value.name ;
    name = find_local_static_name( yyvsp[-3].string_value.name, yyvsp[-2].string_value.name, yyvsp[-1].string_value.name ) ;
    if( !name ) name = find_static_name( yyvsp[-3].string_value.name, yyvsp[-2].string_value.name, yyvsp[-1].string_value.name ) ;
    if( !name ) {
      int len ;
      char *full_name ;
      len = strlen(yyvsp[-3].string_value.name)+strlen(yyvsp[-2].string_value.name)+strlen(yyvsp[-1].string_value.name)+5 ;
      full_name = (char *)calloc( len, sizeof(char) ) ;
      strcat( full_name, yyvsp[-3].string_value.name ) ;
      strcat( full_name, "." ) ;
      strcat( full_name, yyvsp[-2].string_value.name ) ;
      strcat( full_name, "." ) ;
      strcat( full_name, yyvsp[-1].string_value.name ) ;
      ERROR_VARIABLE_NAME_NOT_FOUND( full_name ) ;  
      exit(1) ;
    }
    yyval.code->code_name = name ;
  ;
    break;}
case 305:

{
    /* store_sig_static */
    JD_name *name ;
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-9].keyword ;  
    yyval.code->array_access = yyvsp[-8].inst ;
    yyval.code->isubtype = yyvsp[-7].keyword ;
    yyval.code->bit_type = yyvsp[-6].inst ;
    yyval.code->sd_type = yyvsp[-5].inst ;
    yyval.code->strength = yyvsp[-4].integer_value ;
    yyval.code->class = yyvsp[-3].string_value.name ;
    yyval.code->func = yyvsp[-2].string_value.name ;
    yyval.code->name = yyvsp[-1].string_value.name ;
    name = find_local_static_name( yyvsp[-3].string_value.name, yyvsp[-2].string_value.name, yyvsp[-1].string_value.name ) ;
    if( !name ) name = find_static_name( yyvsp[-3].string_value.name, yyvsp[-2].string_value.name, yyvsp[-1].string_value.name ) ;
    if( !name ) {
      int len ;
      char *full_name ;
      len = strlen(yyvsp[-3].string_value.name)+strlen(yyvsp[-2].string_value.name)+strlen(yyvsp[-1].string_value.name)+5 ;
      full_name = (char *)calloc( len, sizeof(char) ) ;
      strcat( full_name, yyvsp[-3].string_value.name ) ;
      strcat( full_name, "." ) ;
      strcat( full_name, yyvsp[-2].string_value.name ) ;
      strcat( full_name, "." ) ;
      strcat( full_name, yyvsp[-1].string_value.name ) ;
      ERROR_VARIABLE_NAME_NOT_FOUND( full_name ) ;  
      exit(1) ;
    }
    yyval.code->code_name = name ;
  ;
    break;}
case 306:

{
    /* store_obj */
    JD_name *cls, *vr ;
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-4].keyword ;  
    yyval.code->sd_type = yyvsp[-3].inst ;
    yyval.code->class = yyvsp[-2].string_value.name ;
    yyval.code->name = yyvsp[-1].string_value.name ;
    cls = find_name( yyvsp[-2].string_value.name ) ;
    if( cls == NULL )
    {
      ERROR_CLASS_NAME_NOT_FOUND( yyvsp[-2].string_value.name ) ;
    }
    vr = find_name_in_scope( yyvsp[-1].string_value.name, cls->child ) ; ;
    if( vr == NULL || vr->type != JD_variable_name ) {
      ERROR_MEMBER_NAME_NOT_FOUND( yyvsp[-2].string_value.name, yyvsp[-1].string_value.name ) ;
    }
    yyval.code->index = vr->info->is.variable_dcl.index ;
  ;
    break;}
case 307:

{
    /* aluops */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-2].keyword ;  
    yyval.code->sd_type = yyvsp[-1].inst ;
  ;
    break;}
case 308:

{
    /* alloc_func */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-2].keyword ;
    yyval.code->index = atoi(yyvsp[-1].string_value.name) ;
    yyval.code->name = yyvsp[0].string_value.name ;
  ;
    break;}
case 309:

{
    /* overwrite */
    JD_name *cls, *func ;
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-3].keyword ;  
    yyval.code->parent = yyvsp[-2].string_value.name ;
    yyval.code->class = yyvsp[-1].string_value.name ;
    yyval.code->func = yyval.code->name = yyvsp[0].string_value.name ;
    cls = find_name( yyvsp[-2].string_value.name ) ;
    if( cls == NULL )
    {
      ERROR_CLASS_NAME_NOT_FOUND( yyvsp[-2].string_value.name ) ;
    }
    func = find_name_in_scope( yyvsp[0].string_value.name, cls->child ) ; ;
    if( func == NULL || func->type != JD_mfunc_name ) {
      ERROR_MEMBER_FUNCTION_NOT_FOUND( yyvsp[-2].string_value.name, yyvsp[0].string_value.name ) ;
    }
    yyval.code->index = func->info->index ;
    if( func->info->index < 0 ) {
      ERROR_ILLEGAL_OVERWRITE_INDEX_FOR_MEMBER_FUNCTION( yyvsp[-2].string_value.name, yyvsp[0].string_value.name ) ;
    }
    cls = find_name( yyvsp[-1].string_value.name ) ;
    if( cls == NULL )
    {
      ERROR_CLASS_NAME_NOT_FOUND( yyvsp[-1].string_value.name ) ;
    }
    func = find_name_in_scope( yyvsp[0].string_value.name, cls->child ) ; ;
    if( func == NULL || func->type != JD_mfunc_name ) {
      ERROR_MEMBER_FUNCTION_NOT_FOUND( yyvsp[-1].string_value.name, yyvsp[0].string_value.name ) ;
    }
  ;
    break;}
case 310:

{
    /* overwrite */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-2].keyword ;  
    yyval.code->index = atoi(yyvsp[-1].string_value.name) ;
    yyval.code->func = yyval.code->name = yyvsp[0].string_value.name ;
  ;
    break;}
case 311:

{
    /* advice_op */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-2].keyword ;  
    yyval.code->fname = yyvsp[-1].name ;
  ;
    break;}
case 312:

{
    /* advice_op */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-2].keyword ;  
    yyval.code->fname = yyvsp[-1].name ;
  ;
    break;}
case 313:

{
    /* simple */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-1].keyword ;  
  ;
    break;}
case 314:

{
    /* simple */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-1].keyword ;  
  ;
    break;}
case 315:

{
    /* simple */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-1].keyword ;  
  ;
    break;}
case 316:

{
    /* simple */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[-1].keyword ;  
  ;
    break;}
case 317:

{
    /* simple */
    yyval.code = ALLOC(vinst_info) ;
    yyval.code->type = VVI_inst ;
    yyval.code->inum = inst_num++ ;
    yyval.code->itype = yyvsp[0].keyword ;  
  ;
    break;}
case 318:

{
    yyval.name = yyvsp[0].name ; 
  ;
    break;}
case 319:

{
    /* NameChain */
    JD_name *last = yyvsp[-1].name ;
    if( yyvsp[-1].name ) {
      while(last->next) {
        last = last->next ;
      }
      last->next = yyvsp[0].name ;
      yyval.name = yyvsp[-1].name ;
    }
    else yyval.name = yyvsp[0].name ;
  ;
    break;}
case 320:

{
    /* pointcut_spec */
    yyval.name = ALLOC(JD_name) ;
    yyval.name->name = yyvsp[-1].string_value.name ;
    yyval.name->class_name = yyvsp[0].name ;
  ;
    break;}
case 321:

{
    yyval.name = NULL ;
  ;
    break;}
case 322:

{
    /* class_pointcut */
    yyval.name = ALLOC(JD_name) ;
    yyval.name->name = yyvsp[-1].string_value.name ;
  ;
    break;}
case 323:

{
    /* stype */
    yyval.inst = ALLOC(vcode_info) ;
    yyval.inst->is.sd_type.a = yyvsp[-1].integer_value ;
  ;
    break;}
case 324:

{
    /* dtype */
    yyval.inst = ALLOC(vcode_info) ;
    yyval.inst->is.sd_type.a = yyvsp[-3].integer_value ;
    yyval.inst->is.sd_type.b = yyvsp[-1].integer_value ;
  ;
    break;}
case 325:

{
    /* dtype */
    yyval.inst = ALLOC(vcode_info) ;
    yyval.inst->is.sd_type.a = yyvsp[-5].integer_value ;
    yyval.inst->is.sd_type.b = yyvsp[-3].integer_value ;
    yyval.inst->is.sd_type.c = yyvsp[-1].integer_value ;
  ;
    break;}
case 326:

{
    yyval.inst = NULL ;
  ;
    break;}
case 327:

{
    /* static_flag */
    yyval.inst = ALLOC(vcode_info) ;
    yyval.inst->is.attrib.is_static = 1 ;
  ;
    break;}
case 328:

{
    /* clone_flag */
    yyval.inst = ALLOC(vcode_info) ;
    yyval.inst->is.attrib.is_clone = 1 ;
  ;
    break;}
case 329:

{
    /* static_clone_flag */
    yyval.inst = ALLOC(vcode_info) ;
    yyval.inst->is.attrib.is_static = 1 ;
    yyval.inst->is.attrib.is_clone = 1 ;
  ;
    break;}
case 330:

{
    /* static_clone_flag */
    yyval.inst = ALLOC(vcode_info) ;
    yyval.inst->is.attrib.is_static = 1 ;
    yyval.inst->is.attrib.is_clone = 1 ;
  ;
    break;}
case 331:

{
    yyval.integer_value = yyvsp[0].keyword ; 
  ;
    break;}
case 332:

{
    yyval.integer_value = yyvsp[0].keyword ; 
  ;
    break;}
case 333:

{
    yyval.integer_value = yyvsp[0].keyword ; 
  ;
    break;}
case 334:

{
    yyval.integer_value = yyvsp[0].keyword ; 
  ;
    break;}
case 335:

{
    yyval.integer_value = yyvsp[0].keyword ; 
  ;
    break;}
case 336:

{
    yyval.integer_value = yyvsp[0].keyword ; 
  ;
    break;}
case 337:

{
    yyval.integer_value = yyvsp[0].keyword ; 
  ;
    break;}
case 338:

{
    yyval.integer_value = yyvsp[0].keyword ; 
  ;
    break;}
case 339:

{
    yyval.integer_value = yyvsp[0].keyword ; 
  ;
    break;}
case 340:

{
    yyval.integer_value = yyvsp[0].keyword ; 
  ;
    break;}
case 341:

{
    yyval.integer_value = yyvsp[0].keyword ; 
  ;
    break;}
case 342:

{
    yyval.integer_value = yyvsp[0].keyword ; 
  ;
    break;}
case 343:

{
    yyval.integer_value = yyvsp[0].keyword ; 
  ;
    break;}
case 344:

{
    yyval.integer_value = yyvsp[0].keyword ; 
  ;
    break;}
case 345:

{
    yyval.integer_value = yyvsp[0].keyword ; 
  ;
    break;}
case 346:

{
    yyval.exp = yyvsp[0].exp ; 
  ;
    break;}
case 347:

{
    /* string_const */
    yyval.exp = ALLOC(expression_node) ;
    yyval.exp->type = JD_string_lit ;
    yyval.exp->is.string = yyvsp[0].string_value.name ;  
  ;
    break;}
case 348:

{
    /* string_const */
    yyval.exp = ALLOC(expression_node) ;
    yyval.exp->type = JD_enum_member ;
    yyval.exp->is.enum_const.type = yyvsp[-2].string_value.name ;  
    yyval.exp->is.enum_const.member = yyvsp[-1].string_value.name ;
    yyval.exp->is.enum_const.index = atoi(yyvsp[0].string_value.name) ;
  ;
    break;}
case 349:

{
    yyval.string_value = yyvsp[0].string_value ; 
  ;
    break;}
case 350:

{
    yyval.integer_value = yyvsp[0].keyword ; 
  ;
    break;}
case 351:

{
    yyval.integer_value = yyvsp[0].keyword ; 
  ;
    break;}
case 352:

{
    yyval.integer_value = yyvsp[0].keyword ; 
  ;
    break;}
case 353:

{
    yyval.integer_value = yyvsp[0].keyword ; 
  ;
    break;}
case 354:

{
    yyval.inst = NULL ;
  ;
    break;}
case 355:

{
    /* array_array_range */
    yyval.inst = ALLOC(vcode_info) ;
    yyval.inst->is.array_range.type = yyvsp[-1].keyword ;
    yyval.inst->is.array_range.size = atoi(yyvsp[0].string_value.name) ;
  ;
    break;}
case 356:

{
    /* marray_array_range */
    yyval.inst = ALLOC(vcode_info) ;
    yyval.inst->is.array_range.type = yyvsp[-4].keyword ;
    yyval.inst->is.array_range.size = atoi(yyvsp[-3].string_value.name) ;
    yyval.inst->is.array_range.range_list = yyvsp[-1].inst ;
  ;
    break;}
case 357:

{
    /* array_range */
    yyval.inst = ALLOC(vcode_info) ;
    yyval.inst->is.array_range.type = yyvsp[0].keyword ;
  ;
    break;}
case 358:

{
    /* array_range */
    yyval.inst = ALLOC(vcode_info) ;
    yyval.inst->is.array_range.type = yyvsp[0].keyword ;
  ;
    break;}
case 359:

{
    /* array_range */
    yyval.inst = ALLOC(vcode_info) ;
    yyval.inst->is.array_range.type = yyvsp[0].keyword ;
  ;
    break;}
case 360:

{
    yyval.integer_value = 0 ;
  ;
    break;}
case 361:

{
    yyval.integer_value = 1 ;
  ;
    break;}
case 362:

{
    yyval.inst = NULL ;
  ;
    break;}
case 363:

{
    /* static_clone_flag */
    yyval.inst = ALLOC(vcode_info) ;
    yyval.inst->is.bit_type.type = yyvsp[-5].keyword ;
    yyval.inst->is.bit_type.ub = atoi(yyvsp[-3].string_value.name) ;
    yyval.inst->is.bit_type.lb = atoi(yyvsp[-1].string_value.name) ;
  ;
    break;}
case 364:

{
    /* static_clone_flag */
    yyval.inst = ALLOC(vcode_info) ;
    yyval.inst->is.bit_type.type = yyvsp[-5].keyword ;
    yyval.inst->is.bit_type.ub = atoi(yyvsp[-3].string_value.name) ;
    yyval.inst->is.bit_type.lb = atoi(yyvsp[-1].string_value.name) ;
  ;
    break;}
case 365:

{
    /* real_num */
    yyval.exp = ALLOC(expression_node) ;
    yyval.exp->type = JD_real_num ;
    yyval.exp->is.real_value = atof(yyvsp[0].string_value.name) ;  
    yyval.exp->string = yyvsp[0].string_value.name ;
  ;
    break;}
case 366:

{
    /* decimal_num */
    yyval.exp = ALLOC(expression_node) ;
    yyval.exp->type = JD_int_const ;
    yyval.exp->is.int_value = atoi(yyvsp[0].string_value.name) ;  
    yyval.exp->string = yyvsp[0].string_value.name ;
  ;
    break;}
case 367:

{
    /* v_decimal_num */
    yyval.exp = ALLOC(expression_node) ;
    scan_v_decimal( yyvsp[0].string_value.name, yyval.exp ) ;
    yyval.exp->string = yyvsp[0].string_value.name ;
  ;
    break;}
case 368:

{
    /* c_decimal_num */
    yyval.exp = ALLOC(expression_node) ;
    scan_c_decimal( yyvsp[0].string_value.name, yyval.exp ) ;
    yyval.exp->string = yyvsp[0].string_value.name ;
  ;
    break;}
case 369:

{
    /* v_hex_num */
    yyval.exp = ALLOC(expression_node) ;
    scan_v_hex( yyvsp[0].string_value.name, yyval.exp ) ;
    yyval.exp->string = yyvsp[0].string_value.name ;
  ;
    break;}
case 370:

{
    /* c_hex_num */
    yyval.exp = ALLOC(expression_node) ;
    scan_c_hex( yyvsp[0].string_value.name, yyval.exp ) ;
    yyval.exp->string = yyvsp[0].string_value.name ;
  ;
    break;}
case 371:

{
    /* v_octal_num */
    yyval.exp = ALLOC(expression_node) ;
    scan_v_octal( yyvsp[0].string_value.name, yyval.exp ) ;
    yyval.exp->string = yyvsp[0].string_value.name ;
  ;
    break;}
case 372:

{
    /* c_octal_num */
    yyval.exp = ALLOC(expression_node) ;
    scan_c_octal( yyvsp[0].string_value.name, yyval.exp ) ;
    yyval.exp->string = yyvsp[0].string_value.name ;
  ;
    break;}
case 373:

{
    /* v_bin_num */
    yyval.exp = ALLOC(expression_node) ;
    scan_v_binary( yyvsp[0].string_value.name, yyval.exp ) ;
    yyval.exp->string = yyvsp[0].string_value.name ;
  ;
    break;}
case 374:

{
    /* c_bin_num */
    yyval.exp = ALLOC(expression_node) ;
    scan_c_binary( yyvsp[0].string_value.name, yyval.exp ) ;
    yyval.exp->string = yyvsp[0].string_value.name ;
  ;
    break;}
case 375:

{
    yyval.integer_value = yyvsp[0].keyword ; 
  ;
    break;}
case 376:

{
    yyval.integer_value = yyvsp[0].keyword ; 
  ;
    break;}
case 377:

{
    yyval.integer_value = yyvsp[0].keyword ; 
  ;
    break;}
case 378:

{
    yyval.string_value = yyvsp[0].string_value ; 
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


