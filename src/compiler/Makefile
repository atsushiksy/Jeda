
MK = gmake

CC = gcc -lang-c-c++-comments -g -DYYRESTART -DEVALEXP 

CMP_SRC = bit_operations.c parse_tree.c constant.c symtab.c jd_main.c \
           output.c codegen.c parse_init.c find_clock.c expression.c \
           headergen.c
CMP_OBJ = ${CMP_SRC:.c=.o}

INC_DIR = ../include
CMP_INC = ${INC_DIR}/error_msg.h   ${INC_DIR}/bit_operations.h \
          ${INC_DIR}/symtab.h      ${INC_DIR}/compiler_macro.h \
          ${INC_DIR}/constant.h    ${INC_DIR}/parse_tree.h \
          ${INC_DIR}/parse_types.h ${INC_DIR}/output_macro.h

LEX_PINC = jeda_operator_lex.h
LEX_INC =  y_tab.h \
           jeda_keyword_table.h 

LEX_SRC = lex.yy.c
LEX_OBJ = ${LEX_SRC:.c=.o}

YACC_PINC = jeda_operator_token.h \
            jeda_keyword_token.h \
            jeda_nonterminal_node_type.h 

YACC_SRC = y_tab.c
YACC_OBJ = ${YACC_SRC:.c=.o}
YACC_HDR = y_tab.h
YACC_DBGOBJ = y_tab_dbg.o

PARSE_SRC = vz_parse.c
PARSE_OBJ = ${PARSE_SRC:.c=.o}

#SPEC_DIR = ../../doc/spec
#SPEC_FILE = ${SPEC_DIR}/jeda_parser.spec
#SPEC_PREP = ${SPEC_DIR}/jeda_parser.prep

PERL_SCRIPTS = gen_opr_lex.pl gen_opr_token.pl gen_table.pl \
   gen_token.pl gen_node_type.pl gen_syntax.pl gen_parser.pl gen_action.pl \
   gen_spec.pl

DRVD_FILES = jeda_operator_lex.h jeda_operator_token.h \
             jeda_keyword_table.h jeda_keyword_token.h \
             jeda_nonterminal_node_type.h jeda.y
 
TARGET = jedacmp 
#TARGET = ${YACC_OBJ} ${SPEC_FILE} ${CMP_OBJ} ${LEX_OBJ}

all: ${TARGET}

jedacmp: ${YACC_OBJ} ${CMP_OBJ} ${LEX_OBJ}
	gcc -o $@ -dn ${YACC_OBJ} ${CMP_OBJ} ${LEX_OBJ} -lfl -lpthread -lnsl -lm -ll -lc -ltermcap  

dbgcmp: ${YACC_DBGOBJ} ${CMP_OBJ} ${LEX_OBJ}
	gcc -o $@ -dn ${YACC_DBGOBJ} ${CMP_OBJ} ${LEX_OBJ} -lfl -lpthread -lnsl -lm -ll -lc -ltermcap  

err: ${YACC_SRC} ${CMP_INC}
	${CC} -c ${YACC_SRC} &> $@

perr: ${PARSE_SRC} ${CMP_INC}
	${CC} -c ${PARSE_SRC} &> $@

${CMP_OBJ}: ${CMP_INC}

${YACC_DBGOBJ}: ${YACC_SRC} ${CMP_INC}
	${CC} -c -g -o $@ -DYYDEBUG=1 ${YACC_SRC}

${YACC_SRC}: jeda.y
	btyacc -d jeda.y
	cp $@ $@.t
	perl gen_jd_parse.pl < y_tab.c.t > y_tab.c

${YACC_HDR}: jeda.y
	btyacc -d jeda.y

${PARSE_SRC}: ${YACC_SRC}
	sed '/^# line/d' y_tab.c | sed '/^#line/d' > $@

# delived file generation
jeda_operator_lex.h: jeda_operators.data ${PERL_SCRIPTS}
	perl gen_opr_lex.pl < jeda_operators.data > $@

jeda_operator_token.h: jeda_operators.data ${PERL_SCRIPTS}
	perl gen_opr_token.pl < jeda_operators.data > $@

jeda_keyword_table.h: jeda_keyword.data ${PERL_SCRIPTS}
	perl gen_table.pl < jeda_keyword.data > $@

jeda_keyword_token.h: jeda_keyword.data ${PERL_SCRIPTS}
	perl gen_token.pl < jeda_keyword.data > $@

jeda_nonterminal_node_type.h: jeda_nonterminal_node_type.data \
                                      ${PERL_SCRIPTS}
	perl gen_node_type.pl < jeda_nonterminal_node_type.data > $@


jeda.y: jeda.y.pp jeda_keyword.data \
            jeda_operators.data \
            jeda_parser_action.data ${PERL_SCRIPTS} ${YACC_PINC}
	perl gen_parser.pl < jeda.y.pp | perl gen_action.pl > $@

#${SPEC_FILE}: ${SPEC_PREP} jeda.y.pp gen_spec.pl
#	perl gen_syntax.pl < jeda.y.pp
#	perl gen_spec.pl < ${SPEC_PREP} > $@


## lex generation

jeda.l: gen_lfile.pl jeda.l.pp ${LEX_PINC}
	perl gen_lfile.pl > $@


${LEX_SRC}: jeda.l 
	flex jeda.l
	cp $@ $@.t
	perl gen_jd_parse.pl < $@.t > $@

${LEX_OBJ}: ${LEX_INC} ${LEX_INC}

${VZLEX_OBJ}: ${LEX_INC}  ${LEX_INC}

lerr: ${LEX_SRC} ${LEX_INC} ${LEX_INC}
	${CC} -c ${LEX_SRC} &> $@


clean:
	${RM} ${DRVD_FILES}
	${RM} ${CMP_OBJ}
	${RM} ${TARGET}
