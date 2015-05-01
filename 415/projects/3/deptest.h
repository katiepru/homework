/**********************************************
        CS415  Project 3
        Spring  2015
        Student version
**********************************************/

#ifndef _DEPTEST_H
#define _DEPTEST_H

#include "attr.h"

extern
FILE *depstats;

extern 
char *CommentBuffer;

extern
void emitComment(char *comment);

extern
void emitFoundTrueDependenceWithDistance(char *arrayName, int distance);

extern
void emitAssumeTrueDependence(char *arrayName);

extern
void emitFoundOutputDependence(char *arrayName);

extern
void emitAssumeOutputDependence(char *arrayName);

extern
void emitFoundNoDependenciesAndWillVectorize();

extern
void emitFoundDependenciesAndWillNotVectorize();

extern
char depTest(ctrldeps ind, depInfo lhs, depInfo rhs);

extern
char zivTest(ctrldeps ind, depInfo lhs, depInfo rhs);

extern
char sivTest(ctrldeps ind, depInfo lhs, depInfo rhs);

#endif


  
