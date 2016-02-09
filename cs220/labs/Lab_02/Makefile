all: factc 
.PHONY: clean test

factc: factc.c
	gcc -g -o factc factc.c

factcu: factcu.c
	gcc -g -o factcu factcu.c

facts: facts.c
	gcc -g -o facts facts.c

factsu: factsu.c
	gcc -g -o factsu factsu.c

facti: facti.c
	gcc -g -o facti facti.c

factiu: factiu.c
	gcc -g -o factiu factiu.c

factil: factil.c
	gcc -g -o factil factil.c

test: factc factcu facts factsu facti factiu factil
	./factc
	./factcu
	./facts
	./factsu
	./facti
	./factiu
	./factil

clean:
	rm factc
	rm factcu
	rm facts
	rm factsu
	rm facti
	rm factiu
	rm factil
