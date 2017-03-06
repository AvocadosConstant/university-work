#include "Map.hpp"

#include <iostream>
#include <assert.h>

int main() {
  cs540::Map<int, int>::SkipList skip;
  for(int i = 0; i < 10; i++) {
    skip.insert({i, i});
  }
  skip.print();

  std::cout << "Found this at key 9: " << skip.at(9) << std::endl;

  skip.at(10);
  return 0;
}
