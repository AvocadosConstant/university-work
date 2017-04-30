#ifndef _CACHE_HPP_
#define _CACHE_HPP_

#include <cmath>

struct Cache {
  // Vector of instructions
  // Each instruction is defined as follows:
  //    first:  true if load instr, false if store instr
  //    second: byte address target of instr
  typedef std::vector<std::pair<bool, unsigned long long>> TraceType;

  // Contains all instructions in seed trace
  const TraceType &trace;

  Cache(const TraceType &trace) : trace(trace) {};

  void print_trace() {
    std::cout << "Printing entire trace" << std::endl;
    for(auto instr : trace) {
      std::cout << ((instr.first) ? "L" : "S") << "\t" << std::hex << instr.second << std::endl;
    }
  }
}; // struct Cache

#endif
