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
        Type_Expression type;
        int targetRegister;
        int offset;
        int label;
        int nextLabel;
        char *vars[50];
        } regInfo;

typedef struct {
    char *vars[100];
    int numVars;
} varInfo;

typedef struct {
    Type_Expression type;
    int len;
} varTypeInfo;

#endif


  
