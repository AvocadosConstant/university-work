#include <stdlib.h>
#include <stdio.h>

int main(int argc,char **argv) {

	char buffer[1024];

	while (fgets(buffer,sizeof(buffer),stdin)) {
		char * rest=buffer;
		unsigned char hexVal;
		while (rest[0]!='\0') {
			if (1==sscanf(rest,"0x%2hhx",&hexVal)) {
				printf("%c",hexVal);
				rest+=4;
			} else {
				printf("%c",rest[0]);
				rest++;
			}
		}
		
	}
	return 0;
}	
