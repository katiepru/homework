/**********************************************
        CS415  Project 2
        Spring  2015
        Student Version
**********************************************/

#include "attr.h" 
#include <string.h>

char eq(depInfo *a, depInfo *b) {
    int s1, s2;
    s1 = s2 = -1;

    if(a->varname == 0) {
        if(b->varname == 0)
            s1 = 0;
        else
            s1 = 1;
    } else if(b->varname == 0)
        s1 = 1;

    if(a->indname == 0) {
        if(b->indname == 0)
            s2 = 0;
        else
            s2 = 1;
    } else if(b->indname == 0)
        s2 = 1;

    if(s1 == -1)
        s1 = strcmp(a->varname, b->varname);
    if(s2 == -1)
        s2 = strcmp(a->indname, b->indname);

    if(a->complete == b->complete && a->a == b->a && a->c == b->c && a->hasA == b->hasA && a->hasC == b->hasC
       && s1 == 0 && s2 == 0)
        return 0;
    return 1;
}
