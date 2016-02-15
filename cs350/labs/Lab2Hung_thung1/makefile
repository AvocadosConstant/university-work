CC=gcc
CFLAGS= -std=c99 -Wall
SOURCES=lab2.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=lab2

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

clean: 
	rm $(OBJECTS) *~ *.swp lab2
