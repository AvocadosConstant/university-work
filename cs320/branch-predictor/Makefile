CC=gcc
CXX=g++
CFLAGS=-c -g -Wall -std=c11
CXXFLAGS=-c -g -O3 -Wall -Wextra -std=c++11
LDFLAGS=-pthread
SOURCES=main.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=predictors
RUN=./$(EXECUTABLE)
RUNV=valgrind --leak-check=full --track-origins=yes $(RUN)
RUNS=perf stat $(RUN)

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	-rm $(EXECUTABLE) *.o

test: all
	# Short Tests
	$(RUN) ./spec/traces/short_trace1.txt out.txt
	diff out.txt ./spec/correct_outputs/short_trace1-output.txt
	$(RUN) ./spec/traces/short_trace2.txt out.txt
	diff out.txt ./spec/correct_outputs/short_trace2-output.txt
	$(RUN) ./spec/traces/short_trace3.txt out.txt
	diff out.txt ./spec/correct_outputs/short_trace3-output.txt
	# Long Tests
	$(RUN) ./spec/traces/long_trace1.txt out.txt
	diff out.txt ./spec/correct_outputs/long_trace1-output.txt
	$(RUN) ./spec/traces/long_trace2.txt out.txt
	diff out.txt ./spec/correct_outputs/long_trace2-output.txt
	$(RUN) ./spec/traces/long_trace3.txt out.txt
	diff out.txt ./spec/correct_outputs/long_trace3-output.txt
