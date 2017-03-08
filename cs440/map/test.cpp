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
  for(int i = 0; i < 10; i++) {
    cout << "Incrementing iterator to " << (*it).first << endl;
    ++it;
  }
  for(int i = 0; i < 10; i++) {
    --it;
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

  cout << "Printing in order with const iterator: ";
  cs540::Map<int, char>::ConstIterator m_it = map.begin();
  for(; m_it != map.end(); m_it++) {
    cout << (*m_it).second << " ";
  } cout << endl;

  cout << "Printing in reverse with ReverseIterator: ";
  cs540::Map<int, char>::ReverseIterator r_it = map.rbegin();
  for(; r_it != map.rend(); r_it++) {
    cout << (*r_it).second << " ";
  } cout << endl;

  m_it = map.find(3);
  cout << "Printing in order with const iterator starting from key 3 and value " << (*m_it).second << ": ";
  for(; m_it != map.end(); m_it++) {
    cout << (*m_it).second << " ";
  } cout << endl;

  it = map.find(7);
  assert((*it).second == 'a' + 7);
  cout << "Modifying map[7] = &" << endl;
  map[7] = '&';
  cout << "Printing in order with normal iterator starting from key 7 and value " << (*it).second << ": ";
  for(; it != map.end(); it++) {
    cout << (*it).second << " ";
  } cout << endl;
  map.print();

  cs540::Map<int, char> map_copy;
  map_copy = map;
  assert(map_copy == map);
  cout << "Printing a different map that was operator= assigned to" << endl;
  map_copy.print();

  cout << "Modifying map_copy[7] to '?'" << endl;
  map_copy[7] = '?';
  assert(map_copy[7] != map[7]);
  assert(map_copy != map);

  cout << "Printing map_copy" << endl;
  map_copy.print();
  cout << "Printing map, should still be '&' not '?'" << endl;
  map.print();

  map[3] = 'a';
  assert(map < map_copy);
  assert(!(map_copy < map));
  assert(!(map_copy == map));

  // Test deletions of rest of elements with iterators
  while(!map.empty()) {
    map.erase(map.begin());
  }

  assert(map.size() == 0);
  map.print();

  return 0;
}
