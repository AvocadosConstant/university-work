#ifndef _SET_HPP_
#define _SET_HPP_

#include <vector>

struct Set {

  /** Stores the tags for the elements in the set */
  std::vector<unsigned long> tags_;

  std::vector<bool> valid_bits_;

  std::vector<unsigned long> lru_bits_;

  Set(unsigned long ways) : tags_(ways, 0), valid_bits_(ways, false), lru_bits_(ways) {
    // Seed lru values from 0 to n-1
    for(unsigned int i = 0; i < lru_bits_.size(); i++) {
      lru_bits_[i] = i;
    }
  };

  bool access(unsigned long tag) {
    for(unsigned int i = 0; i < tags_.size(); i++) {
      if(valid_bits_[i] && (tags_[i] == tag)) {
        this->update_lru(i);
        return true;
      }
    }
    // For now just replacing first tag
    // TODO: Implement LRU

    unsigned int lru_i = this->lru_index();

    valid_bits_[lru_i] = true;
    tags_[lru_i] = tag;

    this->update_lru(lru_i);
    return false;
  }

  unsigned int lru_index() {
    for(unsigned int i = 0; i < lru_bits_.size(); i++) {
      if(lru_bits_[i] == 0) return i;
    }
    // Should never happen
    return 0;
  }

  void update_lru(unsigned int mru_i) {
    unsigned int old_val = lru_bits_[mru_i];
    for(auto &cur : lru_bits_) {
      if(cur == old_val) {
        cur = lru_bits_.size() - 1;
      } else if(cur > old_val) {
        --cur;
      }
    }

    //for(auto cur : lru_bits_) std::cerr << cur << " ";
    //std::cerr << std::endl;
  }

}; // struct Set

#endif
