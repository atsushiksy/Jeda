/* 
   
   Copyright (C) 2002 Juniper Networks Inc.

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

/* structure for the lwp
struct JD_lwp_s {
  void (*ret)() ;      0 
  int  *ret_fp ;       4 
  int  *ret_sp ;       8 
  void (*func)() ;     12 
  int  *arg ;          16 
  int  *fp ;           20 
  int  *sp ;           24 
  void (*reta)() ;     28
  void (*retb)() ;     32
  int  *save_stack ;   36 
  int  stack_size ;    40 
  int  *stack_base ;   44
} ;
*/

#define  LWP_RET           0
#define  LWP_RET_FP        4
#define  LWP_RET_SP        8
#define  LWP_FUNC         12
#define  LWP_ARG          16
#define  LWP_FP           20
#define  LWP_SP           24
#define  LWP_RETA         28
#define  LWP_RETC         32
#define  LWP_STACK        36
#define  LWP_SIZE         40
#define  LWP_BASE         44

#define  LWP_STACK_OFFSET  2048

#define ST_FLUSH_WINDOWS 3

.section	".text"
	.align 4
	.global JD_call_lwp
	.type	 JD_call_lwp,#function
	.proc	020
JD_call_lwp:
	!#PROLOGUE# 0
	save %sp,-120,%sp
	!#PROLOGUE# 1
        /* arg = lwp */
	st %i0,[%fp+68]
        mov %i0,%g1   
	st %fp,[%g1+LWP_RET_FP]
	st %sp,[%g1+LWP_RET_SP]
	st %i7,[%g1+LWP_RETA]
	/*
	 * Do a "flush register windows trap".  The trap handler in the
	 * kernel writes all the register windows to their stack slots, and
	 * marks them all as invalid (needing to be sucked up from the
	 * stack when used).  This ensures that all information needed to
	 * unwind to these callers is in memory, not in the register
	 * windows.
	 */
	ta	ST_FLUSH_WINDOWS
/*
	sethi %hi(JD_call_lwp_return),%o2
	or %o2,%lo(JD_call_lwp_return),%o1
	st %o1,[%g1+LWP_RET]
*/
	ld [%g1+LWP_STACK],%o1
	cmp %o1,0
	bne .LL2
	nop
	ld [%g1+LWP_BASE],%o1
	ld [%o1],%o0
	cmp %o0,0
	bne .LL0
	nop
	/* first time to call lwp, so add offset to stack to preserve
	   some offset in the case of unleveled call 
	*/
	sub %sp,LWP_STACK_OFFSET,%o0
	mov %o0,%sp
	st  %o0,[%o1]
	b   .LL1
	nop
.LL0:
	cmp %o0,%sp
	bl  .LL01
	nop
	mov %sp, %o1
	call JD_stack_base_error,0
	nop
	ret
	restore
.LL01:
	mov %o0,%sp
.LL1:	
	ld [%g1+LWP_FUNC],%o2
	ld [%g1+LWP_ARG],%o0
	call %o2,0
	nop
	ld [%fp+68],%g1
	st  %g0,[%g1+LWP_STACK]
	ret
	restore
.LL2:
        /* construct the stack frame as it used to be */
	ld [%g1+LWP_STACK],%o0
	ld [%g1+LWP_SIZE],%o1
	ld [%g1+LWP_BASE],%o3
	ld [%o3],%o2
.LL4:
	cmp %o1,0
	be .LL5
	nop
	add %o1,-4,%o3
	mov %o3,%o1
	add %o2,-4,%o3
	mov  %o3,%o2
	ld [%o0],%o3
	st %o3,[%o2]
	add %o0,4,%o3
	mov %o3,%o0
	b .LL4
	nop
.LL5:
	ld [%fp+68],%g1
	ld [%g1+LWP_STACK],%o0
	call free,0
	nop
	/*
	mov %g1,%o0
	call JD_print_lwp,0
	nop
	*/
	ld [%fp+68],%g1
	st  %g0,[%g1+LWP_STACK]
	ld [%g1+LWP_SP],%sp
	ld [%g1+LWP_FP],%fp
	ld [%g1+LWP_FUNC],%o2
	ld [%g1+LWP_RETC],%i7
	jmp %o2
	nop
/*
.LLfe1:
	.size	 JD_call_lwp,.LLfe1-JD_call_lwp
*/

	.align 4
	.global JD_call_lwp_return
	.type	 JD_call_lwp_return,#function
	.proc	020
JD_call_lwp_return:
.LL_return:
	ret
	restore

	.align 4
	.global JD_sleep_lwp
	.type	 JD_sleep_lwp,#function
	.proc	020
JD_sleep_lwp:
	!#PROLOGUE# 0
	save %sp,-128,%sp
	!#PROLOGUE# 1
	/* arg = lwp */
	st %i0,[%fp+68]
        mov %i0,%g1    
	st %fp,[%g1+LWP_FP]
	st %sp,[%g1+LWP_SP]
	st %i7,[%g1+LWP_RETC]
	/*
	 * Do a "flush register windows trap".  The trap handler in the
	 * kernel writes all the register windows to their stack slots, and
	 * marks them all as invalid (needing to be sucked up from the
	 * stack when used).  This ensures that all information needed to
	 * unwind to these callers is in memory, not in the register
	 * windows.
	 */
	ta	ST_FLUSH_WINDOWS
	/*
	sethi %hi(JD_sleep_lwp_jump),%o2
	or %o2,%lo(JD_sleep_lwp_jump),%o1
	st %o1,[%g1+LWP_FUNC]
	*/
	ld [%g1+LWP_BASE],%o0
	ld [%o0],%o1
	sub %o1,%sp,%o2
	add %o2,4,%o0
	st  %o0,[%g1+LWP_SIZE] ;
	call malloc,0
	nop
	ld [%fp+68],%g1
	st %o0,[%g1+LWP_STACK]
	/*
	copy stack frame to the save_stack
	*/
	ld [%g1+LWP_BASE],%o0
	ld [%o0],%o0
	ld [%g1+LWP_SIZE],%o1
	ld [%g1+LWP_STACK],%o2
	
.LL6:
	cmp %o1,0
	be .LL7
	nop
	add %o1,-4,%o3
	mov %o3,%o1
	add %o0,-4,%o3
	mov  %o3,%o0
	ld [%o0],%o3
	st %o3,[%o2]
	add %o2,4,%o3
	mov %o3,%o2
	b .LL6
	nop
.LL7:
	/*
	mov %g1,%o0
	call JD_print_lwp,0
	nop
	*/
	ld [%fp+68],%g1
	ld [%g1+LWP_RET_FP],%fp
	ld [%g1+LWP_RET_SP],%sp
	ld [%g1+LWP_RET],%o2
	ld [%g1+LWP_RETA],%i7
	jmp %o2
	nop

	.align 4
	.global JD_sleep_lwp_jump
	.type	 JD_sleep_lwp_jump,#function
	.proc	020
JD_sleep_lwp_jump:
	ret
	restore
