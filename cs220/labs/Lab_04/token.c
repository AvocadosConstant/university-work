#include "token.h"

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <assert.h>

struct tokenStruct {
	enum typeEnum type;
	int index;
	int length;
	long int value;
	char *name;
};

char * spaces(int n);

struct {
	char * vname;
	long int vvalue;
	bool valset;
} varSpace[50];

void setVar(char * name,long int val);
long int getVar(char *name);

token * newToken() {
	token * result=(token *)malloc(sizeof(token));
	setType(result,NOTHING);
	setIndex(result,0);
	setLength(result,0);
	setValue(result,0);
	setName(result,NULL);
	return result;
}

void freeToken(token *t) {
	assert(t!=NULL);
	if (getType(t)==VARIABLE) free(t->name);
	free(t);
}

void setType(token *t,enum typeEnum value) {
	assert(t!=NULL);
	t->type=value;
}

enum typeEnum getType(token *t) {
	if (t==NULL) return NOTHING;
	return t->type;
}

void setIndex(token *t,int idx) {
	assert(t!=NULL);
	t->index=idx;
}

int getIndex(token *t) {
	assert(t!=NULL);
	return t->index;
}

void setLength(token *t,int lgt) {
	assert(t!=NULL);
	t->length=lgt;
}

int getLength(token *t) {
	if (t==NULL) return 0;
	return t->length;
}

void setValue(token *t,long val) {
	assert(t!=NULL);
	if (getType(t)==VARIABLE) setVar(getName(t),val);
	else t->value=val;
}

long getValue(token *t) {
	assert(t!=NULL);
	if (getType(t)==VARIABLE) {
		return getVar(getName(t));
	}
	return t->value;
}

void setName(token *t, char * name) {
	assert(t!=NULL);
	if (name==NULL) t->name=NULL;
	else t->name=strdup(name);
}

char * getName(token *t) {
	assert(t!=NULL);
	return t->name;
}

void printToken(token * t) {
	char ttype[20];

	if (t==NULL) {
		printf(" NULL token\n");
		return;
	}

	switch(getType(t)) {
		case INTEGER : strcpy(ttype,"Integer"); break;
		case LPAREN  : strcpy(ttype,"Left Paren"); break;
		case RPAREN  : strcpy(ttype,"Right Paren"); break;
		case PLUS    : strcpy(ttype,"Plus sign"); break;
		case MULTIPLY: strcpy(ttype,"Times sign"); break;
		case DIVIDE  : strcpy(ttype,"Divide sign"); break;
		case MINUS   : strcpy(ttype,"Minus sign"); break;
		case EOS : strcpy(ttype,"End of Statement"); break;
		case WHILE : strcpy(ttype,"While"); break;
		case SET: strcpy(ttype,"Set"); break;
		case LCURLY: strcpy(ttype,"Left Curly Brace"); break;
		case RCURLY: strcpy(ttype,"Right Curly Brace"); break;
		case VARIABLE: strcpy(ttype,"Variable"); break;
	}

	printf("Token at %3d len=%2d Type=%s",getIndex(t),getLength(t),ttype);
	if (getType(t)==INTEGER) printf(" Value=%ld",getValue(t));
	if (getType(t)==VARIABLE) printf(" Name=%s",getName(t));
	printf("\n");
}

token * nextToken(token *current,char *buffer,bool delPrev) {
	token * result=newToken();

	if (current!=NULL) {
		setIndex(result,getIndex(current)+getLength(current));
		if (delPrev) freeToken(current);
	}

	while(getLength(result)==0) {
		if (getIndex(result) >= strlen(buffer)) {
			free(result);
			return NULL;
		}
		switch(buffer[getIndex(result)]) {
			case ' ':
			case '\t':
				setIndex(result,getIndex(result)+1);
				continue;
			case '+':
				setType(result,PLUS);
				setLength(result,1);
				break;
			case '-':
				setType(result,MINUS);
				setLength(result,1);
				break;
			case '*':
				setType(result,MULTIPLY);
				setLength(result,1);
				break;
			case '/':
				setType(result,DIVIDE);
				setLength(result,1);
				break;
			case ';' :
				setType(result,EOS);
				setLength(result,1);
				break;
			case '(':
				setType(result,LPAREN);
				setLength(result,1);
				break;
			case ')':
				setType(result,RPAREN);
				setLength(result,1);
				break;
			case '{':
				setType(result,LCURLY);
				setLength(result,1);
				break;
			case '}' :
				setType(result,RCURLY);
				setLength(result,1);
				break;

			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				setType(result,INTEGER);
				char *endptr;
				setValue(result,strtol(buffer+getIndex(result),&endptr,10));
				setLength(result,(endptr-buffer)-getIndex(result));
				break;

			case 's':
				if (0==strncmp(buffer+getIndex(result),"set",3)) {
					if ( (strlen(buffer)<=getIndex(result)+3) ||
					     (buffer[getIndex(result)+3]==' ')   ) {
						setType(result,SET);
						setLength(result,3);
						break;
					}
				}
				int ei=0;
				char varName[20];
				while(isalpha(buffer[getIndex(result)+ei])) {
					varName[ei]=buffer[getIndex(result)+ei];
					if (ei>=strlen(buffer)) break;
					ei++;
					varName[ei]='\0';
				}
				setType(result,VARIABLE);
				setName(result,varName);
				setLength(result,ei);
				break;

			case 'w':
				if (0==strncmp(buffer+getIndex(result),"while",5)) {
					if ( (strlen(buffer)<=getIndex(result)+5) ||
					     (buffer[getIndex(result)+5]==' ') ||
					     (buffer[getIndex(result)+5]=='(') ) {
							setType(result,WHILE);
							setLength(result,5);
							break;
					}
				}
				ei=0;
				while(isalpha(buffer[getIndex(result)+ei])) {
					varName[ei]=buffer[getIndex(result)+ei];
					if (ei>=strlen(buffer)) break;
					ei++;
					varName[ei]='\0';
				}
				setType(result,VARIABLE);
				setName(result,varName);
				setLength(result,ei);
				break;

			default:

				if (isalpha(buffer[getIndex(result)])) {
					ei=0;
					while(isalpha(buffer[getIndex(result)+ei])) {
						varName[ei]=buffer[getIndex(result)+ei];
						if (ei>=strlen(buffer)) break;
						ei++;
						varName[ei]='\0';
					}
					setType(result,VARIABLE);
					setName(result,varName);
					setLength(result,ei);
					break;
				}

				tokenError("Unrecognized token in expression",buffer,result);
				free(result);
				exit(0);
		}
	}
	return result;
}

void tokenError(char * reason,char *exp,token *t) {
	printf("Token error : %s\n",reason);
	printf("  Expression: %s\n",exp);
	if (t==NULL) return;
	char buffer[100];
	memset(buffer,' ',getIndex(t));
	buffer[getIndex(t)]=0;
	printf("  Error     : %s^\n",buffer);
}

void setVar(char * name,long int val) {
	int i=0;
	for(i=0; i<50; i++) {
		if (varSpace[i].vname==NULL) {
			varSpace[i].vname=name;
			break;
		}
		if (0==strcmp(varSpace[i].vname,name)) break;
	}
	assert(i<50);
	varSpace[i].vvalue=val;
	varSpace[i].valset=true;
}

long int getVar(char *name) {
	int i;
	for(i=0;i<50;i++) {
		if (!varSpace[i].vname) {
			printf("Variable %s has never been assigned a value\n",name);
			exit(1);
		}
		if (0==strcmp(varSpace[i].vname,name)) {
			if (varSpace[i].valset) return varSpace[i].vvalue;
			printf("Variable %s has never been assigned a value\n",name);
			exit(1);
		}
	}
	assert(0);
}
