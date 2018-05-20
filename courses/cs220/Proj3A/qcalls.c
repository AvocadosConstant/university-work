#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "functions.h"
#include "function.h"

bool readObject(char * objFile,functions fnList);

int main(int argc, char **argv) {

	setbuf(stdout,0);
	setbuf(stderr,0);

	if (argc!=2) {
		printf("Invocation: %s <executable_file>\n",argv[0]);
		exit(0);
	}
	functions fnList = functions_new();
	readObject(argv[1],fnList);
	// May need to do something here to find out if a function is reachable from main

	functions_report(fnList,true);
	functions_delete(fnList,true);
	exit(1);
}

bool readObject(char * objFile,functions fnList) {

	char pcmd[1024] = "objdump -d ";
	char dumpLine[1024];
	strcat(pcmd,objFile);

	FILE *pipe;
	pipe=popen(pcmd,"r");
	if (!pipe) {
		perror("Unable to open objdump command");
		return false;
	}

	bool inFunction=false;
	void *addr;
	void *fnStart;
	char fnName[1024];
	function current;

	while(fgets(dumpLine,sizeof(dumpLine),pipe)) {

		// D: 0000000100401000 <WinMainCRTStartup>:
		if (2==sscanf(dumpLine,"%p <%[^>]>:\n",&addr,fnName)) {
			inFunction=true;
			fnStart=addr;
/* DBG printf("In function: %s which starts at %p\n",fnName,fnStart); */
			current = function_new(fnName);
			functions_add(fnList,current);
			//sets addr
			function_setAddr(current, fnStart);
			continue;
		}
		if (0==strcmp(dumpLine,"\n")) {
			inFunction=false;
			continue;
		}
		if (inFunction) {
			unsigned char opCode;
			char mnemonic[20]="???";
			char parms[100]="";
			//int offset;
			int nr;
			int objBytes=0;
			char *rest;
			if (1==sscanf(dumpLine," %p:\t%n",&addr,&nr)) {
				rest=dumpLine+nr;
				char objText[64];
				if (1==sscanf(rest,"%[^\t]\t%n",objText,&nr)) {
					int l=strlen(objText)-1;
					while (l>=0 && objText[l]==' ') { // Strip trailing blanks
						objText[l]='\0';
						l--;
					}
					objBytes = (strlen(objText)+1)/3;
					sscanf(objText,"%02hhx",&opCode);
					rest=rest+nr;
					sscanf(rest,"%s %[^\n]\n",mnemonic,parms);
				}
			}

			function_addSize(current, objBytes);


			//offset=addr - fnStart;
/* DBG printf ("<%s+%03x> %s %s\n",fnName,offset,mnemonic,parms); */
			// May want to set other attributes of a function here
			if (0==strncmp(mnemonic,"call",4)) {
				char calledFunc[256]="";
				char * parmPtr=parms;
				void * calledAddr;
				if (1==sscanf(parmPtr,"*%[^#]#%n",calledFunc,&nr)) {
					parmPtr +=nr;
					calledFunc[0]='\0';
				}
				if (2==sscanf(parmPtr,"%p <%[^>]>",&calledAddr,calledFunc)) {
/* DBG  printf("Calling %s @ %p\n",calledFunc,calledAddr); */

					function_addCalls(current, calledFunc);

				} else {
/* DBG printf("Not sure what is being called in %s\n",parms); */
				}
			}
		}
	}

	int i;
	function func;
	//loop through fnList
	for(i=0; i < functions_getCount(fnList); i++) {
		func=*(functions_getFnVecPtr(fnList)+i);
		int j;
		for(j = 0; j < function_getCallsIndex(func); j++)
		{
			char* call = function_getCallsListItem(func, j);
			function found = functions_find(fnList, call);

			function_addCalledBy(found, function_getName(func));
		}
	}

	pclose(pipe);
	return true;
}
