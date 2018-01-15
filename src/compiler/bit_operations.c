/***************************************************************************
 *
 *  bit_operations.c: Bit operation Calculation Functions
 *
 *  Author: Atsushi Kasuya
 *
 *
 ***************************************************************************/
/* 
   
   Copyright (C) 1999, 2000, 2001 Juniper Networks Inc.

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

#include "../include/compiler_macro.h"
#include <stdio.h>

extern int RET ;

/* bit '<' bit */
int JD_bit_lt_bit( int *A0, int *B0, int S0, int *A1, int *B1, int S1 ) {
  /* register int RET ; */
  /**MACRO BIT_LT_BIT**/
  {
    register int i, w0, w1 ;
    w0 = LASTWORD(S0) ;
    w1 = LASTWORD(S1) ;
    for( i = 0 ; i <= w0 ; i++ ) {
      if( B0[i] ) {
        RET = -1 ;
        goto eval_end ;
      }
    }
    for( i = 0 ; i <= w1 ; i++ ) {
      if( B1[i] ) {
        RET = -1 ;
        goto eval_end ;
      }
    }
    for( i = w0+1 ; i <= w1 ; i++ ) {
      if( A1[i] ) {
        RET = 1 ;
        goto eval_end ;  /* A0 < A1 */
      }
    }
    for( i = w0 ; i >= 0 ; i-- ) {
      if( i > w1 ) {
        if( A0[i] ) {
          RET = 0 ;
          goto eval_end ;  /* A0 > A1 */
        }
      }
      else {
        if( (unsigned)A0[i] > (unsigned)A1[i] ) {
          RET =  0 ;
          goto eval_end ;  /* A0 > A1 */
        }
        if( (unsigned)A0[i] < (unsigned)A1[i] ) {
          RET = 1 ;
          goto eval_end ;  /* A0 < A1 */
        }
      }
    }
    RET = 0 ; /* A0 == A1 */
    
  eval_end:
  }
  /**END MACRO BIT_LT_BIT**/
  return RET ;
}

/* bit '>' bit */
int JD_bit_gt_bit( int *A0, int *B0, int S0, int *A1, int *B1, int S1 ) {
  /* register int RET ; */
  /**MACRO BIT_GT_BIT**/
  {
    register int i, w0, w1 ;
    w0 = LASTWORD(S0) ;
    w1 = LASTWORD(S1) ;
    for( i = 0 ; i <= w0 ; i++ ) {
      if( B0[i] ) {
        RET = -1 ;
        goto eval_end ;
      }
    }
    for( i = 0 ; i <= w1 ; i++ ) {
      if( B1[i] ) {
        RET = -1 ;
        goto eval_end ;
      }
    }
    for( i = w0+1 ; i <= w1 ; i++ ) {
      if( A1[i] ) {
        RET = 0 ;
        goto eval_end ;  /* A0 < A1 */
      }
    }
    for( i = w0 ; i >= 0 ; i-- ) {
      if( i > w1 ) {
        if( A0[i] ) {
          RET = 1 ;
          goto eval_end ;  /* A0 > A1 */
        }
      }
      else {
        if( (unsigned)A0[i] > (unsigned)A1[i] ) {
          RET = 1 ;
          goto eval_end ;  /* A0 > A1 */
        }
        if( (unsigned)A0[i] < (unsigned)A1[i] ) {
          RET = 0 ;
          goto eval_end ;  /* A0 < A1 */
        }
      }
    }
    RET = 0 ; /* A0 == A1 */
    
  eval_end:
  }
  /**END MACRO BIT_GT_BIT**/
  return RET ;
}

/* bit '==' bit */
int JD_bit_eqeq_bit( int *A0, int *B0, int S0, int *A1, int *B1, int S1 ) {
  /* register int RET ; */
  /**MACRO BIT_EQEQ_BIT**/
  {
    register int i, w0, w1 ;
    w0 = LASTWORD(S0) ;
    w1 = LASTWORD(S1) ;
    for( i = 0 ; i <= w0 ; i++ ) {
      if( B0[i] ) {
        RET = -1 ;
        goto eval_end ;
      }
    }
    for( i = 0 ; i <= w1 ; i++ ) {
      if( B1[i] ) {
        RET = -1 ;
        goto eval_end ;
      }
    }
    for( i = w0+1 ; i <= w1 ; i++ ) {
      if( A1[i] ) {
        RET = 0 ;
        goto eval_end ;  /* A0 < A1 */
      }
    }
    for( i = w0 ; i >= 0 ; i-- ) {
      if( i > w1 ) {
        if( A0[i] ) {
          RET = 0 ;
          goto eval_end ;  /* A0 > A1 */
        }
      }
      else {
        if( A0[i] > A1[i] ) {
          RET = 0 ;
          goto eval_end ;  /* A0 > A1 */
        }
        if( A0[i] < A1[i] ) {
          RET = 0 ;
          goto eval_end ;  /* A0 < A1 */
        }
      }
    }
    RET = 1 ; /* A0 == A1 */
    
  eval_end:
  }
  /**END MACRO BIT_EQEQ_BIT**/
  return RET ;
}

/* bit '<=' bit */
int JD_bit_le_bit( int *A0, int *B0, int S0, int *A1, int *B1, int S1 ) {
  /* register int RET ; */
  /**MACRO BIT_LE_BIT**/
  {
    register int i, w0, w1 ;
    w0 = LASTWORD(S0) ;
    w1 = LASTWORD(S1) ;
    for( i = 0 ; i <= w0 ; i++ ) {
      if( B0[i] ) {
        RET = -1 ;
        goto eval_end ;
      }
    }
    for( i = 0 ; i <= w1 ; i++ ) {
      if( B1[i] ) {
        RET = -1 ;
        goto eval_end ;
      }
    }
    for( i = w0+1 ; i <= w1 ; i++ ) {
      if( A1[i] ) {
        RET = 1 ;
        goto eval_end ;  /* A0 < A1 */
      }
    }
    for( i = w0 ; i >= 0 ; i-- ) {
      if( i > w1 ) {
        if( A0[i] ) {
          RET = 0 ;
          goto eval_end ;  /* A0 > A1 */
        }
      }
      else {
        if( (unsigned)A0[i] > (unsigned)A1[i] ) {
          RET = 0 ;
          goto eval_end ;  /* A0 > A1 */
        }
        if( (unsigned)A0[i] < (unsigned)A1[i] ) {
          RET = 1 ;
          goto eval_end ;  /* A0 < A1 */
        }
      }
    }
    RET = 1 ; /* A0 == A1 */
    
  eval_end:
  }
  /**END MACRO BIT_LE_BIT**/
  return RET ;
}

/* bit '>=' bit */
int JD_bit_ge_bit( int *A0, int *B0, int S0, int *A1, int *B1, int S1 ) {
  /* register int RET ; */
  /**MACRO BIT_GE_BIT**/
  {
    register int i, w0, w1 ;
    w0 = LASTWORD(S0) ;
    w1 = LASTWORD(S1) ;
    for( i = 0 ; i <= w0 ; i++ ) {
      if( B0[i] ) {
        RET = -1 ;
        goto eval_end ;
      }
    }
    for( i = 0 ; i <= w1 ; i++ ) {
      if( B1[i] ) {
        RET = -1 ;
        goto eval_end ;
      }
    }
    for( i = w0+1 ; i <= w1 ; i++ ) {
      if( A1[i] ) {
        RET = 0 ;
        goto eval_end ;  /* A0 < A1 */
      }
    }
    for( i = w0 ; i >= 0 ; i-- ) {
      if( i > w1 ) {
        if( A0[i] ) {
          RET = 1 ;
          goto eval_end ;  /* A0 > A1 */
        }
      }
      else {
        if( (unsigned)A0[i] > (unsigned)A1[i] ) {
          RET = 1 ;
          goto eval_end ;  /* A0 > A1 */
        }
        if( (unsigned)A0[i] < (unsigned)A1[i] ) {
          RET = 0 ;
          goto eval_end ;  /* A0 < A1 */
        }
      }
    }
    RET = 1 ; /* A0 == A1 */
    
  eval_end:
  }
  /**END MACRO BIT_GE_BIT**/
  return RET ;
}

/* static version of bit_ge_bit, which does not check B0/B1 */
static int bit_ge_bit( int *A0, int S0, int *A1, int S1 ) {
  /* register int RET ; */
  {
    register int i, w0, w1 ;
    w0 = LASTWORD(S0) ;
    w1 = LASTWORD(S1) ;
    for( i = w0+1 ; i <= w1 ; i++ ) {
      if( A1[i] ) {
        return 0 ;
      }
    }
    for( i = w0 ; i >= 0 ; i-- ) {
      if( i > w1 ) {
        if( A0[i] ) {
          return 1 ;
        }
      }
      else {
        if( (unsigned)A0[i] > (unsigned)A1[i] ) {
          return 1 ;  /* A0 > A1 */
        }
        if( (unsigned)A0[i] < (unsigned)A1[i] ) {
          return 0 ;  /* A0 < A1 */
        }
      }
    }
    return 1 ; /* A0 == A1 */
  }
}

/* bit '!=' bit */
int JD_bit_ne_bit( int *A0, int *B0, int S0, int *A1, int *B1, int S1 ) {
  /* register int RET ; */
  /**MACRO BIT_NE_BIT**/
  {
    register int i, w0, w1 ;
    w0 = LASTWORD(S0) ;
    w1 = LASTWORD(S1) ;
    for( i = 0 ; i <= w0 ; i++ ) {
      if( B0[i] ) {
        RET = -1 ;
        goto eval_end ;
      }
    }
    for( i = 0 ; i <= w1 ; i++ ) {
      if( B1[i] ) {
        RET = -1 ;
        goto eval_end ;
      }
    }
    for( i = w0+1 ; i <= w1 ; i++ ) {
      if( A1[i] ) {
        RET = 1 ;
        goto eval_end ;  /* A0 < A1 */
      }
    }
    for( i = w0 ; i >= 0 ; i-- ) {
      if( i > w1 ) {
        if( A0[i] ) {
          RET = 1 ;
          goto eval_end ;  /* A0 > A1 */
        }
      }
      else {
        if( A0[i] > A1[i] ) {
          RET = 1 ;
          goto eval_end ;  /* A0 > A1 */
        }
        if( A0[i] < A1[i] ) {
          RET = 1 ;
          goto eval_end ;  /* A0 < A1 */
        }
      }
    }
    RET = 0 ; /* A0 == A1 */
    
  eval_end:
  }
  /**END MACRO BIT_NE_BIT**/
  return RET ;
}

/* bit '===' bit */
int JD_bit_eqeqeq_bit( int *A0, int *B0, int S0, int *A1, int *B1, int S1 ) {
  /* register int RET ; */
  /**MACRO BIT_EQEQEQ_BIT**/
  {
    register int i, w0, w1 ;
    w0 = LASTWORD(S0) ;
    w1 = LASTWORD(S1) ;
    for( i = w0+1 ; i <= w1 ; i++ ) {
      if( A1[i] || B1[i] ) {
        RET = 0 ;
        goto eval_end ;  /*  */
      }
    }
    for( i = w0 ; i >= 0 ; i-- ) {
      if( i > w1 ) {
        if( A0[i] || B0[i] ) {
          RET = 0 ;
          goto eval_end ;  /* A0 > A1 */
        }
      }
      else {
        if( A0[i] != A1[i] || B0[i] != B1[i] ) {
          RET = 0 ;
          goto eval_end ;  /*  */
        }
      }
    }
    RET = 1 ; /* A0 == A1 */
    
  eval_end:
  }
  /**END MACRO BIT_EQEQEQ_BIT**/
  return RET ;
}

/* bit '!==' bit */
int JD_bit_neqeq_bit( int *A0, int *B0, int S0, int *A1, int *B1, int S1 ) {
  /* register int RET ; */
  /**MACRO BIT_NEQEQ_BIT**/
  {
    register int i, w0, w1 ;
    w0 = LASTWORD(S0) ;
    w1 = LASTWORD(S1) ;
    for( i = w0+1 ; i <= w1 ; i++ ) {
      if( A1[i] || B1[i] ) {
        RET = 1 ;
        goto eval_end ;  /*  */
      }
    }
    for( i = w0 ; i >= 0 ; i-- ) {
      if( i > w1 ) {
        if( A0[i] || B0[i] ) {
          RET = 1 ;
          goto eval_end ;  /* A0 > A1 */
        }
      }
      else {
        if( A0[i] != A1[i] || B0[i] != B1[i] ) {
          RET = 1 ;
          goto eval_end ;  /*  */
        }
      }
    }
    RET = 0 ; /* A0 == A1 */
    
  eval_end:
  }
  /**END MACRO BIT_NEQEQ_BIT**/
  return RET ;
}

/* bit '=?=' bit */
int JD_bit_eqQeq_bit( int *A0, int *B0, int S0, int *A1, int *B1, int S1 ) {
  /* register int RET ; */
  /**MACRO BIT_EQQEQ_BIT**/
  {
    register int i, w0, w1 ;
    w0 = LASTWORD(S0) ;
    w1 = LASTWORD(S1) ;
    for( i = w0+1 ; i <= w1 ; i++ ) {
      if( A1[i] & ~B1[i] ) {
        RET = 0 ;
        goto eval_end ;  /*  */
      }
    }
    for( i = w0 ; i >= 0 ; i-- ) {
      if( i > w1 ) {
        if( A0[i] & ~B0[i] ) {
          RET = 0 ;
          goto eval_end ;  /* A0 > A1 */
        }
      }
      else {
        /* mask both size */
        if( ((A0[i] & ~B0[i] & ~B1[i]) != (A1[i] & ~B1[i] & ~B0[i])) ) 
        {
          RET = 0 ;
          goto eval_end ;  /*  */
        }
      }
    }
    RET = 1 ; /* A0 == A1 */
    
  eval_end:
  }
  /**END MACRO BIT_EQQEQ_BIT**/
  return RET ;
}

/* bit '==?' bit */
int JD_bit_eqeqQ_bit( int *A0, int *B0, int S0, int *A1, int *B1, int S1 ) {
  /* register int RET ; */
  /**MACRO BIT_EQEQQ_BIT**/
  {
    register int i, w0, w1 ;
    w0 = LASTWORD(S0) ;
    w1 = LASTWORD(S1)  ;
    for( i = w0+1 ; i <= w1 ; i++ ) {
      if( A1[i] & ~B1[i] ) {
        RET = 0 ;
        goto eval_end ;  /*  */
      }
    }
    for( i = w0 ; i >= 0 ; i-- ) {
      if( i > w1 ) {
        if( A0[i] || B0[i] ) {
          RET = 0 ;
          goto eval_end ;  /* A0 > A1 */
        }
      }
      else {
        if( ( (A0[i] & ~B1[i]) != (A1[i] & ~B1[i]) ) || (B0[i] & ~B1[i]) ){
          RET = 0 ;
          goto eval_end ;  /*  */
        }
      }
    }
    RET = 1 ; /* A0 == A1 */
    
  eval_end:
  }
  /**END MACRO BIT_EQEQQ_BIT**/
  return RET ;
}

/* bit '?==' bit */
int JD_bit_Qeqeq_bit( int *A0, int *B0, int S0, int *A1, int *B1, int S1 ) {
  /* register int RET ; */
  /**MACRO BIT_QEQEQ_BIT**/
  {
    register int i, w0, w1 ;
    w0 = LASTWORD(S0) ;
    w1 = LASTWORD(S1) ;
    for( i = w0+1 ; i <= w1 ; i++ ) {
      if( A1[i] || B1[i] ) {
        RET = 0 ;
        goto eval_end ;  /*  */
      }
    }
    for( i = w0 ; i >= 0 ; i-- ) {
      if( i > w1 ) {
        if( A0[i] & ~B0[i] ) {
          RET = 0 ;
          goto eval_end ;  /* A0 > A1 */
        }
      }
      else {
        if( ( (A0[i] & ~B0[i]) != (A1[i] & ~B0[i]) ) || (B1[i] & ~B0[i]) ) {
          RET = 0 ;
          goto eval_end ;  /*  */
        }
      }
    }
    RET = 1 ; /* A0 == A1 */
    
  eval_end:
  }
  /**END MACRO BIT_QEQEQ_BIT**/
  return RET ;
}

/* bit '!?=' bit */
int JD_bit_Qne_bit( int *A0, int *B0, int S0, int *A1, int *B1, int S1 ) {
  /* register int RET ; */
  /**MACRO BIT_QNE_BIT**/
  {
    register int i, w0, w1 ;
    w0 = LASTWORD(S0) ;
    w1 = LASTWORD(S1) ;
    for( i = w0+1 ; i <= w1 ; i++ ) {
      if( A1[i] & ~B1[i] ) {
        RET = 0 ;
        goto eval_end ;  /*  */
      }
    }
    for( i = w0 ; i >= 0 ; i-- ) {
      if( i > w1 ) {
        if( A0[i] & ~B0[i] ) {
          RET = 1 ;
          goto eval_end ;  /* A0 > A1 */
        }
      }
      else {
        /* mask both size */
        if( ((A0[i] & ~B0[i] & ~B1[i]) != (A1[i] & ~B1[i] & ~B0[i])) ) 
        {
          RET = 1 ;
          goto eval_end ;  /*  */
        }
      }
    }
    RET = 0 ; /* A0 == A1 */
    
  eval_end:
  }
  /**END MACRO BIT_QNE_BIT**/
  return RET ;
}

/* bit '=:=' bit : bit */
int JD_bit_eqCeq_bit_bit( 
  int *AR, int *BR, int SR, int *A0, int *B0, int S0, int *A1, int *B1, int S1 
) {
  /* register int RET ; */
  /**MACRO BIT_EQCEQ_BIT_BIT**/
  {
    register int i, w0, w1, w2, m ;
    w0 = LASTWORD(S0) ;
    w1 = LASTWORD(S1) ;
    w2 = LASTWORD(SR) ;
    for( i = w0+1 ; i <= w1 ; i++ ) {
      m = (i <= w2) ? AR[i]|BR[i] : 0 ;
      if( A1[i] & m ) {
        RET = 0 ;
        goto eval_end ;  /*  */
      }
    }
    for( i = w0 ; i >= 0 ; i-- ) {
      m = (i <= w2) ? AR[i]|BR[i] : 0 ;
      if( i > w1 ) {
        if( A0[i] & m ) {
          RET = 0 ;
          goto eval_end ;  /* A0 > A1 */
        }
      }
      else {
        /* mask both size */
        if( ((A0[i] & m) != (A1[i] & m)) ) 
        {
          RET = 0 ;
          goto eval_end ;  /*  */
        }
      }
    }
    RET = 1 ; /* A0 == A1 */
    
  eval_end:
  }
  /**END MACRO BIT_EQCEQ_BIT_BIT**/
  return RET ;
}

/* bit '-' bit   */
void JD_bit_minus_bit( int *A0, int *B0, int S0, int *A1, int *B1, int S1 ) {
  /**MACRO BIT_MINUS_BIT**/
  {
    register int i, w0, w1;
    register unsigned int m0, t, a, ap, acy, cy ;
    
    w0 = LASTWORD(S0) ;
    w1 = LASTWORD(S1) ;
    cy = 0 ;
    m0 = (unsigned)0xffffffff >> (32- (S0%32))%32 ;
    for( i = 0 ; i <= w0 ; i++ ) {
      if( B0[i] || (i <= w1 && B1[i]) ) {
        cy = 1 ;
        break ;
      }
    }
    if( cy ) {
      for( i = 0 ; i <= w0 ; i++ ) {
        A0[i] = B0[i] = 0xffffffff ;
      }
      A0[w0] &= m0 ;
      B0[w0] &= m0 ;
    }
    else {
      cy = 1 ;
      acy = 0 ;
      for( i = 0 ; i <= w0 ; i++ ) {
        if( i <= w1 ) {  
          a = ~A1[i] + cy ;
          if( i == w1 ) {
            if( a & 0x80000000 ) cy = 1 ;
            else cy = 0 ;
          }
          else {
            if( a == 0 ) cy = 1 ;
            else cy = 0 ;
          }
        }
        else a = cy?0xffffffff:0 ;
        ap = A0[i] ;
        A0[i] = t = ap + a + acy ;
        if( (t < a) || (t < ap) || (t < acy) ) acy = 1 ;
        else acy = 0 ;
      }
      A0[w0] &= m0 ;
    }
  }
  /**END MACRO BIT_MINUS_BIT**/
} 

void JD_bit_sub_bit( int *A0, int S0, int *A1, int S1 ) {
  /**MACRO BIT_SUB_BIT**/
  {
    register int i, w0, w1;
    register unsigned int m0, t, a, ap, acy, cy ;
    
    w0 = LASTWORD(S0) ;
    w1 = LASTWORD(S1) ;
    m0 = (unsigned)0xffffffff >> (32- (S0%32))%32 ;
    cy = 1 ;
    acy = 0 ;
    for( i = 0 ; i <= w0 ; i++ ) {
      if( i <= w1 ) {  
        a = ~A1[i] + cy ;
        if( i == w1 ) {
          if( a & 0x80000000 ) cy = 1 ;
          else cy = 0 ;
        }
        else {
          if( a == 0 ) cy = 1 ;
          else cy = 0 ;
        }
      }
      else a = cy?0xffffffff:0 ;
      ap = A0[i] ;
      A0[i] = t =  ap + a + acy ;
      if( (t < a) || (t < ap) || (t < acy) ) acy = 1 ;
      else acy = 0 ;
    }
    A0[w0] &= m0 ;
  }
  /**END MACRO BIT_SUB_BIT**/
} 

/* bit '+' bit   */
void JD_bit_plus_bit( int *A0, int *B0, int S0, int *A1, int *B1, int S1 ) 
{
  /**MACRO BIT_PLUS_BIT**/
  {
    register int i, w0, w1;
    register unsigned int m0, t, a, ap, acy ;
    
    w0 = LASTWORD(S0) ;
    w1 = LASTWORD(S1) ;
    acy = 0 ;
    m0 = (unsigned)0xffffffff >> (32- (S0%32))%32 ;
    for( i = 0 ; i <= w0 ; i++ ) {
      if( B0[i] || (i <= w1 && B1[i]) ) {
        acy = 1 ;
        break ;
      }
    }
    if( acy ) {
      for( i = 0 ; i <= w0 ; i++ ) {
        A0[i] = B0[i] = 0xffffffff ;
      }
      A0[w0] &= m0 ;
      B0[w0] &= m0 ;
    }
    else {
      acy = 0 ;
      for( i = 0 ; i <= w0 ; i++ ) {
        if( i <= w1 ) {  
          a = A1[i] ;
        }
        else a = 0 ;
        ap = A0[i] ;
        A0[i] = t = (ap+a+acy) & ( (i==w0)?m0:0xffffffff ) ;
        if( (t < a) || (t < ap) || (t < acy) ) acy = 1 ;
        else acy = 0 ;
      }
      A0[w0] &= m0 ;
    }
  }
  /**END MACRO BIT_PLUS_BIT**/
}

void JD_bit_add_bit( int *A0, int S0, int *A1, int S1 ) 
{
  /**MACRO BIT_ADD_BIT**/
  {
    register int i, w0, w1;
    register unsigned int m0, t, a, ap, acy ;
    
    w0 = LASTWORD(S0) ;
    w1 = LASTWORD(S1) ;
    acy = 0 ;
    m0 = (unsigned)0xffffffff >> (32- (S0%32))%32 ;
    for( i = 0 ; i <= w0 ; i++ ) {
      if( i <= w1 ) {  
        a = A1[i] ;
      }
      else a = 0 ;
      ap = A0[i] ;
      A0[i] = t =  ap + a + acy ;
      if( (t < a) || (t < ap) || (t < acy) ) acy = 1 ;
      else acy = 0 ;
    }
    A0[w0] &= m0 ;
  }
  /**END MACRO BIT_ADD_BIT**/
}

/* bit '*' bit   */
/* Field in AR must be sufficient length, and cleared to zero */ 
void JD_bit_times_bit( 
  int *AR, int *BR, int SR, int *A0, int *B0, int S0, int *A1, int *B1, int S1 
) 
{    
  /**MACRO BIT_TIMES_BIT**/
  {
    unsigned mr ;
    int wr ;
    register int i, j, k, w0, w1, w2 ;
    register unsigned t0, t1, t2 ;

    w0 = NWORD(S0) ;
    w1 = NWORD(S1) ;
    w2 = NWORD(SR) ;
    mr = (unsigned)0xffffffff >> (32- (SR%32))%32 ;
    t0 = 0 ;
    /* first detecting if X or Z exists. If so result be all x */
    for( i = 0 ; i < w0 ; i++ ) {
      if( B0[i] ) {
        t0 = 1 ;
        break ;
      }
    }
    if( !t0 ) {
      for( i = 0 ; i < w1 ; i++ ) {
        if( B1[i] ) {
          t0 = 1 ;
          break ;
        }
      }
    }
    if( t0 ) {
      for( i = 0 ; i < w2 ; i++ ) {
        AR[i] = BR[i] = 0xffffffff ;
      }
      AR[w2-1] &= mr ;
      BR[w2-1] &= mr ;
    }
    else {
      /* multiplication process starts here.  Calculation is
       * done as 16-bit multiplication
       */
      for( i = 0 ; i < w2 ; i++ ) {
        AR[i] = BR[i] = 0 ;
      }
      w0 <<= 1 ;  /* now count half word */
      w1 <<= 1 ;
      wr = w2 ;
      w2 <<= 1 ;
      for( i = 0 ; i < w0 ; i++ ) {
        t0 = GETSHORT(A0,i) ;
        for( j = 0 ; j < w1 ; j++ ) { 
          t1 = GETSHORT(A1,j) ;
          if( t0 && t1 ) {
            t1 = t0 * t1 ;
            /* setting back the result */
            k = i + j ;    /* index to sum the result */
            /* get 16-bit word from result area */
            t2 = GETSHORT(AR, k) ;
            /* add low 16-bit of multiply to it */
            t2 = t2 + (t1 & 0xffff) ;
            SETSHORT(AR, (t2 & 0xffff), k ) ;
            t1 = (t1>>16) + (t2>>16) ;
            k++ ;
            /* carry(t1) propagation on AR */
            for(  ; t1 && k <= w2 ; k++ ) {
              t2 = GETSHORT(AR, k) + t1 ;
              SETSHORT(AR, (t2 & 0xffff), k ) ;
              t1 = t2 >> 16 ;
            }  /* end of foo( ; cy && k <= wr ; .. */
          }  /* end of if( t0 &&  t1 ) */
        }  /* end of for( j = .. */
      }  /* end of for( i = .. */
      AR[wr-1] &= mr ;
    } /* end of else */
  }
  /**END MACRO BIT_TIMES_BIT**/
}

/* preallocated Y register */
static unsigned *Y = NULL ;
static int Y_SIZE = 0 ;

/* bit '/' bit   */
void JD_bit_div_bit( 
  int *AR, int *BR, int SR,
  int *A0, int *B0, int S0, 
  int *A1, int *B1, int S1, 
  int *ERR                   /* error will be set on zero-divide */
) 
{       
  /**MACRO BIT_ODIV_BIT**/
  {
    unsigned m0 ;
    register int i, j, k ;
    register int w0, w1, w2, cy ;
    
    w0 = LASTWORD(S0) ;
    w1 = LASTWORD(S1) ;
    w2 = LASTWORD(SR) ;
    *ERR = 0 ;
    
    /* Y register allocation*/
    if( Y_SIZE <= w0 || Y == NULL ) {
      if( Y == NULL ) {
        Y = (unsigned *)calloc( (w0>256)?w0:256 , sizeof(unsigned) ) ;
        Y_SIZE = (w0>256)?w0:256 ;
      }
      else {
        free(Y) ;
        Y = (unsigned *)calloc( w0, sizeof(unsigned) ) ;
        Y_SIZE = w0 ;
      }
    }
    m0 = (unsigned)0xffffffff >> (32- (SR%32))%32 ;
    cy = 0 ;
    for( i = 0 ; i <= w0 ; i++ ) {
      if( B0[i] ) {
        cy = 1 ;
        break ;
      }
    }
    if( !cy ) {
      for( i = 0 ; i <= w1 ; i++ ) {
        if( B1[i] ) {
          cy = 1 ;
          break ;
        } 
      }
    }
    if( cy ) {
      for( i = 0 ; i <= w0 ; i++ ) {
        AR[i] = BR[i] = 0xffffffff ;
      }
      for(  ; i <= w2 ; i++ ) AR[i] = BR[i] = 0xffffffff ;
      AR[w2] &= m0 ;
      BR[w2] &= m0 ;
    }
    else {
      if( w1 == 0 ) {
        /* divider is one word, thus check if power of two */
        /* optimization */
        switch( *A1 ) {
          case 0: *ERR = 1 ; break ; /* zero divide error */
          case 0x00000001: cy = -1 ; break ;
          case 0x00000002: cy = 1 ; break ;
          case 0x00000004: cy = 2 ; break ;
          case 0x00000008: cy = 3 ; break ;
          case 0x00000010: cy = 4 ; break ;
          case 0x00000020: cy = 5 ; break ;
          case 0x00000040: cy = 6 ; break ;
          case 0x00000080: cy = 7 ; break ;
          case 0x00000100: cy = 8 ; break ;
          case 0x00000200: cy = 9 ; break ;
          case 0x00000400: cy = 10 ; break ;
          case 0x00000800: cy = 11 ; break ;
          case 0x00001000: cy = 12 ; break ;
          case 0x00002000: cy = 13 ; break ;
          case 0x00004000: cy = 14 ; break ;
          case 0x00008000: cy = 15 ; break ;
          case 0x00010000: cy = 16 ; break ;
          case 0x00020000: cy = 17 ; break ;
          case 0x00040000: cy = 18 ; break ;
          case 0x00080000: cy = 19 ; break ;
          case 0x00100000: cy = 20 ; break ;
          case 0x00200000: cy = 21 ; break ;
          case 0x00400000: cy = 22 ; break ;
          case 0x00800000: cy = 23 ; break ;
          case 0x01000000: cy = 24 ; break ;
          case 0x02000000: cy = 25 ; break ;
          case 0x04000000: cy = 26 ; break ;
          case 0x08000000: cy = 27 ; break ;
          case 0x10000000: cy = 28 ; break ;
          case 0x20000000: cy = 29 ; break ;
          case 0x40000000: cy = 30 ; break ;
          case 0x80000000: cy = 31 ; break ;
        } /* end of switch .. */
        if( cy != 0 ) {
          for( i = 0 ; i <= w0 ; i++ ) AR[i] = A0[i] ;
          for( ; i < w2 ; i++ ) AR[i] = 0 ;
          if( cy > 0 ) {
            BIT_RIGHT_SHIFT( AR, w2, cy ) ;
          }
        }
      } /* end of if( w1 = 0 ) */
      if( !cy && !*ERR ) {
        /* normal bit division */
        cy = 1 ;
        /* check for zero-divide */
        for( i = 0 ; i <= w0 ; i++ ) {
          if( A1[i] ) {
            cy = 0 ;
            break ;
          }
        }
        if( cy ) *ERR = 1 ;  /* zero divide error */
        else {
          /* eventually, normal divide */
          for( i = 0 ; i <= w1 ; i++ ) Y[i] = 0 ;
          for( j = S0 - 1 ; j >= 0 ; j-- ) {
            BIT_LEFT_SHIFT(Y,w1,1) ;
            BIT_LEFT_SHIFT(AR,w2,1) ;
            Y[0] |= GET_BIT(A0,j) ;
            if( bit_ge_bit( Y, S0, A1, S1 ) ) {
              JD_bit_sub_bit( Y, S0, A1, S1 ) ;
              AR[0] |= 1 ;
            }
          }
        }
      }
    }
    if( *ERR ) {
      for( i = 0 ; i <= w0 ; i++ ) {
        AR[i] = BR[i] = 0xffffffff ;
      }
      for(  ; i <= w2 ; i++ ) AR[i] = BR[i] = 0xffffffff ;
      AR[w2] &= m0 ;
      BR[w2] &= m0 ;
    }
  }
  /**END MACRO BIT_ODIV_BIT**/  
}

/* simple virsion for macro output */
void JD_bit_div_bit_ts (
  int *AR, int SR,
  int *A0, int S0, 
  int *A1, int S1, 
  int *mod 
) 
{       
  unsigned m0 ;
  register int i, j, k ;
  register int w0, w1, w2, cy ;
    
  w0 = LASTWORD(S0) ;
  w1 = LASTWORD(S1) ;
  w2 = LASTWORD(SR) ;
  /* Y register allocation*/
  if( Y_SIZE <= w0 || Y == NULL ) {
    if( Y == NULL ) {
      Y = (unsigned *)calloc( (w0>256)?w0:256 , sizeof(unsigned) ) ;
      Y_SIZE = (w0>256)?w0:256 ;
    }
    else {
      free(Y) ;
      Y = (unsigned *)calloc( w0, sizeof(unsigned) ) ;
      Y_SIZE = w0 ;
    }
  }
  m0 = (unsigned)0xffffffff >> (32- (SR%32))%32 ;
  /* eventually, normal divide */
  for( i = 0 ; i <= w1 ; i++ ) Y[i] = 0 ;
  for( j = S0 - 1 ; j >= 0 ; j-- ) {
    BIT_LEFT_SHIFT(Y,w1,1) ;
    BIT_LEFT_SHIFT(AR,w2,1) ;
    Y[0] |= GET_BIT(A0,j) ;
    if( bit_ge_bit( Y, S0, A1, S1 ) ) {
      JD_bit_sub_bit( Y, S0, A1, S1 ) ;
      AR[0] |= 1 ;
    }
  }
  *mod = Y[0] ;
}

/* bit '%' bit   */
void JD_bit_mod_bit( 
  int *AR, int *BR, int SR,
  int *A0, int *B0, int S0, 
  int *A1, int *B1, int S1, 
  int *ERR 
) 
{       
  /**MACRO BIT_OMOD_BIT**/
  {
    unsigned m0 ;
    register int i, j, k ;
    register int w0, w1, w2, cy ;
    
    w0 = LASTWORD(S0) ;
    w1 = LASTWORD(S1) ;
    w2 = LASTWORD(SR) ;
    *ERR = 0 ;
    if( Y_SIZE <= w0 || Y == NULL ) {
      if( Y == NULL ) {
        Y = (unsigned *)calloc( (w0>256)?w0:256 , sizeof(unsigned) ) ;
        Y_SIZE = (w0>256)?w0:256 ;
      }
      else {
        free(Y) ;
        Y = (unsigned *)calloc( w0, sizeof(unsigned) ) ;
        Y_SIZE = w0 ;
      }
    }
    m0 = (unsigned)0xffffffff >> (32- (SR%32))%32 ;
    cy = 0 ;
    for( i = 0 ; i <= w0 ; i++ ) {
      if( B0[i] ) {
        cy = 1 ;
        break ;
      }
    }
    if( !cy ) {
      for( i = 0 ; i <= w1 ; i++ ) {
        if( B1[i] ) {
          cy = 1 ;
          break ;
        } 
      }
    }
    if( cy ) {
      for( i = 0 ; i <= w0 ; i++ ) {
        AR[i] = BR[i] = 0xffffffff ;
      }
      for(  ; i <= w2 ; i++ ) AR[i] = BR[i] = 0xffffffff ;
      AR[w2] &= m0 ;
      BR[w2] &= m0 ;
    }
    else {
      if( w1 == 0 ) {
        /* divider is one word, thus check if power of two */
        /* optimization */
        switch( *A1 ) {
          case 0: *ERR = 1 ; break ; /* zero divide error */
          case 0x00000001: cy = -1 ; break ;
          case 0x00000002: cy = 0x00000001 ; break ;
          case 0x00000004: cy = 0x00000003 ; break ;
          case 0x00000008: cy = 0x00000007 ; break ;
          case 0x00000010: cy = 0x0000000f ; break ;
          case 0x00000020: cy = 0x0000001f ; break ;
          case 0x00000040: cy = 0x0000003f ; break ;
          case 0x00000080: cy = 0x0000007f ; break ;
          case 0x00000100: cy = 0x000000ff ; break ;
          case 0x00000200: cy = 0x000001ff ; break ;
          case 0x00000400: cy = 0x000003ff ; break ;
          case 0x00000800: cy = 0x000007ff ; break ;
          case 0x00001000: cy = 0x00000fff ; break ;
          case 0x00002000: cy = 0x00001fff ; break ;
          case 0x00004000: cy = 0x00003fff ; break ;
          case 0x00008000: cy = 0x00007fff ; break ;
          case 0x00010000: cy = 0x0000ffff ; break ;
          case 0x00020000: cy = 0x0001ffff ; break ;
          case 0x00040000: cy = 0x0003ffff ; break ;
          case 0x00080000: cy = 0x0007ffff ; break ;
          case 0x00100000: cy = 0x000fffff ; break ;
          case 0x00200000: cy = 0x001fffff ; break ;
          case 0x00400000: cy = 0x003fffff ; break ;
          case 0x00800000: cy = 0x007fffff ; break ;
          case 0x01000000: cy = 0x00ffffff ; break ;
          case 0x02000000: cy = 0x01ffffff ; break ;
          case 0x04000000: cy = 0x03ffffff ; break ;
          case 0x08000000: cy = 0x07ffffff ; break ;
          case 0x10000000: cy = 0x0fffffff ; break ;
          case 0x20000000: cy = 0x1fffffff ; break ;
          case 0x40000000: cy = 0x3fffffff ; break ;
          case 0x80000000: cy = 0x7fffffff ; break ;
        } /* end of switch .. */
        if( cy != 0 ) {
          if( cy > 0 ) AR[0] = A0[0] & cy ;
          else AR[0] = 0 ;
          for( i = 1 ; i <= w2 ; i++ ) AR[i] = 0 ;
        }
      } /* end of if( w1 = 0 ) */
      if( !cy && !*ERR ) {
        /* normal bit division */
        cy = 1 ;
        for( i = 0 ; i <= w0 ; i++ ) {
          if( A1[i] ) {
            cy = 0 ;
            break ;
          }
        }
        if( cy ) *ERR = 1 ;  /* zero divide error */
        else {
          /* eventually, normal divide */
          for( i = 0 ; i <= w1 ; i++ ) Y[i] = 0 ;
          for( j = S0 - 1 ; j >= 0 ; j-- ) {
            BIT_LEFT_SHIFT(Y,w1,1) ;
            Y[0] |= GET_BIT(A0,j) ;
            if( bit_ge_bit( Y, S0, A1, S1 ) ) {
              JD_bit_sub_bit( Y, S0, A1, S1 ) ;
            }
          }
          for( i = 0 ; i <= w0 ; i++ ) AR[i] = Y[i] ;
        }
      }
    }
    if( *ERR ) {
      for( i = 0 ; i <= w0 ; i++ ) {
        AR[i] = BR[i] = 0xffffffff ;
      }
      for(  ; i <= w2 ; i++ ) AR[i] = BR[i] = 0xffffffff ;
      AR[w2] &= m0 ;
      BR[w2] &= m0 ;
    }
  }
  /**END MACRO BIT_OMOD_BIT**/  
}

static void bit_mod_bit (
  int *AR, int *BR, int SR,
  int *A0, int *B0, int S0, 
  int *A1, int *B1, int S1, 
  int *ERR 
) 
{       
  /**MACRO BIT_MOD_BIT**/
  {
    unsigned m0 ;
    register int i, j, k ;
    register int w0, w1, w2, cy ;
    
    w0 = LASTWORD(S0) ;
    w1 = LASTWORD(S1) ;
    w2 = LASTWORD(SR) ;
    *ERR = 0 ;
    if( Y_SIZE <= w0 || Y == NULL ) {
      if( Y == NULL ) {
        Y = (unsigned *)calloc( (w0>256)?w0:256 , sizeof(unsigned) ) ;
        Y_SIZE = (w0>256)?w0:256 ;
      }
      else {
        free(Y) ;
        Y = (unsigned *)calloc( w0, sizeof(unsigned) ) ;
        Y_SIZE = w0 ;
      }
    }
    m0 = (unsigned)0xffffffff >> (32- (SR%32))%32 ;
    cy = 0 ;
    for( i = 0 ; i <= w0 ; i++ ) {
      if( B0[i] ) {
        cy = 1 ;
        break ;
      }
    }
    if( !cy ) {
      for( i = 0 ; i <= w1 ; i++ ) {
        if( B1[i] ) {
          cy = 1 ;
          break ;
        } 
      }
    }
    if( cy ) {
      for( i = 0 ; i <= w0 ; i++ ) {
        AR[i] = BR[i] = 0xffffffff ;
      }
      for(  ; i <= w2 ; i++ ) AR[i] = BR[i] = 0xffffffff ;
      AR[w2] &= m0 ;
      BR[w2] &= m0 ;
    }
    else {
      cy = 1 ;
      for( i = 0 ; i <= w0 ; i++ ) {
        if( A1[i] ) {
          cy = 0 ;
          break ;
        }
      }
      if( cy ) *ERR = 1 ;  /* zero divide error */
      else {
        /* eventually, normal divide */
        for( i = 0 ; i <= w1 ; i++ ) Y[i] = 0 ;
        for( j = S0 - 1 ; j >= 0 ; j-- ) {
          BIT_LEFT_SHIFT(Y,w1,1) ;
          BIT_LEFT_SHIFT(AR,w2,1) ;
          Y[0] |= GET_BIT(A0,j) ;
          if( bit_ge_bit( Y, S0, A1, S1 ) ) {
            JD_bit_sub_bit( Y, S0, A1, S1 ) ;
          }
        }
        for( i = 0 ; i <= w0 ; i++ ) AR[i] = Y[i] ;
      }
    }
    if( *ERR ) {
      for( i = 0 ; i <= w0 ; i++ ) {
        AR[i] = BR[i] = 0xffffffff ;
      }
      for(  ; i <= w2 ; i++ ) AR[i] = BR[i] = 0xffffffff ;
      AR[w2] &= m0 ;
      BR[w2] &= m0 ;
    }
  }
  /**END MACRO BIT_MOD_BIT**/  
}

static void bit_div_bit (
  int *AR, int *BR, int SR,
  int *A0, int *B0, int S0, 
  int *A1, int *B1, int S1, 
  int *ERR 
) 
{       
  /**MACRO BIT_DIV_BIT**/
  {
    unsigned m0 ;
    register int i, j, k ;
    register int w0, w1, w2, cy ;
    
    w0 = LASTWORD(S0) ;
    w1 = LASTWORD(S1) ;
    w2 = LASTWORD(SR) ;
    *ERR = 0 ;
    if( Y_SIZE <= w0 || Y == NULL ) {
      if( Y == NULL ) {
        Y = (unsigned *)calloc( (w0>256)?w0:256 , sizeof(unsigned) ) ;
        Y_SIZE = (w0>256)?w0:256 ;
      }
      else {
        free(Y) ;
        Y = (unsigned *)calloc( w0, sizeof(unsigned) ) ;
        Y_SIZE = w0 ;
      }
    }
    m0 = (unsigned)0xffffffff >> (32- (SR%32))%32 ;
    cy = 0 ;
    for( i = 0 ; i <= w0 ; i++ ) {
      if( B0[i] ) {
        cy = 1 ;
        break ;
      }
    }
    if( !cy ) {
      for( i = 0 ; i <= w1 ; i++ ) {
        if( B1[i] ) {
          cy = 1 ;
          break ;
        } 
      }
    }
    if( cy ) {
      for( i = 0 ; i <= w0 ; i++ ) {
        AR[i] = BR[i] = 0xffffffff ;
      }
      for(  ; i <= w2 ; i++ ) AR[i] = BR[i] = 0xffffffff ;
      AR[w2] &= m0 ;
      BR[w2] &= m0 ;
    }
    else {
      cy = 1 ;
      /* check for zero divide */
      for( i = 0 ; i <= w0 ; i++ ) {
        if( A1[i] ) {
          cy = 0 ;
          break ;
        }
      }
      if( cy ) *ERR = 1 ;  /* zero divide error */
      else {
        /* eventually, normal divide */
        for( i = 0 ; i <= w1 ; i++ ) Y[i] = 0 ;
        for( j = S0 - 1 ; j >= 0 ; j-- ) {
          BIT_LEFT_SHIFT(Y,w1,1) ;
          BIT_LEFT_SHIFT(AR,w2,1) ;
          Y[0] |= GET_BIT(A0,j) ;
          if( bit_ge_bit( Y, S0, A1, S1 ) ) {
            JD_bit_sub_bit( Y, S0, A1, S1 ) ;
            AR[0] |= 1 ;
          }
        }
      }
    }
    if( *ERR ) {
      for( i = 0 ; i <= w0 ; i++ ) {
        AR[i] = BR[i] = 0xffffffff ;
      }
      for(  ; i <= w2 ; i++ ) AR[i] = BR[i] = 0xffffffff ;
      AR[w2] &= m0 ;
      BR[w2] &= m0 ;
    }
  }
  /**END MACRO BIT_DIV_BIT**/  
}

/* bit '&' bit   */
void JD_bit_and_bit( int *A0, int *B0, int S0, int *A1, int *B1, int S1 ) { 
  /**MACRO BIT_AND_BIT**/
  {
    register int i, w0, w1, a0, b0, a1, b1 ;
    w0 = LASTWORD(S0) ;
    w1 = LASTWORD(S1) ;
    for( i = w0 ; i >= 0 ; i-- ) {
      if( i > w1 ) {
        A0[i] = 0 ;
        B0[i] = 0 ;
      }
      else {
        a0 = A0[i] ; b0 = B0[i] ; a1 = A1[i] ; b1 = B1[i] ;
        A0[i] = (a0 & a1) | (a0 & b1) | (b0 & a1) | (b0 & b1) ;
        B0[i] =         (a0 & b1) | (b0 & a1) | (b0 & b1) ;
      }
    }
  }
  /**END MACRO BIT_AND_BIT**/
}

/* bit '|' bit   */
void JD_bit_or_bit( int *A0, int *B0, int S0, int *A1, int *B1, int S1 ) { 
  /**MACRO BIT_OR_BIT**/
  {
    register int i, w0, w1, a0, b0, a1, b1, m0 ;
    w0 = LASTWORD(S0) ;
    w1 = LASTWORD(S1) ;
    m0 = (unsigned)0xffffffff >> (32- (S0%32))%32 ;
    for( i = w0 ; i >= 0 ; i-- ) {
      if( i <= w1 ) {
        a0 = A0[i] ; b0 = B0[i] ; a1 = A1[i] ; b1 = B1[i] ;
        A0[i] = a0 | a1 | b0 | b1 ;
        B0[i] = (~a1 & b0) | (~a0 & b1) | (b0 & b1) ;
      }
    }
    A0[w0] &= m0 ;
    B0[w0] &= m0 ;
  }
  /**END MACRO BIT_OR_BIT**/
}

/* bit '^' bit   */
void JD_bit_eor_bit( int *A0, int *B0, int S0, int *A1, int *B1, int S1 ) {
  /**MACRO BIT_EOR_BIT**/
  {
    register int i, w0, w1, a0, b0, a1, b1, m0 ;
    w0 = LASTWORD(S0) ;
    w1 = LASTWORD(S1) ;
    m0 = (unsigned)0xffffffff >> (32- (S0%32))%32 ;
    for( i = w0 ; i >= 0 ; i-- ) {
      a0 = A0[i] ; b0 = B0[i] ;
      if( i <= w1 ) {
         a1 = A1[i] ; b1 = B1[i] ;
      }
      else {
        a1 = 0 ; b1 = 0 ;
      }
      A0[i] = (a0 & ~a1) | (~a0 & a1) | b0 | b1 ;
      B0[i] = b0 | b1 ;
    }
    A0[w0] &= m0 ;
    B0[w0] &= m0 ;
  }
  /**END MACRO BIT_EOR_BIT**/
}

/* bit '&~' bit  */
/* bit '~&' bit  */
void JD_bit_nand_bit( int *A0, int *B0, int S0, int *A1, int *B1, int S1 ) {
  /**MACRO BIT_NAND_BIT**/
  {
    register int i, w0, w1, a0, b0, a1, b1, m0 ;
    w0 = LASTWORD(S0) ;
    w1 = LASTWORD(S1) ;
    m0 = (unsigned)0xffffffff >> (32- (S0%32))%32 ;
    for( i = w0 ; i >= 0 ; i-- ) {
      a0 = A0[i] ; b0 = B0[i] ;
      if( i <= w1 ) {
         a1 = A1[i] ; b1 = B1[i] ;
      }
      else {
        a1 = 0 ; b1 = 0 ;
      }
      A0[i] = ~(a0 & a1) | (a0 & b1) | (b0 & a1) | b0 & b1 ;
      B0[i] =           (a0 & b1) | (b0 & a1) | (b0 & b1) ;
    }
    A0[w0] &= m0 ;
    B0[w0] &= m0 ;
  }
  /**END MACRO BIT_NAND_BIT**/
}

/* bit '|~' bit  */
/* bit '~|' bit  */
void JD_bit_nor_bit( int *A0, int *B0, int S0, int *A1, int *B1, int S1 ) {
  /**MACRO BIT_NOR_BIT**/
  {
    register int i, w0, w1, a0, b0, a1, b1, m0 ;
    w0 = LASTWORD(S0) ;
    w1 = LASTWORD(S1) ;
    m0 = (unsigned)0xffffffff >> (32- (S0%32))%32 ;
    for( i = w0 ; i >= 0 ; i-- ) {
      a0 = A0[i] ; b0 = B0[i] ;
      if( i <= w1 ) {
         a1 = A1[i] ; b1 = B1[i] ;
      }
      else {
        a1 = 0 ; b1 = 0 ;
      }
      A0[i] = ~(a0 | a1) | ( ~a1 & b0 ) | ( ~a0 & b1 ) | (b0 & b1) ;
      B0[i] = ( ~a1 & b0 ) | ( ~a0 & b1 ) | (b0 & b1) ;
    }
    A0[w0] &= m0 ;
    B0[w0] &= m0 ;
  }
  /**END MACRO BIT_NOR_BIT**/
}

/* bit '^~' bit  */
void JD_bit_neor_bit( int *A0, int *B0, int S0, int *A1, int *B1, int S1 ) {
  /**MACRO BIT_NEOR_BIT**/
  {
    register int i, w0, w1, a0, b0, a1, b1, m0 ;
    w0 = LASTWORD(S0) ;
    w1 = LASTWORD(S1) ;
    m0 = (unsigned)0xffffffff >> (32- (S0%32))%32 ;
    for( i = w0 ; i >= 0 ; i-- ) {
      a0 = A0[i] ; b0 = B0[i] ;
      if( i <= w1 ) {
         a1 = A1[i] ; b1 = B1[i] ;
      }
      else {
        a1 = 0 ; b1 = 0 ;
      }
      A0[i] = ~(a0 & ~a1) | ~(~a0 & a1) | b0 | b1 ;
      B0[i] = b0 | b1 ;
    }
    A0[w0] &= m0 ;
    B0[w0] &= m0 ;
  }
  /**END MACRO BIT_NEOR_BIT**/
}


/**********************************************************************/
/*  Unary operations                                                  */
/**********************************************************************/

/* '-' bit  */
void JD_u_minus_bit( int *A0, int *B0, int S0 ) {
  /**MACRO U_MINUS_BIT**/
  {
    register int i, w0, a, cy, t, m0 ;
    w0 = LASTWORD(S0) ;
    m0 = (unsigned)0xffffffff >> (32- (S0%32))%32 ;
    cy = 0 ;
    for( i = 0 ; i <= w0 ; i++ ) {
      if( B0[i] ) {
        cy = 1 ;
        break ;
      }
    }
    if( cy ) {
      for( i = 0 ; i <= w0 ; i++ ) {
        A0[i] = B0[i] = 0xffffffff ;
      }
    }
    else {
      cy = 1 ;
      for( i = 0 ; i <= w0 ; i++ ) {
        a = A0[i] ;
        A0[i] = t = (a+cy) ;
        if( (t < a) || (t < cy) ) cy = 1 ;
        else cy = 0 ;
      }
    }
    A0[w0] &= m0 ;
    B0[w0] &= m0 ;
  }
  /**END MACRO U_MINUS_BIT**/
}

/* '~' bit  */
void JD_u_tilde_bit( int *A0, int *B0, int S0 ) {
  /**MACRO U_TILDE_BIT**/
  {
    register int i, w0, m0 ;
    w0 = LASTWORD(S0) ;
    m0 = (unsigned)0xffffffff >> (32- (S0%32))%32 ;
    for( i = 0 ; i <= w0 ; i++ ) {
      A0[i] = ~A0[i] | B0[i] ;  /* bit becomes x if x or z */
    }
    A0[w0] &= m0 ;
    B0[w0] &= m0 ;
  }
  /**END MACRO U_TILDE_BIT**/
}

/* '!' bit  */
int JD_u_not_bit( int *A0, int *B0, int S0 ) {
  /* register int RET ; */
  /**MACRO U_NOT_BIT**/
  {
    register int i, w0, m0, a ;
    w0 = LASTWORD(S0) ;
    m0 = (unsigned)0xffffffff >> (32- (S0%32))%32 ;
    RET = 1 ;
    for( i = 0 ; i <= w0 ; i++ ) {
      a = B0[i] ;
      if( i = w0 ) a &= m0 ;
      if( a ) {
        RET = 0 ;
        break ;
      }
      a = A0[i] ;
      if( i = w0 ) a &= m0 ;
      if( a ) {
        RET = 0 ;
        break ;
      }
    }
    
  }
  /**END MACRO U_NOT_BIT**/
  return RET ;
}

/* '&' bit  */
/* returns bit 0 as a, bit 1 as b */
int JD_u_and_bit( int *A0, int *B0, int S0 ) {
  /* register int RET ; */
  /**MACRO U_AND_BIT**/
  {
    register int i, j, rm, w0, a, e ;
    w0 = LASTWORD(S0) ;
    rm = (S0%32) ;
    rm = rm?rm:32 ;
    RET = 1 ;
    for( i = 0 ; i <= w0 ; i++ ) {
      if( B0[i] ) {
        RET = 3 ; /* a = 1, b = 1 means X */
        break ;
      }
      else {
        a = A0[i] ;
        e = (i == w0)?rm:32 ;
        for( j = 0 ; j < e ; j++ ){
           RET &= ( (a>>j) & 1) ;
        }
      }
    }
  }
  /**END MACRO U_AND_BIT**/
  return RET ;
}

/* '|' bit  */
/* returns bit 0 as a, bit 1 as b */
int JD_u_or_bit( int *A0, int *B0, int S0 ) {
  /* register int RET ; */
  /**MACRO U_OR_BIT**/
  {
    register int i, rm, w0, a, j, e ;
    w0 = LASTWORD(S0) ;
    rm = (S0%32) ;
    rm = rm?rm:32 ;
    RET = 0 ;
    for( i = 0 ; i <= w0 ; i++ ) {
      if( B0[i] ) {
        RET = 3 ; /* a = 1, b = 1 means X */
        break ;
      }
      else {
        a = A0[i] ;
        e = (i == w0)?rm:32 ;
        for( j = 0 ; j < e ; j++ ) RET |= ( (a>>j) & 1 ) ;
      }
    }
  }
  /**END MACRO U_OR_BIT**/
  return RET ;
}

/* '^' bit  */
/* returns bit 0 as a, bit 1 as b */
int JD_u_eor_bit( int *A0, int *B0, int S0 ) {
  /* register int RET ; */
  /**MACRO U_EOR_BIT**/
  {
    register int i, rm, w0, a, j, e ;
    w0 = LASTWORD(S0) ;
    rm = (S0%32) ;
    rm = rm?rm:32 ;
    RET = 0 ;
    for( i = 0 ; i <= w0 ; i++ ) {
      if( B0[i] ) {
        RET = 3 ; /* a = 1, b = 1 means X */
        break ;
      }
      else {
        a = A0[i] ;
        e = (i == w0)?rm:32 ;
        for( j = 0 ; j < e ; j++ ) RET ^= ( (a>>j) & 1 ) ;
      }
    }
  }
  /**END MACRO U_EOR_BIT**/
  return RET ;
}

/* '~&' bit  */
/* returns bit 0 as a, bit 1 as b */
int JD_u_nand_bit( int *A0, int *B0, int S0 ) {
  /* register int RET ; */
  /**MACRO U_NAND_BIT**/
  {
    register int i, rm, w0, a, j, e ;
    w0 = LASTWORD(S0) ;
    rm = (S0%32) ;
    rm = rm?rm:32 ;
    RET = 1 ;
    for( i = 0 ; i <= w0 ; i++ ) {
      if( B0[i] ) {
        RET = 3 ; /* a = 1, b = 1 means X */
        goto eval_end ;
      }
      else {
        a = A0[i] ;
        e = (i == w0)?rm:32 ;
        for( j = 0 ; j < e ; j++ ) {
          RET &= ( (a>>j) & 1) ;
        }
      }
    }
    RET = ~RET & 1 ;
    eval_end:
  }
  /**END MACRO U_NAND_BIT**/
  return RET ;
}

/* '~|' bit  */
/* returns bit 0 as a, bit 1 as b */
int JD_u_nor_bit( int *A0, int *B0, int S0 ) {
  /* register int RET ; */
  /**MACRO U_NOR_BIT**/
  {
    register int i, rm, w0, a, j, e ;
    w0 = LASTWORD(S0) ;
    rm = (S0%32) ;
    rm = rm?rm:32 ;
    RET = 0 ;
    for( i = 0 ; i <= w0 ; i++ ) {
      if( B0[i] ) {
        RET = 3 ; /* a = 1, b = 1 means X */
        goto eval_end ;
      }
      else {
        a = A0[i] ;
        e = (i == w0)?rm:32 ;
        for( j = 0 ; j < e ; j++ ) RET |= ( (a>>j) & 1 ) ;
      }
    }
    RET = ~RET & 1 ;
    eval_end:
  }
  /**END MACRO U_NOR_BIT**/
  return RET ;
}

/* '~^' bit  */
/* returns bit 0 as a, bit 1 as b */
int JD_u_neor_bit( int *A0, int *B0, int S0 ) {
  /* register int RET ; */
  /**MACRO U_NEOR_BIT**/
  {
    register int i, rm, w0, a, j, e ;
    w0 = LASTWORD(S0) ;
    rm = (S0%32) ;
    rm = rm?rm:32 ;
    RET = 0 ;
    for( i = 0 ; i <= w0 ; i++ ) {
      if( B0[i] ) {
        RET = 3 ; /* a = 1, b = 1 means X */
        goto eval_end ;
      }
      else {
        a = A0[i] ;
        e = (i == w0)?rm:32 ;
        for( j = 0 ; j < e ; j++ ) RET ^= ( (a>>j) & 1 ) ;
      }
    }
    RET = ~RET & 1 ;
    eval_end:
  }
  /**END MACRO U_NEOR_BIT**/
  return RET ;
}

#ifdef BIT_AOP_BIT_MAIN

main() {

  int A0[3] ;
  int B0[3] ;
  int size0 ;
  int A1[3] ;
  int B1[3] ;
  int size1 ;
  
  
  A0[0] = 99 ;
  A1[0] = 100 ;
  B0[0] = 0 ;
  B1[0] = 0 ;
  size0 = 8 ;
  size1 = 8 ;


  printf( 
    "sizeof(int) = %d, sizeof(long) = %0d\n", sizeof(int), sizeof(long) 
  ) ;
  JD_bit_minus_bit( A0, B0, size0, A1, B1, size1 ) ;
  
  printf( " %x %x, %d %d \n", A0[0], B0[0], A0[0], B0[0] ) ;
  
  
  A0[0] = 99 ;
  A0[1] = 0x1ff ;
  A1[0] = 99 ;
  A1[1] = 0xff ;
  B0[0] = 0 ;
  B0[1] = 0 ;
  B1[0] = 0 ;
  B1[1] = 0 ;
  size0 = 41 ;
  size1 = 40 ;

  JD_bit_minus_bit( A0, B0, size0, A1, B1, size1 ) ;
  
  printf( 
    " %x_%x  %x_%x, %d_%d %d_%d \n", 
    A0[1], A0[0], B0[1], B0[0],
    A0[1], A0[0], B0[1], B0[0]
  ) ;

}

#endif
