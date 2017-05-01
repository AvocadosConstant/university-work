#ifndef _SET_HPP_
#define _SET_HPP_

#include <vector>

struct Set {

  /** Stores the tags for the elements in the set */
  std::vector<unsigned long> tags_;

  std::vector<bool> valid_bits_;

  Set(unsigned long ways) : tags_(ways, 0), valid_bits_(ways, false) {};

  bool access(unsigned long tag) {
    for(unsigned int i = 0; i < tags_.size(); i++) {
      if(valid_bits_[i] && (tags_[i] == tag)) {
        return true;
      }
    }
    // For now just replacing first tag
    // TODO: Implement LRU
    valid_bits_[0] = true;
    tags_[0] = tag;
    return false;
  }

}; // struct Set

#endif
