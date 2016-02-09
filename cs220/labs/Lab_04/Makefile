all: calc
.PHONY: clean test

calc: calc.c tree.h tree.c token.h token.c 
	gcc -g -o calc calc.c tree.c token.c 
	
	
test: calc exp1.txt
	./calc <exp1.txt
	
clean:
	rm calc