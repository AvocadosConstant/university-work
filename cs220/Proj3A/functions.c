#include "functions.h"
#include "function.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct functions_struct {
	int count; // Number of functions described
	int max; // Number of functions which can be included
	function * fnVecPtr; // Pointer to the first in a list of functions

	int visitedIndex;
	char visitedList[100][256];

};

functions functions_new() {
	functions this=malloc(sizeof(struct functions_struct));
	this->count=0;
	this->max=16;
	this->fnVecPtr=malloc(this->max * sizeof(function));
	if (this->fnVecPtr==NULL) return NULL;
	return this;
}

bool functions_delete(functions fl,bool deleteMembers) {
	int i;
	if (deleteMembers) {
		for(i=0; i<fl->count; i++) function_delete(*(fl->fnVecPtr+i));
	}
	free(fl->fnVecPtr);
	free(fl);
	return true;
}

bool functions_add(functions fl,function fn) {
	// Probably want to check to make sure the function is not already defined here
	if (fl->count == fl->max) {
		fl->max *=2;
		fl->fnVecPtr=realloc(fl->fnVecPtr,fl->max * sizeof(function));
		if (fl->fnVecPtr==NULL) {
			fl->max=0;
			fl->count=0;
			return false;
		}
	}
	*(fl->fnVecPtr + fl->count) = fn;
	fl->count++;
	return true;
}

// Other method definitions go here
function functions_find(functions fl,char * fnName)
{
	int i;
	function tf;
	//loops through all functions
	for(i=0; i<fl->count; i++) {
		tf=*(fl->fnVecPtr+i);
		if(0==strcmp(fnName, function_getName(tf)))	return tf;
	}
	return NULL;
}

//function functions_next(functions fl,function fn){}

int functions_getCount(functions fl) {return fl->count;}

function* functions_getFnVecPtr(functions fl) {return fl->fnVecPtr;}

void functions_addVisited(functions fl, char *name)
{
	strcpy(fl->visitedList[fl->visitedIndex], name);
	fl->visitedIndex++;
}

int functions_getVisitedIndex(functions fl) {return fl->visitedIndex;}

void functions_setVisitedIndex(functions fl, int index) {fl->visitedIndex = index;}

char* functions_getVisitedListItem(functions fl, int index) {return fl->visitedList[index];}

//for debugging purposes
void functions_printVisited(functions fl)
{
	printf(">>>visited:\n");

	int i;
	for(i=0; i < functions_getVisitedIndex(fl); i++) {
		printf("      %s : ",functions_getVisitedListItem(fl, i));
	}
}


//helper function
bool reachableByMain(functions fl, function fn) {
	functions_setVisitedIndex(fl, 0);
	return recur_rbm(fl, fn);
}

//recursive function
bool recur_rbm(functions fl, function fn) {

	if(strcmp(function_getName(fn), "main") == 0)
	{
		return true;
	}


	bool reachable = false;
	int i;
	for(i = 0; i < function_getCalledByIndex(fn); i++)
	{
		function found = functions_find(fl, function_getCalledByListItem(fn, i));
		if(found == NULL) continue;

		// Check if fn is in visited list
		int i;
		bool matched = false;
		// Visited List loop
		for(i = 0; i < functions_getVisitedIndex(fl); i++)
		{
			// If fn's name matches visitedListItem name
			if(0 == strcmp(function_getName(found), functions_getVisitedListItem(fl, i)))
			{
				matched = true;
			}
		}
		if(!matched) functions_addVisited(fl, function_getName(fn));
		else continue;

		if(strcmp(function_getName(found), "main") == 0)
		{
			return true;
		}

		if(recur_rbm(fl, found)) reachable = true;
	}

	if(reachable == true) return true;
	return false;
}

bool functions_report(functions fl,bool reachable) {
	int i,j;
	char prefix[16];
	printf("Function list has %d functions...\n",fl->count);
	function tf;

	//loops through all functions
	for(i=0,j=1; i<fl->count; i++) {
		tf=*(fl->fnVecPtr+i);

//		functions_printVisited(fl);
//		printf(">trying to print function %s\n",function_getName(tf));

		if(!reachableByMain(fl, tf)) continue;

		sprintf(prefix,"%3d. ",j++);
		function_report(tf,prefix);
	}
	return true;
}
