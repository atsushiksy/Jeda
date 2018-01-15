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

int eval_to_int ( expression_node *exp, int *ret ) ;

char *eval_to_bitstr( expression_node *exp ) ;

expression_node  *const_aop_const( 
  JD_expression_type ops, expression_node *op1, expression_node *op2
) ;

expression_node  *const_lop_const( 
  JD_expression_type ops, expression_node *op1, expression_node *op2
) ;

expression_node  *const_sop_const( 
  JD_expression_type ops, expression_node *op1, expression_node *op2
) ;

expression_node  *const_cop_const( 
  JD_expression_type ops, expression_node *op1, expression_node *op2
) ;

expression_node  *aop_int( 
  JD_expression_type ops, expression_node *op1
) ;

expression_node  *aop_bit( 
  JD_expression_type ops, expression_node *op1
) ;

expression_node  *aop_const( 
  JD_expression_type ops, expression_node *op1
) ;

expression_node  *mask_comp_const( 
  expression_node *op0, expression_node *op1, expression_node *op2
) ;

expression_node  *conditional_const( 
  expression_node *op0, expression_node *op1, expression_node *op2
) ;

expression_node  *concatenate_const( 
  expression_node *op0
) ;

expression_node  *concatenate_const( 
  expression_node *op0
) ;

/* TBI */
expression_node  *duplicate_const( 
  expression_node *op0, expression_node *op1
) ;

expression_node  *concatenate_string( 
  expression_node *op0
) ;

expression_node  *duplicate_string( 
  expression_node *op0, expression_node *op1
) ;

