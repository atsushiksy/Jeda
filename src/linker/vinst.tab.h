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


extern YYSTYPE yylval;