#ifndef _CACHE_HPP_
#define _CACHE_HPP_

#include <cmath>
#include "Set.hpp"

struct Cache {

  // Vector of instructions
  // Each instruction is defined as follows:
  //    first:  true if load instr, false if store instr
  //    second: byte address target of instr
  typedef std::vector<std::pair<bool, unsigned long long>> TraceType;

  const TraceType &trace_;
  unsigned long hits_;
  unsigned long cache_size_;
  unsigned long line_size_;
  unsigned long ways_;
  unsigned long num_sets_;
  std::vector<Set> data_;

  Cache(const TraceType &trace, unsigned long cache_size, unsigned long ways) :
    trace_(trace),
    hits_(0),
    cache_size_(cache_size),
    line_size_(32),
    ways_(ways),
    num_sets_(cache_size_ / (line_size_ * ways_)),
    data_(num_sets_, Set(ways)) {
      std::cerr << "Cache initialized with " << num_sets_
        << " sets of " << ways_ << " lines each for a total of "
        << (cache_size_ / line_size_) << " lines in the cache." << std::endl;
  };

  void print_trace() {
    std::cout << "Printing entire trace" << std::endl;
    for(auto instr : trace_) {
      std::cout << ((instr.first) ? "L" : "S") << "\t" << std::hex << instr.second << std::endl;
    }
  }
}; // struct Cache

#endif
