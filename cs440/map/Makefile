CXX=g++
CXXFLAGS=-g -std=c++11 -Wall -Wextra -pedantic
BIN=test

SRC=$(wildcard *.cpp)
OBJ=$(SRC:%.cpp=%.o)

all: $(OBJ)
	$(CXX) -Wl,--no-as-needed -ldl -o $(BIN) $^

%.o: %.c
	$(CXX) $@ -c $<

clean:
	rm -f *.o
	rm $(BIN)
