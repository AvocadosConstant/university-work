#include "Map.hpp"

#include <iostream>
#include <cassert>

using std::cout;
using std::endl;

int main() {
  cs540::Map<int, char>::SkipList skip;

  assert(skip.size() == 0);
  assert(skip.empty());

  for(int i = 0; i < 20; i++) {
    skip.insert({i, 'a' + i});
  }

  assert(skip.size() == 20);
  assert(!skip.empty());

  skip.print();

  assert(skip.at(19) == ('a' + 19));

  // Test skiplist iterators
  skip.end();
  cs540::Map<int, char>::SkipList::Iterator it = skip.begin();
  for(int i = 0; i < 10; i++) {
    it++;
    std::cout << (*it).first << std::endl;
  }
  for(int i = 0; i < 5; i++) {
    it--;
    std::cout << (*it).first << std::endl;
  }
  (*it).second = '!';

  cs540::Map<int, char>::SkipList::Iterator begin = skip.begin();
  auto copy_it = cs540::Map<int, char>::SkipList::Iterator(begin);
  std::cout << copy_it->first << std::endl;

  skip.print();
  return 0;
}
