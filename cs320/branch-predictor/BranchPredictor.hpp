#ifndef _BRANCHPREDICTOR_HPP_
#define _BRANCHPREDICTOR_HPP_

#include <cmath>

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

  unsigned long size() { return trace.size(); };

  // Measures the accurracy of the "Always Taken and Always Non-Taken" branch predictors
  unsigned long always(bool taken) {
    unsigned long correct = 0;
    for(auto branch : trace) {
      if(taken == branch.second) correct++;
    }
    return correct;
  }

  // Measures the accurracy of the "Bimodal" branch predictors
  unsigned long bimodal(bool single_bit, int table_size) {
    bool bht[table_size];
    // BHT is seeded with taken
    std::fill_n(bht, table_size, true);
    unsigned long correct = 0;
    for(auto branch : trace) {
      int bht_index = branch.first % table_size;
      if(bht[bht_index] && branch.second) correct++;
      else if(!(bht[bht_index] || branch.second)) correct++;
      bht[bht_index] = branch.second;
    }
    return correct;
  }

  // Measures the accurracy of the "GShare" branch predictor
  unsigned long gshare(int history_length) {
    // TODO
    return 0;
  }

  // Measures the accurracy of the "tournament" branch predictor
  unsigned long tournament(){
    // TODO
    return 0;
  }

  // Runs all tests according to project specifications
  std::string test_all() {
    std::ostringstream output;
    output << always(true) << "," << size() << ";" << std::endl;
    output << always(false) << "," << size() << ";" << std::endl;

    std::ostringstream bim_one_output, bim_two_output, gshare_output;
    // We want to loop from a table size of 16 (2^4), to 2048 (2^11) excluding 64 (2^6)
    for(int pow_of_2 = 4; pow_of_2 <= 11; pow_of_2++) {
      if(pow_of_2 == 6) continue;
      unsigned long table_size = std::pow(2, pow_of_2);
      bim_one_output << bimodal(true, table_size) << "," << size() << ((pow_of_2 < 11) ? "; " : ";");
      bim_two_output << bimodal(false, table_size) << "," << size() << ((pow_of_2 < 11) ? "; " : ";");
    }
    output << bim_one_output.str() << std::endl << bim_two_output.str() << std::endl;

    for(int history_length = 3; history_length <= 11; history_length++) {
      gshare_output << gshare(history_length) << "," << size() << ((history_length < 11) ? "; " : ";");
    }
    output << gshare_output.str() << std::endl;

    output << tournament() << "," << size() << ";" << std::endl;

    return output.str();
  }
};

#endif
