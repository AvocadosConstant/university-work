SRC_FILES = Makefile README qcalls.c functions.h functions.c function.h function.c

qcalls : qcalls.c functions.h functions.c function.h function.c
	gcc -g -Wall -o qcalls qcalls.c functions.c function.c
	
xmpCmd:  xmpCmd.c
	gcc -g -Wall -o xmpCmd xmpCmd.c
	
dump.txt: xmpCmd
	objdump -d xmpCmd | tee dump.txt	
	
test : qcalls xmpCmd
	./qcalls xmpCmd | tee log.txt
	
gdb : qcalls xmpCmd
	gdb -x dbg.txt qcalls
	
dist:		$(SRC_FILES)
		mkdir $(USER)_proj3
		cp $(SRC_FILES) $(USER)_proj3
		tar -cvzf $(USER)_proj3.tar.gz $(USER)_proj3
		rm -r $(USER)_proj3
	
clean:
	rm qcalls log.txt xmpCmd dump.txt
