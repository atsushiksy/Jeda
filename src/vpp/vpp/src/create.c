#include "common.h"
#include "yacc_stuff.h"
#include "proto.h"
#include <math.h>

extern var_list *head_table;
extern var_values *head_variable_list;
extern int nl_count;
loop_stack *Head_Loop_Stack = NULL;
int operand_is_string;
char *ptr_to_string_operand;


/* Copyright (C) 1996 Himanshu M. Thaker

This file is part of vpp.

Vpp is distributed in the hope that it will be useful,
but without any warranty.  No author or distributor
accepts responsibility to anyone for the consequences of using it
or for whether it serves any particular purpose or works at all,
unless he says so in writing.

Everyone is granted permission to copy, modify and redistribute
vpp, but only under the conditions described in the
document "vpp copying permission notice".   An exact copy
of the document is supposed to have been given to you along with
vpp so that you can know how you may redistribute it all.
It should be in a file named COPYING.  Among other things, the
copyright notice and this notice must be preserved on all copies.  */

/**************************************************************************
 *
 * Function Name : expr *create_identifier()
 * Parameters :
 *      int ident - type of expression operation to create (NMBR or STRING)
 *      char *name - name of identifier to create
 * Return Value :
 *	expr * - pointer to created identifier
 * Purpose :
 *	Currently, store the name as a text... eventually can either
 * lookup the name in a database and point to that, or return the
 * string if not found.  CURRENTLY ONLY THE STRING IS STORED!
 *
 **************************************************************************/
expr *create_identifier(ident,name)
int ident;
char *name;
{
    expr *tmp_ep;

/*printf("call to create_identifier with ident=%d,name=%s\n",ident,name);*/
    tmp_ep = MALLOC(expr);
    tmp_ep->ident = ident;
    tmp_ep->u.strg = NMALLOC(strlen(name)+1,char);
    strcpy(tmp_ep->u.strg, name);
    tmp_ep->ep1 = NULL;
    tmp_ep->ep2 = NULL;
    tmp_ep->ep3 = NULL;
    tmp_ep->elp = NULL;
    tmp_ep->vvp = NULL;

    return(tmp_ep);
}

/**************************************************************************
 *
 * Function Name : expr *create_expression()
 * Parameters :
 *      int ident - type of expression operation to create (e.g. ADD)
 *      expr *ep1, *ep2, *ep3 - pointers to the two expressions to operate on
 * Return Value :
 *	expr * - pointer to created expession
 * Purpose :
 *
 **************************************************************************/
expr *create_expression(ident, ep1, ep2, ep3)
int ident;
expr *ep1;
expr *ep2;
expr *ep3;
{
    expr *tmp_ep;

    tmp_ep = MALLOC(expr);
    tmp_ep->ident = ident;
    tmp_ep->ep1 = ep1;
    tmp_ep->ep2 = ep2;
    tmp_ep->ep3 = ep3;
    tmp_ep->elp = NULL;
    tmp_ep->vvp = NULL;

    return(tmp_ep);
}

/**************************************************************************
 *
 * Function Name : expr *create_primary()
 * Parameters :
 *      int ident
 *      char *name
 *      expr *ep1, *ep2
 *      expr_list *elp;
 * Return Value :
 *	expr * - pointer to created expession
 * Purpose :
 *
 **************************************************************************/
expr *create_primary(ident, name, ep1, ep2, elp)
int ident;
char *name;
expr *ep1;
expr *ep2;
expr_list *elp;
{
    expr *tmp_ep;
    char tmp_strg[MAXSTR];
    int i;

    tmp_ep = MALLOC(expr);
    tmp_ep->vvp = NULL;
    if (name != NULL) {
	tmp_ep->u.strg = NMALLOC(strlen(name)+1,char);
	strcpy(tmp_ep->u.strg, name);
	tmp_ep->vvp = get_variable_ref(ident,name);
    }
    else tmp_ep->u.strg = NULL;
    tmp_ep->ident = ident;
    tmp_ep->ep1 = ep1;
    tmp_ep->ep2 = ep2;
    tmp_ep->ep3 = NULL;
    tmp_ep->elp = elp;

    return(tmp_ep);
}

void traverse_expression(ep)
expr *ep;
{
    if (ep != NULL) {
	if (ep->ident == SUBSTR) {
	    printf("SUBSTR(");
	    traverse_expression(ep->ep1);
	    printf(",");
	    traverse_expression(ep->ep2);
	    printf(",");
	    traverse_expression(ep->ep3);
	    printf(")");
	}
	else if ((ep->ep1 != NULL) && (ep->ep2 != NULL)) {
	    printf("(");
	    traverse_expression(ep->ep1);
	    print_operand(ep);
	    traverse_expression(ep->ep2);
	    printf(")");
	}
	else if ((ep->ep1 != NULL) && (ep->ep2 == NULL)) {
	    printf("(");
	    print_operand(ep);
	    traverse_expression(ep->ep1);
	    printf(")");
	}
	else print_operand(ep);
    }
}

void print_operand(ep)
expr *ep;
{
	if ((ep->ident == STRING) || (ep->ident == NMBR) ||
	    (ep->ident == REAL)) printf("%s",ep->u.strg);
	else if (ep->ident == QUOTED_STRING) printf("%s",ep->u.strg);
	else if (ep->ident == ADD) printf(" PLUS ");
	else if (ep->ident == SUB) printf(" MINUS ");
	else if (ep->ident == MULT) printf(" MULT ");
	else if (ep->ident == DIV) printf(" DIV ");
	else if (ep->ident == MOD) printf(" MOD ");
	else if (ep->ident == EQ) printf(" EQ ");
	else if (ep->ident == NEQ) printf(" NEQ ");
	else if (ep->ident == CASEEQ) printf(" CASEEQ ");
	else if (ep->ident == CASENEQ) printf(" CASENEQ ");
	else if (ep->ident == AND) printf(" AND ");
	else if (ep->ident == OR) printf(" OR ");
	else if (ep->ident == LT) printf(" LT ");
	else if (ep->ident == LEQ) printf(" LEQ ");
	else if (ep->ident == GT) printf(" GT ");
	else if (ep->ident == GEQ) printf(" GEQ ");
	else if (ep->ident == BITAND) printf(" BITAND ");
	else if (ep->ident == BITOR) printf(" BITOR ");
	else if (ep->ident == BITXOR) printf(" BITXOR ");
	else if (ep->ident == BITXNOR) printf(" BITXNOR ");
	else if (ep->ident == SHIFTR) printf(" SHIFTR ");
	else if (ep->ident == SHIFTL) printf(" SHIFTL ");
	else if (ep->ident == UPLUS) printf(" UPLUS ");
	else if (ep->ident == UMINUS) printf(" UMINUS ");
	else if (ep->ident == UNOT) printf(" UNOT ");
	else if (ep->ident == UTILDA) printf(" UTILDA ");
	else if (ep->ident == UAND) printf(" UAND ");
	else if (ep->ident == UNAND) printf(" UNAND ");
	else if (ep->ident == UNOR) printf(" UNOR ");
	else if (ep->ident == UPIPE) printf(" UPIPE ");
	else if (ep->ident == UCARAT) printf(" UCARAT ");
	else if (ep->ident == UXNOR) printf(" UXNOR ");
	else if (ep->ident == POWER) printf(" ** ");
	else if (ep->ident == LOG2) printf(" LOG2 ");
	else if (ep->ident == ROUND) printf(" ROUND ");
	else if (ep->ident == CEIL) printf(" CEIL ");
	else if (ep->ident == FLOOR) printf(" FLOOR ");
	else if (ep->ident == MAX) printf(" MAX ");
	else if (ep->ident == MIN) printf(" MIN ");
	else if (ep->ident == ABS) printf(" ABS ");
	else if (ep->ident == STOI) printf(" STOI ");
	else if (ep->ident == ITOS) printf(" ITOS ");
	else if (ep->ident == SUBSTR) printf(" SUBSTR ");
	else if (ep->ident == SYSTEM) printf(" SYSTEM ");
	else if (ep->ident == EVEN) printf(" EVEN ");
	else if (ep->ident == ODD) printf(" ODD ");
	else printf(" UNKNOWN ");
}

/**************************************************************************
 *
 * Function Name : expr_list *create_expression_list()
 * Parameters :
 *     expr_list *head - pointer to head of list
 *     expr *ep - pointer to the expression to add to list
 * Return Value :
 *     expr_list * - pointer to an expression list
 * Purpose :
 *     Add to a linked list the given expression.  Note that if
 * a new list is being created, "head" should be null.
 * SUGGESTIONS FOR SPEED IMPROVEMENT (SFSI) : send tail of linked
 * list also... then don't have to search thru till end every time.
 * We will first have to see how often this function is called, and
 * if often enough, add the extra overhead of the tail.
 *
 **************************************************************************/
expr_list *create_expression_list(head, ep)
expr_list *head;
expr *ep;
{
    expr_list *tmp_head;

    if (head == NULL) {
	head = MALLOC(expr_list);
	head->ep = ep;
	head->next = NULL;
    }
    else {
	tmp_head = head;
	while (tmp_head != NULL) {
	    if (tmp_head->next == NULL) {
		tmp_head->next = MALLOC(expr_list);
		tmp_head = tmp_head->next;
		tmp_head->ep = ep;
		tmp_head->next = NULL;
	    }
	    tmp_head = tmp_head->next;
	} /* of while loop */
    } /* of else of if head is null */
   
    return (head);
}

/**************************************************************************
 *
 * Function Name : create_statement
 * Parameters :
 * Return Value : stmt * - pointer to a statement
 * Purpose :
 *
 **************************************************************************/
stmt *create_statement(ident, ep, sp1, sp2, lp)
int ident;
expr *ep;
stmt *sp1;
stmt *sp2;
t_ident_ref *lp;
{
    stmt *tmp_sp;

    tmp_sp = MALLOC(stmt);
    tmp_sp->ident = ident;
    tmp_sp->ep = ep;
    tmp_sp->sp1 = sp1;
    tmp_sp->sp2 = sp2;
    tmp_sp->lp = lp;

    return (tmp_sp);
}

/**************************************************************************
 *
 * Function Name : t_ident_ref *create_ident_ref()
 * Parameters :
 *      int ident
 *      char *name
 *      expr *ep1, *ep2
 * Return Value :
 *	t_ident_ref * - pointer to created lvalue
 * Purpose :
 *
 **************************************************************************/
t_ident_ref *create_ident_ref(ident, name, ep1, ep2)
int ident;
char *name;
expr *ep1;
expr *ep2;
{
    t_ident_ref *tmp_ptr;

    tmp_ptr = MALLOC(t_ident_ref);
    if (name != NULL) {
	tmp_ptr->strg = NMALLOC(strlen(name)+1,char);
	strcpy(tmp_ptr->strg, name);
    }
    else tmp_ptr->strg = NULL;
    tmp_ptr->ident = ident;
    tmp_ptr->ep1 = ep1;
    tmp_ptr->ep2 = ep2;

    return(tmp_ptr);
}

/**************************************************************************
 *
 * Function Name : create_variable_ref()
 * Parameters :
 *    char *name - name of variable
 *    expr *ep - expression used to assign to variable
 *    int do_eval - whether or not to do evaluation
 *                  ... this is important in cases like x=x-1
 * Return Value :
 *    varp * - pointer to record created for variable
 * Purpose :
 *    This function first creates a unique record that holds
 * the name, expression to evaluate to assign to that variable,
 * then inserts the variable into a table, and finally, evaluates
 * the expression.
 *
 **************************************************************************/
varp *create_variable_ref(name, ep, do_eval)
char *name;
expr *ep;
int do_eval;
{
    var_values *tmp2;
    varp *tmp;
    int val;

/*
printf("call to create_variable_ref('%s',",name);traverse_expression(ep);
printf(", %d)\n",do_eval);
*/
    tmp = MALLOC(varp);
    if (name != NULL) {
	tmp->name = NMALLOC(strlen(name)+1, char);
	strcpy(tmp->name, name);
    }
    else tmp->name = NULL;
    tmp->vvp = get_variable_ref(STRING,name);
    if (tmp->vvp == NULL) {
	tmp->vvp = create_variable(name, evaluate_expression(ep));
	if (operand_is_string) {
	    tmp->vvp->strg = NMALLOC(strlen(ptr_to_string_operand)+1,char);
	    strcpy(tmp->vvp->strg, ptr_to_string_operand);
	}
    }
    else if (do_eval) {
	tmp->vvp->strg = double2ascii(evaluate_expression(ep));
	if (operand_is_string) {
	    free(tmp->vvp->strg);
	    tmp->vvp->strg = NMALLOC(strlen(ptr_to_string_operand)+1,char);
	    strcpy(tmp->vvp->strg, ptr_to_string_operand);
	}
    }
    tmp->strg = NULL;
    tmp->ep = ep;
/*printf("create_variable_ref() exiting, and set '%s' to '%s'\n",name,tmp->vvp->strg);*/

    return(tmp);
}

/**************************************************************************
 *
 * Function Name : 
 * Parameters :
 * Return Value :
 *    var_values * - pointer to where variable is stored
 * Purpose :
 *    This function creates a new variable.  If one already exits,
 * its value is overwritten with the new one.
 *
 **************************************************************************/
var_values *create_variable(name, dval)
char *name;
double dval;
{
    var_values *tmp_head;

/*printf("call to create_varaible('%s',%f)\n",name,dval);*/

    if (head_variable_list == NULL) {
	head_variable_list = MALLOC(var_values);
        tmp_head = head_variable_list;
    }
    else {
	tmp_head = head_variable_list;
	while (tmp_head != NULL) {
	    if (strcmp(name,tmp_head->name) == 0) {
		tmp_head->value = (int)dval;
		tmp_head->vtype = NMBR;
		if (tmp_head->strg != NULL) free (tmp_head->strg);
		tmp_head->strg = double2ascii(dval);
		return(tmp_head);
	    }
	    else if (tmp_head->next == NULL) {
		tmp_head->next = MALLOC(var_values);
		tmp_head = tmp_head->next;
		break;
	    }
	    tmp_head = tmp_head->next;
	}
    }
	
    tmp_head->name = NMALLOC(strlen(name)+1,char);
    strcpy(tmp_head->name,name);
    tmp_head->value = (int)dval;
    tmp_head->dval = 0.0;
    tmp_head->vtype = NMBR;
/*
    if (operand_is_string) {
	tmp_head->strg = NMALLOC(strlen(ptr_to_string_operand)+1,char);
	strcpy(tmp_head->strg, ptr_to_string_operand);
    }
    else 
*/
    tmp_head->strg = double2ascii(dval);
    tmp_head->next = NULL;

    return (tmp_head);
}

/**************************************************************************
 *
 * Function Name : 
 * Parameters :
 *    int ident - either NMBR, REAL or STRING
 *    char *name - name to get reference for
 * Return Value :
 * Purpose :
 *    If ident is NMBR, name is converted into an integer, a varp * pointer
 * created, and returned.  The user is responsible for destroying the returned
 * pointer.  If ident is STRING, the name is looked up.  If found, a pointer
 * to the reference is returned.
 *
 **************************************************************************/
var_values *get_variable_ref(ident,name)
int ident;
char *name;
{
    var_values *tmp;
    var_values *ttt;
    double dval;

/*printf("call to get_variable_ref(%d,'%s')\n",ident,name);*/
    if ((ident == NMBR) || (ident == REAL)) {
	ttt = MALLOC(var_values);
	ttt->name = NULL;
	ttt->next = NULL;
	ascii2int(name, &(ttt->value));
	ttt->dval = 0.0;
	ascii2double(name,&dval);
	ttt->strg = double2ascii(dval);
	ttt->vtype = NMBR;
	return (ttt);
    }
    else {
	tmp = head_variable_list;
	while (tmp != NULL) {
	    if (strcmp(name, tmp->name) == 0) return (tmp);
	    tmp = tmp->next;
	}
/*
	sprintf(tchar, "can't find identifier '%s'",name);
	yyerror(tchar);
	printf("ERROR : can't find identifier '%s'\n",name);
*/
    }
    return(NULL);
}

/**************************************************************************
 *
 * Function Name : ascii2int()
 * Parameters :
 *    char *name - string to convert
 *    int *nmbr - pointer to where to store the converted number
 * Return Value :
 *    int
 * Purpose :
 *    Given a string, convert it to an integer.  Note that if
 * ANY non-digit is encountered, FALSE is returned, and *nmbr is
 * set to zero.  Otherwise, TRUE is returned and the convered number
 * is stored in *nmbr.
 *
 **************************************************************************/
int ascii2int(name, nmbr)
char *name;
int *nmbr;
{
    int i;
    int val;

    val = 0;
    for (i=0; i<strlen(name); i++)
      {
	  if (!isdigit(name[i])) {
	      *nmbr = 0;
	      return (FALSE);
	  }
	  val = val*10 + name[i] - '0';
      }

    *nmbr = val;
    return(TRUE);
}

/**************************************************************************
 *
 * Function Name : ascii2double()
 * Parameters :
 *    char *name - string to convert
 *    double *nmbr - pointer to where to store the converted number
 * Return Value :
 *    int
 * Purpose :
 *    Given a string, convert it to double precision floating point number.
 *  Note that if ANY non-digit is encountered, FALSE is returned, and *nmbr is
 * set to zero.  Otherwise, TRUE is returned and the convered number
 * is stored in *nmbr.
 *
 **************************************************************************/
int ascii2double(name, nmbr)
char *name;
double *nmbr;
{
    int i,j;
    double val;
    int len;
    double div;

    int is_neg;

/*printf("been asked to convert '%s' to double\n",name);*/
    val = 0.0;
    is_neg = FALSE;

    /*
     * strip off leading blanks...
     */
    j = 0;
    len = strlen(name);
    while ((j<len) && (name[j] == ' ') || (name[j] == '\t')) j++;
    if ((!isdigit(name[j])) && (name[j] != '.') && (name[j] != '-') &&
	(name[j] != '+') ) {
	*nmbr = 0.0;
	return (FALSE);
    }
    
    /*
     * The first non blank can be a "-" or a "+"
     */
    if (name[j] == '-') {
	is_neg = TRUE;
	j++;
    }
    else if (name[j] == '+') {
	j++;
    }
    
    for (i=j; i<len; i++)
      {
	  if (!isdigit(name[i])) {
	      j = i;
	      break;
	  }
	  val = val*10.0 + (name[i] - '0');
      }

    if (name[j] == '.') {
	div = 10.0;
	j++;
	for (i=j; i<len; i++) {
	    if (!isdigit(name[i])) {
		/*
		 * now, strip off trailing blanks
		 */
		for (j=i; j<len; j++) {
		    if ((name[j] != ' ') && (name[j] != '\t')) {
			*nmbr = 0.0;
			return(FALSE);
		    }
		}
		break;
	    }
	    val = val + (name[i]-'0')/div;
	    div = div*10.0;
            j = i;
	}
    }

    if (is_neg) val = -1.0 * val;

    *nmbr = val;
    return(TRUE);
}

/**************************************************************************
 *
 * Function Name : double2ascii()
 * Parameters :
 *    double nmbr - number to convert
 *    char *name - where to store the converted string
 * Return Value :
 *    int
 * Purpose :
 *    Given a double, convert it to the string equivalent.  Note that only
 * 5 digits after the period are stored.  For memory conservation, the
 * calling routine is responsible for ensuring that "name" has been freed
 * before calling this function.
 *
 **************************************************************************/
char *double2ascii(nmbr)
double nmbr;
{
    char *name;
    char tmp[MAXSTR];
    char tmp2[MAXSTR];
    double rem;
    int value;
    int is_negative;
    int len;
    int i,j;
    int digit;

/*printf("double2ascii() has been asked to convert %f\n",nmbr);*/
    if (nmbr < 0.0) {
	is_negative = TRUE;
	nmbr = nmbr * -1.0;
    }
    else is_negative = FALSE;

    value = (int) floor(nmbr);
    i = 0;
    if (value == 0) tmp2[i++] = '0';
    while (value > 0) {
	tmp2[i++] = ((value % 10) + '0');
	value = value / 10;
    }
    tmp2[i++] = '\0'; /* null terminate the string */

    if (is_negative) tmp[0] = '-';
    len = strlen(tmp2);
    for (i=0; i<len; i++) {
	tmp[i+is_negative]=tmp2[len-i-1];
    }

    tmp[i+is_negative]='.';
    i++;

    rem = nmbr - floor(nmbr) + DELTA/1000.0;
    for(j=0; j<6; j++) {
        digit = (int) (rem*10.0);
	tmp[i+is_negative] = digit + '0';
	i++;
	rem = (rem*10.0 - digit*1.0);
    }

    tmp[i+is_negative] = '\0';

    /*
     * finally, allocate space for the new string and return it
     */
    name = NMALLOC(strlen(tmp)+1,char);
    strcpy(name,tmp);

/*printf("double2ascii converted %f to '%s'\n",nmbr,name);*/
    return (name);
}

/**************************************************************************
 *
 * Function Name : 
 * Parameters :
 * Return Value :
 * Purpose :
 *
 **************************************************************************/
void free_expr(ep)
expr *ep;
{
    if (ep != NULL) {
	free_expr(ep->ep1);
	free_expr(ep->ep2);
	free_expr(ep->ep3);
	/*
	 * ++++ note note note!!! need to add code
	 * to properly free up elp and vp...
	 * we are ignoring it for now...
	 */
	free(ep);
    }
}

/**************************************************************************
 *
 * Function Name : 
 * Parameters :
 * Return Value :
 * Purpose :
 *
 **************************************************************************/
double evaluate_expression(ep)
expr *ep;
{
    double tmp1, tmp2;
    double val;

    val = 0.0;
    if (ep != NULL) {
	if ((ep->ep1 != NULL) && (ep->ep2 != NULL)) {
	    tmp1 = evaluate_expression(ep->ep1);
	    tmp2 = evaluate_expression(ep->ep2);
	    val = perform_operand(2, tmp1, tmp2, ep);
	}
	else if ((ep->ep1 != NULL) && (ep->ep2 == NULL)) {
	    tmp1 = evaluate_expression(ep->ep1);
	    val = perform_operand(1, tmp1, 0.0, ep);
	}
	else {
	    val = perform_operand(0, 0.0, 0.0, ep);
	}
    } /* of if ep is not NULL */
/*printf("about to return from evaluate_expression with %f\n",val);*/
    return (val);
}

/**************************************************************************
 *
 * Function Name : 
 * Parameters :
 *    np - number of parameters
 *    tmp1 - parameter 1
 *    tmp2 - parameter 2
 *    ep - expression
 * Return Value :
 * Purpose :
 *
 **************************************************************************/
double perform_operand(np, tmp1, tmp2, ep)
int np;
double tmp1;
double tmp2;
expr *ep;
{
    double val;
    int int1,int2,intval;
    char msg[MAXSTR];

/*
printf("---------------------vvvv call to perform_operand\n");
printf("np=%d,tmp1=%f,tmp2=%f,ep=",np,tmp1,tmp2);
traverse_expression(ep); printf("\n");
printf("ep->ident=%d\n",ep->ident);
*/

    val = 0.0;
    operand_is_string = FALSE;
    if ((ep->ident == NMBR) || (ep->ident == REAL) || (ep->ident == STRING)) {
	if (ep->vvp == NULL) {
	    val = get_define_value(ep->u.strg);

	    sprintf(msg,"ERROR : variable `%s has not been assigned prior to use",ep->u.strg);
	    yyerror(msg);
/*
printf("ERROR : value '%s' has not been assigned!\n", ep->u.strg);
*/
	}
	else {
            ascii2double(ep->vvp->strg, &val);
	}
    }
    else if ((ep->ident == QUOTED_STRING)) {
        operand_is_string = TRUE;
        ptr_to_string_operand = ep->u.strg;
    }
    else if (ep->ident == ADD) {
	if (np == 2) val = tmp1 + tmp2;
	else val = tmp1;
    }
    else if (ep->ident == SUB) {
	if (np == 2) val = tmp1 - tmp2;
	else val = - tmp1;
    }
    else if (ep->ident == MULT) {
	val = tmp1 * tmp2;
    }
    else if (ep->ident == DIV) {
	if (tmp2 == 0) fprintf(stderr,"ERROR : attempted div by zero!\n");
	else val = tmp1 / tmp2;
    }
    else if (ep->ident == MOD) {
	if (tmp2 == 0) fprintf(stderr,"ERROR : attempted mod by zero!\n");
	else val = (double) (((int) tmp1) % ((int) tmp2));
    }
    else if (ep->ident == POWER) {
	val = pow(tmp1,tmp2);
    }
    else if (ep->ident == EQ) {
	if (is_an_integer(tmp1) && is_an_integer(tmp2)) {
	    int1 = (int) rint(tmp1);
	    int2 = (int) rint(tmp2);
	    intval = (int1 == int2);
	    val = intval * 1.0;
	}
	else {
	    val = (tmp1 == tmp2);
	}
    }
    else if (ep->ident == NEQ) {
	if (is_an_integer(tmp1) && is_an_integer(tmp2)) {
	    int1 = (int) rint(tmp1);
	    int2 = (int) rint(tmp2);
	    intval = (int1 != int2);
	    val = intval * 1.0;
	}
	else {
	    val = (tmp1 != tmp2);
	}
    }
    else if (ep->ident == AND) {
	int1 = (int) rint(tmp1);
	int2 = (int) rint(tmp2);
	intval = (int1 && int2);
	val = intval * 1.0;
    }
    else if (ep->ident == OR) {
	int1 = (int) rint(tmp1);
	int2 = (int) rint(tmp2);
	intval = (int1 || int2);
	val = intval * 1.0;
    }
    else if (ep->ident == LT) {
	if (is_an_integer(tmp1) && is_an_integer(tmp2)) {
	    int1 = (int) rint(tmp1);
	    int2 = (int) rint(tmp2);
	    intval = (int1 < int2);
	    val = intval * 1.0;
	}
	else {
	    val = (tmp1 < tmp2);
	}
    }
    else if (ep->ident == LEQ) {
	if (is_an_integer(tmp1) && is_an_integer(tmp2)) {
	    int1 = (int) rint(tmp1);
	    int2 = (int) rint(tmp2);
	    intval = (int1 <= int2);
	    val = intval * 1.0;
	}
	else {
	    val = (tmp1 <= tmp2);
	}
    }
    else if (ep->ident == GT) {
	if (is_an_integer(tmp1) && is_an_integer(tmp2)) {
	    int1 = (int) rint(tmp1);
	    int2 = (int) rint(tmp2);
	    intval = (int1 > int2);
	    val = intval * 1.0;
	}
	else {
	    val = (tmp1 > tmp2);
	}
    }
    else if (ep->ident == GEQ) {
	if (is_an_integer(tmp1) && is_an_integer(tmp2)) {
	    int1 = (int) rint(tmp1);
	    int2 = (int) rint(tmp2);
	    intval = (int1 >= int2);
	    val = intval * 1.0;
	}
	else {
	    val = (tmp1 >= tmp2);
	}
    }
    else if (ep->ident == BITAND) {
	int1 = (int) rint(tmp1);
	int2 = (int) rint(tmp2);
	intval = int1 & int2;
	val = intval * 1.0;
    }
    else if (ep->ident == BITOR) {
	int1 = (int) rint(tmp1);
	int2 = (int) rint(tmp2);
	intval = int1 | int2;
	val = intval * 1.0;
    }
    else if (ep->ident == BITXOR) {
	int1 = (int) rint(tmp1);
	int2 = (int) rint(tmp2);
	intval = int1 ^ int2;
	val = intval * 1.0;
    }
    else if (ep->ident == BITXNOR) {
	int1 = (int) rint(tmp1);
	int2 = (int) rint(tmp2);
	intval = ~(int1 ^ int2);
	val = intval * 1.0;
    }
    else if (ep->ident == SHIFTR) {
	int1 = (int) rint(tmp1);
	int2 = (int) rint(tmp2);
	intval = int1 >> int2;
	val = intval * 1.0;
    }
    else if (ep->ident == SHIFTL) {
	int1 = (int) rint(tmp1);
	int2 = (int) rint(tmp2);
	intval = int1 << int2;
	val = intval * 1.0;
    }
    else if (ep->ident == UPLUS) {
	val = tmp1;
    }
    else if (ep->ident == UMINUS) {
	val = tmp1 * -1.0;
    }
    else if (ep->ident == UNOT) {
	int1 = (int) rint(tmp1);
	if (int1 == 0) intval = TRUE;
	else intval = FALSE;
	val = intval * 1.0;
    }
    else if (ep->ident == UTILDA) {
	int1 = (int) rint(tmp1);
	intval = ~int1;
	val = intval * 1.0;
    }
/*
    else if (ep->ident == UAND) {
printf("NOTE : need to add UAND!\n");
    }
    else if (ep->ident == UNAND) {
printf("NOTE : need to add UNAND!\n");
    }
    else if (ep->ident == UNOR) {
printf("NOTE : need to add UNOR!\n");
    }
    else if (ep->ident == UPIPE) {
printf("NOTE : need to add UPIPE!\n");
    }
    else if (ep->ident == UCARAT) {
printf("NOTE : need to add UCARAT!\n");
    }
    else if (ep->ident == UXNOR) {
printf("NOTE : need to add UXNOR!\n");
    }
 */
 /*
    else if (ep->ident == LOG2) {
	if (tmp1 <= 0.0) {
	    fprintf(stderr,"ERROR : vpp : cannot do a `LOG2 of a negative or zero!\n");
	}
	else {
	    val = log2(tmp1);
	}
    }
  */
    else if (ep->ident == ROUND) {
	val = rint(tmp1);
    }
    else if (ep->ident == CEIL) {
	val = ceil(tmp1);
    }
    else if (ep->ident == FLOOR) {
	val = floor(tmp1);
    }
    else if (ep->ident == EVEN) {
	if (((int)tmp1) == 0) val = 1.0;
	else {
	    int1 = (int) (tmp1 / 2.0);
	    int2 = (int) ((tmp1 - 1.0) / 2.0);
	    if (int1 == int2)
	      val = 0.0;
	    else
	      val = 1.0;
	}
    }
    else if (ep->ident == ODD) {
	if (((int)tmp1) == 0) val = 0.0;
	else {
	    int1 = (int) (tmp1 / 2.0);
	    int2 = (int) ((tmp1 - 1.0) / 2.0);
	    if (int1 == int2)
	      val = 1.0;
	    else
	      val = 0.0;
	}
    }
    else if (ep->ident == MAX) {
	if (tmp1 > tmp2) val = tmp1;
	else val = tmp2;
    }
    else if (ep->ident == MIN) {
	if (tmp1 < tmp2) val = tmp1;
	else val = tmp2;
    }
    else if (ep->ident == ABS) {
	if (tmp1 < 0.0) val = -1.0*tmp1;
	else val = tmp1;
    }
    else if (ep->ident == STOI) {
fprintf(stderr,"STOI call not currently implemented.\n");
    }
    else if (ep->ident == ITOS) {
fprintf(stderr,"ITOS call not currently implemented.\n");
    }
    else if (ep->ident == SUBSTR) {
fprintf(stderr,"SUBSTR call not currently implemented.\n");
    }
    else if (ep->ident == SYSTEM) {
fprintf(stderr,"SYSTEM call not currently implemented.\n");
    }

/*
printf("perform_operand() about to return value %f\n",val);
printf("---------------------^^^^\n");
*/
    return (val);
}

/**************************************************************************
 *
 * Function Name : gen_ll *create_generic_ll()
 * Parameters :
 *    int ll_type - type of linked list
 *    gen_ll *head - pointer to the head of the list, NULL if first item 
 *    ll_generic *item - pointer to item of interest
 * Return Value :
 *    gen_ll * - pointer to the head of the linked list
 * Purpose :
 *
 *************************************************************************/
gen_ll *create_generic_ll(ll_type, head, item)
int ll_type;
gen_ll *head;
ll_generic *item;
{
    gen_ll *tmp_head;


    if (head == NULL) {
	head = MALLOC(gen_ll);
	head->ll_type = ll_type;
	head->line_count = nl_count;
	head->item = item;
	head->next = NULL;
	head->prev = NULL;
    }
    else {
	/*
	 * search thru the linked list to find the tail, and append
	 * the newly created variable to the end of this list
	 */
	tmp_head = head;
	while (tmp_head != NULL) {
	    if (tmp_head->next == NULL) {
		tmp_head->next = MALLOC(gen_ll);
		tmp_head->next->prev = tmp_head;
		tmp_head = tmp_head->next;
		tmp_head->ll_type = ll_type;
		tmp_head->line_count = nl_count;
		tmp_head->item = item;
		tmp_head->next = NULL;
	    }
	    tmp_head = tmp_head->next;
	} /* of while loop */
    }

    return (head);
}

/**************************************************************************
 *
 * Function Name : gen_ll *append_generic_ll(a,b)
 * Parameters :
 *    gen_ll *a;
 *    gen_ll *b;
 * Return Value :
 *    gen_ll * - pointer to the head of the linked list
 * Purpose :
 *    Merge list b to the end of list a.
 *
 *************************************************************************/
gen_ll *append_generic_ll(a,b)
gen_ll *a;
gen_ll *b;
{
    gen_ll *tmp;

/*
printf("=============================\n");
printf("... call to append_generic_ll\n");
printf("a :\n");
print_out_generic_ll(a);
printf("b :\n");
print_out_generic_ll(b);
*/
    
    if (a == NULL) return (b);

    tmp = a;
    if (tmp == NULL) return (b);
    while (tmp != NULL) {
	if (tmp->next == NULL) {
	    tmp->next = b;
	    b->prev = tmp;
	    break;
	}
	tmp = tmp->next;
    }

    return (a);
}

print_out_generic_ll(a)
gen_ll *a;
{
    printf("-----------------------------------\n");
    if (a == NULL) printf("list is null.\n");
    else { 
	while (a != NULL) {
	    printf("name is '%s'\n",a->item);
	    a = a->next;
	}
    }
    printf("-----------------------------------\n");
}

/**************************************************************************
 *
 * Function Name : 
 * Parameters :
 * Return Value :
 * Purpose :
 *
 **************************************************************************/
gen_ll *push(stack, item)
gen_ll *stack;
gen_ll *item;
{
    item->prev = stack;
    if (stack != NULL) {
	stack->next = item;
	stack = stack->next;
    }
    else stack = item;

    return (stack);
}

/**************************************************************************
 *
 * Function Name : 
 * Parameters :
 * Return Value :
 * Purpose :
 *
 **************************************************************************/
gen_ll *pop(stack)
gen_ll *stack;
{
    if (stack == NULL) return (NULL);
    else stack = stack->prev;

    return (stack);
}

/**************************************************************************
 *
 * Function Name : 
 * Parameters :
 * Return Value :
 * Purpose :
 *
 **************************************************************************/
gen_ll *create_if_structure(ep)
expr *ep;
{
    return ((gen_ll *) ep);
}

/**************************************************************************
 *
 * Function Name : 
 * Parameters :
 * Return Value :
 * Purpose :
 *
 **************************************************************************/
gen_ll *create_switch_structure(ep)
expr *ep;
{
    switch_struct *tmp;

    tmp = MALLOC(switch_struct);
    tmp->ep = ep;
    tmp->done_a_case = FALSE;
    tmp->status_on_entry = TRUE;
    return ((gen_ll *) tmp);
}

/**************************************************************************
 *
 * Function Name : 
 * Parameters :
 * Return Value :
 * Purpose :
 *
 **************************************************************************/
gen_ll *create_case_structure(ep)
expr *ep;
{
    return ((gen_ll *) ep);
}

/**************************************************************************
 *
 * Function Name : 
 * Parameters :
 * Return Value :
 * Purpose :
 *
 **************************************************************************/
gen_ll *create_while_structure(ep)
expr *ep;
{
    while_struct *tmp;

    tmp = MALLOC(while_struct);
    tmp->ep = ep;
    tmp->eval_first = TRUE;
    return ((gen_ll *) tmp);
}

/**************************************************************************
 *
 * Function Name : 
 * Parameters :
 * Return Value :
 * Purpose :
 *
 **************************************************************************/
int eval_stack(ll)
gen_ll *ll;
{
    gen_ll *head; /* will point to head where ll starts */
    int ltype; /* current linked list type */

    loop_stack stack[20];

    int active; /* indicates if current region is active */
    varp *vp;

    gen_ll *loop_back_to;
    gen_ll *loop_back_from;
    var_values *vvp;
    for_struct *forp;
    while_struct *whilep;
    switch_struct *switchp;

    gen_ll *for_stack[20];
    int fsp; /* pointer into above stack */
    gen_ll *switch_stack[20];
    int ssp; /* pointer into above stack */
    gen_ll *tmp;
    gen_ll_mgr *cp;

    int last_was_code;

    fsp = 0;
    ssp = 0;
    last_was_code = FALSE;

    /*
     * first, find the head of the stack..
     */
    while (ll != NULL) {
	if (ll->prev == NULL) break;
	ll = ll->prev;
    }

    head = ll;
    active = TRUE;
    while (ll != NULL) {
	ltype = ll->ll_type;
/*printf("ltype=%d\n",ltype);*/
	if (ltype == IF_STRUCTURE) {
/*printf("DISPLAY : if_structure...\n");*/
	    active = check_for_if(ll->item, ltype);
	}
	else if (ltype == IFDEF_STRUCTURE) {
/*printf("DISPLAY : ifdef_structure...\n");*/
	    active = check_for_if(ll->item, ltype);
	    if (active) {
		printf("`ifdef ");
		traverse_expression(ll->item);
	    }
	    printf("\n");
	}
	else if (ltype == IFNDEF_STRUCTURE) {
/*printf("DISPLAY : ifndef_structure...\n");*/
	    active = check_for_if(ll->item, ltype);
	    if (active) {
		printf("`ifdef ");
		traverse_expression(ll->item);
	        printf("\n");
		printf("`else\n");
	    }
	}
	else if (ltype == ELSE_STRUCTURE) {
/*printf("DISPLAY : else_structure...\n");*/
	    active = check_for_else();
	}
	else if (ltype == ENDIF_STRUCTURE) {
/*printf("DISPLAY : endif_structure...\n");*/
	    active = handle_endif();
	}
	else if ((ltype == CODE_POINTER) && active) {
	  cp = (gen_ll_mgr *) ll->item;
	  tmp = cp->head;
	  if (last_was_code == FALSE) do_comment_count(TRUE,tmp->line_count);
	  
	  print_out_code(ll->item);
	}
	else if ((ltype == LET_STRUCTURE) && active) {
	    vp = (varp *) ll->item;
	    vp->vvp->strg = double2ascii(evaluate_expression(vp->ep));
	}
	else if ((ltype == FOR_STRUCTURE) && active) {
/*printf("DISPLAY : for_structure...\n");*/
	    forp = (for_struct *) ll->item;
	    if (forp->eval_first == TRUE) {
		forp->eval_first = FALSE;
		forp->vp1->vvp->strg = double2ascii(evaluate_expression(forp->vp1->ep));
		if (fsp == 20) fprintf(stderr,"ERROR : cannot nest more than 20 for loops!\n");
		else {
		    for_stack[fsp++] = ll;
		}
	    }
	    else {
		/*
		 * at this point, we've just looped back from the endfor,
		 * and now must do the last for statement, and then reevaluate
		 * to see if we exit.
		 */
		vvp = get_variable_ref(STRING,forp->name2);
		vvp->strg = double2ascii(evaluate_expression(forp->ep2));

		if (!((int) evaluate_expression(forp->ep_eval))) {
		    forp->eval_first = TRUE;
		    tmp = ll;
		    ll = for_stack[fsp-1];
		    for_stack[fsp-1] = tmp;
		    if (fsp > 0) fsp--;
		}
		else {
		    tmp = ll;
		    for_stack[fsp-1] = tmp;
		}
	    }
	}
	else if ((ltype == ENDFOR_STRUCTURE)) {
/*printf("DISPLAY : endfor_structure...\n");*/
	    if (fsp >0) {
		tmp = ll;
		ll = for_stack[fsp-1];
		for_stack[fsp-1] = tmp;
		continue;
	    }
	    else {
		fprintf(stderr,"ERROR : `endfor encountered without matching `for\n");
	    }
	}
	else if ((ltype == WHILE_STRUCTURE) && active) {
/*printf("DISPLAY : WHILE_STRUCTURE...\n");*/
	    whilep = (while_struct *) ll->item;
	    if (whilep->eval_first == TRUE) {
		whilep->eval_first = FALSE;
		if (fsp == 20) fprintf(stderr,"ERROR : cannot nest more than 20 for loops!\n");
		else {
		    for_stack[fsp++] = ll;

		    /*
		     * if upon entry to the while loop, the expression is
		     * false, we must skip ahead to the next ENDWHILE structure
		     */
		    if (!((int)evaluate_expression(whilep->ep))) {
			whilep->eval_first = TRUE;
			if (fsp > 0) fsp--;
			tmp = ll;
			while (tmp != NULL) {
			    if (tmp->ll_type == ENDWHILE_STRUCTURE) break;
			    tmp = tmp->next;
			}
			ll = tmp;
		    }
		}
	    }
	    else {
		/*
		 * at this point, we've just looped back from the endwhile,
		 * so we must reevaluate expression to see if we must exit.
		 */

		if (!((int)evaluate_expression(whilep->ep))) {
		    whilep->eval_first = TRUE;
		    tmp = ll;
		    ll = for_stack[fsp-1];
		    for_stack[fsp-1] = tmp;
		    if (fsp > 0) fsp--;
		}
		else {
		    tmp = ll;
		    for_stack[fsp-1] = tmp;
		}
	    }
	}
	else if ((ltype == ENDWHILE_STRUCTURE)) {
/*printf("DISPLAY : ENDWHILE_STRUCTURE...\n");*/
	    if (fsp >0) {
		tmp = ll;
		ll = for_stack[fsp-1];
		for_stack[fsp-1] = tmp;
		continue;
	    }
	    else {
		fprintf(stderr,"ERROR : `endwhile encountered without matching `while\n");
	    }
	}
	else if ((ltype == SWITCH_STRUCTURE) && active) {
	    switchp = (switch_struct *) ll->item;
	    switchp->done_a_case = FALSE;
	    switchp->status_on_entry = active;
	    switchp->ignore_more_matches = FALSE;
	    switchp->value = (int) evaluate_expression (switchp->ep);

	    active = FALSE;
	    if (ssp == 20) fprintf(stderr,"ERROR : cannot nest more than 20 for loops!\n");
	    else {
		    switch_stack[ssp++] = ll;
	    }
	}
	else if (ltype == CASE_STRUCTURE) {
	    if (ssp > 0) {
	        switchp = (switch_struct *) switch_stack[ssp-1]->item;
		if ((switchp->status_on_entry) && /* active on entry */
		    (switchp->ignore_more_matches == FALSE)) {
		    active = (((int)evaluate_expression(ll->item)) == switchp->value);
		    if (active) {
			switchp->done_a_case = TRUE;
		    }
		}
	    }
	}
	else if (ltype == BREAKSW_STRUCTURE) {
	    if (ssp > 0) {
	        switchp = (switch_struct *) switch_stack[ssp-1]->item;
		if (switchp->status_on_entry) { /* active on entry */
		    if (active) {
			switchp->ignore_more_matches = TRUE;
                        active = FALSE;
		    }
		}
	    }
	}
	else if (ltype == DEFAULT_STRUCTURE) {
	    if (ssp > 0) {
	        switchp = (switch_struct *) switch_stack[ssp-1]->item;
		if (switchp->status_on_entry) { /* active on entry */
		    active = (switchp->done_a_case == FALSE);
		}
	    }
	}
	else if ((ltype == ENDSWITCH_STRUCTURE)) {
	    if (ssp > 0) {
		switchp = (switch_struct *) switch_stack[ssp-1]->item;
		active = switchp->status_on_entry;
		ssp--;
	    }
	}

	if ((ltype == CODE_POINTER) || (ltype == CODE_POINTER2))
	  last_was_code = TRUE;
	else
	  last_was_code = FALSE;
	
	/*
	 * Now, if we are still active, we should print out a message as
	 * to what the new line_count info should be...
	if ((active) &&
	    ((ltype == IF_STRUCTURE) || (ltype == FOR_STRUCTURE) ||
	     (ltype == CASE_STRUCTURE) || (ltype == DEFAULT_STRUCTURE) ||
	     (ltype == WHILE_STRUCTURE)))
	  {
	      do_comment_count(TRUE,ll->line_count);
	  }
	 */

	if (ll != NULL) ll = ll->next; /* need in case there is no matching endwhile */
    }
    do_comment_count(TRUE,nl_count);

    /*
     * code to free stack goes here... ++++
    ll = head;
    while (ll != NULL) {
	tmp = ll->next;
	
	ll = tmp;
    }
     */
}

int print_out_code(cp)
gen_ll_mgr *cp;
{
    gen_ll *tmp;
    var_values *vvp;

    if (cp == NULL) return;
    else {
	tmp = cp->head;
	while (tmp != NULL) {
/*
	  if ((!last_was_code) && (tmp->ll_type == CODE_POINTER))
	    do_comment_count(TRUE,tmp->line_count);
*/
	  if (tmp->ll_type == CODE_POINTER2) {
	    print_variable(tmp->item, TRUE);
	  }
	  else printf("%s", tmp->item);
	  
	  tmp = tmp->next;
	} /* of while loop */
    } /* of else of if cp is NULL */
}

/**************************************************************************
 *
 * Function Name : 
 * Parameters :
 *    char *name1 - name of first assignment identifier
 *    expr *ep1 - expression to assign the first ident to
 *    expr *ep_eval - expression to evaluate to determine ending for for
 *    char *name2 - name of 2nd assignment identifier
 *    expr *ep2 - expression to assign the 2nd ident to
 * Return Value :
 *    gen_ll * - pointer to the newly created for structure
 * Purpose :
 *    This function is passed the for constructs parameters, and it is
 * to create a pointer to a for structure populated with these parameters.
 *
 **************************************************************************/
gen_ll *create_for_structure(vp1,ep_eval,name2,ep2)
varp *vp1;
expr *ep_eval;
char *name2;
expr *ep2;
{
    for_struct *tmp;

    tmp = MALLOC(for_struct);
    tmp->vp1 = vp1;
    tmp->ep_eval = ep_eval;
    tmp->name2 = name2;
    tmp->ep2 = ep2;
    tmp->eval_first = TRUE;

    return ((gen_ll *) tmp);
}

/**************************************************************************
 *
 * Function Name : int check_for_if(ep, ll_type)
 * Parameters :
 * Return Value :
 *      int - indicates if the region is active or not
 * Purpose :
 *
 **************************************************************************/
check_for_if(ep, ll_type)
expr *ep;
int ll_type;
{
    loop_stack *tmp_stack;
    int region_active;

/*printf("call to check_for_if..., ll_type=%d\n",ll_type);*/
    /*
     * Determine if the current region is active or not... If we have
     * no defs on the stack, then we are obviously active.  If there is
     * a def on the stack, check to see if that def is active.  If it is,
     * then we too are active.
     */
    if (Head_Loop_Stack == NULL) region_active = TRUE;
    else region_active = Head_Loop_Stack->active;

    /*
     * First, create the stack template, and put it on the 
     * head of the stack.
     */
    if (Head_Loop_Stack == NULL) { /* nothing on the stack */
	tmp_stack = Head_Loop_Stack = MALLOC(loop_stack);
	tmp_stack->next = NULL;	
    } /* of if Head_Loop_Stack is NULL */
    else {
	tmp_stack = MALLOC(loop_stack);
	tmp_stack->next = Head_Loop_Stack;
	Head_Loop_Stack = tmp_stack;
    }

    tmp_stack->ll_type = ll_type;
    tmp_stack->mate = NULL;
    tmp_stack->ep = ep;

    tmp_stack->region_active = region_active;
    if (((ll_type == IFDEF_STRUCTURE) || (ll_type == IFNDEF_STRUCTURE)) && region_active) {
      tmp_stack->active = TRUE;
    }
    else if (region_active && ((int) evaluate_expression(ep))) {
      tmp_stack->active = TRUE;
    }
    else tmp_stack->active = FALSE;

/*printf("active=%d\n",tmp_stack->active);*/
    return(tmp_stack->active);
}

/**************************************************************************
 *
 * Function Name : int check_for_else()
 * Parameters :
 *      int - indicates if the region is now active
 * Return Value :
 * Purpose :
 *
 **************************************************************************/
check_for_else()
{
    if (Head_Loop_Stack == NULL) {
	fprintf(stderr,"WARNING : vpp : check_for_else() : got an unmatched `else!!!\n");
	return(TRUE);
    }
    else {
	if ((Head_Loop_Stack->ll_type == IFDEF_STRUCTURE) || 
	    (Head_Loop_Stack->ll_type == IFNDEF_STRUCTURE)) {
	    if (Head_Loop_Stack->region_active) {
	        if (Head_Loop_Stack->ll_type == IFNDEF_STRUCTURE) {
		    printf("`endif\n");
		    printf("`ifdef ");
		    traverse_expression(Head_Loop_Stack->ep);
		    printf("\n");
		}
		else printf("`else\n");
	    }
	    return (Head_Loop_Stack->region_active);
	}
	else if (Head_Loop_Stack->region_active) {
	    Head_Loop_Stack->active = !Head_Loop_Stack->active;
	    return(Head_Loop_Stack->active);
	}
	else return(FALSE);
    }
}

/**************************************************************************
 *
 * Function Name : int handle_endif()
 * Parameters :
 *      int - indicates if the region is now active
 * Return Value :
 * Purpose :
 *
 **************************************************************************/
handle_endif()
{
    loop_stack *tmp_head;

    tmp_head = Head_Loop_Stack;

    if (Head_Loop_Stack == NULL) {
	fprintf(stderr,"WARNING : vpp : handle_endif() : got an unmatched `endif!!!\n");
    }
    else {
	if (((Head_Loop_Stack->ll_type == IFDEF_STRUCTURE) || 
	     (Head_Loop_Stack->ll_type == IFNDEF_STRUCTURE)) &&
	    (Head_Loop_Stack->region_active)) {
	    printf("`endif\n");
	}
	Head_Loop_Stack = Head_Loop_Stack->next;
	free(tmp_head);
    }
    if (Head_Loop_Stack == NULL) return (1);
    else return (Head_Loop_Stack->active);
}

/**************************************************************************
 *
 * Function Name : 
 * Parameters :
 * Return Value :
 * Purpose :
 *
 **************************************************************************/
int is_an_integer(value)
double value;
{
    double tmp_val;
    double abs_value;

    if (value < 0.0) abs_value = value * -1.0;
    else abs_value = value;

    abs_value = rint(abs_value) - abs_value;
    if (abs_value < 0.0) abs_value = abs_value * -1.0;

    if (abs_value <= DELTA) {
	return (TRUE);
    }
    else {
	return (FALSE);
    }
}

/**************************************************************************
 *
 * Function Name : 
 * Parameters :
 * Return Value :
 * Purpose :
 *
 **************************************************************************/

