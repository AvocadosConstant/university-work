#include "Map.hpp"

#include <iostream>
#include <cassert>

using std::cout;
using std::endl;

int main() {
  cs540::Map<int, char>::SkipList skip;

  // Test size and empty
  assert(skip.size() == 0);
  assert(skip.empty());

  // Test insertion
  for(int i = 0; i < 20; i++) {
    skip.insert({i, 'a' + i});
  }

  // Test size and empty after insertions
  assert(skip.size() == 20);
  assert(!skip.empty());

  skip.print();

  // Test find and at
  assert(skip.find(6)->second == ('a' + 6));
  assert(skip.at(19) == ('a' + 19));

  // Test skiplist iterators
  skip.end();
  cs540::Map<int, char>::SkipList::Iterator it = skip.begin();
  for(int i = 0; i < 10; i++) {
    it++;
    cout << (*it).first << endl;
  }
  for(int i = 0; i < 5; i++) {
    it--;
    cout << (*it).first << endl;
  }
  (*it).second = '!';

  cs540::Map<int, char>::SkipList::Iterator begin = skip.begin();
  auto copy_it = cs540::Map<int, char>::SkipList::Iterator(begin);
  cout << copy_it->first << endl;

  skip.print();

  return 0;
}
