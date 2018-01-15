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
#define INITAL_STACK_SIZE 128
#define INITAL_OBJ_TABLE_SIZE 32

#ifdef DEBUG_MEM_PROBLEM
# define THE_ADDRESS jeda_debug_address
# ifndef JEDA_DEBUG_ADDRESS
    extern void *jeda_debug_address ;
# endif
#endif

/* non optimized data allocacion/deallocation macros */
/* These macro will be optimized to improve performance in the future */

#  define PUSH_TO_STACK(x) \
  { \
    STACK[SP++] = (x) ; \
    CHECK_REALLOC_STACK ; \
  }

#define CHECK_REALLOC_STACK \
  if( THREAD->stack_size == SP ) { \
    THREAD->stack = STACK = \
      JD_realloc_stack( STACK, THREAD->stack_size, INITAL_STACK_SIZE ) ; \
    THREAD->stack_size += INITAL_STACK_SIZE ; \
  }


#define ALLOC_STRING(x) (char *)calloc( x + 1, sizeof(char) ) 
#define DEALLOC_STRING(x) free(x)

#define ALLOC_DIM_SIZE(x) (int *)calloc( x+1, sizeof(int *) ) 
#define DEALLOC_DIM_SIZE(x) free(x)

#define ALLOC_FUNC_ARG (JD_func_arg *)calloc( 1, sizeof(JD_func_arg) ) 
#define DEALLOC_FUNC_ARG(x) free(x)

#define ALLOC_QUEUE (JD_queue *)calloc(1,sizeof(JD_queue)) 
#define DEALLOC_QUEUE(x) free(x)

#define ALLOC_TOP (JD_toplevel *)calloc(1,sizeof(JD_toplevel))
#define DEALLOC_TOP(x) free(x)

#ifdef PREALLOCATE_CODE
JD_data **JD_allocate_stack( int size ) ;
void JD_deallocate_stack( JD_data **old, int size ) ;
JD_data **JD_reallocate_stack( JD_data **old, int size, int old_size ) ;
JD_thread *JD_allocate_thread() ;
void JD_deallocate_thread( JD_thread *thrd ) ;
int *JD_allocate_bit( int size ) ;
void JD_deallocate_bit( int *bit, int size ) ;
JD_hash_entry **JD_allocate_hash_tbl( int size ) ;
void JD_deallocate_hash_tbl( JD_hash_entry **hst ) ;
JD_hash_entry *JD_allocate_hash_entry() ;
void JD_deallocate_hash_entry(JD_hash_entry *hsh) ;
JD_data_unit *JD_allocate_data_unit( int size ) ;
void JD_deallocate_data_unit(JD_data_unit *du, int size) ;
JD_object_table *JD_allocate_obj_table() ;
void JD_deallocate_obj_table(JD_object_table *obj) ;
JD_data **JD_allocate_obj_table_table( int size ) ;
void JD_deallocate_obj_table_table(JD_data **tbl, int size) ;
void JD_reallocate_obj_table_table(JD_object_table *tb) ;
void *JD_allocate_pli_rq() ;
void JD_deallocate_pli_rq( void *rq ) ;



#define ALLOC_STACK(x) JD_allocate_stack(x)
#define DEALLOC_STACK(x,y) JD_deallocate_stack(x,y)
#define REALLOC_STACK(x,y,z) JD_reallocate_stack(x,y,z)


#define ALLOC_THREAD JD_allocate_thread()
#define DEALLOC_THREAD(x) JD_deallocate_thread(x)

/*
#define ALLOC_BIT(x) JD_allocate_bit(x)
#define DEALLOC_BIT(x,y) JD_deallocate_bit(x,y)
*/
#define ALLOC_BIT(x) (int *)calloc(x,sizeof(int))
#define DEALLOC_BIT(x,y) free(x)

#define ALLOC_HASH_TBL(x) JD_allocate_hash_tbl(x)
#define DEALLOC_HASH_TBL(x) JD_deallocate_hash_tbl(x)

#define ALLOC_HASH_ENTRY JD_allocate_hash_entry()
#define DEALLOC_HASH_ENTRY(x) JD_deallocate_hash_entry(x)

#define ALLOC_DATA_UNIT(x) JD_allocate_data_unit(x)
#define DEALLOC_DATA_UNIT(x,y) JD_deallocate_data_unit(x,y)

#define ALLOC_OBJ_TABLE JD_allocate_obj_table()
#define DEALLOC_OBJ_TABLE(x) JD_deallocate_obj_table(x)

#define ALLOC_OBJ_TABLE_TABLE(x) JD_allocate_obj_table_table(x)
#define DEALLOC_OBJ_TABLE_TABLE(x, y) JD_deallocate_obj_table_table(x,y)
#define REALLOC_OBJ_TABLE_TABLE(x) JD_reallocate_obj_table_table(x)
 
#define ALLOC_PLI_RQ JD_allocate_pli_rq() 
#define DEALLOC_PLI_RQ(x) JD_deallocate_pli_rq(x)

#else

#define ALLOC_STACK(x) (JD_data **)calloc(x, sizeof(JD_data *))
#define DEALLOC_STACK(x,y) free(x)
#define REALLOC_STACK(x,y,z) (JD_data **)realloc(x, (y)*sizeof(JD_data *))


#define ALLOC_THREAD (JD_thread *)calloc(1,sizeof(JD_thread))
#define DEALLOC_THREAD(x) free(x)

#define ALLOC_TOP (JD_toplevel *)calloc(1,sizeof(JD_toplevel))
#define DEALLOC_TOP(x) free(x)

#define ALLOC_BIT(x) (int *)calloc(x,sizeof(int))
#define DEALLOC_BIT(x,y) free(x)

#define ALLOC_HASH_TBL(x) (JD_hash_entry **)calloc(x,sizeof(JD_hash_entry *))
#define DEALLOC_HASH_TBL(x) free(x)

#define ALLOC_HASH_ENTRY (JD_hash_entry *)calloc(1,sizeof(JD_hash_entry))
#define DEALLOC_HASH_ENTRY(x) free(x)

#define ALLOC_DATA_UNIT(x) (JD_data_unit *)calloc(x,sizeof(JD_data_unit))
#define DEALLOC_DATA_UNIT(x,y) free(x)

#define ALLOC_OBJ_TABLE (JD_object_table *)calloc(1,sizeof(JD_object_table))
#define DEALLOC_OBJ_TABLE(x) free(x)

#define ALLOC_OBJ_TABLE_TABLE(x) (JD_data **)calloc(x, sizeof(JD_data *))
#define DEALLOC_OBJ_TABLE_TABLE(x, y) free(x)
#define REALLOC_OBJ_TABLE_TABLE(x) \
x->tbl_size += INITAL_OBJ_TABLE_SIZE ; \
x->table = (JD_data **)realloc(x->table, (x->tbl_size)*sizeof(JD_data *)) 
 
#define ALLOC_PLI_RQ (JD_pli_request *)calloc( 1, sizeof(JD_pli_request) ) 
#define DEALLOC_PLI_RQ(x) free(x)

#endif
