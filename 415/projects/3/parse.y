%{
#include <stdio.h>
#include "attr.h"
#include "instrutil.h"
#include "deptest.h"
int yylex();
void yyerror(char * s);
#include "symtab.h"

FILE *outfile;
FILE *depstats;
char *CommentBuffer;

%}

%union {
    tokentype token;
    regInfo targetReg;
    varInfo varList;
    varTypeInfo varType;
}

%token PROG PERIOD VAR
%token INT BOOL PRINT THEN IF FI ENDWHILE DO ENDFOR
%token ARRAY OF
%token BEG END ASG
%token EQ NEQ LT LEQ AND OR TRUE FALSE
%token ELSE
%token <token> FOR
%token <token> ID ICONST WHILE

%type <targetReg> exp
%type <targetReg> lhs
%type <targetReg> ifhead
%type <targetReg> condexp
%type <targetReg> stmt
%type <targetReg> ifstmt
%type <targetReg> astmt
%type <targetReg> cmpdstmt
%type <targetReg> stmtlist
%type <targetReg>  ctrlexp

%type <varList> idlist

%type <varType> type stype

%start program

%nonassoc EQ NEQ LT LEQ GT GEQ
%left '+' '-' AND
%left '*' OR

%nonassoc THEN
%nonassoc ELSE

%%
program : {emitComment("Assign STATIC_AREA_ADDRESS to register \"r0\"");
           emit(NOLABEL, LOADI, STATIC_AREA_ADDRESS, 0, EMPTY);}
           PROG ID ';' block PERIOD {}
	;

block	: variables cmpdstmt {}
	;

variables: /* empty */
	| VAR vardcls { }
	;

vardcls	: vardcls vardcl ';' { }
	| vardcl ';' { }
	| error ';' { yyerror("***Error: illegal variable declaration\n");}
	;

vardcl	: idlist ':' type { int i;
                            SymTabEntry *s;
                            for(i=0; i<$1.numVars; i++){
                                s = lookup($1.vars[i]);
                                s->type = $3.type;
                                s->size = $3.len;
                                if($3.len > -1)
                                    s->offset = NextOffset($3.len);
                                else
                                    s->offset = NextOffset(1);
                            }
                          }
	;

idlist	: idlist ',' ID {
                            int vars = $1.numVars;
                            int i;
                            $$.numVars = $1.numVars + 1;
                            if(lookup($3.str) != NULL)
                                printf("\n ***ERROR***: Variable %s already declared.\n", $3.str);
                            else {
                                insert($3.str, TYPE_INT, 1, 0);
                                for(i=0; i < vars; i++) {
                                    $$.vars[i] = $1.vars[i];
                                }
                                $$.vars[i] = $3.str;
                            }
                        }
        | ID		    {
                            $$.numVars = 1;
                            if(lookup($1.str) != NULL)
                                printf("***ERROR***: Variable %s already declared.\n", $1.str);
                            else {
                                insert($1.str, TYPE_INT, 1, 0);
                                $$.vars[0] = $1.str;
                            }
                        }
	;


type	: ARRAY '[' ICONST ']' OF stype { $$.len = $3.num;
                                          $$.type = $6.type
                                        }

        | stype { $$.len = -1; $$.type = $1.type;}
	;

stype	: INT {$$.type = TYPE_INT;}
        | BOOL {$$.type = TYPE_BOOL}
	;

stmtlist : stmtlist ';' stmt {}
	| stmt {}
    | error { yyerror("***Error: ';' expected or illegal statement \n");}
	;

stmt    : ifstmt {}
	| fstmt {}
	| wstmt {}
	| astmt {
                int i;
                for(i=0; i < $1.dnum; i++) {
                    $$.deps[i] = $1.deps[i];
                }
                $$.dnum = $1.dnum;
            }
	| writestmt {}
	| cmpdstmt {}
	;

cmpdstmt: BEG stmtlist END {}
	;

ifstmt :  ifhead 
          THEN stmt {
                        emit(NOLABEL, BR, $1.nextLabel, EMPTY, EMPTY);
                        emit($1.label, NOP, EMPTY, EMPTY, EMPTY);
                    }
  	  ELSE
          stmt
          FI    {
                    emit($1.nextLabel, NOP, EMPTY, EMPTY, EMPTY);
                }
	;

ifhead : IF condexp {
                        int trueL = NextLabel();
                        $$.label = NextLabel();
                        $$.nextLabel = NextLabel();

                        emit(NOLABEL, CBR, $2.targetRegister, trueL, $$.label);
                        emit(trueL, NOP, EMPTY, EMPTY, EMPTY);
                    }
        ;

writestmt: PRINT '(' exp ')' { int newOffset = NextOffset(1); /* call generates byte address */
                                 emitComment("Code for \"PRINT\"");
                                 emit(NOLABEL, STOREAI, $3.targetRegister, 0, newOffset);
                                //emit 1 instr, make sure label is gone
                                 emit(NOLABEL,
                                      OUTPUT,
                                      STATIC_AREA_ADDRESS + newOffset,
                                      EMPTY,
                                      EMPTY);
                             }
	;

fstmt	: FOR {
              } ctrlexp DO
          stmt {
                   int r1 = NextRegister();
                   int r2 = NextRegister();

                   emit(NOLABEL, LOADAI, 0, $3.offset, r1);
                   emit(NOLABEL, ADDI, r1, 1, r2);
                   emit(NOLABEL, STOREAI, r2, 0, $3.offset);
                   emit(NOLABEL, BR, $3.label, EMPTY, EMPTY);

                   char dep = depTest($3.cdeps, $5.deps[0], &$5.deps[1], $5.dnum - 1);
                   if($5.dnum == 0 || $5.deps[0].complete == -1) {
                      emitComment("Not an array assignment\n");
                      dep = 1;
                   }
                   if(dep == 0) {
                       emitFoundNoDependenciesAndWillVectorize();
                       emit($3.vLabel, VECTON, EMPTY, EMPTY, EMPTY);
                   } else {
                       emitFoundDependenciesAndWillNotVectorize();
                       emit($3.vLabel, NOP, EMPTY, EMPTY, EMPTY);
                   }
                   emit(NOLABEL, BR, $3.label, EMPTY, EMPTY);

          }
          ENDFOR {emit($3.nextLabel, VECTOFF, EMPTY, EMPTY, EMPTY);}
	;

wstmt	: WHILE  { $1.num = NextLabel();
                   emit($1.num, NOP, EMPTY, EMPTY, EMPTY);
                 }
          condexp { int whileLabel = NextLabel();
                    $3.nextLabel = NextLabel();
                    emit(NOLABEL, CBR, $3.targetRegister, whileLabel, $3.nextLabel);
                    emit(whileLabel, NOP, EMPTY, EMPTY, EMPTY);
                  }
          DO stmt  { emit(NOLABEL, BR, $1.num, EMPTY, EMPTY);
                     emit($3.nextLabel, NOP, EMPTY, EMPTY, EMPTY);}
          ENDWHILE
	;


astmt : lhs ASG exp {
                        int i, j;
 				        if (! ((($1.type == TYPE_INT) && ($3.type == TYPE_INT)) ||
				               (($1.type == TYPE_BOOL) && ($3.type == TYPE_BOOL)))) {
				            printf(ASSIGNMENT_TYPES);
				        }

                        if($1.is_arr == 1) {
                            printf(NOT_SCALAR, $1.vars[0]);
                        }

                        $$.deps[0] = $1.deps[0];
                        if($$.deps[0].complete == 1) {
                            j = 1;
                            for(i=1; i <= $3.dnum; i++) {
                                if(strcmp($3.deps[i-1].varname, $$.deps[0].varname) == 0) {
                                    $$.deps[j] = $3.deps[i-1];
                                    j++;
                                }
                            }
                            $$.dnum = j;
                        }

				        emit(NOLABEL, STORE, $3.targetRegister, $1.targetRegister, EMPTY);
                    }
	;

lhs	: ID			{ /* BOGUS  - needs to be fixed */
                        int newReg1 = NextRegister();
                        int newReg2 = NextRegister();

                        $$.targetRegister = newReg2;

                        $$.vars[0] = $1.str;
                        $$.deps[0].varname = $1.str;
                        $$.deps[0].scalar = 1;
                        $$.deps[0].complete = -1;


                        SymTabEntry *e = lookup($1.str);
                        $$.type = e->type;
                        $$.is_arr = e->size >= 0 ? 1 : 0;
				        emit(NOLABEL, LOADI, e->offset, newReg1, EMPTY);
				        emit(NOLABEL, ADD, 0, newReg1, newReg2);

                    }


    |  ID '[' exp ']' {
                        int newReg1 = NextRegister();
                        int newReg2 = NextRegister();
                        int newReg3 = NextRegister();
                        int newReg4 = NextRegister();
                        int newReg5 = NextRegister();

                        $$.deps[0].varname = $1.str;
                        $$.deps[0].a = $3.deps[0].a;
                        $$.deps[0].c = $3.deps[0].c;
                        $$.deps[0].hasA = $3.deps[0].hasA;
                        $$.deps[0].hasC = $3.deps[0].hasC;
                        $$.deps[0].indname = $3.deps[0].indname;
                        $$.deps[0].complete = 1;
                        $$.dnum = 1;

                        $$.targetRegister = newReg5;
                        $$.type = $3.type;
                        $$.vars[0] = $1.str;

                        SymTabEntry *e = lookup($1.str);
                        if(e == NULL) {
                            printf(UNDECLARED, $1.str);
                            return;
                        }
                        if(e->size <0) {
                            printf(NOT_ARRAY, $1.str);
                        }
                        if($3.type != TYPE_INT || $3.is_arr == 1) {
                            printf(ARR_IND);
                        }
                        emit(NOLABEL, LOADI, 4, newReg1, EMPTY);
                        emit(NOLABEL, MULT, $3.targetRegister, newReg1, newReg2);
                        emit(NOLABEL, ADDI, 0, e->offset, newReg3);
                        emit(NOLABEL, ADD, newReg3, newReg2, newReg4);
                        $$.targetRegister = newReg4;
                      }
    ;


exp	: exp '+' exp		{
                            int newReg = NextRegister();
                            int i, j, in;
                            if (! (($1.type == TYPE_INT) && ($3.type == TYPE_INT))) {
    				            printf(OP_INT);
                            }
                            $$.type = $1.type;
                            $$.is_arr = 0;

                            emit(NOLABEL,
                                 ADD,
                                 $1.targetRegister,
                                 $3.targetRegister,
                                 newReg);
                            $$.targetRegister = newReg;

                            for(i=0; i < $1.dnum; i++) {
                                in = 0;
                                for(j=0; j < $$.dnum; j++) {
                                    if(eq(&$1.deps[i], &$$.deps[j]) == 0) {
                                        in = 1;
                                        break;
                                    }
                                }
                               if(($1.deps[i].complete == 1 || $1.deps[i].complete == -1) && in == 0) {
                                   $$.deps[$$.dnum] = $1.deps[i];
                                   $$.dnum++;
                               }
                            }

                            for(i=0; i < $3.dnum; i++) {
                                in = 0;
                                for(j=0; j < $$.dnum; j++) {
                                    if(eq(&$3.deps[i], &$$.deps[j]) == 0) {
                                        in = 1;
                                        break;
                                    }
                                }
                               if(($3.deps[i].complete == 1 || $3.deps[i].complete == -1) && in == 0) {
                                   $$.deps[$$.dnum] = $3.deps[i];
                                   $$.dnum++;
                               }
                            }

                            if($1.deps[$1.dnum].complete != -1 && $3.deps[$3.dnum].complete != -1 && $1.deps[$1.dnum].hasA && $1.deps[$1.dnum].indname != 0 && $3.deps[$3.dnum].hasC) {
                                $$.deps[$$.dnum].a = $1.deps[$1.dnum].a;
                                $$.deps[$$.dnum].hasA = 1;
                                $$.deps[$$.dnum].indname = $1.deps[$1.dnum].indname;
                                $$.deps[$$.dnum].c = $3.deps[$3.dnum].c;
                                $$.deps[$$.dnum].hasC = 1;
                            } else if($1.deps[$1.dnum].complete != -1 && $3.deps[$3.dnum].complete != -1 && $1.deps[$1.dnum].indname != 0 && $3.deps[$3.dnum].hasC) {
                                $$.deps[$$.dnum].indname = $1.deps[$1.dnum].indname;
                                $$.deps[$$.dnum].c = $3.deps[$3.dnum].c;
                                $$.deps[$$.dnum].hasC = 1;
                                $$.deps[$$.dnum].hasA = 0;
                            }
                            else {
                                $$.deps[$$.dnum].complete = -1;
                            }
                        }

        | exp '-' exp		{ int newReg = NextRegister();
                              int i, j, in;
                              if (! (($1.type == TYPE_INT) && ($3.type == TYPE_INT))) {
    				              printf(OP_INT);
                              }
                              $$.type = $1.type;
                              $$.is_arr = 0;

                            for(i=0; i < $1.dnum; i++) {
                                in = 0;
                                for(j=0; j < $$.dnum; j++) {
                                    if(eq(&$1.deps[i], &$$.deps[j]) == 0) {
                                        in = 1;
                                        break;
                                    }
                                }
                               if(($1.deps[i].complete == 1 || $1.deps[i].complete == -1)  && in == 0) {
                                   $$.deps[$$.dnum] = $1.deps[i];
                                   $$.dnum++;
                               }
                            }

                            for(i=0; i < $3.dnum; i++) {
                                in = 0;
                                for(j=0; j < $$.dnum; j++) {
                                    if(eq(&$3.deps[i], &$$.deps[j]) == 0) {
                                        in = 1;
                                        break;
                                    }
                                }
                               if(($3.deps[i].complete == 1 || $3.deps[i].complete == -1) && in == 0) {
                                   $$.deps[$$.dnum] = $3.deps[i];
                                   $$.dnum++;
                               }
                            }

                            if($1.deps[$1.dnum].complete != -1 && $3.deps[$3.dnum].complete != -1 && $1.deps[$1.dnum].hasA && $1.deps[$1.dnum].indname != 0 && $3.deps[$3.dnum].hasC) {
                                $$.deps[$$.dnum].a = $1.deps[$1.dnum].a;
                                $$.deps[$$.dnum].hasA = 1;
                                $$.deps[$$.dnum].indname = $1.deps[$1.dnum].indname;
                                $$.deps[$$.dnum].c = -1*$3.deps[$3.dnum].c;
                                $$.deps[$$.dnum].hasC = 1;
                            } else if($1.deps[$1.dnum].complete != -1 && $3.deps[$3.dnum].complete != -1 && $1.deps[$1.dnum].indname != 0 && $3.deps[$3.dnum].hasC) {
                                $$.deps[$$.dnum].indname = $1.deps[$1.dnum].indname;
                                $$.deps[$$.dnum].c = -1*$3.deps[$3.dnum].c;
                                $$.deps[$$.dnum].hasC = 1;
                                $$.deps[$$.dnum].hasA = 0;
                            }
                            else {
                                $$.deps[$$.dnum].complete = -1;
                            }

                              emit(NOLABEL, SUB , $1.targetRegister, $3.targetRegister, newReg);
                              $$.targetRegister = newReg;
                            }

        | exp '*' exp		{ int newReg = NextRegister();
                              int i, j, in;
                              if (! (($1.type == TYPE_INT) && ($3.type == TYPE_INT))) {
    				              printf(OP_INT);
                              }
                              $$.is_arr = 0;
                              $$.type = $1.type;
                              emit(NOLABEL, MULT, $1.targetRegister, $3.targetRegister, newReg);
                              $$.targetRegister = newReg;

                            for(i=0; i < $1.dnum; i++) {
                                in = 0;
                                for(j=0; j < $$.dnum; j++) {
                                    if(eq(&$1.deps[i], &$$.deps[j]) == 0) {
                                        in = 1;
                                        break;
                                    }
                                }
                               if(($1.deps[i].complete == 1 || $1.deps[i].complete == -1) && in == 0) {
                                   $$.deps[$$.dnum] = $1.deps[i];
                                   $$.dnum++;
                               }
                            }

                            for(i=0; i < $3.dnum; i++) {
                                in = 0;
                                for(j=0; j < $$.dnum; j++) {
                                    if(eq(&$3.deps[i], &$$.deps[j]) == 0) {
                                        in = 1;
                                        break;
                                    }
                                }
                               if(($3.deps[i].complete == 1 || $3.deps[i].complete == -1) && in == 0) {
                                   $$.deps[$$.dnum] = $3.deps[i];
                                   $$.dnum++;
                               }
                            }

                              if($1.deps[$1.dnum].complete != -1 && $3.deps[$3.dnum].complete != -1 && $1.deps[$1.dnum].hasC && $3.deps[$3.dnum].indname != 0) {
                                  $$.deps[$$.dnum].a = $1.deps[$1.dnum].c;
                                  $$.deps[$$.dnum].indname = $3.deps[$3.dnum].indname;
                                  $$.deps[$$.dnum].hasA = 1;
                                  $$.deps[$$.dnum].hasC = 0;
                              } else {
                                  $$.deps[$$.dnum].complete = -1;
                              }
                            }

        | exp AND exp		{ int newReg = NextRegister();
                              if (! (($1.type == TYPE_BOOL) && ($3.type == TYPE_BOOL))) {
    				              printf(OP_BOOL);
                              }
                              $$.is_arr = 0;
                              $$.type = $1.type;
                              emit(NOLABEL, AND_INSTR, $1.targetRegister, $3.targetRegister, newReg);
                              $$.targetRegister = newReg;
                            }


        | exp OR exp       	{ int newReg = NextRegister();
                              if (! (($1.type == TYPE_BOOL) && ($3.type == TYPE_BOOL))) {
    				              printf(OP_BOOL);
                              }
                              $$.is_arr = 0;
                              $$.type = $1.type;
                              emit(NOLABEL, OR_INSTR, $1.targetRegister, $3.targetRegister, newReg);
                              $$.targetRegister = newReg;
                            }


        | ID			{ /* BOGUS  - needs to be fixed */
	                        int newReg = NextRegister();
                            SymTabEntry *id = lookup($1.str);
                            if(id == NULL) {
                                printf(UNDECLARED, $1.str);
                                return;
                            }

	                        $$.targetRegister = newReg;
				            $$.type = id->type;
                            $$.is_arr = id->size >=0 ? 1 : 0;
				            emit(NOLABEL, LOADAI, 0, id->offset, newReg);

                            $$.deps[$$.dnum].indname = $1.str;
	                    }

        | ID '[' exp ']'	{
                                int newReg1 = NextRegister();
                                int newReg2 = NextRegister();
                                int newReg3 = NextRegister();
                                int newReg4 = NextRegister();
                                SymTabEntry *e = lookup($1.str);
                                if(e == NULL) {
                                    printf(UNDECLARED, $1.str);
                                    return;
                                }

                                if($3.type != TYPE_INT || $3.is_arr) {
                                    printf(ARR_IND, $1.str);
                                }
                                if(e->size < 0) {
                                    printf(NOT_ARRAY, $1.str);
                                }
                                $$.type = e->type;
                                $$.is_arr = 0;
                                emit(NOLABEL, LOADI, 4, newReg1, EMPTY);
                                emit(NOLABEL, MULT, $3.targetRegister, newReg1, newReg2);
                                emit(NOLABEL, ADDI, 0, e->offset, newReg3);
                                emit(NOLABEL, LOADAO, newReg3, newReg2, newReg4);
                                $$.targetRegister = newReg4;

                                if($3.deps[0].complete == -1 || $3.dnum > 0) {
                                    $$.deps[$$.dnum].complete = -1;
                                    $$.deps[$$.dnum].varname = $1.str;
                                    $$.dnum++;
                                } else
                                if((!($3.deps[0].hasA && $3.deps[0].indname == 0)) && ($3.deps[0].hasA || $3.deps[0].hasC || $3.deps[0].indname != 0)) {
                                    $$.deps[$$.dnum] = $3.deps[0];
                                    $$.deps[$$.dnum].varname = $1.str;
                                    $$.deps[$$.dnum].complete = 1;
                                    $$.dnum++;
                                } else {
                                    $$.deps[$$.dnum].complete = -1;
                                    $$.deps[$$.dnum].varname = $1.str;
                                    $$.dnum++;
                                }

                            }
 


	| ICONST                {
                                int newReg = NextRegister();
	                            $$.targetRegister = newReg;
				                $$.type = TYPE_INT;
                                $$.is_arr = 0;
				                emit(NOLABEL, LOADI, $1.num, newReg, EMPTY);

                                $$.deps[$$.dnum].c = $1.num;
                                $$.deps[$$.dnum].hasC = 1;


                            }

        | TRUE              {
                                int newReg = NextRegister(); /* TRUE is encoded as value '1' */
	                            $$.targetRegister = newReg;
				                $$.type = TYPE_BOOL;
                                $$.is_arr = 0;
				                emit(NOLABEL, LOADI, 1, newReg, EMPTY);
                            }

        | FALSE             {
                                int newReg = NextRegister(); /* TRUE is encoded as value '0' */
	                            $$.targetRegister = newReg;
				                $$.type = TYPE_BOOL;
                                $$.is_arr = 0;
				                emit(NOLABEL, LOADI, 0, newReg, EMPTY);
                            }

	| error { yyerror("***Error: illegal expression\n");}
	;


ctrlexp	: ID ASG ICONST ',' ICONST
                        {
                            $$.label = NextLabel();
                            int forLabel = NextLabel();
                            int addr1 = NextRegister();
                            int addr2 = NextRegister();
                            int min = NextRegister();
                            int max = NextRegister();

                            $$.cdeps.ub = $5.num;
                            $$.cdeps.lb = $3.num;
                            $$.cdeps.indname = $1.str;

                            emitComment("Initialize ind. variable");
                            SymTabEntry *s = lookup($1.str);
                            if(s == NULL) {
                                printf(FOR_UNDECLARED, $1.str);
                                return;
                            }
                            if(s->type != TYPE_INT || s->size >= 0) {
                                printf(FOR_NONINT, $1.str);
                            }
                            $$.offset = s->offset;
                            $$.vLabel = NextLabel();
                            emit(NOLABEL, LOADI, s->offset, addr1, EMPTY);
                            emit(NOLABEL, ADD, 0, addr1, addr2);
                            emit(NOLABEL, LOADI, $3.num, min, EMPTY);
                            emit(NOLABEL, LOADI, $5.num, max, EMPTY);
                            emit(NOLABEL, STORE, min, addr2, EMPTY);
                            emit(NOLABEL, BR, $$.vLabel, EMPTY, EMPTY);

                            emitComment("Generate control code for \"FOR\"");
                            int loadVal = NextRegister();
                            int compRes = NextRegister();
                            emit($$.label, LOADAI, 0, s->offset, loadVal);
                            emit(NOLABEL, CMPLE, loadVal, max, compRes);
                            $$.nextLabel = NextLabel();
                            emit(NOLABEL, CBR, compRes, forLabel, $$.nextLabel);
                            emit(forLabel, NOP, EMPTY, EMPTY, EMPTY);

                        }
        ;


condexp	: exp NEQ exp		{
                                int newReg = NextRegister();
                                if($1.type != $3.type) {
                                    printf(EQ_DIFF);
                                }
                                $$.type = $1.type;
                                $$.is_arr = 0;
                                $$.targetRegister = newReg;
                                emit(NOLABEL, CMPNE, $1.targetRegister, $3.targetRegister, newReg);
                            }

        | exp EQ exp		{
                                int newReg = NextRegister();
                                if($1.type != $3.type || $1.is_arr == 1 || $1.is_arr == 1) {
                                    printf(EQ_DIFF);
                                }
                                $$.type = $1.type;
                                $$.is_arr = 0;
                                $$.targetRegister = newReg;
                                emit(NOLABEL, CMPEQ, $1.targetRegister, $3.targetRegister, newReg);
                            }

        | exp LT exp		{
                                int newReg = NextRegister();
                                if($1.type != TYPE_INT || $3.type != TYPE_INT || $1.is_arr || $3.is_arr) {
                                    printf(REL_ILL);
                                }
                                $$.type = TYPE_BOOL;
                                $$.is_arr = 0;
                                $$.targetRegister = newReg;
                                emit(NOLABEL, CMPLT, $1.targetRegister, $3.targetRegister, newReg);
                            }

        | exp LEQ exp		{
                                int newReg = NextRegister();
                                if($1.type != TYPE_INT || $3.type != TYPE_INT || $1.is_arr || $3.is_arr) {
                                    printf(REL_ILL);
                                }
                                $$.type = TYPE_BOOL;
                                $$.is_arr = 0;
                                $$.targetRegister = newReg;
                                emit(NOLABEL, CMPLE, $1.targetRegister, $3.targetRegister, newReg);
                            }

	| exp GT exp		    {
                                int newReg = NextRegister();
                                if($1.type != TYPE_INT || $3.type != TYPE_INT || $1.is_arr || $3.is_arr) {
                                    printf(REL_ILL);
                                }
                                $$.type = TYPE_BOOL;
                                $$.is_arr = 0;
                                $$.targetRegister = newReg;
                                emit(NOLABEL, CMPGT, $1.targetRegister, $3.targetRegister, newReg);
                            }

	| exp GEQ exp		    {
                                int newReg = NextRegister();
                                if($1.type != TYPE_INT || $3.type != TYPE_INT || $1.is_arr || $3.is_arr) {
                                    printf(REL_ILL);
                                }
                                $$.type = TYPE_BOOL;
                                $$.is_arr = 0;
                                $$.targetRegister = newReg;
                                emit(NOLABEL, CMPGE, $1.targetRegister, $3.targetRegister, newReg);
                            }

	| error { yyerror("***Error: illegal conditional expression\n");}
        ;

%%

void yyerror(char* s) {
        fprintf(stderr,"%s\n",s);
        }

int optimize_flag = 0;


int
main(int argc, char* argv[]) {

  printf("\n     CS415 Spring 2015 Compiler\n\n");

  outfile = fopen("iloc.out", "w");
  if (outfile == NULL) {
    printf("ERROR: cannot open output file \"iloc.out\".\n");
    return -1;
  }

  depstats = fopen("deps.out", "w");
  if (outfile == NULL) {
    printf("ERROR: cannot open output file \"deps.out\".\n");
    return -1;
  }

  CommentBuffer = (char *) malloc(500);
  InitSymbolTable();

  printf("1\t");
  yyparse();
  printf("\n");

  PrintSymbolTable();

  fclose(outfile);

  return 1;
}
