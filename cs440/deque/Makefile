CXXFLAGS=-g -Wall -Wextra -pedantic -std=c++11 -O4
SRCEXT=cpp
HEADEREXT=hpp
LDFLAGS=-ldl
SOURCES=$(shell find . -type f -name "*.$(SRCEXT)")
OBJECTS=$(patsubst %.$(SRCEXT),%.o,$(SOURCES))
HEADERS=$(shell find . -type f -name "*.$(HEADEREXT)")
EXECUTABLE=deque
RUN=./$(EXECUTABLE)
RUNV=valgrind --leak-check=full --track-origins=yes $(RUN)

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(LDFLAGS) $^ -o $@

%.o:%.$(SRCEXT)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	-rm $(EXECUTABLE) *.o

test: all
	$(RUN)

valgrind: all
	$(RUNV)

lint: $(SOURCES) $(HEADERS)
	cpplint $^

.PHONY: clean
