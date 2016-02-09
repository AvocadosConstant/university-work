#include "function.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct function_struct {
	char *name;
	int size;
	void *addr;

	int callsIndex;
	char callsList[100][256];

	int calledByIndex;
	char calledByList[100][256];
};

function function_new(char *name) {
	function this = malloc(sizeof(struct function_struct));
	this->name=strdup(name);
	this->callsIndex = 0;
	this->calledByIndex = 0;

/* DBG	printf("Created function object for function %s\n",name); */
	return this;
}

bool function_delete(function fn) {
	free(fn->name);
	//Free other alocated sub-fields here
	free(fn);
	return true;
}

char * function_getName(function fn) {return fn->name;}


// Other function methods go here
void function_addSize(function fn, int objBytes) {fn->size += objBytes;}

int function_getSize(function fn) {return fn->size;}

void function_setAddr(function fn, void *a) {fn->addr = a;}

void* function_getAddr(function fn) {return fn->addr;}

void function_addCalls(function fn, char *name)
{
	strcpy(fn->callsList[fn->callsIndex], name);
	fn->callsIndex++;
}

int function_getCallsIndex(function fn) {return fn->callsIndex;}

char* function_getCallsListItem(function fn, int index) {return fn->callsList[index];}

void function_addCalledBy(function fn, char *name)
{
	strcpy(fn->calledByList[fn->calledByIndex], name);
	fn->calledByIndex++;
}

int function_getCalledByIndex(function fn) {return fn->calledByIndex;}

char* function_getCalledByListItem(function fn, int index){return fn->calledByList[index];}

bool function_report(function fn,char *prefix) {
	printf("%sFunction @ %p size= %3d %s\n",prefix, fn->addr, fn->size, fn->name);
	// expand this function report method to print interesting things about this function
	if(fn->callsIndex > 0)
	{
		char output[1000];
		strcpy(output, "        Called Functions   : ");
		int i;
		for(i = 0; i < fn->callsIndex; i++)
		{
			strcat(output, fn->callsList[i]);
			if(i != fn->callsIndex - 1) strcat(output, ", ");
		}
		printf("%s\n",output);
	}

	if(fn->calledByIndex > 0)
	{
		char output[1000];
		strcpy(output, "        Called By Functions: ");
		int i;
		for(i = 0; i < fn->calledByIndex; i++)
		{
			strcat(output, fn->calledByList[i]);
			if(i != fn->calledByIndex - 1) strcat(output, ", ");
		}
		printf("%s\n",output);
	}

	return true;
}
