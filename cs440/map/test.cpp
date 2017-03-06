#include "Map.hpp"

#include <iostream>
#include <assert.h>

int main() {
  cs540::Map<int, int> int_int_map;

  cs540::Map<int, int>::SkipList skip;
  for(int i = 0; i < 10; i++) {
    skip.insert({i, i});
  }
  skip.print();
  return 0;
}
