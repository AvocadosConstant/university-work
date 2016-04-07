CC=gcc
CFLAGS= -std=c99 -Wall
SOURCES=lab6.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=lab6
LIB=-pthread

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LIB)

clean: 
	rm $(OBJECTS) *~ *.swp lab6
