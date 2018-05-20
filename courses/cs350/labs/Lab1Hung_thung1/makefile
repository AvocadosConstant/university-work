CC=gcc
CFLAGS=-g -Wall
SOURCES=lab1.c mytimer.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=lab1

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

clean: 
	rm $(OBJECTS) *~ *.swp lab1
