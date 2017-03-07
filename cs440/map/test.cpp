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
  for(int i = 0; i < 20; i+=2) {
    skip.insert({i, 'a' + i});
  }

  // Test size and empty after insertions
  assert(skip.size() == 10);
  assert(!skip.empty());

  skip.print();

  // Test find and at
  assert(skip.find(6)->second == ('a' + 6));
  assert(skip.at(8) == ('a' + 8));

  // Test skiplist iterators
  skip.end();
  cs540::Map<int, char>::SkipList::Iterator it = skip.begin();
  for(int i = 0; i < 10; i++) {
    cout << "Incrementing iterator to " << (*it).first << endl;
    it++;
  }
  for(int i = 0; i < 5; i++) {
    it--;
    cout << "Decrementing iterator to " << (*it).first << endl;
  }
  (*it).second = '!';

  cs540::Map<int, char>::SkipList::Iterator begin = skip.begin();
  auto copy_it = cs540::Map<int, char>::SkipList::Iterator(begin);
  cout << copy_it->first << endl;

  skip.print();

  // Testing insertion in between other elements
  for(int i = 1; i < 20; i+=2) {
    assert(skip.insert({i, 'a' + i}).second);
  }

  assert(skip.size() == 20);
  skip.print();
  
  // Printing in order using ranged based for loop
  for(auto i : skip) {
    cout << i.second << " ";
  } cout << endl;

  // Test insertion of duplicate key
  assert(!skip.insert({5, '?'}).second);

  // Test deletions of all even elements
  for(int i = 0; i < 20; i+=2) {
    skip.erase(i);
  }

  assert(skip.size() == 10);
  skip.print();

  // Delete after find
  skip.erase(skip.find(5));

  assert(skip.size() == 9);
  skip.print();

  // Test deletions of rest of elements with iterators
  while(!skip.empty()) {
    skip.erase(skip.begin());
  }

  assert(skip.size() == 0);
  skip.print();

  return 0;
}
