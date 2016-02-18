CC=gcc
CFLAGS= -std=c99 -Wall
SOURCES=lab3.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=lab3

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

clean: 
	rm $(OBJECTS) *~ *.swp lab3
