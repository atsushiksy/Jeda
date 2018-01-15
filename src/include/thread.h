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
JD_queue *JD_alloc_queue( JD_toplevel *top ) ;
void JD_dealloc_queue( JD_queue *old, JD_toplevel *top ) ;
void JD_enqueue( JD_queue *queue, JD_thread *thrd ) ;
void JD_enqueue_top( JD_queue *queue, JD_thread *thrd ) ;
void JD_dequeue( JD_queue *queue, JD_thread *thrd ) ;
JD_thread *JD_dequeue_top( JD_queue *queue ) ;
void JD_enqueue_timeq( JD_thread **queue, JD_thread *thrd ) ;
void JD_dequeue_timeq( JD_thread **queue, JD_thread *thrd ) ;
JD_data **JD_alloc_stack( int size ) ;
JD_data **JD_realloc_stack( JD_data **old, int old_size, int additional ) ;
JD_thread *JD_alloc_thread( int id ) ;
JD_toplevel *JD_alloc_top() ;
void JD_enqueue_wait( JD_wait_queue *wq, JD_thread *th ) ;
void JD_dequeue_wait( JD_thread *th ) ;
void JD_thread_sleep( JD_thread *this ) ;
void JD_thread_pause( JD_thread *this ) ;
void JD_thread_exit( JD_thread *this ) ;
void JD_thread_return() ;
JD_thread *JD_copy_thread( JD_thread *parent ) ;
void JD_sync_exec() ;
void JD_thread_return() ;


