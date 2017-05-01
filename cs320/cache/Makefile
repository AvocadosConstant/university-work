CC=gcc
CXX=g++
CFLAGS=-c -g -Wall -std=c11
CXXFLAGS=-c -g -O3 -Wall -Wextra -std=c++11
LDFLAGS=-pthread
SOURCES=main.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=cache-sim
RUN=./$(EXECUTABLE)
RUNV=valgrind --leak-check=full --track-origins=yes $(RUN)
RUNS=perf stat $(RUN)

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	-rm $(EXECUTABLE) *.o out.txt

test: all
	#
	# Testing all 3 given traces
	#
	# -----------
	$(RUN) ./spec/traces/trace1.txt out.txt
	#diff out.txt ./spec/correct_outputs/trace1_output.txt
	# Trace 1 passed!
	#
	$(RUN) ./spec/traces/trace2.txt out.txt
	#diff out.txt ./spec/correct_outputs/trace2_output.txt
	# Trace 2 passed!
	#
	$(RUN) ./spec/traces/trace3.txt out.txt
	#diff out.txt ./spec/correct_outputs/trace3_output.txt
	# Trace 3 passed!
	#
	#
	# All cases passed!
