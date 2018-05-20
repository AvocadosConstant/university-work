#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <stdbool.h>
typedef struct functions_struct * functions;
#include "function.h"

/* --------------------------------------------------------------------------------------------
	C Object Oriented definitions for a function dictionary

	Methods:
		functions functions_new()
			Creates a new list of functions
		bool functions_delete(functions fl,bool deleteMembers)
			Deletes a list of functions. if deleteMembers, deletes member functions

		bool functions_add(functions fl,function fn)
			Adds function object to the list of functions
		...

		getter/setter methods
			int functions_getCount(functions fl)
			...

		bool functions_report(functions fl,bool reachable);
			Reports on the list of functions


---------------------------------------------------------------------------------------------*/

functions functions_new();
bool functions_delete(functions fl,bool deleteMembers);

bool functions_add(functions fl,function fn);
function functions_find(functions fl,char * fnName);
//function functions_next(functions fl,function fn);

int functions_getCount(functions fl);
function* functions_getFnVecPtr(functions fl);

void functions_addVisited(functions fl, char *name);
int functions_getVisitedIndex(functions fl);
void functions_setVisitedIndex(functions fl, int index);
char* functions_getVisitedListItem(functions fl, int index);
void functions_printVisited(functions fl);

bool reachableByMain(functions fl, function fn);
bool recur_rbm(functions fl, function fn);

bool functions_report(functions fl,bool reachable);

#endif

