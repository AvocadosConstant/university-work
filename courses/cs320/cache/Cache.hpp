#ifndef _CACHE_HPP_
#define _CACHE_HPP_

#include <cmath>
#include <bitset>
#include "Set.hpp"

struct Cache {

  #define DEFAULT_LINE_SIZE 32

  // Represents a memory instruction
  //    first:  true if load instr, false if store instr
  //    second: byte address target of instr
  typedef std::pair<bool, unsigned long long> InstrType;

  // Vector of instructions
  typedef std::vector<InstrType> TraceType;

  const TraceType &trace_;
  unsigned long cache_size_;
  unsigned long line_size_;
  unsigned long ways_;
  unsigned long num_sets_;

  bool no_alloc_on_miss_;
  bool prefetch_;
  bool prefetch_on_miss_;

  unsigned int offset_bits_;
  unsigned int index_bits_;
  unsigned int tag_bits_;

  unsigned long set_mask_;
  unsigned long tag_mask_;

  std::vector<Set> sets_;

  Cache(const TraceType &trace, unsigned long cache_size, unsigned long ways,
      bool no_alloc_on_miss, bool prefetch, bool prefetch_on_miss) :
    trace_(trace),
    cache_size_(cache_size),
    line_size_(DEFAULT_LINE_SIZE),
    ways_(ways),
    num_sets_(cache_size_ / (line_size_ * ways_)),

    no_alloc_on_miss_(no_alloc_on_miss),
    prefetch_(prefetch),
    prefetch_on_miss_(prefetch_on_miss),

    offset_bits_((unsigned int)log2(line_size_)),
    index_bits_((unsigned int)log2(num_sets_)),
    tag_bits_(line_size_ - (offset_bits_ + index_bits_)),

    set_mask_((num_sets_ - 1) << offset_bits_),
    tag_mask_(((1 << tag_bits_) - 1) << (offset_bits_ + index_bits_)),

    sets_(num_sets_, Set(ways)) {
      // These are debugging outputs, TODO: remove before submission
      /*
      std::cerr << "Cache initialized with " << std::dec << num_sets_
        << " sets of " << ways_ << " lines each for a total of "
        << (cache_size_ / line_size_) << " lines in the cache of total size " << cache_size_ << std::endl;

      std::cerr << offset_bits_ << " offset_bits" << std::endl;
      std::cerr << index_bits_ << " index_bits" << std::endl;
      std::cerr << tag_bits_ << " tag_bits" << std::endl;

      std::cerr << "Sample address:" << std::hex << trace_[0].second << std::endl;
      std::cerr << "\t\t" << std::bitset<32>(trace_[0].second) << std::endl << std::endl;
      std::cerr << "Set index:\t" << std::bitset<32>(get_set_index(trace_[0].second)) << std::endl;
      std::cerr << "Tag:\t\t" << std::bitset<32>(get_tag(trace_[0].second)) << std::endl;
      */
  };

  /** Processes all the instructions in the trace */
  std::string process() {
    unsigned long hits = 0;
    for(auto instr : trace_) {
      if(this->access(instr, false)) hits++;
    }
    std::ostringstream output;
    output << hits << "," << trace_.size() << ";";
    return output.str();
  }

  /** Accesses cache at given address */
  bool access(InstrType instr, bool during_prefetch) {

    bool is_load = instr.first;
    unsigned long long addr = instr.second;

    if(during_prefetch) addr += 32;

    unsigned long index = get_set_index(addr);
    unsigned long tag = get_tag(addr);

    bool hit;

    if(!is_load && no_alloc_on_miss_) {
      hit = sets_[index].access(tag, false);
    } else {
      hit = sets_[index].access(tag, true);
    }

    if(!during_prefetch && prefetch_) {
      if(prefetch_on_miss_) {
        if(!hit) this->access(instr, true);
      } else {
        this->access(instr, true);
      }
    }

    return hit;
  }

  /** Returns set index for given address */
  unsigned long get_set_index(unsigned long addr) {
    return (addr & set_mask_) >> offset_bits_;
  }

  /** Returns tag for given address */
  unsigned long get_tag(unsigned long addr) {
    return (addr & tag_mask_) >> (index_bits_ + offset_bits_);
  }

  /** Prints entire trace */
  void print_trace() {
    for(auto instr : trace_) {
      std::cout << ((instr.first) ? "L" : "S") << "\t" << std::hex << instr.second << std::endl;
    }
  }
}; // struct Cache

#endif
