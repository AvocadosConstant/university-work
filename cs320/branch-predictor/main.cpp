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
  //pred.print_trace();

  // TODO Write to output file or standard out if none specified
  std::cout << pred.always(true) << std::endl;
  std::cout << pred.always(false) << std::endl;
  std::cout << pred.bimodal(true) << std::endl;
  std::cout << pred.bimodal(false) << std::endl;
  std::cout << pred.gshare() << std::endl;
  std::cout << pred.tournament() << std::endl;

  return 0;
}
