#include "Map.hpp"

#include <iostream>
#include <cassert>

using std::cout;
using std::endl;

int main() {

  cs540::Map<int, char> map{{1, 'a'}, {2, 'b'}, {3, 'c'}, {4, 'd'}};
  map.print();

  // Test size and empty
  assert(map.size() == 4);
  assert(!map.empty());

  map.clear();

  // Test size and empty
  assert(map.size() == 0);
  assert(map.empty());
  assert(map.begin() == map.end());

  // Test insertion
  for(int i = 0; i < 20; i+=2) {
    map.insert({i, 'a' + i});
  }

  // Test size and empty after insertions
  assert(map.size() == 10);
  assert(!map.empty());

  map.print();

  // Test find and at
  assert(map.find(6)->second == ('a' + 6));
  assert(map.at(8) == ('a' + 8));

  // Test maplist iterators
  map.end();
  cs540::Map<int, char>::Iterator it = map.begin();
  for(int i = 0; i < 10; i++) {
    cout << "Incrementing iterator to " << (*it).first << endl;
    it++;
  }
  for(int i = 0; i < 10; i++) {
    it--;
    cout << "Decrementing iterator to " << (*it).first << endl;
  }

  // Test iterator equality
  assert(it == map.begin());
  assert(it != map.end());

  (*it).second = '!';

  cs540::Map<int, char>::Iterator begin = map.begin();
  auto copy_it = cs540::Map<int, char>::Iterator(begin);
  cout << copy_it->first << endl;

  map.print();

  // Testing insertion in between other elements
  for(int i = 1; i < 20; i+=2) {
    assert(map.insert({i, 'a' + i}).second);
  }

  assert(map.size() == 20);
  map.print();
  
  // Printing in order using ranged based for loop
  for(auto i : map) {
    cout << i.second << " ";
  } cout << endl;

  // Test insertion of duplicate key
  assert(!map.insert({5, '?'}).second);

  // Test deletions of all even elements
  for(int i = 0; i < 20; i+=2) {
    map.erase(i);
  }

  assert(map.size() == 10);
  map.print();

  // Delete after find
  map.erase(map.find(5));

  assert(map.size() == 9);
  map.print();

  cout << "Printing in order with const iterator" << endl;
  cs540::Map<int, char>::ConstIterator m_it = map.begin();
  for(; m_it != map.end(); m_it++) {
    cout << (*m_it).second << " ";
  } cout << endl;

  cout << "Printing in reverse with ReverseIterator" << endl;
  cs540::Map<int, char>::ReverseIterator r_it = map.rbegin();
  for(; r_it != map.rend(); r_it++) {
    cout << (*r_it).second << " ";
  } cout << endl;

  // Test deletions of rest of elements with iterators
  while(!map.empty()) {
    map.erase(map.begin());
  }

  assert(map.size() == 0);
  map.print();

  return 0;
}
