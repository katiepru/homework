%{
#include <stdio.h>
#include "attr.h"
#include "instrutil.h"
int yylex();
void yyerror(char * s);
#include "symtab.h"

FILE *outfile;
char *CommentBuffer;

%}

%union {tokentype token;
        regInfo targetReg;
       }

%token PROG PERIOD VAR
%token INT BOOL PRINT THEN IF FI ENDWHILE DO ENDFOR
%token ARRAY OF
%token BEG END ASG
%token EQ NEQ LT LEQ AND OR TRUE FALSE
%token ELSE
%token WHILE FOR
%token <token> ID ICONST

%type <targetReg> exp
%type <targetReg> lhs ifhead condexp

%type <targetReg> stmt stmtlist
%type <targetReg> ifstmt fstmt wstmt astmt writestmt cmpdstmt ctrlexp

%start program

%nonassoc EQ NEQ LT LEQ GT GEQ
%left '+' '-' AND
%left '*' OR

%nonassoc THEN
%nonassoc ELSE

%%
program : {emitComment("Assign STATIC_AREA_ADDRESS to register \"r0\"");
           emit(NOLABEL, LOADI, STATIC_AREA_ADDRESS, 0, EMPTY);}
           PROG ID ';' block PERIOD { }
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

vardcl	: idlist ':' type {  }
	;

idlist	: idlist ',' ID {
                            if(lookup($3.str) != NULL)
                                printf("Variable %s already declared.\n", $3.str);
                            else
                                insert($3.str, TYPE_INT, 1, NextOffset(1));
                        }
        | ID		    {
                            if(lookup($1.str) != NULL)
                                printf("Variable %s already declared.\n", $1.str);
                            else
                                insert($1.str, TYPE_INT, 1, NextOffset(1));
                        }
	;


type	: ARRAY '[' ICONST ']' OF stype {  }

        | stype {  }
	;

stype	: INT {  }
        | BOOL {  }
	;

stmtlist : stmtlist ';' stmt {}
	| stmt { }
    | error { yyerror("***Error: ';' expected or illegal statement \n");}
	;

stmt    : ifstmt {}
	| fstmt {}
	| wstmt {}
	| astmt {}
	| writestmt {}
	| cmpdstmt {}
	;

cmpdstmt: BEG stmtlist END {}
	;

ifstmt :  ifhead
          THEN stmt
  	  ELSE
          stmt
          FI    {
                    int labelT = NextLabel();
                    int labelF = NextLabel();
                    emit(NOLABEL, CBR, $1.targetRegister, labelT, labelF);
                    //emit 1 instr, make sure label is gone
                }
	;

ifhead : IF condexp {
                        $$.targetRegister = $2.targetRegister;
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

fstmt	: FOR ctrlexp DO
          stmt {
                   int r1 = NextRegister();
                   int r2 = NextRegister();
                   emit(NOLABEL, LOADAI, 0, $2.offset, r1);
                   emit(NOLABEL, ADDI, r1, 1, r2);
                   emit(NOLABEL, STOREAI, r2, 0, $2.offset);
                   emit(NOLABEL, BR, $2.label, EMPTY, EMPTY);
               }
          ENDFOR {emit($2.nextLabel, NOP, EMPTY, EMPTY, EMPTY);}
	;

wstmt	: WHILE  {  }
          condexp {  }
          DO stmt  {  }
          ENDWHILE
	;


astmt : lhs ASG exp {
 				        if (! ((($1.type == TYPE_INT) && ($3.type == TYPE_INT)) ||
				               (($1.type == TYPE_BOOL) && ($3.type == TYPE_BOOL)))) {
				            printf("*** ERROR ***: Assignment types do not match.\n");
				        }


				        emit(NOLABEL, STORE, $3.targetRegister, $1.targetRegister, EMPTY);
                    }
	;

lhs	: ID			{ /* BOGUS  - needs to be fixed */
                        int newReg1 = NextRegister();
                        int newReg2 = NextRegister();

                        $$.targetRegister = newReg2;
                        $$.type = TYPE_INT;

                        SymTabEntry *e = lookup($1.str);
				        emit(NOLABEL, LOADI, e->offset, newReg1, EMPTY);
				        emit(NOLABEL, ADD, 0, newReg1, newReg2);

                    }


    |  ID '[' exp ']' { }
    ;


exp	: exp '+' exp		{
                            int newReg = NextRegister();
                            //if (! (($1.type == TYPE_INT) && ($3.type == TYPE_INT))) {
    				        //    printf("*** ERROR ***: Operator types must be integer.\n");
                            //}
                            //$$.type = $1.type;

                            emit(NOLABEL,
                                 ADD,
                                 $1.targetRegister,
                                 $3.targetRegister,
                                 newReg);
                            $$.targetRegister = newReg;
                        }

        | exp '-' exp		{ int newReg = NextRegister();
                              $$.type = $1.type;
                              emit(NOLABEL, SUB , $1.targetRegister, $3.targetRegister, newReg);
                              $$.targetRegister = newReg;
                            }

        | exp '*' exp		{ int newReg = NextRegister();
                              $$.type = $1.type;
                              emit(NOLABEL, MULT, $1.targetRegister, $3.targetRegister, newReg);
                              $$.targetRegister = newReg;
                            }

        | exp AND exp		{ int newReg = NextRegister();
                              $$.type = $1.type;
                              emit(NOLABEL, AND_INSTR, $1.targetRegister, $3.targetRegister, newReg);
                              $$.targetRegister = newReg;
                            }


        | exp OR exp       	{ int newReg = NextRegister();
                              $$.type = $1.type;
                              emit(NOLABEL, OR_INSTR, $1.targetRegister, $3.targetRegister, newReg);
                              $$.targetRegister = newReg;
                            }


        | ID			{ /* BOGUS  - needs to be fixed */
	                        int newReg = NextRegister();
                            SymTabEntry *id = lookup($1.str);
                            if(id == NULL)
                                printf("Variable %s not declared.\n", $1.str);

	                        $$.targetRegister = newReg;
				            $$.type = TYPE_INT;
				            emit(NOLABEL, LOADAI, 0, id->offset, newReg);
	                    }

        | ID '[' exp ']'	{   }
 


	| ICONST                {
                                int newReg = NextRegister();
	                            $$.targetRegister = newReg;
				                $$.type = TYPE_INT;
				                emit(NOLABEL, LOADI, $1.num, newReg, EMPTY);
                            }

        | TRUE              {
                                int newReg = NextRegister(); /* TRUE is encoded as value '1' */
	                            $$.targetRegister = newReg;
				                $$.type = TYPE_BOOL;
				                emit(NOLABEL, LOADI, 1, newReg, EMPTY);
                            }

        | FALSE             {
                                int newReg = NextRegister(); /* TRUE is encoded as value '0' */
	                            $$.targetRegister = newReg;
				                $$.type = TYPE_BOOL;
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

                            emitComment("Initialize ind. variable");
                            SymTabEntry *s = lookup($1.str);
                            if(s == NULL)
                                printf("\n*** ERROR ***: Variable %s not declared.\n", $1.str);
                            $$.offset = s->offset;
                            emit(NOLABEL, LOADI, s->offset, addr1, EMPTY);
                            emit(NOLABEL, ADD, 0, addr1, addr2);
                            emit(NOLABEL, LOADI, $3.num, min, EMPTY);
                            emit(NOLABEL, LOADI, $5.num, max, EMPTY);
                            emit(NOLABEL, STORE, min, addr2, EMPTY);

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
                                if($1.type != $3.type)
                                    printf("\n*** ERROR ***: == or != operator with different types.\n");
                                $$.type = $1.type;
                                $$.targetRegister = newReg;
                                emit(NOLABEL, CMPNE, $1.targetRegister, $3.targetRegister, newReg);
                            }

        | exp EQ exp		{
                                int newReg = NextRegister();
                                if($1.type != $3.type)
                                    printf("\n*** ERROR ***: == or != operator with different types.\n");
                                $$.type = $1.type;
                                $$.targetRegister = newReg;
                                emit(NOLABEL, CMPEQ, $1.targetRegister, $3.targetRegister, newReg);
                            }

        | exp LT exp		{
                                int newReg = NextRegister();
                                if($1.type != TYPE_INT || $3.type != TYPE_INT)
                                    printf("\n*** ERROR ***: Operand type must be integer.\n");
                                $$.type = $1.type;
                                $$.targetRegister = newReg;
                                emit(NOLABEL, CMPLT, $1.targetRegister, $3.targetRegister, newReg);
                            }

        | exp LEQ exp		{
                                int newReg = NextRegister();
                                if($1.type != TYPE_INT || $3.type != TYPE_INT)
                                    printf("\n*** ERROR ***: Operand type must be integer.\n");
                                $$.type = $1.type;
                                $$.targetRegister = newReg;
                                emit(NOLABEL, CMPLE, $1.targetRegister, $3.targetRegister, newReg);
                            }

	| exp GT exp		    {
                                int newReg = NextRegister();
                                if($1.type != TYPE_INT || $3.type != TYPE_INT)
                                    printf("\n*** ERROR ***: Operand type must be integer.\n");
                                $$.type = $1.type;
                                $$.targetRegister = newReg;
                                emit(NOLABEL, CMPGT, $1.targetRegister, $3.targetRegister, newReg);
                            }

	| exp GEQ exp		    {
                                int newReg = NextRegister();
                                if($1.type != TYPE_INT || $3.type != TYPE_INT)
                                    printf("\n*** ERROR ***: Operand type must be integer.\n");
                                $$.type = $1.type;
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

  CommentBuffer = (char *) malloc(500);
  InitSymbolTable();

  printf("1\t");
  yyparse();
  printf("\n");

  PrintSymbolTable();

  fclose(outfile);

  return 1;
}
