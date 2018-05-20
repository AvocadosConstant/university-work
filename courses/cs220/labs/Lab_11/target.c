#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

bool getString();
bool target();

int main(int argc, char *argv[])
{

	bool gs;

	while( (gs=getString())) {};
	printf("Yech.... you failed to run the target.\n");	
	return 1;
   
}

bool getString() {

	char buffer[81];
	buffer[0]='\0';
	gets(buffer);
	if (strlen(buffer)>0) {
		printf("Read line: %s\n",buffer); 
		return true;
	}
	return false;
	
}

bool target() {
	printf("Whooopeeee... you ran the target!\n");
	exit(0);	
}
