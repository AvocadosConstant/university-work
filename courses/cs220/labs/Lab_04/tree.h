#ifndef TREE_H
#define TREE_H

#include "token.h"

/* Grammar...

	expression :=
		term + expression |
		term - expression |
		term

	term :=
		factor * term |
		factor / term |
		factor

	factor :=
		( expression ) |
		integer

	expList :=
		expression |
		expression ; expList

*/

/* "tree" is a synonym for an "opaque" treeStruct structure */
typedef struct treeStruct tree;

tree * newTree();

void setLeft(tree *t,tree *l);
tree * getLeft(tree *t);
void setRight(tree *t,tree *r);
tree * getRight(tree *t);
void setRoot(tree *t,token *r);
token * getRoot(tree *t);
enum typeEnum getRootType(tree *t);

void printTree(tree * t,char * prefix);
long int evalTree(tree *t,bool debug);
void freeTree(tree * t);

tree * getExpression(token ** tokenPtr,char *buffer);
tree * getTerm(token ** tokenPtr,char *buffer);
tree * getFactor(token ** tokenPtr,char *buffer);

tree * getExpList(token **tokenPtr,char *buffer);

#endif
