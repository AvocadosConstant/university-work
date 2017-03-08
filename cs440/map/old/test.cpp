#include "Map.hpp"

#include <iostream>
#include <cassert>

using std::cout;
using std::endl;

int main() {

  ///////////////////////////////////////////////////////// Testing SkipList

  /*
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
  */

  ///////////////////////////////////////////////////////// Testing Map

  cout << "-----------------------------------------------Testing map" << endl;

  cs540::Map<int, char> map;

  map.print();

  // Test size and empty
  assert(map.size() == 0);
  assert(map.empty());

  map.insert({1, 'a'});

  map.print();

  // Test size and empty
  assert(map.size() == 1);
  assert(!map.empty());

  cs540::Map<int, char> map2{{1, 'a'}, {2, 'b'}, {3, 'c'}, {4, 'd'}};

  map2.print();

  // Test size and empty
  assert(map2.size() == 4);
  assert(!map2.empty());

  // Test adding duplicate key
  map2.insert({1, 'a'});

  map2.print();

  // Size should stay the same
  assert(map2.size() == 4);

  map.clear();

  map.print();

  // Test size and empty
  assert(map.size() == 0);
  assert(map.empty());













  // Test insertion
  for(int i = 0; i < 20; i+=2) {
    map.insert({i, 'a' + i});
  }

  map.print();

  // Test size and empty after insertions
  assert(map.size() == 10);
  assert(!map.empty());

  // Test find and at
  assert(map.find(6)->second == ('a' + 6));
  assert(map.at(8) == ('a' + 8));

  map.print();

  // Test maplist iterators

  cout << "map.begin() = " << ((map.begin()))->first << ", " << (*(map.begin())).second << endl;
  cout << "map.begin() = " << (*(map.begin())).first << ", " << (*(map.begin())).second << endl;
  cout << "map.begin() = " << (*(map.begin())).first << ", " << (*(map.begin())).second << endl;
  cout << "map.begin() = " << (*(map.begin())).first << ", " << (*(map.begin())).second << endl;

  auto m_it = map.begin();

  cout << "m_it = " << (*m_it).first << ", " << (*m_it).second << endl;
  cout << "m_it = " << (*m_it).first << ", " << (*m_it).second << endl;
  cout << "m_it = " << (*m_it).first << ", " << (*m_it).second << endl;
  cout << "m_it = " << (*m_it).first << ", " << (*m_it).second << endl;

  /*
  map.end();
  cs540::Map<int, char>::Iterator m_it(map.begin());
  ++m_it;
  cout << "map.begin() = " << (*m_it).first << ", " << (*m_it).second << endl;
  //++m_it;
  cout << "map.begin() = " << (*m_it).first << ", " << (*m_it).second << endl;
  cout << "map.begin() = " << (*m_it).first << ", " << (*m_it).second << endl;
  cout << "map.begin() = " << (*m_it).first << ", " << (*m_it).second << endl;

  map.print();
  for(int i = 0; i < 10; i++) {
    cout << "Incrementing iterator to " << (*m_it).first << endl;
    m_it++;
  }

  map.print();
  for(int i = 0; i < 5; i++) {
    m_it--;
    cout << "Decrementing iterator to " << (*m_it).first << endl;
  }
  (*m_it).second = '!';

  cs540::Map<int, char>::Iterator begin = map.begin();
  auto copy_it = cs540::Map<int, char>::Iterator(begin);
  //cout << copy_it->first << endl;

  // Testing insertion in between other elements
  for(int i = 1; i < 20; i+=2) {
    assert(map.insert({i, 'a' + i}).second);
  }

  assert(map.size() == 20);
  
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

  // Delete after find
  map.erase(map.find(5));

  assert(map.size() == 9);

  // Test deletions of rest of elements with iterators
  while(!map.empty()) {
    map.erase(map.begin());
  }

  assert(map.size() == 0);
  */
  return 0;
}
