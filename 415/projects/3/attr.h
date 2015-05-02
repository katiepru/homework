/**********************************************
        CS415  Project 2
        Spring  2015
        Student Version
**********************************************/

#ifndef ATTR_H
#define ATTR_H

typedef union {int num; char *str;} tokentype;

typedef enum type_expression {TYPE_INT=0, TYPE_BOOL, TYPE_ERROR} Type_Expression;

typedef struct {
    char complete;
    char *varname;
    int a;
    int c;
    char *indname;

    char hasA;
    char hasC;
} depInfo;

typedef struct {
    char *indname;
    int ub;
    int lb;
} ctrldeps;

typedef struct {
        Type_Expression type;
        int is_arr;
        int targetRegister;
        int offset;
        int label;
        int nextLabel;
        int foo;
        char *vars[50];
        depInfo deps[50];
        ctrldeps cdeps;
        int dnum;
        } regInfo;

typedef struct {
    char *vars[100];
    int numVars;
} varInfo;

typedef struct {
    Type_Expression type;
    int len;
} varTypeInfo;

// Error messages

#define FOR_UNDECLARED "\n*** ERROR ***: Induction variable %s not declared.\n"
#define FOR_NONINT "\n*** ERROR ***: Induction variable %s not a scalar of type integer.\n"
#define ASSIGNMENT_TYPES "\n*** ERROR ***: Assignment types do not match.\n"
#define UNDECLARED "\n*** ERROR ***: Variable %s not declared.\n"
#define NOT_SCALAR "\n*** ERROR ***: Variable %s is not a scalar variable.\n"
#define NOT_ARRAY "\n*** ERROR ***: Variable %s is not an array variable.\n"
#define ARR_IND "\n*** ERROR ***: Array variable %s index type must be integer.\n"
#define OP_INT "\n*** ERROR ***: Operand type must be integer.\n"
#define OP_BOOL "\n*** ERROR ***: Operand type must be boolean.\n"
#define EQ_DIFF "\n*** ERROR ***: == or != operator with different types.\n"
#define REL_ILL "\n*** ERROR ***: Relational operator with illegal type.\n"

#endif
