/* 
   
   Copyright (C) 2003 Jeda Technologies, Inc.

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
  int  *save_ebx ;     48 
} ;
*/

/*
   Calling stack 
      [   ptr to JD_lwp   ]
      [   ret ip          ]   <-  %esp
      
   Creating call stack frame
      [   ptr to JD_lwp   ]
      [   ret ip          ]   <-  %ebp
      [   prev  %ebp      ]   <-  %esp
      
   Adding 2K buff to %esp at the first call
      [   ptr to JD_lwp   ]
      [   ret ip          ]   <-  %ebp
      [   prev  %ebp      ]      
      [                   ]    -+
               .                | 2048 buff
               .                |
      [                   ]   <-  %esp  =  LWP_BASE
      
   Adding 2K buff to %esp at the first call
      [   ptr to JD_lwp   ]
      [   ret ip          ]   <-  %ebp
      [   prev  %ebp      ]      
      [                   ]    -+
               .                | 2048 buff
               .                |
      [                   ]   <--  LWP_BASE
      [                   ]               -+
               .                           |  Copy this area to 
               .                           |    stack frame in LWP_STACK
      [                   ]   <-  %esp    -+
      

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
#define  LWP_EBX          48

#define  LWP_STACK_OFFSET  $2048

.text
	.align 4
.global JD_call_lwp
	.type	 JD_call_lwp,@function
JD_call_lwp:
	popl	%ecx		/* ip  */
	popl	%edx		/* pointer to jmp_buf */
	pushl	%edx
	/* 1 pop should give the level of stack before 'call' */
	movl	%esp,LWP_RET_SP(%edx)
	movl	%ecx,LWP_RETA(%edx)
	push	%ecx
	/* back to the stack level after 'call' */
	/* creating 'stack frame' for debugger ? */
	movl	%ebp,LWP_RET_FP(%edx)
	movl	%ebx,LWP_RET(%edx)
	/* enter */
	pushl   %ebp
	movl    %esp,%ebp
	
	/* check if this is the first call */
	movl    LWP_STACK(%edx),%eax
	orl     %eax,%eax
	jne     .LL2          /* lwp stack is set, means wakeup from sleep */
	/* check if buffed stack point is set */
	movl    LWP_BASE(%edx),%ecx
	movl    (%ecx),%ecx
	cmpl    $0,%ecx
	jne    .LL0
	
	/* first time to call lwp, so add offset to stack to preserve
	   some offset in the case of unleveled call 
	*/
	movl    %esp,%ecx
	subl    LWP_STACK_OFFSET,%ecx
	movl    %ecx,%esp
	movl    LWP_BASE(%edx),%eax
	movl    %ecx,(%eax)
	jmp     .LL1
	
.LL0:
        /* stack base is set (%ecx), check it is smaller than current %esp */
	cmpl    %ecx,%esp
	jae     .LL01
	movl    %esp,%eax
	addl    $-12,%esp
	pushl   %ecx
	pushl   %eax
	/* JD_stack_base_error( int *bp, int *sp )  */
	call    JD_stack_base_error
	addl    $20,%esp
	/* leave */
	movl    %ebp,%esp
	popl   %ebp
	ret
	
	/* This point we have a buffed sp for lwp on %ecx */
.LL01:
	mov     %ecx,%esp
.LL1:	
       /* First time call to the function */
	movl    LWP_FUNC(%edx),%ecx
	movl    LWP_ARG(%edx),%eax
	pushl   %edx     /* save frame */
	pushl   %ebx     /* save ebx */
	addl    $-12,%esp
	pushl   %eax     /* arg */
	call    *%ecx
	addl    $16,%esp
	popl    %ebx     /* ebx */
	popl    %edx     /* frame */
	/* leave */
	movl    %ebp,%esp
	popl   %ebp
	ret
.LL2:
        /* This point is to restart the lwp */
        /* construct the stack frame as it used to be */
	pushl   %edx
	pushl   %ebx
	movl    LWP_STACK(%edx),%ecx
	movl    LWP_SIZE(%edx),%eax
	movl    LWP_BASE(%edx),%edx
	movl    (%edx),%edx
.LL4:
        /* stack frame copy loop */
	orl     %eax,%eax
	je      .LL5
	subl    $4,%eax
	subl    $4,%edx
	movl    (%ecx),%ebx
	movl    %ebx,(%edx)
	addl    $4,%ecx
	jmp     .LL4
.LL5:
        /* free( stack_frame ) */
        popl    %ebx
        popl    %edx
	movl    LWP_STACK(%edx),%eax
	pushl   %edx
	pushl   %ebx
	addl    $-12,%esp
	pushl   %eax     /* arg */
	call    free
	addl    $16,%esp
	popl    %ebx
	popl    %edx
	/*
	pushl   %ebx
	addl    $-12,%esp
	pushl   %ebx
	call    JD_print_lwp
	addl    $16,%esp
	popl    %ebx
	*/
	movl    $0,LWP_STACK(%edx)
	movl    LWP_SP(%edx),%esp
	movl    LWP_FP(%edx),%ebp
	movl    LWP_RETC(%edx),%ecx
	movl    LWP_EBX(%edx),%ebx
	jmp     *%ecx




	.align 4
	.global JD_call_lwp_return
	.type	 JD_call_lwp_return,@function
JD_call_lwp_return:
	ret

	.align 4
	.global JD_sleep_lwp
	.type	 JD_sleep_lwp,@function
JD_sleep_lwp:
	popl	%ecx		/* ip  */
	popl	%edx		/* pointer to jmp_buf */
	pushl	%edx
	movl    %esp,LWP_SP(%edx)
	movl    %ecx,LWP_RETC(%edx)
	pushl	%ecx
	movl	%ebp,LWP_FP(%edx)
	movl    %ebx,LWP_EBX(%edx)
	/* enter */
	pushl   %ebp
	movl    %esp,%ebp
	
	movl    LWP_BASE(%edx),%ecx
	movl    (%ecx),%ecx
	movl    %ecx,%eax
	subl    %esp,%eax     /* ! OK?? */
	addl    $4,%eax
	movl    %eax,LWP_SIZE(%edx) ;
	pushl   %edx
	pushl   %ebx
	addl    $-12,%esp
	pushl   %eax
	call    malloc
	addl    $16,%esp
	popl    %ebx
	popl    %edx
	movl    %eax,LWP_STACK(%edx)
	/*
	copy stack frame to the save_stack
	*/
        pushl   %ebx
        pushl   %edx
	movl    LWP_BASE(%edx),%ecx
	movl    (%ecx),%ecx
	movl    LWP_SIZE(%edx),%eax
	movl    LWP_STACK(%edx),%edx
.LL6:   /* stack frame copy loop */
	orl     %eax,%eax
	je      .LL7
	subl    $4,%eax
	subl    $4,%ecx
	movl    (%ecx),%ebx
	movl    %ebx,(%edx)
	addl    $4,%edx
	jmp     .LL6
.LL7:
        popl    %edx 
        popl    %ebx 
	/*
	addl    $-12,%esp
	push    %ebx
	call JD_print_lwp
	addl    $16,%esp
	pop     %ebx
	*/
	mov     LWP_RET_SP(%edx),%esp
	mov     LWP_RET_FP(%edx),%ebp
	mov     LWP_RETA(%edx),%ecx
	movl	LWP_RET(%edx),%ebx
	jmp     *%ecx

	.align 4
	.global JD_sleep_lwp_jump
	.type	 JD_sleep_lwp_jump,@function
JD_sleep_lwp_jump:
	ret
