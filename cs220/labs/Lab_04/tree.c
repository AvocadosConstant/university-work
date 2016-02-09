#include "tree.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

struct treeStruct {
	struct treeStruct * left;
	token * action;
	struct treeStruct * right;
};

tree * newTree() {
	tree* result=(tree *)malloc(sizeof(tree));
	result->left=NULL;
	result->action=NULL;
	result->right=NULL;
	return result;
}

void setLeft(tree *t,tree *l) {
	assert(t!=NULL);
	t->left=l;
}

tree * getLeft(tree *t) {
	if (t==NULL) return NULL;
	return t->left;
}

void setRight(tree *t,tree *r) {
	assert(t!=NULL);
	t->right=r;
}

tree * getRight(tree *t) {
	if (t==NULL) return NULL;
	return t->right;
}

void setRoot(tree *t,token *r) {
	assert(t!=NULL);
	t->action=r;
}
token * getRoot(tree *t) {
	if (t==NULL) return NULL;
	return t->action;
}

enum typeEnum getRootType(tree *t) {
	return getType(getRoot(t));
}

tree * getExpression(token ** tokenPtr,char *buffer) {

	assert((*tokenPtr)!=NULL);
	tree * term1=getTerm(tokenPtr,buffer);
	if ((*tokenPtr)==NULL) return term1;
	if ((getType(*tokenPtr)==PLUS) || (getType(*tokenPtr)==MINUS)) {
		tree* result=newTree();
		setLeft(result,term1);
		setRoot(result,(*tokenPtr));
		(*tokenPtr)=nextToken((*tokenPtr),buffer,false);
		if ((*tokenPtr)==NULL) {
			printf("Syntax error... incomplete expression, expected another term");
			exit(0);
		}
		setRight(result,getExpression(tokenPtr,buffer));
		return result;
	}
	return term1;

}

tree * getTerm(token ** tokenPtr,char *buffer) {

	assert((*tokenPtr) != NULL);
	tree * fact1=getFactor(tokenPtr,buffer);
	if ((*tokenPtr)==NULL) return fact1;
	if ((getType(*tokenPtr)==MULTIPLY) || (getType(*tokenPtr)==DIVIDE)) {
		tree* result=newTree();
		setLeft(result,fact1);
		setRoot(result,(*tokenPtr));
		(*tokenPtr)=nextToken((*tokenPtr),buffer,false);
		if ((*tokenPtr)==NULL) {
			printf("Syntax error... incomplete expression, expected another factor");
			exit(0);
		}
		setRight(result,getTerm(tokenPtr,buffer));
		return result;
	}
	return fact1;
}

tree * getFactor(token ** tokenPtr,char *buffer) {

	assert((*tokenPtr) != NULL);
	if (getType(*tokenPtr)==LPAREN) {
		(*tokenPtr)=nextToken((*tokenPtr),buffer,true);
		if ((*tokenPtr)==NULL) {
			printf("Syntax error... incomplete expression, expected an expression after open parens\n");
			exit(0);
		}
		tree * result=getExpression(tokenPtr,buffer);
		if (getType(*tokenPtr) != RPAREN) {
			tokenError("Syntax error... expected closing parenthesis after expression",buffer,(*tokenPtr));
			exit(0);
		}
		(*tokenPtr)=nextToken((*tokenPtr),buffer,true);
		return result;
	}

	if (getType(*tokenPtr)==INTEGER) {
		tree * result=newTree();
		setRoot(result,(*tokenPtr));
		(*tokenPtr)=nextToken((*tokenPtr),buffer,false);
		return result;
	}

	printf("Syntax error... expected a factor");
	exit(0);
}


void printTree(tree * t,char * prefix) {

	char newPrefix[100];
	printf("%s Root: ",prefix);
	printToken(getRoot(t));

	if (getRootType(t)==INTEGER) return;

	strcpy(newPrefix,prefix);
	strcat(newPrefix,".L");
	if (getLeft(t)==NULL) printf("%s NULL\n",newPrefix);
	else printTree(getLeft(t),newPrefix);
	strcpy(newPrefix,prefix);
	strcat(newPrefix,".R");
	if (getRight(t)==NULL) printf("%s NULL\n",newPrefix);
	else printTree(getRight(t),newPrefix);
}

long int evalTree(tree *t,bool debug) {

	if (getRootType(t)==INTEGER) return getValue(getRoot(t));
	long int left=evalTree(getLeft(t),debug);
	long int right=evalTree(getRight(t),debug);
	switch (getRootType(t)) {
		case PLUS:
			if (debug) printf("%ld + %ld = %ld\n",left,right,left+right);
			return left+right;

		case MINUS:
			if (debug) printf("%ld - %ld = %ld\n",left,right,left-right);
			return left-right;

		case MULTIPLY:
			if (debug) printf("%ld * %ld = %ld\n",left,right,left*right);
			return left*right;

		case DIVIDE:
			if (debug) printf("%ld / %ld = %ld\n",left,right,left/right);
			return left/right;

		case EOS:
			if (debug) printf("%ld ; %ld\n",left,right);
			return right;

		default:
			printf("Unrecognized action for subtree Token: ");
			printToken(getRoot(t));
	}
	return 0;
}



void freeTree(tree * t) {

	if (getLeft(t) != NULL) freeTree(getLeft(t));
	if (getRight(t) != NULL) freeTree(getRight(t));
	if (getRoot(t) != NULL) free(getRoot(t));
	free(t);

}

tree * getExpList(token **tokenPtr,char *buffer) {
	
	assert((*tokenPtr)!=NULL);
	tree * exp1 = getExpression(tokenPtr,buffer);
	if ((*tokenPtr)==NULL) return exp1;
	
	//If tokenPtr is a semicolon	
	if ((getType(*tokenPtr)==EOS)) {
		tree* result=newTree();
		setLeft(result,exp1);
		setRoot(result,(*tokenPtr));
		(*tokenPtr)=nextToken((*tokenPtr),buffer,false);
		if ((*tokenPtr)==NULL) {
			printf("Syntax error... incomplete expression, expected another expression");
			exit(0);
		}
		setRight(result,getExpList(tokenPtr,buffer));
		return result;
	}
	return exp1;

}





















