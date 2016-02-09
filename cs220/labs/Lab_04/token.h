#ifndef TOKEN_H
#define TOKEN_H
#include <stdbool.h>

/* "token" is a synonym for an "opaque" tokenStruct structure */
typedef struct tokenStruct token;

enum typeEnum {
		NOTHING,
		INTEGER,
		LPAREN,
		RPAREN,
		PLUS,
		MULTIPLY,
		DIVIDE,
		MINUS,
		EOS,
		WHILE,
		LCURLY,
		RCURLY,
		SET,
		VARIABLE
};

token * newToken();
void freeToken(token *t);

void setType(token *t,enum typeEnum value);
enum typeEnum getType(token *t);
void setIndex(token *t,int idx);
int getIndex(token *t);
void setLength(token *t,int lgt);
int getLength(token *t);
void setValue(token *t,long val);
long getValue(token *t);
void setName(token *t,char *name);
char * getName(token *t);

void printToken(token * token);
token * nextToken(token *current,char *buffer,bool delPrev);
void tokenError(char * reason,char *exp,token *t);



#endif