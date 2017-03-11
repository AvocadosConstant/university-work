#CC=gcc
#CXX=g++
CFLAGS=-c -g -Wall -std=c11
CXXFLAGS=-c -g -O3 -Wall -Wextra -std=c++11
LDFLAGS=-pthread
SOURCES=main.cpp #always.cpp bimodalsingle.cpp bimodalsaturating.cpp gshare.cpp tournament.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=predictors
RUN=./$(EXECUTABLE)
RUNV=valgrind --leak-check=full --track-origins=yes $(RUN)
RUNS=perf stat $(RUN)

ccred:=$(shell echo "\033[0;31m")
ccgreen:=$(shell echo "\033[0;92m")
ccyellow:=$(echo -e "\033[0;33m")
ccend:=$(shell echo "\033[0m")

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	-rm $(EXECUTABLE) *.o

test: all
	$(RUN) ./spec/traces/short_trace1.txt
	#tar -xf traces.tar.gz
	#$(RUN) ./traces/short_trace1.txt out.txt
	#diff out.txt ./correct_outputs/short_trace1-output.txt
	#$(RUN) ./traces/short_trace2.txt out.txt
	#diff out.txt ./correct_outputs/short_trace2-output.txt
	#$(RUN) ./traces/short_trace3.txt out.txt
	#diff out.txt ./correct_outputs/short_trace3-output.txt
	## Long
	#$(RUN) ./traces/long_trace1.txt out.txt
	#diff out.txt ./correct_outputs/long_trace1-output.txt
	#$(RUN) ./traces/long_trace2.txt out.txt
	#diff out.txt ./correct_outputs/long_trace2-output.txt
	#$(RUN) ./traces/long_trace3.txt out.txt
	#diff out.txt ./correct_outputs/long_trace3-output.txt
