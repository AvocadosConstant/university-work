#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

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

  // TODO Remove this block
  // Test print the parsed trace
  for(auto branch : trace) {
    std::cout << std::hex << branch.first << "\t" << ((branch.second) ? "Taken" : "Not taken") << std::endl;
  }

  // TODO Run all the predictors here
  
  // TODO Write to output file or standard out if none specified

  return 0;
}
