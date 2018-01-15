#define Identifier 257
#define Real_num 258
#define Real_or_Depth 259
#define Decimal_num 260
#define V_decimal_num 261
#define C_decimal_num 262
#define V_hex_num 263
#define C_hex_num 264
#define V_octal_num 265
#define C_octal_num 266
#define V_bin_num 267
#define C_bin_num 268
#define String_literal 269
#define JDT_abstract 270
#define JDT_advice 271
#define JDT_array 272
#define JDT_aspect 273
#define JDT_assoc 274
#define JDT_bit 275
#define JDT_bothedge 276
#define JDT_break 277
#define JDT_breakpoint 278
#define JDT_call 279
#define JDT_case 280
#define JDT_casex 281
#define JDT_casez 282
#define JDT_class 283
#define JDT_class_type 284
#define JDT_clone 285
#define JDT_connectionset 286
#define JDT_continue 287
#define JDT_default 288
#define JDT_depth 289
#define JDT_do 290
#define JDT_double 291
#define JDT_drive 292
#define JDT_else 293
#define JDT_enum 294
#define JDT_export 295
#define JDT_extends 296
#define JDT_extern 297
#define JDT_false 298
#define JDT_final 299
#define JDT_float 300
#define JDT_for 301
#define JDT_forever 302
#define JDT_fork 303
#define JDT_global 304
#define JDT_if 305
#define JDT_initial 306
#define JDT_inout 307
#define JDT_input 308
#define JDT_int 309
#define JDT_join 310
#define JDT_knife 311
#define JDT_list 312
#define JDT_local 313
#define JDT_long 314
#define JDT_marray 315
#define JDT_negedge 316
#define JDT_new 317
#define JDT_node 318
#define JDT_null 319
#define JDT_output 320
#define JDT_overwrite 321
#define JDT_p_and 322
#define JDT_p_or 323
#define JDT_port 324
#define JDT_portset 325
#define JDT_posedge 326
#define JDT_protected 327
#define JDT_randcase 328
#define JDT_repeat 329
#define JDT_return 330
#define JDT_sample 331
#define JDT_signal 332
#define JDT_skew 333
#define JDT_spoon 334
#define JDT_static 335
#define JDT_string 336
#define JDT_stridx 337
#define JDT_strong 338
#define JDT_super 339
#define JDT_teminate 340
#define JDT_terminate 341
#define JDT_this 342
#define JDT_true 343
#define JDT_typedef 344
#define JDT_var 345
#define JDT_verilog 346
#define JDT_void 347
#define JDT_weak 348
#define JDT_while 349
#define JDT_RND 350
#define JDT_CYC 351
#define JD_ANDTILDA 352
#define JD_TILDAAND 353
#define JD_ORTILDA 354
#define JD_TILDAOR 355
#define JD_EORTILDA 356
#define JD_TILDAEOR 357
#define JD_LAND 358
#define JD_LOR 359
#define JD_RSHIFT 360
#define JD_URSHIFT 361
#define JD_LSHIFT 362
#define JD_EQEQ 363
#define JD_LE 364
#define JD_GE 365
#define JD_NE 366
#define JD_EQEQEQ 367
#define JD_NEQEQ 368
#define JD_Q_EQ 369
#define JD_Q_NEQ 370
#define JD_QEQEQ 371
#define JD_EQEQQ 372
#define JD_C_EQEQ 373
#define JD_D_PLUS 374
#define JD_D_MINUS 375
#define JD_D_AT 376
#define JD_C_EQ 377
#define JD_PLUS_EQ 378
#define JD_MINUS_EQ 379
#define JD_STAR_EQ 380
#define JD_SLSH_EQ 381
#define JD_MOD_EQ 382
#define JD_LSHIFT_EQ 383
#define JD_RSHIFT_EQ 384
#define JD_URSHIFT_EQ 385
#define JD_AND_EQ 386
#define JD_OR_EQ 387
#define JD_EOR_EQ 388
#define JD_NAND_EQ 389
#define JD_ANDN_EQ 390
#define JD_NOR_EQ 391
#define JD_ORN_EQ 392
#define JD_NEOR_EQ 393
#define JD_EORN_EQ 394
typedef union {
  int  integer_value ;
  struct {
    char *name ;
    int  linenum ;
    char *filename ;
  } string_value ;
  struct {
    int  linenum ;
    char *filename ;
  } keyword ;
  struct {
    int is_static_flag ;
    int is_extern_flag ;
    int is_local_flag ;
    int is_protected_flag ;
    int is_abstract_flag ;
    int is_overwrite_flag ;
    int is_final_flag ;
    int is_export_flag ;
  } flags ;
  block_node *block ;
  JD_assign_operator assign_opr ;
  expression_node *expression ;
  statement_node  *statement ;
  JD_join_type    join ;
  JD_edge_type    edge ;
  JD_vf_attrib    *vfattr ; 
  JD_port_type     portdir ;
  named_node      *name ;
  JD_expression_type exp_type ;
  JD_strength     strength ;
  connection_node  *connection ;
} YYSTYPE;
extern YYSTYPE yylval;