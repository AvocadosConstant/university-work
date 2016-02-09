all: target hex2char
.PHONY: clean test gdb

target : target.c
	gcc -g -Wall -m32 -o target target.c
	
hex2char : hex2char.c 
	gcc -g -Wall -m32 -o hex2char hex2char.c	
	
testhex.txt : test.txt hex2char
	./hex2char <test.txt >testhex.txt	
	
test: target testhex.txt
	./target <testhex.txt
	
gdb: target testhex.txt
	gdb -x dbg.txt target	
	
target.asm.txt : target
	objdump -d target >target.asm.txt	
		
clean:
	-rm target target.asm.txt testhex.txt hex2char
