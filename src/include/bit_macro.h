/********************************************/
/* macro generated from bit_operations.c    */
/********************************************/
#define BIT_LT_BIT \
  { \
    register int i, w0, w1 ; \
    w0 = LASTWORD(S0) ; \
    w1 = LASTWORD(S1) ; \
    for( i = 0 ; i <= w0 ; i++ ) { \
      if( B0[i] ) { \
        RET = -1 ; \
        goto eval_end ; \
      } \
    } \
    for( i = 0 ; i <= w1 ; i++ ) { \
      if( B1[i] ) { \
        RET = -1 ; \
        goto eval_end ; \
      } \
    } \
    for( i = w0+1 ; i <= w1 ; i++ ) { \
      if( A1[i] ) { \
        RET = 1 ; \
        goto eval_end ;  /* A0 < A1 */ \
      } \
    } \
    for( i = w0 ; i >= 0 ; i-- ) { \
      if( i > w1 ) { \
        if( A0[i] ) { \
          RET = 0 ; \
          goto eval_end ;  /* A0 > A1 */ \
        } \
      } \
      else { \
        if( (unsigned)A0[i] > (unsigned)A1[i] ) { \
          RET =  0 ; \
          goto eval_end ;  /* A0 > A1 */ \
        } \
        if( (unsigned)A0[i] < (unsigned)A1[i] ) { \
          RET = 1 ; \
          goto eval_end ;  /* A0 < A1 */ \
        } \
      } \
    } \
    RET = 0 ; /* A0 == A1 */ \
     \
  eval_end: \
  } \

#define BIT_GT_BIT \
  { \
    register int i, w0, w1 ; \
    w0 = LASTWORD(S0) ; \
    w1 = LASTWORD(S1) ; \
    for( i = 0 ; i <= w0 ; i++ ) { \
      if( B0[i] ) { \
        RET = -1 ; \
        goto eval_end ; \
      } \
    } \
    for( i = 0 ; i <= w1 ; i++ ) { \
      if( B1[i] ) { \
        RET = -1 ; \
        goto eval_end ; \
      } \
    } \
    for( i = w0+1 ; i <= w1 ; i++ ) { \
      if( A1[i] ) { \
        RET = 0 ; \
        goto eval_end ;  /* A0 < A1 */ \
      } \
    } \
    for( i = w0 ; i >= 0 ; i-- ) { \
      if( i > w1 ) { \
        if( A0[i] ) { \
          RET = 1 ; \
          goto eval_end ;  /* A0 > A1 */ \
        } \
      } \
      else { \
        if( (unsigned)A0[i] > (unsigned)A1[i] ) { \
          RET = 1 ; \
          goto eval_end ;  /* A0 > A1 */ \
        } \
        if( (unsigned)A0[i] < (unsigned)A1[i] ) { \
          RET = 0 ; \
          goto eval_end ;  /* A0 < A1 */ \
        } \
      } \
    } \
    RET = 0 ; /* A0 == A1 */ \
     \
  eval_end: \
  } \

#define BIT_EQEQ_BIT \
  { \
    register int i, w0, w1 ; \
    w0 = LASTWORD(S0) ; \
    w1 = LASTWORD(S1) ; \
    for( i = 0 ; i <= w0 ; i++ ) { \
      if( B0[i] ) { \
        RET = -1 ; \
        goto eval_end ; \
      } \
    } \
    for( i = 0 ; i <= w1 ; i++ ) { \
      if( B1[i] ) { \
        RET = -1 ; \
        goto eval_end ; \
      } \
    } \
    for( i = w0+1 ; i <= w1 ; i++ ) { \
      if( A1[i] ) { \
        RET = 0 ; \
        goto eval_end ;  /* A0 < A1 */ \
      } \
    } \
    for( i = w0 ; i >= 0 ; i-- ) { \
      if( i > w1 ) { \
        if( A0[i] ) { \
          RET = 0 ; \
          goto eval_end ;  /* A0 > A1 */ \
        } \
      } \
      else { \
        if( A0[i] > A1[i] ) { \
          RET = 0 ; \
          goto eval_end ;  /* A0 > A1 */ \
        } \
        if( A0[i] < A1[i] ) { \
          RET = 0 ; \
          goto eval_end ;  /* A0 < A1 */ \
        } \
      } \
    } \
    RET = 1 ; /* A0 == A1 */ \
     \
  eval_end: \
  } \

#define BIT_LE_BIT \
  { \
    register int i, w0, w1 ; \
    w0 = LASTWORD(S0) ; \
    w1 = LASTWORD(S1) ; \
    for( i = 0 ; i <= w0 ; i++ ) { \
      if( B0[i] ) { \
        RET = -1 ; \
        goto eval_end ; \
      } \
    } \
    for( i = 0 ; i <= w1 ; i++ ) { \
      if( B1[i] ) { \
        RET = -1 ; \
        goto eval_end ; \
      } \
    } \
    for( i = w0+1 ; i <= w1 ; i++ ) { \
      if( A1[i] ) { \
        RET = 1 ; \
        goto eval_end ;  /* A0 < A1 */ \
      } \
    } \
    for( i = w0 ; i >= 0 ; i-- ) { \
      if( i > w1 ) { \
        if( A0[i] ) { \
          RET = 0 ; \
          goto eval_end ;  /* A0 > A1 */ \
        } \
      } \
      else { \
        if( (unsigned)A0[i] > (unsigned)A1[i] ) { \
          RET = 0 ; \
          goto eval_end ;  /* A0 > A1 */ \
        } \
        if( (unsigned)A0[i] < (unsigned)A1[i] ) { \
          RET = 1 ; \
          goto eval_end ;  /* A0 < A1 */ \
        } \
      } \
    } \
    RET = 1 ; /* A0 == A1 */ \
     \
  eval_end: \
  } \

#define BIT_GE_BIT \
  { \
    register int i, w0, w1 ; \
    w0 = LASTWORD(S0) ; \
    w1 = LASTWORD(S1) ; \
    for( i = 0 ; i <= w0 ; i++ ) { \
      if( B0[i] ) { \
        RET = -1 ; \
        goto eval_end ; \
      } \
    } \
    for( i = 0 ; i <= w1 ; i++ ) { \
      if( B1[i] ) { \
        RET = -1 ; \
        goto eval_end ; \
      } \
    } \
    for( i = w0+1 ; i <= w1 ; i++ ) { \
      if( A1[i] ) { \
        RET = 0 ; \
        goto eval_end ;  /* A0 < A1 */ \
      } \
    } \
    for( i = w0 ; i >= 0 ; i-- ) { \
      if( i > w1 ) { \
        if( A0[i] ) { \
          RET = 1 ; \
          goto eval_end ;  /* A0 > A1 */ \
        } \
      } \
      else { \
        if( (unsigned)A0[i] > (unsigned)A1[i] ) { \
          RET = 1 ; \
          goto eval_end ;  /* A0 > A1 */ \
        } \
        if( (unsigned)A0[i] < (unsigned)A1[i] ) { \
          RET = 0 ; \
          goto eval_end ;  /* A0 < A1 */ \
        } \
      } \
    } \
    RET = 1 ; /* A0 == A1 */ \
     \
  eval_end: \
  } \

#define BIT_NE_BIT \
  { \
    register int i, w0, w1 ; \
    w0 = LASTWORD(S0) ; \
    w1 = LASTWORD(S1) ; \
    for( i = 0 ; i <= w0 ; i++ ) { \
      if( B0[i] ) { \
        RET = -1 ; \
        goto eval_end ; \
      } \
    } \
    for( i = 0 ; i <= w1 ; i++ ) { \
      if( B1[i] ) { \
        RET = -1 ; \
        goto eval_end ; \
      } \
    } \
    for( i = w0+1 ; i <= w1 ; i++ ) { \
      if( A1[i] ) { \
        RET = 1 ; \
        goto eval_end ;  /* A0 < A1 */ \
      } \
    } \
    for( i = w0 ; i >= 0 ; i-- ) { \
      if( i > w1 ) { \
        if( A0[i] ) { \
          RET = 1 ; \
          goto eval_end ;  /* A0 > A1 */ \
        } \
      } \
      else { \
        if( A0[i] > A1[i] ) { \
          RET = 1 ; \
          goto eval_end ;  /* A0 > A1 */ \
        } \
        if( A0[i] < A1[i] ) { \
          RET = 1 ; \
          goto eval_end ;  /* A0 < A1 */ \
        } \
      } \
    } \
    RET = 0 ; /* A0 == A1 */ \
     \
  eval_end: \
  } \

#define BIT_EQEQEQ_BIT \
  { \
    register int i, w0, w1 ; \
    w0 = LASTWORD(S0) ; \
    w1 = LASTWORD(S1) ; \
    for( i = w0+1 ; i <= w1 ; i++ ) { \
      if( A1[i] || B1[i] ) { \
        RET = 0 ; \
        goto eval_end ;  /*  */ \
      } \
    } \
    for( i = w0 ; i >= 0 ; i-- ) { \
      if( i > w1 ) { \
        if( A0[i] || B0[i] ) { \
          RET = 0 ; \
          goto eval_end ;  /* A0 > A1 */ \
        } \
      } \
      else { \
        if( A0[i] != A1[i] || B0[i] != B1[i] ) { \
          RET = 0 ; \
          goto eval_end ;  /*  */ \
        } \
      } \
    } \
    RET = 1 ; /* A0 == A1 */ \
     \
  eval_end: \
  } \

#define BIT_NEQEQ_BIT \
  { \
    register int i, w0, w1 ; \
    w0 = LASTWORD(S0) ; \
    w1 = LASTWORD(S1) ; \
    for( i = w0+1 ; i <= w1 ; i++ ) { \
      if( A1[i] || B1[i] ) { \
        RET = 1 ; \
        goto eval_end ;  /*  */ \
      } \
    } \
    for( i = w0 ; i >= 0 ; i-- ) { \
      if( i > w1 ) { \
        if( A0[i] || B0[i] ) { \
          RET = 1 ; \
          goto eval_end ;  /* A0 > A1 */ \
        } \
      } \
      else { \
        if( A0[i] != A1[i] || B0[i] != B1[i] ) { \
          RET = 1 ; \
          goto eval_end ;  /*  */ \
        } \
      } \
    } \
    RET = 0 ; /* A0 == A1 */ \
     \
  eval_end: \
  } \

#define BIT_EQQEQ_BIT \
  { \
    register int i, w0, w1 ; \
    w0 = LASTWORD(S0) ; \
    w1 = LASTWORD(S1) ; \
    for( i = w0+1 ; i <= w1 ; i++ ) { \
      if( A1[i] & ~B1[i] ) { \
        RET = 0 ; \
        goto eval_end ;  /*  */ \
      } \
    } \
    for( i = w0 ; i >= 0 ; i-- ) { \
      if( i > w1 ) { \
        if( A0[i] & ~B0[i] ) { \
          RET = 0 ; \
          goto eval_end ;  /* A0 > A1 */ \
        } \
      } \
      else { \
        /* mask both size */ \
        if( ((A0[i] & ~B0[i] & ~B1[i]) != (A1[i] & ~B1[i] & ~B0[i])) )  \
        { \
          RET = 0 ; \
          goto eval_end ;  /*  */ \
        } \
      } \
    } \
    RET = 1 ; /* A0 == A1 */ \
     \
  eval_end: \
  } \

#define BIT_EQEQQ_BIT \
  { \
    register int i, w0, w1 ; \
    w0 = LASTWORD(S0) ; \
    w1 = LASTWORD(S1)  ; \
    for( i = w0+1 ; i <= w1 ; i++ ) { \
      if( A1[i] & ~B1[i] ) { \
        RET = 0 ; \
        goto eval_end ;  /*  */ \
      } \
    } \
    for( i = w0 ; i >= 0 ; i-- ) { \
      if( i > w1 ) { \
        if( A0[i] || B0[i] ) { \
          RET = 0 ; \
          goto eval_end ;  /* A0 > A1 */ \
        } \
      } \
      else { \
        if( ( (A0[i] & ~B1[i]) != (A1[i] & ~B1[i]) ) || (B0[i] & ~B1[i]) ){ \
          RET = 0 ; \
          goto eval_end ;  /*  */ \
        } \
      } \
    } \
    RET = 1 ; /* A0 == A1 */ \
     \
  eval_end: \
  } \

#define BIT_QEQEQ_BIT \
  { \
    register int i, w0, w1 ; \
    w0 = LASTWORD(S0) ; \
    w1 = LASTWORD(S1) ; \
    for( i = w0+1 ; i <= w1 ; i++ ) { \
      if( A1[i] || B1[i] ) { \
        RET = 0 ; \
        goto eval_end ;  /*  */ \
      } \
    } \
    for( i = w0 ; i >= 0 ; i-- ) { \
      if( i > w1 ) { \
        if( A0[i] & ~B0[i] ) { \
          RET = 0 ; \
          goto eval_end ;  /* A0 > A1 */ \
        } \
      } \
      else { \
        if( ( (A0[i] & ~B0[i]) != (A1[i] & ~B0[i]) ) || (B1[i] & ~B0[i]) ) { \
          RET = 0 ; \
          goto eval_end ;  /*  */ \
        } \
      } \
    } \
    RET = 1 ; /* A0 == A1 */ \
     \
  eval_end: \
  } \

#define BIT_QNE_BIT \
  { \
    register int i, w0, w1 ; \
    w0 = LASTWORD(S0) ; \
    w1 = LASTWORD(S1) ; \
    for( i = w0+1 ; i <= w1 ; i++ ) { \
      if( A1[i] & ~B1[i] ) { \
        RET = 0 ; \
        goto eval_end ;  /*  */ \
      } \
    } \
    for( i = w0 ; i >= 0 ; i-- ) { \
      if( i > w1 ) { \
        if( A0[i] & ~B0[i] ) { \
          RET = 1 ; \
          goto eval_end ;  /* A0 > A1 */ \
        } \
      } \
      else { \
        /* mask both size */ \
        if( ((A0[i] & ~B0[i] & ~B1[i]) != (A1[i] & ~B1[i] & ~B0[i])) )  \
        { \
          RET = 1 ; \
          goto eval_end ;  /*  */ \
        } \
      } \
    } \
    RET = 0 ; /* A0 == A1 */ \
     \
  eval_end: \
  } \

#define BIT_EQCEQ_BIT_BIT \
  { \
    register int i, w0, w1, w2, m ; \
    w0 = LASTWORD(S0) ; \
    w1 = LASTWORD(S1) ; \
    w2 = LASTWORD(SR) ; \
    for( i = w0+1 ; i <= w1 ; i++ ) { \
      m = (i <= w2) ? AR[i]|BR[i] : 0 ; \
      if( A1[i] & m ) { \
        RET = 0 ; \
        goto eval_end ;  /*  */ \
      } \
    } \
    for( i = w0 ; i >= 0 ; i-- ) { \
      m = (i <= w2) ? AR[i]|BR[i] : 0 ; \
      if( i > w1 ) { \
        if( A0[i] & m ) { \
          RET = 0 ; \
          goto eval_end ;  /* A0 > A1 */ \
        } \
      } \
      else { \
        /* mask both size */ \
        if( ((A0[i] & m) != (A1[i] & m)) )  \
        { \
          RET = 0 ; \
          goto eval_end ;  /*  */ \
        } \
      } \
    } \
    RET = 1 ; /* A0 == A1 */ \
     \
  eval_end: \
  } \

#define BIT_MINUS_BIT \
  { \
    register int i, w0, w1; \
    register unsigned int m0, t, a, ap, acy, cy ; \
     \
    w0 = LASTWORD(S0) ; \
    w1 = LASTWORD(S1) ; \
    cy = 0 ; \
    m0 = (unsigned)0xffffffff >> (32- (S0%32))%32 ; \
    for( i = 0 ; i <= w0 ; i++ ) { \
      if( B0[i] || (i <= w1 && B1[i]) ) { \
        cy = 1 ; \
        break ; \
      } \
    } \
    if( cy ) { \
      for( i = 0 ; i <= w0 ; i++ ) { \
        A0[i] = B0[i] = 0xffffffff ; \
      } \
      A0[w0] &= m0 ; \
      B0[w0] &= m0 ; \
    } \
    else { \
      cy = 1 ; \
      acy = 0 ; \
      for( i = 0 ; i <= w0 ; i++ ) { \
        if( i <= w1 ) {   \
          a = ~A1[i] + cy ; \
          if( i == w1 ) { \
            if( a & 0x80000000 ) cy = 1 ; \
            else cy = 0 ; \
          } \
          else { \
            if( a == 0 ) cy = 1 ; \
            else cy = 0 ; \
          } \
        } \
        else a = cy?0xffffffff:0 ; \
        ap = A0[i] ; \
        A0[i] = t = ap + a + acy ; \
        if( (t < a) || (t < ap) || (t < acy) ) acy = 1 ; \
        else acy = 0 ; \
      } \
      A0[w0] &= m0 ; \
    } \
  } \

#define BIT_SUB_BIT \
  { \
    register int i, w0, w1; \
    register unsigned int m0, t, a, ap, acy, cy ; \
     \
    w0 = LASTWORD(S0) ; \
    w1 = LASTWORD(S1) ; \
    m0 = (unsigned)0xffffffff >> (32- (S0%32))%32 ; \
    cy = 1 ; \
    acy = 0 ; \
    for( i = 0 ; i <= w0 ; i++ ) { \
      if( i <= w1 ) {   \
        a = ~A1[i] + cy ; \
        if( i == w1 ) { \
          if( a & 0x80000000 ) cy = 1 ; \
          else cy = 0 ; \
        } \
        else { \
          if( a == 0 ) cy = 1 ; \
          else cy = 0 ; \
        } \
      } \
      else a = cy?0xffffffff:0 ; \
      ap = A0[i] ; \
      A0[i] = t =  ap + a + acy ; \
      if( (t < a) || (t < ap) || (t < acy) ) acy = 1 ; \
      else acy = 0 ; \
    } \
    A0[w0] &= m0 ; \
  } \

#define BIT_PLUS_BIT \
  { \
    register int i, w0, w1; \
    register unsigned int m0, t, a, ap, acy ; \
     \
    w0 = LASTWORD(S0) ; \
    w1 = LASTWORD(S1) ; \
    acy = 0 ; \
    m0 = (unsigned)0xffffffff >> (32- (S0%32))%32 ; \
    for( i = 0 ; i <= w0 ; i++ ) { \
      if( B0[i] || (i <= w1 && B1[i]) ) { \
        acy = 1 ; \
        break ; \
      } \
    } \
    if( acy ) { \
      for( i = 0 ; i <= w0 ; i++ ) { \
        A0[i] = B0[i] = 0xffffffff ; \
      } \
      A0[w0] &= m0 ; \
      B0[w0] &= m0 ; \
    } \
    else { \
      acy = 0 ; \
      for( i = 0 ; i <= w0 ; i++ ) { \
        if( i <= w1 ) {   \
          a = A1[i] ; \
        } \
        else a = 0 ; \
        ap = A0[i] ; \
        A0[i] = t = (ap+a+acy) & ( (i==w0)?m0:0xffffffff ) ; \
        if( (t < a) || (t < ap) || (t < acy) ) acy = 1 ; \
        else acy = 0 ; \
      } \
      A0[w0] &= m0 ; \
    } \
  } \

#define BIT_ADD_BIT \
  { \
    register int i, w0, w1; \
    register unsigned int m0, t, a, ap, acy ; \
     \
    w0 = LASTWORD(S0) ; \
    w1 = LASTWORD(S1) ; \
    acy = 0 ; \
    m0 = (unsigned)0xffffffff >> (32- (S0%32))%32 ; \
    for( i = 0 ; i <= w0 ; i++ ) { \
      if( i <= w1 ) {   \
        a = A1[i] ; \
      } \
      else a = 0 ; \
      ap = A0[i] ; \
      A0[i] = t =  ap + a + acy ; \
      if( (t < a) || (t < ap) || (t < acy) ) acy = 1 ; \
      else acy = 0 ; \
    } \
    A0[w0] &= m0 ; \
  } \

#define BIT_TIMES_BIT \
  { \
    unsigned mr ; \
    int wr ; \
    register int i, j, k, w0, w1, w2 ; \
    register unsigned t0, t1, t2 ; \
 \
    w0 = NWORD(S0) ; \
    w1 = NWORD(S1) ; \
    w2 = NWORD(SR) ; \
    mr = (unsigned)0xffffffff >> (32- (SR%32))%32 ; \
    t0 = 0 ; \
    /* first detecting if X or Z exists. If so result be all x */ \
    for( i = 0 ; i < w0 ; i++ ) { \
      if( B0[i] ) { \
        t0 = 1 ; \
        break ; \
      } \
    } \
    if( !t0 ) { \
      for( i = 0 ; i < w1 ; i++ ) { \
        if( B1[i] ) { \
          t0 = 1 ; \
          break ; \
        } \
      } \
    } \
    if( t0 ) { \
      for( i = 0 ; i < w2 ; i++ ) { \
        AR[i] = BR[i] = 0xffffffff ; \
      } \
      AR[w2-1] &= mr ; \
      BR[w2-1] &= mr ; \
    } \
    else { \
      /* multiplication process starts here.  Calculation is \
       * done as 16-bit multiplication \
       */ \
      for( i = 0 ; i < w2 ; i++ ) { \
        AR[i] = BR[i] = 0 ; \
      } \
      w0 <<= 1 ;  /* now count half word */ \
      w1 <<= 1 ; \
      wr = w2 ; \
      w2 <<= 1 ; \
      for( i = 0 ; i < w0 ; i++ ) { \
        t0 = GETSHORT(A0,i) ; \
        for( j = 0 ; j < w1 ; j++ ) {  \
          t1 = GETSHORT(A1,j) ; \
          if( t0 && t1 ) { \
            t1 = t0 * t1 ; \
            /* setting back the result */ \
            k = i + j ;    /* index to sum the result */ \
            /* get 16-bit word from result area */ \
            t2 = GETSHORT(AR, k) ; \
            /* add low 16-bit of multiply to it */ \
            t2 = t2 + (t1 & 0xffff) ; \
            SETSHORT(AR, (t2 & 0xffff), k ) ; \
            t1 = (t1>>16) + (t2>>16) ; \
            k++ ; \
            /* carry(t1) propagation on AR */ \
            for(  ; t1 && k <= w2 ; k++ ) { \
              t2 = GETSHORT(AR, k) + t1 ; \
              SETSHORT(AR, (t2 & 0xffff), k ) ; \
              t1 = t2 >> 16 ; \
            }  /* end of foo( ; cy && k <= wr ; .. */ \
          }  /* end of if( t0 &&  t1 ) */ \
        }  /* end of for( j = .. */ \
      }  /* end of for( i = .. */ \
      AR[wr-1] &= mr ; \
    } /* end of else */ \
  } \

#define BIT_ODIV_BIT \
  { \
    unsigned m0 ; \
    register int i, j, k ; \
    register int w0, w1, w2, cy ; \
     \
    w0 = LASTWORD(S0) ; \
    w1 = LASTWORD(S1) ; \
    w2 = LASTWORD(SR) ; \
    *ERR = 0 ; \
     \
    /* Y register allocation*/ \
    if( Y_SIZE <= w0 || Y == NULL ) { \
      if( Y == NULL ) { \
        Y = (unsigned *)calloc( (w0>256)?w0:256 , sizeof(unsigned) ) ; \
        Y_SIZE = (w0>256)?w0:256 ; \
      } \
      else { \
        free(Y) ; \
        Y = (unsigned *)calloc( w0, sizeof(unsigned) ) ; \
        Y_SIZE = w0 ; \
      } \
    } \
    m0 = (unsigned)0xffffffff >> (32- (SR%32))%32 ; \
    cy = 0 ; \
    for( i = 0 ; i <= w0 ; i++ ) { \
      if( B0[i] ) { \
        cy = 1 ; \
        break ; \
      } \
    } \
    if( !cy ) { \
      for( i = 0 ; i <= w1 ; i++ ) { \
        if( B1[i] ) { \
          cy = 1 ; \
          break ; \
        }  \
      } \
    } \
    if( cy ) { \
      for( i = 0 ; i <= w0 ; i++ ) { \
        AR[i] = BR[i] = 0xffffffff ; \
      } \
      for(  ; i <= w2 ; i++ ) AR[i] = BR[i] = 0xffffffff ; \
      AR[w2] &= m0 ; \
      BR[w2] &= m0 ; \
    } \
    else { \
      if( w1 == 0 ) { \
        /* divider is one word, thus check if power of two */ \
        /* optimization */ \
        switch( *A1 ) { \
          case 0: *ERR = 1 ; break ; /* zero divide error */ \
          case 0x00000001: cy = -1 ; break ; \
          case 0x00000002: cy = 1 ; break ; \
          case 0x00000004: cy = 2 ; break ; \
          case 0x00000008: cy = 3 ; break ; \
          case 0x00000010: cy = 4 ; break ; \
          case 0x00000020: cy = 5 ; break ; \
          case 0x00000040: cy = 6 ; break ; \
          case 0x00000080: cy = 7 ; break ; \
          case 0x00000100: cy = 8 ; break ; \
          case 0x00000200: cy = 9 ; break ; \
          case 0x00000400: cy = 10 ; break ; \
          case 0x00000800: cy = 11 ; break ; \
          case 0x00001000: cy = 12 ; break ; \
          case 0x00002000: cy = 13 ; break ; \
          case 0x00004000: cy = 14 ; break ; \
          case 0x00008000: cy = 15 ; break ; \
          case 0x00010000: cy = 16 ; break ; \
          case 0x00020000: cy = 17 ; break ; \
          case 0x00040000: cy = 18 ; break ; \
          case 0x00080000: cy = 19 ; break ; \
          case 0x00100000: cy = 20 ; break ; \
          case 0x00200000: cy = 21 ; break ; \
          case 0x00400000: cy = 22 ; break ; \
          case 0x00800000: cy = 23 ; break ; \
          case 0x01000000: cy = 24 ; break ; \
          case 0x02000000: cy = 25 ; break ; \
          case 0x04000000: cy = 26 ; break ; \
          case 0x08000000: cy = 27 ; break ; \
          case 0x10000000: cy = 28 ; break ; \
          case 0x20000000: cy = 29 ; break ; \
          case 0x40000000: cy = 30 ; break ; \
          case 0x80000000: cy = 31 ; break ; \
        } /* end of switch .. */ \
        if( cy != 0 ) { \
          for( i = 0 ; i <= w0 ; i++ ) AR[i] = A0[i] ; \
          for( ; i < w2 ; i++ ) AR[i] = 0 ; \
          if( cy > 0 ) { \
            BIT_RIGHT_SHIFT( AR, w2, cy ) ; \
          } \
        } \
      } /* end of if( w1 = 0 ) */ \
      if( !cy && !*ERR ) { \
        /* normal bit division */ \
        cy = 1 ; \
        /* check for zero-divide */ \
        for( i = 0 ; i <= w0 ; i++ ) { \
          if( A1[i] ) { \
            cy = 0 ; \
            break ; \
          } \
        } \
        if( cy ) *ERR = 1 ;  /* zero divide error */ \
        else { \
          /* eventually, normal divide */ \
          for( i = 0 ; i <= w1 ; i++ ) Y[i] = 0 ; \
          for( j = S0 - 1 ; j >= 0 ; j-- ) { \
            BIT_LEFT_SHIFT(Y,w1,1) ; \
            BIT_LEFT_SHIFT(AR,w2,1) ; \
            Y[0] |= GET_BIT(A0,j) ; \
            if( bit_ge_bit( Y, S0, A1, S1 ) ) { \
              JD_bit_sub_bit( Y, S0, A1, S1 ) ; \
              AR[0] |= 1 ; \
            } \
          } \
        } \
      } \
    } \
    if( *ERR ) { \
      for( i = 0 ; i <= w0 ; i++ ) { \
        AR[i] = BR[i] = 0xffffffff ; \
      } \
      for(  ; i <= w2 ; i++ ) AR[i] = BR[i] = 0xffffffff ; \
      AR[w2] &= m0 ; \
      BR[w2] &= m0 ; \
    } \
  } \

#define BIT_OMOD_BIT \
  { \
    unsigned m0 ; \
    register int i, j, k ; \
    register int w0, w1, w2, cy ; \
     \
    w0 = LASTWORD(S0) ; \
    w1 = LASTWORD(S1) ; \
    w2 = LASTWORD(SR) ; \
    *ERR = 0 ; \
    if( Y_SIZE <= w0 || Y == NULL ) { \
      if( Y == NULL ) { \
        Y = (unsigned *)calloc( (w0>256)?w0:256 , sizeof(unsigned) ) ; \
        Y_SIZE = (w0>256)?w0:256 ; \
      } \
      else { \
        free(Y) ; \
        Y = (unsigned *)calloc( w0, sizeof(unsigned) ) ; \
        Y_SIZE = w0 ; \
      } \
    } \
    m0 = (unsigned)0xffffffff >> (32- (SR%32))%32 ; \
    cy = 0 ; \
    for( i = 0 ; i <= w0 ; i++ ) { \
      if( B0[i] ) { \
        cy = 1 ; \
        break ; \
      } \
    } \
    if( !cy ) { \
      for( i = 0 ; i <= w1 ; i++ ) { \
        if( B1[i] ) { \
          cy = 1 ; \
          break ; \
        }  \
      } \
    } \
    if( cy ) { \
      for( i = 0 ; i <= w0 ; i++ ) { \
        AR[i] = BR[i] = 0xffffffff ; \
      } \
      for(  ; i <= w2 ; i++ ) AR[i] = BR[i] = 0xffffffff ; \
      AR[w2] &= m0 ; \
      BR[w2] &= m0 ; \
    } \
    else { \
      if( w1 == 0 ) { \
        /* divider is one word, thus check if power of two */ \
        /* optimization */ \
        switch( *A1 ) { \
          case 0: *ERR = 1 ; break ; /* zero divide error */ \
          case 0x00000001: cy = -1 ; break ; \
          case 0x00000002: cy = 0x00000001 ; break ; \
          case 0x00000004: cy = 0x00000003 ; break ; \
          case 0x00000008: cy = 0x00000007 ; break ; \
          case 0x00000010: cy = 0x0000000f ; break ; \
          case 0x00000020: cy = 0x0000001f ; break ; \
          case 0x00000040: cy = 0x0000003f ; break ; \
          case 0x00000080: cy = 0x0000007f ; break ; \
          case 0x00000100: cy = 0x000000ff ; break ; \
          case 0x00000200: cy = 0x000001ff ; break ; \
          case 0x00000400: cy = 0x000003ff ; break ; \
          case 0x00000800: cy = 0x000007ff ; break ; \
          case 0x00001000: cy = 0x00000fff ; break ; \
          case 0x00002000: cy = 0x00001fff ; break ; \
          case 0x00004000: cy = 0x00003fff ; break ; \
          case 0x00008000: cy = 0x00007fff ; break ; \
          case 0x00010000: cy = 0x0000ffff ; break ; \
          case 0x00020000: cy = 0x0001ffff ; break ; \
          case 0x00040000: cy = 0x0003ffff ; break ; \
          case 0x00080000: cy = 0x0007ffff ; break ; \
          case 0x00100000: cy = 0x000fffff ; break ; \
          case 0x00200000: cy = 0x001fffff ; break ; \
          case 0x00400000: cy = 0x003fffff ; break ; \
          case 0x00800000: cy = 0x007fffff ; break ; \
          case 0x01000000: cy = 0x00ffffff ; break ; \
          case 0x02000000: cy = 0x01ffffff ; break ; \
          case 0x04000000: cy = 0x03ffffff ; break ; \
          case 0x08000000: cy = 0x07ffffff ; break ; \
          case 0x10000000: cy = 0x0fffffff ; break ; \
          case 0x20000000: cy = 0x1fffffff ; break ; \
          case 0x40000000: cy = 0x3fffffff ; break ; \
          case 0x80000000: cy = 0x7fffffff ; break ; \
        } /* end of switch .. */ \
        if( cy != 0 ) { \
          if( cy > 0 ) AR[0] = A0[0] & cy ; \
          else AR[0] = 0 ; \
          for( i = 1 ; i <= w2 ; i++ ) AR[i] = 0 ; \
        } \
      } /* end of if( w1 = 0 ) */ \
      if( !cy && !*ERR ) { \
        /* normal bit division */ \
        cy = 1 ; \
        for( i = 0 ; i <= w0 ; i++ ) { \
          if( A1[i] ) { \
            cy = 0 ; \
            break ; \
          } \
        } \
        if( cy ) *ERR = 1 ;  /* zero divide error */ \
        else { \
          /* eventually, normal divide */ \
          for( i = 0 ; i <= w1 ; i++ ) Y[i] = 0 ; \
          for( j = S0 - 1 ; j >= 0 ; j-- ) { \
            BIT_LEFT_SHIFT(Y,w1,1) ; \
            Y[0] |= GET_BIT(A0,j) ; \
            if( bit_ge_bit( Y, S0, A1, S1 ) ) { \
              JD_bit_sub_bit( Y, S0, A1, S1 ) ; \
            } \
          } \
          for( i = 0 ; i <= w0 ; i++ ) AR[i] = Y[i] ; \
        } \
      } \
    } \
    if( *ERR ) { \
      for( i = 0 ; i <= w0 ; i++ ) { \
        AR[i] = BR[i] = 0xffffffff ; \
      } \
      for(  ; i <= w2 ; i++ ) AR[i] = BR[i] = 0xffffffff ; \
      AR[w2] &= m0 ; \
      BR[w2] &= m0 ; \
    } \
  } \

#define BIT_MOD_BIT \
  { \
    unsigned m0 ; \
    register int i, j, k ; \
    register int w0, w1, w2, cy ; \
     \
    w0 = LASTWORD(S0) ; \
    w1 = LASTWORD(S1) ; \
    w2 = LASTWORD(SR) ; \
    *ERR = 0 ; \
    if( Y_SIZE <= w0 || Y == NULL ) { \
      if( Y == NULL ) { \
        Y = (unsigned *)calloc( (w0>256)?w0:256 , sizeof(unsigned) ) ; \
        Y_SIZE = (w0>256)?w0:256 ; \
      } \
      else { \
        free(Y) ; \
        Y = (unsigned *)calloc( w0, sizeof(unsigned) ) ; \
        Y_SIZE = w0 ; \
      } \
    } \
    m0 = (unsigned)0xffffffff >> (32- (SR%32))%32 ; \
    cy = 0 ; \
    for( i = 0 ; i <= w0 ; i++ ) { \
      if( B0[i] ) { \
        cy = 1 ; \
        break ; \
      } \
    } \
    if( !cy ) { \
      for( i = 0 ; i <= w1 ; i++ ) { \
        if( B1[i] ) { \
          cy = 1 ; \
          break ; \
        }  \
      } \
    } \
    if( cy ) { \
      for( i = 0 ; i <= w0 ; i++ ) { \
        AR[i] = BR[i] = 0xffffffff ; \
      } \
      for(  ; i <= w2 ; i++ ) AR[i] = BR[i] = 0xffffffff ; \
      AR[w2] &= m0 ; \
      BR[w2] &= m0 ; \
    } \
    else { \
      cy = 1 ; \
      for( i = 0 ; i <= w0 ; i++ ) { \
        if( A1[i] ) { \
          cy = 0 ; \
          break ; \
        } \
      } \
      if( cy ) *ERR = 1 ;  /* zero divide error */ \
      else { \
        /* eventually, normal divide */ \
        for( i = 0 ; i <= w1 ; i++ ) Y[i] = 0 ; \
        for( j = S0 - 1 ; j >= 0 ; j-- ) { \
          BIT_LEFT_SHIFT(Y,w1,1) ; \
          BIT_LEFT_SHIFT(AR,w2,1) ; \
          Y[0] |= GET_BIT(A0,j) ; \
          if( bit_ge_bit( Y, S0, A1, S1 ) ) { \
            JD_bit_sub_bit( Y, S0, A1, S1 ) ; \
          } \
        } \
        for( i = 0 ; i <= w0 ; i++ ) AR[i] = Y[i] ; \
      } \
    } \
    if( *ERR ) { \
      for( i = 0 ; i <= w0 ; i++ ) { \
        AR[i] = BR[i] = 0xffffffff ; \
      } \
      for(  ; i <= w2 ; i++ ) AR[i] = BR[i] = 0xffffffff ; \
      AR[w2] &= m0 ; \
      BR[w2] &= m0 ; \
    } \
  } \

#define BIT_DIV_BIT \
  { \
    unsigned m0 ; \
    register int i, j, k ; \
    register int w0, w1, w2, cy ; \
     \
    w0 = LASTWORD(S0) ; \
    w1 = LASTWORD(S1) ; \
    w2 = LASTWORD(SR) ; \
    *ERR = 0 ; \
    if( Y_SIZE <= w0 || Y == NULL ) { \
      if( Y == NULL ) { \
        Y = (unsigned *)calloc( (w0>256)?w0:256 , sizeof(unsigned) ) ; \
        Y_SIZE = (w0>256)?w0:256 ; \
      } \
      else { \
        free(Y) ; \
        Y = (unsigned *)calloc( w0, sizeof(unsigned) ) ; \
        Y_SIZE = w0 ; \
      } \
    } \
    m0 = (unsigned)0xffffffff >> (32- (SR%32))%32 ; \
    cy = 0 ; \
    for( i = 0 ; i <= w0 ; i++ ) { \
      if( B0[i] ) { \
        cy = 1 ; \
        break ; \
      } \
    } \
    if( !cy ) { \
      for( i = 0 ; i <= w1 ; i++ ) { \
        if( B1[i] ) { \
          cy = 1 ; \
          break ; \
        }  \
      } \
    } \
    if( cy ) { \
      for( i = 0 ; i <= w0 ; i++ ) { \
        AR[i] = BR[i] = 0xffffffff ; \
      } \
      for(  ; i <= w2 ; i++ ) AR[i] = BR[i] = 0xffffffff ; \
      AR[w2] &= m0 ; \
      BR[w2] &= m0 ; \
    } \
    else { \
      cy = 1 ; \
      /* check for zero divide */ \
      for( i = 0 ; i <= w0 ; i++ ) { \
        if( A1[i] ) { \
          cy = 0 ; \
          break ; \
        } \
      } \
      if( cy ) *ERR = 1 ;  /* zero divide error */ \
      else { \
        /* eventually, normal divide */ \
        for( i = 0 ; i <= w1 ; i++ ) Y[i] = 0 ; \
        for( j = S0 - 1 ; j >= 0 ; j-- ) { \
          BIT_LEFT_SHIFT(Y,w1,1) ; \
          BIT_LEFT_SHIFT(AR,w2,1) ; \
          Y[0] |= GET_BIT(A0,j) ; \
          if( bit_ge_bit( Y, S0, A1, S1 ) ) { \
            JD_bit_sub_bit( Y, S0, A1, S1 ) ; \
            AR[0] |= 1 ; \
          } \
        } \
      } \
    } \
    if( *ERR ) { \
      for( i = 0 ; i <= w0 ; i++ ) { \
        AR[i] = BR[i] = 0xffffffff ; \
      } \
      for(  ; i <= w2 ; i++ ) AR[i] = BR[i] = 0xffffffff ; \
      AR[w2] &= m0 ; \
      BR[w2] &= m0 ; \
    } \
  } \

#define BIT_AND_BIT \
  { \
    register int i, w0, w1, a0, b0, a1, b1 ; \
    w0 = LASTWORD(S0) ; \
    w1 = LASTWORD(S1) ; \
    for( i = w0 ; i >= 0 ; i-- ) { \
      if( i > w1 ) { \
        A0[i] = 0 ; \
        B0[i] = 0 ; \
      } \
      else { \
        a0 = A0[i] ; b0 = B0[i] ; a1 = A1[i] ; b1 = B1[i] ; \
        A0[i] = (a0 & a1) | (a0 & b1) | (b0 & a1) | (b0 & b1) ; \
        B0[i] =         (a0 & b1) | (b0 & a1) | (b0 & b1) ; \
      } \
    } \
  } \

#define BIT_OR_BIT \
  { \
    register int i, w0, w1, a0, b0, a1, b1, m0 ; \
    w0 = LASTWORD(S0) ; \
    w1 = LASTWORD(S1) ; \
    m0 = (unsigned)0xffffffff >> (32- (S0%32))%32 ; \
    for( i = w0 ; i >= 0 ; i-- ) { \
      if( i <= w1 ) { \
        a0 = A0[i] ; b0 = B0[i] ; a1 = A1[i] ; b1 = B1[i] ; \
        A0[i] = a0 | a1 | b0 | b1 ; \
        B0[i] = (~a1 & b0) | (~a0 & b1) | (b0 & b1) ; \
      } \
    } \
    A0[w0] &= m0 ; \
    B0[w0] &= m0 ; \
  } \

#define BIT_EOR_BIT \
  { \
    register int i, w0, w1, a0, b0, a1, b1, m0 ; \
    w0 = LASTWORD(S0) ; \
    w1 = LASTWORD(S1) ; \
    m0 = (unsigned)0xffffffff >> (32- (S0%32))%32 ; \
    for( i = w0 ; i >= 0 ; i-- ) { \
      a0 = A0[i] ; b0 = B0[i] ; \
      if( i <= w1 ) { \
         a1 = A1[i] ; b1 = B1[i] ; \
      } \
      else { \
        a1 = 0 ; b1 = 0 ; \
      } \
      A0[i] = (a0 & ~a1) | (~a0 & a1) | b0 | b1 ; \
      B0[i] = b0 | b1 ; \
    } \
    A0[w0] &= m0 ; \
    B0[w0] &= m0 ; \
  } \

#define BIT_NAND_BIT \
  { \
    register int i, w0, w1, a0, b0, a1, b1, m0 ; \
    w0 = LASTWORD(S0) ; \
    w1 = LASTWORD(S1) ; \
    m0 = (unsigned)0xffffffff >> (32- (S0%32))%32 ; \
    for( i = w0 ; i >= 0 ; i-- ) { \
      a0 = A0[i] ; b0 = B0[i] ; \
      if( i <= w1 ) { \
         a1 = A1[i] ; b1 = B1[i] ; \
      } \
      else { \
        a1 = 0 ; b1 = 0 ; \
      } \
      A0[i] = ~(a0 & a1) | (a0 & b1) | (b0 & a1) | b0 & b1 ; \
      B0[i] =           (a0 & b1) | (b0 & a1) | (b0 & b1) ; \
    } \
    A0[w0] &= m0 ; \
    B0[w0] &= m0 ; \
  } \

#define BIT_NOR_BIT \
  { \
    register int i, w0, w1, a0, b0, a1, b1, m0 ; \
    w0 = LASTWORD(S0) ; \
    w1 = LASTWORD(S1) ; \
    m0 = (unsigned)0xffffffff >> (32- (S0%32))%32 ; \
    for( i = w0 ; i >= 0 ; i-- ) { \
      a0 = A0[i] ; b0 = B0[i] ; \
      if( i <= w1 ) { \
         a1 = A1[i] ; b1 = B1[i] ; \
      } \
      else { \
        a1 = 0 ; b1 = 0 ; \
      } \
      A0[i] = ~(a0 | a1) | ( ~a1 & b0 ) | ( ~a0 & b1 ) | (b0 & b1) ; \
      B0[i] = ( ~a1 & b0 ) | ( ~a0 & b1 ) | (b0 & b1) ; \
    } \
    A0[w0] &= m0 ; \
    B0[w0] &= m0 ; \
  } \

#define BIT_NEOR_BIT \
  { \
    register int i, w0, w1, a0, b0, a1, b1, m0 ; \
    w0 = LASTWORD(S0) ; \
    w1 = LASTWORD(S1) ; \
    m0 = (unsigned)0xffffffff >> (32- (S0%32))%32 ; \
    for( i = w0 ; i >= 0 ; i-- ) { \
      a0 = A0[i] ; b0 = B0[i] ; \
      if( i <= w1 ) { \
         a1 = A1[i] ; b1 = B1[i] ; \
      } \
      else { \
        a1 = 0 ; b1 = 0 ; \
      } \
      A0[i] = ~(a0 & ~a1) | ~(~a0 & a1) | b0 | b1 ; \
      B0[i] = b0 | b1 ; \
    } \
    A0[w0] &= m0 ; \
    B0[w0] &= m0 ; \
  } \

#define U_MINUS_BIT \
  { \
    register int i, w0, a, cy, t, m0 ; \
    w0 = LASTWORD(S0) ; \
    m0 = (unsigned)0xffffffff >> (32- (S0%32))%32 ; \
    cy = 0 ; \
    for( i = 0 ; i <= w0 ; i++ ) { \
      if( B0[i] ) { \
        cy = 1 ; \
        break ; \
      } \
    } \
    if( cy ) { \
      for( i = 0 ; i <= w0 ; i++ ) { \
        A0[i] = B0[i] = 0xffffffff ; \
      } \
    } \
    else { \
      cy = 1 ; \
      for( i = 0 ; i <= w0 ; i++ ) { \
        a = A0[i] ; \
        A0[i] = t = (a+cy) ; \
        if( (t < a) || (t < cy) ) cy = 1 ; \
        else cy = 0 ; \
      } \
    } \
    A0[w0] &= m0 ; \
    B0[w0] &= m0 ; \
  } \

#define U_TILDE_BIT \
  { \
    register int i, w0, m0 ; \
    w0 = LASTWORD(S0) ; \
    m0 = (unsigned)0xffffffff >> (32- (S0%32))%32 ; \
    for( i = 0 ; i <= w0 ; i++ ) { \
      A0[i] = ~A0[i] | B0[i] ;  /* bit becomes x if x or z */ \
    } \
    A0[w0] &= m0 ; \
    B0[w0] &= m0 ; \
  } \

#define U_NOT_BIT \
  { \
    register int i, w0, m0, a ; \
    w0 = LASTWORD(S0) ; \
    m0 = (unsigned)0xffffffff >> (32- (S0%32))%32 ; \
    RET = 1 ; \
    for( i = 0 ; i <= w0 ; i++ ) { \
      a = B0[i] ; \
      if( i = w0 ) a &= m0 ; \
      if( a ) { \
        RET = 0 ; \
        break ; \
      } \
      a = A0[i] ; \
      if( i = w0 ) a &= m0 ; \
      if( a ) { \
        RET = 0 ; \
        break ; \
      } \
    } \
     \
  } \

#define U_AND_BIT \
  { \
    register int i, j, rm, w0, a, e ; \
    w0 = LASTWORD(S0) ; \
    rm = (S0%32) ; \
    rm = rm?rm:32 ; \
    RET = 1 ; \
    for( i = 0 ; i <= w0 ; i++ ) { \
      if( B0[i] ) { \
        RET = 3 ; /* a = 1, b = 1 means X */ \
        break ; \
      } \
      else { \
        a = A0[i] ; \
        e = (i == w0)?rm:32 ; \
        for( j = 0 ; j < e ; j++ ){ \
           RET &= ( (a>>j) & 1) ; \
        } \
      } \
    } \
  } \

#define U_OR_BIT \
  { \
    register int i, rm, w0, a, j, e ; \
    w0 = LASTWORD(S0) ; \
    rm = (S0%32) ; \
    rm = rm?rm:32 ; \
    RET = 0 ; \
    for( i = 0 ; i <= w0 ; i++ ) { \
      if( B0[i] ) { \
        RET = 3 ; /* a = 1, b = 1 means X */ \
        break ; \
      } \
      else { \
        a = A0[i] ; \
        e = (i == w0)?rm:32 ; \
        for( j = 0 ; j < e ; j++ ) RET |= ( (a>>j) & 1 ) ; \
      } \
    } \
  } \

#define U_EOR_BIT \
  { \
    register int i, rm, w0, a, j, e ; \
    w0 = LASTWORD(S0) ; \
    rm = (S0%32) ; \
    rm = rm?rm:32 ; \
    RET = 0 ; \
    for( i = 0 ; i <= w0 ; i++ ) { \
      if( B0[i] ) { \
        RET = 3 ; /* a = 1, b = 1 means X */ \
        break ; \
      } \
      else { \
        a = A0[i] ; \
        e = (i == w0)?rm:32 ; \
        for( j = 0 ; j < e ; j++ ) RET ^= ( (a>>j) & 1 ) ; \
      } \
    } \
  } \

#define U_NAND_BIT \
  { \
    register int i, rm, w0, a, j, e ; \
    w0 = LASTWORD(S0) ; \
    rm = (S0%32) ; \
    rm = rm?rm:32 ; \
    RET = 1 ; \
    for( i = 0 ; i <= w0 ; i++ ) { \
      if( B0[i] ) { \
        RET = 3 ; /* a = 1, b = 1 means X */ \
        goto eval_end ; \
      } \
      else { \
        a = A0[i] ; \
        e = (i == w0)?rm:32 ; \
        for( j = 0 ; j < e ; j++ ) { \
          RET &= ( (a>>j) & 1) ; \
        } \
      } \
    } \
    RET = ~RET & 1 ; \
    eval_end: \
  } \

#define U_NOR_BIT \
  { \
    register int i, rm, w0, a, j, e ; \
    w0 = LASTWORD(S0) ; \
    rm = (S0%32) ; \
    rm = rm?rm:32 ; \
    RET = 0 ; \
    for( i = 0 ; i <= w0 ; i++ ) { \
      if( B0[i] ) { \
        RET = 3 ; /* a = 1, b = 1 means X */ \
        goto eval_end ; \
      } \
      else { \
        a = A0[i] ; \
        e = (i == w0)?rm:32 ; \
        for( j = 0 ; j < e ; j++ ) RET |= ( (a>>j) & 1 ) ; \
      } \
    } \
    RET = ~RET & 1 ; \
    eval_end: \
  } \

#define U_NEOR_BIT \
  { \
    register int i, rm, w0, a, j, e ; \
    w0 = LASTWORD(S0) ; \
    rm = (S0%32) ; \
    rm = rm?rm:32 ; \
    RET = 0 ; \
    for( i = 0 ; i <= w0 ; i++ ) { \
      if( B0[i] ) { \
        RET = 3 ; /* a = 1, b = 1 means X */ \
        goto eval_end ; \
      } \
      else { \
        a = A0[i] ; \
        e = (i == w0)?rm:32 ; \
        for( j = 0 ; j < e ; j++ ) RET ^= ( (a>>j) & 1 ) ; \
      } \
    } \
    RET = ~RET & 1 ; \
    eval_end: \
  } \

