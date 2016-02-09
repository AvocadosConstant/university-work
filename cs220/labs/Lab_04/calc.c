#include<stdio.h>
#include<assert.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include "token.h"
#include "tree.h"

int main(int argv,char **argc) {

	char *dbgvptr;
	token * currentToken;
	char buffer[4096];
	size_t linesize=0;
	char *line=NULL;
	tree * t;

	bool debug=false;
	bool debug_tokens=false;
	bool debug_tree=false;

	buffer[0]='\0';
	printf("Enter expression to calculate:\n");

	while(-1!=getline(&line,&linesize,stdin)) {
		if (line[strlen(line)-1]=='\n') line[strlen(line)-1]=' ';
		if (line[strlen(line)-2]=='\r') line[strlen(line)-2]=' '; /* Special case for windows style \r\n newline */
		// printf("Read line: %s\n",line);
		strcat(buffer,line);
	}
	free(line);

	// printf("Buffer: %s\n",buffer);

	if (debug_tokens) {
		currentToken=NULL;
		for (currentToken=nextToken(NULL,buffer,true); currentToken!=NULL; currentToken=nextToken(currentToken,buffer,true)) {
			printToken(currentToken);
		}
	}


	currentToken=nextToken(NULL,buffer,false);
	t=getExpList(&currentToken,buffer);
	if (currentToken != NULL) {
		tokenError("Syntax error... data left after expression parsed",buffer,currentToken);
		buffer[getIndex(currentToken)]=0;
	}
	if (debug_tree) printTree(t,"Root");
	printf("%s = %ld\n",buffer,evalTree(t,debug));
	freeTree(t);

	return 0;

}




