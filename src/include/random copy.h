/*
  Data structures for random functions & classes
 */
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

typedef struct JD_48_random_s JD_48_random ;
typedef struct JD_mt_random_s JD_mt_random ;
typedef struct JD_mt_drandom_s JD_mt_drandom ;

struct JD_48_random_s {
  unsigned short seed[3] ;
} ;

/* #define N 624 */

struct JD_mt_random_s {
  unsigned long mt[624] ; /* the array for the state vector  */
  int mti ; /* mti==N+1 means mt[N] is not initialized */
} ;

#define DEFAULT_PDF_SIZE 64

struct JD_mt_drandom_s {
 unsigned long mt[624]; /* the array for the state vector  */
 int mti; /* mti==N+1 means mt[N] is not initialized */
 int mean ; /* used for exp random */
 double p ; /* used for geom random */
 /* used for pdf random */
 int size ;
 int n ;
 double sum ;
 double *waits ;
 int *values ;
} ;
