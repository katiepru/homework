/**********************************************
        CS415  Project 3
        Spring  2015
        Student version
**********************************************/

#include <stdio.h>
#include <string.h>
#include "deptest.h" 


void emitFoundTrueDependenceWithDistance(char *arrayName, int distance) 
{
  sprintf(CommentBuffer, " <<< Found TRUE dependence on variable \"%s\" with distance %d >>>", 
	  arrayName, distance);
  emitComment(CommentBuffer);
  fprintf(depstats, "%s\n", CommentBuffer);  
}

void emitAssumeTrueDependence(char *arrayName)
{
  sprintf(CommentBuffer, " <<< Assume TRUE dependence on variable \"%s\" with unknown distance >>>", 
	  arrayName);
  emitComment(CommentBuffer);
  fprintf(depstats, "%s\n", CommentBuffer);  
}

void emitFoundOutputDependence(char *arrayName)
{
  sprintf(CommentBuffer, " <<< Found OUTPUT dependence on variable \"%s\" >>>", 
	  arrayName);
  emitComment(CommentBuffer);
  fprintf(depstats, "%s\n", CommentBuffer);  
}

void emitAssumeOutputDependence(char *arrayName)
{
  sprintf(CommentBuffer, " <<< Assume OUTPUT dependence on variable \"%s\" >>>", 
	  arrayName);
  emitComment(CommentBuffer);
  fprintf(depstats, "%s\n", CommentBuffer);  
}

void emitFoundNoDependenciesAndWillVectorize()
{
  sprintf(CommentBuffer, " <<< Found NO dependencies: VECTORIZE >>>");
  emitComment(CommentBuffer);
  fprintf(depstats, "%s\n\n", CommentBuffer);  
}

void emitFoundDependenciesAndWillNotVectorize()
{
  sprintf(CommentBuffer, " <<< Found at least one dependence: CANNOT VECTORIZE >>>");
  emitComment(CommentBuffer);
  fprintf(depstats, "%s\n\n", CommentBuffer);  
}


/* Checks if there is possible dependence preventing vectorization*/
char depTest(ctrldeps ind, depInfo lhs, depInfo rhs)
{

    printf("Ind.indname = %s\n", ind.indname);
    //Check that they use the same variable
    if((lhs.indname && strcmp(lhs.indname, ind.indname) != 0) ||
       (rhs.indname && strcmp(rhs.indname,ind.indname) != 0)) {
        printf("Returning here\n\n\n");
        return 1;
    }

    //Check if one or both sides are missing vars. Run ZIV Test
    if(lhs.indname == 0) {
        emitAssumeOutputDependence(lhs.varname);
        return 1;
    }

    //Run ZIV test
    if(rhs.indname == 0) {
        return zivTest(ind, lhs, rhs);
    }

    return sivTest(ind, lhs, rhs);
}


/*Runs the ZIV Test. Return 1 if dependence*/
char zivTest(ctrldeps ind, depInfo lhs, depInfo rhs) {

    int diff = rhs.c - lhs.c;

    if(diff >= ind.lb && diff <= ind.ub) {
        emitAssumeTrueDependence(lhs.varname);
        return 1;
    }
    return 0;


}


/*Runs the ZIV Test. Return 1 if dependence*/
char sivTest(ctrldeps ind, depInfo lhs, depInfo rhs) {
    int diff, res;
    double div;
    //Set up a if it doesn't exist. C defaulting to 0 is fine.
    if(lhs.hasA == 0)
        lhs.a = 1;
    if(rhs.hasA == 0)
        rhs.a = 1;

    if(lhs.a != rhs.a) {
        emitAssumeTrueDependence(lhs.varname);
        return 1;
    }

    diff = lhs.c - rhs.c;
    div = ((double) diff)/lhs.a;
    res = (int) div;

    if((double) res != div)
        return 0;

    if(res < 0)
        return 0;

    if(res > (ind.ub - ind.lb))
        return 0;

    printf("Varname is %s\n", lhs.varname);
    emitFoundTrueDependenceWithDistance(lhs.varname, res);
    return 1;


}
