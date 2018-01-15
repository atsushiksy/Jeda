/* 
   
   Copyright (C) 1999, 2000, 2001, 2002 Juniper Networks Inc.

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


/* 
   To reduce the number of garbage, use smallinteger when a integer
   number is between 1073741823 (0x3fffffff) and -805306369 (0xcfffffff).
   In such case, the value is pushed as the form of small integer,
   which remove the bit[30] information (as bit[30] == bit[31] ),
   and bit[0] always set. When stack holds the pointer, usually bit[0] 
   never set, so we can distinguish the small integer by testing bit[0]
   of the pointer.
   
     int -> small integer: {bit[31],bit[29:0],1'b1}
     small integer -> int: {bit[31],bit[31],bit[30:1]}
*/

#define IS_SMLINT(x) (((int)x)&1)

#ifdef JEDA_DEBUGGER
#  define FITSMLINT(x) (0) 
#else
#  ifdef JEDA_NO_SMALL_INT
#    define FITSMLINT(x) (0) 
#  else
#    define FITSMLINT(x) \
      ( x->type == JD_D_int && x->sub_type == JD_SD_single && \
        ( ((x->data.int_value & 0xc0000000) == 0) || \
          ((x->data.int_value & 0xc0000000) == 0xc0000000) )    )
#  endif
#endif

#define GENSMLINT(x) \
  ((JD_data *) \
    ( (x->data.int_value &  0x80000000) | \
      (x->data.int_value << 1         ) | 1 )  )

#define INT2SMLINT(x) \
  ((JD_data *) \
    ( (x &  0x80000000) | \
      (x << 1         ) | 1 )  )

#define SMLINT2INT(x) ((int)x >> 1)

#define BIT_A(x) (x->type == JD_D_sbit)?&x->data.sbit.a:x->data.bit.a 
#define BIT_B(x) (x->type == JD_D_sbit)?&x->data.sbit.b:x->data.bit.b
#define DT_BIT_SIZE(x) \
  ((x->type == JD_D_sbit)?x->data.sbit.size:x->data.bit.size)

#define SET_ACC_INT(x) ACC->type = JD_D_int ; ACC->data.int_value = x

/* number of 32-bit word for bit length x */
#define NWORD(x) ( (x-1)/32 + 1 )
#define LASTWORD(x) ( (x-1)/32  )

#define NBYTE(x) ( sizeof(int) * ( (x-1)/32 + 1 ) )

#define BITMASK(x) ( (unsigned)0xffffffff >> ((32 - ((x) % 32)) % 32) )

#define EXTEND_TMP(x) \
  TMP_A = (unsigned int *)realloc( TMP_A, NBYTE(x) ) ; \
  TMP_B = (unsigned int *)realloc( TMP_B, NBYTE(x) ) ; \
  while( TMP_WD <= NWORD(x) ) { \
    TMP_A[TMP_WD] = 0 ; TMP_B[TMP_WD++] = 0 ; \
  }

#define ADJUST_TEMP(s) \
  { \
    int n, m ; \
    n = LASTWORD(s) ; \
    if( TMP_WD < n ) { \
      EXTEND_TMP(s) ; \
    } \
    else { \
      if( (m = LASTWORD(S0)) <= n ) { \
        while( m <= n ) { TMP_A[m] = 0 ; TMP_B[m++] = 0 ; } \
      } \
    } \
  }

#define ADJUST_TEMP_S0(s) \
  if( S0 < s ) { \
    int n, m ; \
    n = LASTWORD(s) ; \
    if( TMP_WD < n ) { \
      EXTEND_TMP(s) ; \
    } \
    else { \
      if( (m = LASTWORD(S0)) <= n ) { \
        while( m <= n ) { TMP_A[m] = 0 ; TMP_B[m++] = 0 ; } \
      } \
    } \
    S0 = s ; \
  }

#define ADJUST_ACCB(s) \
  { \
    int n, m ; \
    n = LASTWORD(s) ; \
    if( ACCB_WD < n ) { \
      ACCB_A = (unsigned int *)realloc( ACCB_A, NBYTE(s) ) ; \
      ACCB_B = (unsigned int *)realloc( ACCB_B, NBYTE(s) ) ; \
      while( ACCB_WD <= NWORD(s) ) { \
        ACCB_A[ACCB_WD] = 0 ; ACCB_B[ACCB_WD++] = 0 ; \
      } \
    } \
    else { \
      if( (m = NWORD(ACCB_SIZE)) <= n ) { \
        while( m <= n ) { ACCB_A[m] = 0 ; ACCB_B[m++] = 0 ; } \
      } \
    } \
    ACCB_SIZE = s ; \
  }

#define PRINT_WRN(x) JEDA_print_warning(x)
#define PRINT_ERR(x) JEDA_print_error(x)

#define WARNING_UNDETERMINISTIC_RESULT \
  PRINT_WRN( "VALUE X detected during boolean calclation\n" )

#define ERROR_ZERO_DIVIDE \
  PRINT_ERR( "Error: Zero divide\n" )

#define SET_ACC_BOOL(x) \
  if( x < 0 ) { \
    ACC_INT = 0 ; \
    WARNING_UNDETERMINISTIC_RESULT ; \
  } \
  else ACC_INT = x ; \
  ACC_TYPE = JD_D_int 
  

#define SET_TMP_BIT_2_ACC(x) \
  {  unsigned int *tmp, ts ; \
     tmp = ACC_A ; \
     ACC_A = TMP_A ; \
     TMP_A = tmp ; \
     tmp = ACC_B ; \
     ACC_B = TMP_B ; \
     TMP_B = tmp ; \
     ACC_SIZE = x ; \
     ts = ACC_WD ; \
     ACC_WD = TMP_WD ; \
     TMP_WD = ts ; \
  }

#define SWAP_ACC \
  {  unsigned int *tmp, ts ; \
     tmp = ACC_A ; \
     ACC_A = ACCB_A ; \
     ACCB_A = tmp ; \
     tmp = ACC_B ; \
     ACC_B = ACCB_B ; \
     ACCB_B = tmp ; \
     ts = ACC_SIZE ; \
     ACC_SIZE = ACCB_SIZE ; \
     ACCB_SIZE = ts ; \
     ts = ACC_WD ; \
     ACC_WD = ACCB_WD ; \
     ACCB_WD = ts ; \
  }
  
/*
  { int i, j ; \
    j = LASTWORD(x) ; \
    if( j ) { \
      if( ACC_WD < j ) { \
        ACC_A = (unsigned int *)realloc( ACC_A, NBYTE(x) ) ; \
        ACC_B = (unsigned int *)realloc( ACC_B, NBYTE(x) ) ; \
        ACC_WD = j ; \
      } \
      for( i = 0 ; i <= j ; i++ ) { \
        ACC_A[i] = TMP_A[i] ; ACC_B[i] = TMP_B[i] ; \
      } \
      ACC_TYPE = JD_D_bit ; \
    } \
    else { \
      ACC_A[0] = TMP_A[0] ; \
      ACC_B[0] = TMP_B[0] ; \
      ACC_TYPE = JD_D_sbit ; \
    } \
    ACC_BIT_SIZE = x ; \
  } \
*/

#define COPY_BIT_2_TMP(x) \
  if (x->type == JD_D_sbit) { \
    *TMP_A = x->data.sbit.a ; \
    *TMP_B = x->data.sbit.b ; \
  } \
  else { \
    int i, j ; \
    j = LASTWORD(x->data.bit.size) ; \
    for( i = 0 ; i <= j ; i++ ) { \
      TMP_A[i] = x->data.bit.a[i] ; \
      TMP_B[i] = x->data.bit.b[i] ; \
    } \
  } 

#define COPY_INT_2_TMP(x,y) \
  TMP_A[0] = x->data.int_value ; \
  TMP_B[0] = 0 ; \
  if (y>32) { \
    int i, j ; \
    j = LASTWORD(y) ; \
    for( i = 1 ; i <= j ; i++ ) { \
      TMP_A[i] = 0 ; \
      TMP_B[i] = 0 ; \
    } \
  }  

#define SET_INT_2_TMP(x,y) \
  TMP_A[0] = x ; \
  TMP_B[0] = 0 ; \
  if (y>32) { \
    int i, j ; \
    j = LASTWORD(y) ; \
    for( i = 1 ; i <= j ; i++ ) { \
      TMP_A[i] = 0 ; \
      TMP_B[i] = 0 ; \
    } \
  }  

#define CLEAR_ACC \
  if( ACC_TYPE == JD_D_string && ACC_STRING != NULL ) { \
    DEALLOC_STRING(ACC_STRING) ; \
    ACC_STRING = NULL ; \
  } \


JD_data *JD_get_stack() ;
JD_data *JD_pop_stack() ;
void JD_up_stack() ;
void JD_push_vstack( JD_data *data ) ;
void JD_push_acc() ;
void JD_pop_bit_to_accb() ;

#define POP_STACK JD_pop_stack()
#define POP_STACK_INT JD_pop_stack_int()
#define POP_BIT_TO_ACCB JD_pop_bit_to_accb() ;
#define GET_STACK(x) JD_get_stack(x)
#define UP_STACK JD_up_stack()

#define PUSH_STACK(x) JD_push_vstack(x)

#define PUSH_ACC JD_push_acc() ;

#ifdef JEDA_INLINE_OPTIMIZATION

#define CALL_SET_OBJECT(x)  EXEC_SET_OBJECT

#define CALL_BIT_LT_BIT  BIT_LT_BIT
#define CALL_BIT_GT_BIT  BIT_GT_BIT
#define CALL_BIT_EQEQ_BIT  BIT_EQEQ_BIT
#define CALL_BIT_LE_BIT  BIT_LE_BIT
#define CALL_BIT_GE_BIT  BIT_GE_BIT
#define CALL_BIT_NE_BIT  BIT_NE_BIT
#define CALL_BIT_EQEQEQ_BIT  BIT_EQEQEQ_BIT
#define CALL_BIT_NEQEQ_BIT  BIT_NEQEQ_BIT
#define CALL_BIT_EQQEQ_BIT  BIT_EQQEQ_BIT
#define CALL_BIT_EQEQQ_BIT  BIT_EQEQQ_BIT
#define CALL_BIT_QEQEQ_BIT  BIT_QEQEQ_BIT
#define CALL_BIT_QNE_BIT  BIT_QNE_BIT
#define CALL_BIT_MINUS_BIT  BIT_MINUS_BIT
#define CALL_BIT_PLUS_BIT  BIT_PLUS_BIT
#define CALL_BIT_TIMES_BIT  BIT_TIMES_BIT
/*
#define CALL_BIT_ODIV_BIT  BIT_ODIV_BIT
#define CALL_BIT_DIV_BIT  BIT_DIV_BIT
#define CALL_BIT_OMOD_BIT  BIT_OMOD_BIT
#define CALL_BIT_MOD_BIT  BIT_MOD_BIT
*/

#define CALL_BIT_ODIV_BIT  JD_bit_odiv_bit(AR, BR SR,, A0, B0, S0, A1, B1, S1, &ERR)
#define CALL_BIT_DIV_BIT  JD_bit_div_bit(AR, BR, SR, A0, B0, S0, A1, B1, S1, &ERR)
#define CALL_BIT_OMOD_BIT  JD_bit_omod_bit(AR, BR, SR, A0, B0, S0, A1, B1, S1, &ERR)
#define CALL_BIT_MOD_BIT  JD_bit_mod_bit(AR, BR, SR, A0, B0, S0, A1, B1, S1, &ERR)

#define CALL_BIT_AND_BIT  BIT_AND_BIT
#define CALL_BIT_OR_BIT  BIT_OR_BIT
#define CALL_BIT_EOR_BIT  BIT_EOR_BIT
#define CALL_BIT_NAND_BIT  BIT_NAND_BIT
#define CALL_BIT_NOR_BIT  BIT_NOR_BIT
#define CALL_BIT_NEOR_BIT  BIT_NEOR_BIT
#define CALL_U_MINUS_BIT  U_MINUS_BIT
#define CALL_U_TILDE_BIT  U_TILDE_BIT
#define CALL_U_NOT_BIT  U_NOT_BIT
#define CALL_U_AND_BIT  U_AND_BIT
#define CALL_U_OR_BIT  U_OR_BIT
#define CALL_U_EOR_BIT  U_EOR_BIT
#define CALL_U_NAND_BIT  U_NAND_BIT
#define CALL_U_NOR_BIT  U_NOR_BIT
#define CALL_U_NEOR_BIT  U_NEOR_BIT

#else

#define CALL_SET_OBJECT(x)  set_object(x)

#define CALL_BIT_LT_BIT  JD_bit_lt_bit(A0, B0, S0, A1, B1, S1)
#define CALL_BIT_GT_BIT  JD_bit_gt_bit(A0, B0, S0, A1, B1, S1)
#define CALL_BIT_EQEQ_BIT  JD_bit_eqeq_bit(A0, B0, S0, A1, B1, S1)
#define CALL_BIT_LE_BIT  JD_bit_le_bit(A0, B0, S0, A1, B1, S1)
#define CALL_BIT_GE_BIT  JD_bit_ge_bit(A0, B0, S0, A1, B1, S1)
#define CALL_BIT_NE_BIT  JD_bit_ne_bit(A0, B0, S0, A1, B1, S1)
#define CALL_BIT_EQEQEQ_BIT  JD_bit_eqeqeq_bit(A0, B0, S0, A1, B1, S1)
#define CALL_BIT_NEQEQ_BIT  JD_bit_neqeq_bit(A0, B0, S0, A1, B1, S1)
#define CALL_BIT_EQQEQ_BIT  JD_bit_eqQeq_bit(A0, B0, S0, A1, B1, S1)
#define CALL_BIT_EQEQQ_BIT  JD_bit_eqeqQ_bit(A0, B0, S0, A1, B1, S1)
#define CALL_BIT_QEQEQ_BIT  JD_bit_Qeqeq_bit(A0, B0, S0, A1, B1, S1)
#define CALL_BIT_QNE_BIT  JD_bit_Qne_bit(A0, B0, S0, A1, B1, S1)
#define CALL_BIT_MINUS_BIT  JD_bit_minus_bit(A0, B0, S0, A1, B1, S1)
#define CALL_BIT_SUB_BIT  JD_bit_sub_bit(A0, B0, S0, A1, B1, S1)
#define CALL_BIT_PLUS_BIT  JD_bit_plus_bit(A0, B0, S0, A1, B1, S1)
#define CALL_BIT_ADD_BIT  JD_bit_add_bit(A0, B0, S0, A1, B1, S1)
#define CALL_BIT_TIMES_BIT  JD_bit_times_bit(AR, BR, SR, A0, B0, S0, A1, B1, S1)
#define CALL_BIT_ODIV_BIT  JD_bit_odiv_bit(AR, BR SR,, A0, B0, S0, A1, B1, S1, &ERR)
#define CALL_BIT_DIV_BIT  JD_bit_div_bit(AR, BR, SR, A0, B0, S0, A1, B1, S1, &ERR)
#define CALL_BIT_OMOD_BIT  JD_bit_omod_bit(AR, BR, SR, A0, B0, S0, A1, B1, S1, &ERR)
#define CALL_BIT_MOD_BIT  JD_bit_mod_bit(AR, BR, SR, A0, B0, S0, A1, B1, S1, &ERR)
#define CALL_BIT_AND_BIT  JD_bit_and_bit(A0, B0, S0, A1, B1, S1)
#define CALL_BIT_OR_BIT  JD_bit_or_bit(A0, B0, S0, A1, B1, S1)
#define CALL_BIT_EOR_BIT  JD_bit_eor_bit(A0, B0, S0, A1, B1, S1)
#define CALL_BIT_NAND_BIT  JD_bit_nand_bit(A0, B0, S0, A1, B1, S1)
#define CALL_BIT_NOR_BIT  JD_bit_nor_bit(A0, B0, S0, A1, B1, S1)
#define CALL_BIT_NEOR_BIT  JD_bit_neor_bit(A0, B0, S0, A1, B1, S1)
#define CALL_U_MINUS_BIT  JD_u_minus_bit(A0, B0, S0)
#define CALL_U_TILDE_BIT  JD_u_tilde_bit(A0, B0, S0)

#define CALL_U_NOT_BIT  RET = JD_u_not_bit(A0, B0, S0)
#define CALL_U_AND_BIT  RET = JD_u_and_bit(A0, B0, S0)
#define CALL_U_OR_BIT  RET = JD_u_or_bit(A0, B0, S0)
#define CALL_U_EOR_BIT  RET = JD_u_eor_bit(A0, B0, S0)
#define CALL_U_NAND_BIT  RET = JD_u_nand_bit(A0, B0, S0)
#define CALL_U_NOR_BIT  RET = JD_u_nor_bit(A0, B0, S0)
#define CALL_U_NEOR_BIT  RET = JD_u_neor_bit(A0, B0, S0)

#endif
