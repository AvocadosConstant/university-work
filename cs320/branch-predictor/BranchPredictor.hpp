#ifndef _BRANCHPREDICTOR_HPP_
#define _BRANCHPREDICTOR_HPP_

struct BranchPredictor {
  typedef std::vector<std::pair<unsigned long long, bool>> TraceType;

  const TraceType &trace;

  BranchPredictor(const TraceType &trace) : trace(trace) {};

  void print_trace() {
    std::cout << "Printing entire trace" << std::endl;
    for(auto branch : trace) {
      std::cout << std::hex << branch.first << "\t" << ((branch.second) ? "Taken" : "Not taken") << std::endl;
    }
  }

  // TODO
  std::string always(bool taken) {
    // Measures accuracy of the "Always Taken and Always Non-Taken" branch predictor
    return taken ? "Always Taken Stats" : "Never Taken Stats";
  }

  // TODO
  std::string bimodal(bool single_bit) {
    // Measures accuracy of the "Bimodal" branch predictors
    return single_bit ? "Single Bit Bimodal Stats" : "Two Bit Bimodal Stats";
  }

  // TODO
  std::string gshare() {
    // Measures accuracy of the "GShare" branch predictor
    return "GShare Stats";
  }

  // TODO
  std::string tournament(){
    // Measures accuracy of the "tournament" branch predictor
    return "Tournament Stats";
  }
};

#endif
