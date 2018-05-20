#ifndef FUNCTION_H
#define FUNCTION_H
#include <stdbool.h>

typedef struct function_struct * function;
#include "functions.h"

/* --------------------------------------------------------------------------------------------
	C Object Oriented definitions for a function

	Methods:
		function function_new(char *name, void* fnstart)
			Creates a new function
		bool function_delete(function fn)
			Deletes a function

		getters/setters:
			char * function_getName(function fn)
			...

		...
		bool function_report(function fn,char *prefix)


---------------------------------------------------------------------------------------------*/


function function_new(char *name);
bool function_delete(function fn);
char * function_getName(function fn);

// Function declarations for other methods go here...
void function_addSize(function fn, int objBytes);
int function_getSize(function fn);

void function_setAddr(function fn, void *a);
void* function_getAddr(function fn);

void function_addCalls(function fn, char *name);
int function_getCallsIndex(function fn);
char* function_getCallsListItem(function fn, int index);

void function_addCalledBy(function fn, char *name);
int function_getCalledByIndex(function fn);
char* function_getCalledByListItem(function fn, int index);

bool function_report(function fn,char *prefix);

#endif
