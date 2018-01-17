/* 
   
   Copyright (C) 2018 Atsushi Kasuya

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
  uint64_t  *ret_sp ;       // 0  
  uint64_t ret_rbp ;        // 8 
  uint64_t ret_rbx ;        // 16
  uint64_t ret_r12 ;        // 24
  uint64_t ret_r13 ;        // 32
  uint64_t ret_r14 ;        // 40
  uint64_t ret_r15 ;        // 48
  void (*func)() ;          // 56 
  JD_thread *arg ;          // 64 
  uint64_t  *sp ;           // 72 
  uint64_t rbp ;            // 80
  uint64_t rbx ;            // 88
  uint64_t r12 ;            // 96
  uint64_t r13 ;            // 104
  uint64_t r14 ;            // 112
  uint64_t r15 ;            // 120
  void (*reta)() ;          // 128 
  void (*retc)() ;          // 136 
  uint64_t  *save_stack ;   // 144 
  uint64_t stack_size ;     // 152 
  uint64_t  **stack_base ;  // 160 + 8 = 168 total size 
} ;
*/

#define  LWP_RET_SP			0
#define  LWP_RET_RBP		8
#define  LWP_RET_RBX		16
#define  LWP_RET_R12		24
#define  LWP_RET_R13		32
#define  LWP_RET_R14		40
#define  LWP_RET_R15		48	
#define  LWP_FUNC			56
#define  LWP_ARG			64
#define  LWP_SP				72
#define  LWP_RBP			80
#define  LWP_RBX			88
#define  LWP_R12			96
#define  LWP_R13			104
#define  LWP_R14			112
#define  LWP_R15			120	
#define  LWP_RETA          	128
#define  LWP_RETC			136
#define  LWP_STACK        	144
#define  LWP_SIZE         	152
#define  LWP_BASE         	160


#define  LWP_STACK_OFFSET  $2048
/*
    Calling stack 
      [   ret ip          ]   <-  %rsp
      
   Creating call stack frame
      [   ret ip          ]   ret ip -> rax
      [   prev  %ebp      ]   <-  %rsp =>  %rbp
      
   Adding 2K buff to %esp at the first call
      [   ret ip          ]   
      [   prev  %ebp      ]    <-  %rbp
      [                   ]    -+
               .                | 2048 buff
               .                |
      [                   ]   <-  %rsp  =  LWP_BASE
      
   Adding 2K buff to %esp at the first call
      [   ret ip          ]   
      [   prev  %ebp      ]    <-  %ebp
      [                   ]    -+
               .                | 2048 buff
               .                |
      [                   ]   <--  LWP_BASE
      [                   ]               -+
               .                           |  Copy this area to 
               .                           |    stack frame in LWP_STACK
      [                   ]   <-  %esp    -+
      
      

*/


.text
	.align 4
.global JD_call_lwp
	.type	 JD_call_lwp,@function
JD_call_lwp:
	popq	%rax		/* ip  */
	/* 1 pop should give the level of stack before 'call' */
	movq	%rsp,LWP_RET_SP(%rdi)
	movq	%rax,LWP_RETA(%rdi)  // poped ip
	movq	%rbp,LWP_RET_RBP(%rdi)
	movq	%rbx,LWP_RET_RBX(%rdi)
	movq	%r12,LWP_RET_R12(%rdi)
	movq	%r13,LWP_RET_R13(%rdi)
	movq	%r14,LWP_RET_R14(%rdi)
	movq	%r15,LWP_RET_R15(%rdi)
	pushq	%rax		/* pushback return ip  */

	/* enter */
	pushq	%rbp
	movq	%rsp, %rbp
	
	/* check if this is the first call */
	movq    LWP_STACK(%rdi),%rax
	orq     %rax,%rax
	jne     .LL2          /* lwp stack is set, means wakeup from sleep */
	/* check if buffed stack point is set */
	movq    LWP_BASE(%rdi),%rax
	movq    (%rax),%rax
	cmpq    $0,%rax
	jne    .LL0
	
	/* first time to call lwp, so add offset to stack to preserve
	   some offset in the case of unleveled call 
	*/
	movq    %rsp,%rax
	subq    LWP_STACK_OFFSET,%rax
	movq    %rax,%rsp
	movq    LWP_BASE(%rdi),%rax
	movq    %rsp,(%rax)           // save stack start to a memory location
	jmp     .LL1
	
.LL0:
        /* stack base is set (%ecx), check if it is smaller than current %esp */
	cmpq    %rax,%rsp
	jae     .LL01
	movq    %rsp,%rdi
	movq    %rbp,%rsi
	/* JD_stack_base_error( int *bp, int *sp )  */
	call    JD_stack_base_error
	/* leave */
	movq	%rbp,%rsp
	popq   	%rbp
	ret
	
	/* This point we have a buffed sp for lwp on %ecx */
.LL01:
	movq     %rax,%rsp
.LL1:	                 /* rsp is set with OFFSET */
       /* First time call to the function */
	movq    LWP_FUNC(%rdi),%rax
	movq    LWP_ARG(%rdi),%rdi
	call    *%rax
	/* leave */
	movq	%rbp,%rsp
	popq   %rbp
	ret
.LL2:
        /* This point is to restart the lwp */
        /* construct the stack frame as it used to be */
	movq    LWP_STACK(%rdi),%rcx
	movq    LWP_SIZE(%rdi),%rax
	movq    LWP_BASE(%rdi),%rdx
	movq    (%rdx),%rdx
.LL4:
        /* stack frame copy loop */
	orq     %rax,%rax
	je      .LL5
	subq    $8,%rax
	subq    $8,%rdx
	movq    (%rcx),%rbx
	movq    %rbx,(%rdx)
	addq    $8,%rcx
	jmp     .LL4
.LL5:
    /* free( stack_frame ) */
    pushq	%rdi
	movq    LWP_STACK(%rdi),%rdi
	call    free
	popq    %rdi
	/*
    pushq	rdi
	call    JD_print_lwp
	popq    %rdi
	*/
	movq    $0,LWP_STACK(%rdi)
	// recover registers
	movq	LWP_RBP(%rdi),%rbp
	movq	LWP_RBX(%rdi),%rbx
	movq	LWP_R12(%rdi),%r12
	movq	LWP_R13(%rdi),%r13
	movq	LWP_R14(%rdi),%r14
	movq	LWP_R15(%rdi),%r15

	movq    LWP_SP(%rdi),%rsp
	movq    LWP_RETC(%rdi),%rcx
	jmp     *%rcx




	.align 4
	.global JD_call_lwp_return
	.type	 JD_call_lwp_return,@function
JD_call_lwp_return:
	ret

	.align 4
	.global JD_sleep_lwp
	.type	 JD_sleep_lwp,@function
JD_sleep_lwp:
	popq	%rcx		/* ip  */
	movq    %rsp,LWP_SP(%rdi)
	movq    %rcx,LWP_RETC(%rdi)
	movq	%rbp,LWP_RBP(%rdi)
	movq	%rbx,LWP_RBX(%rdi)
	movq	%r12,LWP_R12(%rdi)
	movq	%r13,LWP_R13(%rdi)
	movq	%r14,LWP_R14(%rdi)
	movq	%r15,LWP_R15(%rdi)
	/* enter */
	//pushq   %rbp
	//movq    %rsp,%rbp
	
	movq    LWP_BASE(%rdi),%rcx
	movq    (%rcx),%rcx
	movq    %rcx,%rax
	subq    %rsp,%rax     /* ! OK?? */
	addq    $8,%rax
	movq    %rax,LWP_SIZE(%rdi) ;
	pushq   %rdi
	movq   	%rax,%rdi
	call    malloc
	popq    %rdi
	movq    %rax,LWP_STACK(%rdi)
	/*
	copy stack frame to the save_stack
	*/
	movq    LWP_BASE(%rdi),%rcx
	movq    (%rcx),%rcx
	movq    LWP_SIZE(%rdi),%rax
	movq    LWP_STACK(%rdi),%rdx
.LL6:   /* stack frame copy loop */
	orq     %rax,%rax
	je      .LL7
	subq    $8,%rax
	subq    $8,%rcx
	movq    (%rcx),%rbx
	movq    %rbx,(%rdx)
	addq    $8,%rdx
	jmp     .LL6
.LL7:
	/*
    pushq	%rdi
	call    JD_print_lwp
	popq    %rdi
	*/
	movq	LWP_RET_SP(%rdi),%rsp
	movq	LWP_RET_RBP(%rdi),%rbp
	movq	LWP_RET_RBX(%rdi),%rbx
	movq	LWP_RET_R12(%rdi),%r12
	movq	LWP_RET_R13(%rdi),%r13
	movq	LWP_RET_R14(%rdi),%r14
	movq	LWP_RET_R15(%rdi),%r15
	movq    LWP_RETA(%rdi),%rcx
	jmp     *%rcx

	.align 4
	.global JD_sleep_lwp_jump
	.type	 JD_sleep_lwp_jump,@function
JD_sleep_lwp_jump:
	ret
