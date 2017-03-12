#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "BranchPredictor.hpp"

int main(int argc, char *argv[]) {

  // TODO Replace with < 3 after implementing write to output
  if(argc < 2) {
    std::cerr << "Please run with format: ./predictors input_trace.txt output.txt" << std::endl;
    return 1;
  }

  std::vector<std::pair<unsigned long long, bool>> trace;
  unsigned long long addr;
  std::string behavior, line;

  std::ifstream infile(argv[1]);
  while(getline(infile, line)) {
    std::stringstream s(line);
    s >> std::hex >> addr >> behavior;
    trace.push_back(std::make_pair(addr, behavior == "T"));
  }

  BranchPredictor pred(trace);

  //////////////////////
  // Output handling //
  ////////////////////
  std::ostream* output = &std::cout;
  std::ofstream fout;
  if(argc == 3) {
    std::cout << "Outputting to " << argv[2] << "..." << std::endl;
    fout.open(argv[2]);
    output = &fout;
  } else std::cout << "Outputting to standard out..." << std::endl;
  *output << pred.test_all();

  return 0;
}
