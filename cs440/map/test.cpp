#include "Map.hpp"

#include <iostream>
#include <cassert>

using std::cout;
using std::endl;

struct TimKey {
  int data;
  TimKey() = delete;
};
bool operator==(const TimKey &lhs, const TimKey &rhs) {
  return lhs.data == rhs.data;
}
bool operator<(const TimKey &lhs, const TimKey &rhs) {
  return lhs.data < rhs.data;
}

struct TimValue {
  char data;
  TimValue() = delete;
};
bool operator==(const TimValue &lhs, const TimValue &rhs) {
  return lhs.data == rhs.data;
}
bool operator<(const TimValue &lhs, const TimValue &rhs) {
  return lhs.data < rhs.data;
}

int main() {

  cs540::Map<int, char> map{{1, 'a'}, {2, 'b'}, {3, 'c'}, {4, 'd'}};
  //map.print();

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

  //map.print();

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

  // Test modifying with [] after find
  it = map.find(8);
  (*it).second = '!';
  assert(map[8] == '!');

  // Make sure pre and post fix increments and decrements work like they should
  assert((++it)->first == 10);
  assert((--it)->first == 8);
  assert((it++)->first == 8);
  assert((it++)->first == 10);

  // Copying iterators
  cs540::Map<int, char>::Iterator begin = map.begin();
  auto copy_it = cs540::Map<int, char>::Iterator(begin);
  cout << copy_it->first << endl;

  //map.print();

  // Testing insertion in between other elements
  for(int i = 1; i < 20; i+=2) {
    assert(map.insert({i, 'a' + i}).second);
  }

  assert(map.size() == 20);
  //map.print();
  
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
  //map.print();

  // Delete after find
  map.erase(map.find(5));

  assert(map.size() == 9);
  //map.print();

  cout << "Printing in order with const iterator: ";
  cs540::Map<int, char>::ConstIterator c_it = map.begin();
  for(; c_it != map.end(); c_it++) {
    cout << (*c_it).second << " ";
  } cout << endl;

  cout << "Printing in reverse with ReverseIterator: ";
  cs540::Map<int, char>::ReverseIterator r_it = map.rbegin();
  for(; r_it != map.rend(); r_it++) {
    cout << (*r_it).second << " ";
  } cout << endl;

  c_it = map.find(3);
  cout << "Printing in order with const iterator starting from key 3 and value " << (*c_it).second << ": ";
  for(; c_it != map.end(); c_it++) {
    cout << (*c_it).second << " ";
  } cout << endl;

  it = map.find(7);
  assert((*it).second == 'a' + 7);
  cout << "Modifying map[7] = &" << endl;
  map[7] = '&';
  cout << "Printing in order with normal iterator starting from key 7 and value " << (*it).second << ": ";
  for(; it != map.end(); it++) {
    cout << (*it).second << " ";
  } cout << endl;
  //map.print();

  cs540::Map<int, char> map_copy;
  map_copy = map;
  assert(map_copy == map);
  cout << "Printing a different map that was operator= assigned to" << endl;
  //map_copy.print();

  cout << "Modifying map_copy[7] to '?'" << endl;
  map_copy[7] = '?';
  assert(!(map_copy[7] == map[7]));
  assert(map_copy != map);

  cout << "Printing map_copy" << endl;
  //map_copy.print();
  cout << "Printing map, should still be '&' not '?'" << endl;
  //map.print();

  map[3] = 'a';
  assert(map < map_copy);
  assert(!(map_copy < map));
  assert(!(map_copy == map));

  // Test deletions of all elements but that of key 9 
  it = map.find(9);
  cs540::Map<int, char>::Iterator it2 = map.begin();
  while(map.size() > 1) {
    if(it2 != it) {
      map.erase(it2++);
    } else it2++;
  }
  assert(map.size() == 1);
  //map.print();

  // Verify non invalidated iterator
  assert((*it).first == 9);
  assert((*it).second == 'a' + 9);

  map.erase(it);

  assert(map.size() == 0);
  //map.print();


  //-------------------------- Test Map<TimKey, TimValue>
  cout << "\n\n------------------------------------\nTesting Map<TimKey, TimValue>" << endl;

  TimKey tk{1};
  TimValue tv{'a'};

  cout << tk.data << ", " << tv.data << endl;

  cs540::Map<TimKey, TimValue> t_map{
    {TimKey{1}, TimValue{'a'}},
    {TimKey{2}, TimValue{'b'}},
    {TimKey{3}, TimValue{'c'}},
    {TimKey{4}, TimValue{'d'}}
  };

  // Test size and empty
  assert(t_map.size() == 4);
  assert(!t_map.empty());

  t_map.clear();

  // Test size and empty
  assert(t_map.size() == 0);
  assert(t_map.empty());
  assert(t_map.begin() == t_map.end());

  // Test insertion
  for(int i = 0; i < 20; i+=2) {
    char val = 'a' + i;
    t_map.insert({TimKey{i}, TimValue{val}});
  }

  // Test size and empty after insertions
  assert(t_map.size() == 10);
  assert(!t_map.empty());

  // Test find and at
  assert(t_map.find(TimKey{6})->second == (TimValue{'a' + 6}));
  assert(t_map.at(TimKey{8}) == (TimValue{'a' + 8}));

  // Test t_maplist iterators
  t_map.end();
  cs540::Map<TimKey, TimValue>::Iterator t_it = t_map.begin();
  for(int i = 0; i < 10; i++) {
    cout << "Incrementing iterator to " << (*t_it).first.data << endl;
    t_it++;
  }
  for(int i = 0; i < 10; i++) {
    t_it--;
    cout << "Decrementing iterator to " << (*t_it).first.data << endl;
  }
  for(int i = 0; i < 10; i++) {
    cout << "Incrementing iterator to " << (*t_it).first.data << endl;
    ++t_it;
  }
  for(int i = 0; i < 10; i++) {
    --t_it;
    cout << "Decrementing iterator to " << (*t_it).first.data << endl;
  }

  // Test iterator equality
  assert(t_it == t_map.begin());
  assert(t_it != t_map.end());

  // Test modifying with [] after find
  t_it = t_map.find(TimKey{8});
  (*t_it).second = TimValue{'!'};
  assert(t_map[TimKey{8}] == TimValue{'!'});

  // Make sure pre and post fix increments and decrements work like they should
  assert((++t_it)->first == TimKey{10});
  assert((--t_it)->first == TimKey{8});
  assert((t_it++)->first == TimKey{8});
  assert((t_it++)->first == TimKey{10});

  // Copying iterators
  cs540::Map<TimKey, TimValue>::Iterator t_begin = t_map.begin();
  auto t_copy_it = cs540::Map<TimKey, TimValue>::Iterator(t_begin);
  cout << t_copy_it->first.data << endl;

  // Testing insertion in between other elements
  for(int i = 1; i < 20; i+=2) {
    char val = 'a' + i;
    assert(t_map.insert({TimKey{i}, TimValue{val}}).second);
  }

  assert(t_map.size() == 20);
  
  // Printing in order using ranged based for loop
  for(auto i : t_map) {
    cout << i.second.data << " ";
  } cout << endl;

  // Test insertion of duplicate key
  assert(!t_map.insert({TimKey{5}, TimValue{'?'}}).second);

  // Test deletions of all even elements
  for(int i = 0; i < 20; i+=2) {
    t_map.erase(TimKey{i});
  }

  assert(t_map.size() == 10);

  // Delete after find
  t_map.erase(t_map.find(TimKey{5}));

  assert(t_map.size() == 9);

  cout << "Printing in order with const iterator: ";
  cs540::Map<TimKey, TimValue>::ConstIterator t_c_it = t_map.begin();
  for(; t_c_it != t_map.end(); t_c_it++) {
    cout << (*t_c_it).second.data << " ";
  } cout << endl;

  cout << "Printing in reverse with ReverseIterator: ";
  cs540::Map<TimKey, TimValue>::ReverseIterator t_r_it = t_map.rbegin();
  for(; t_r_it != t_map.rend(); t_r_it++) {
    cout << (*t_r_it).second.data << " ";
  } cout << endl;

  t_c_it = t_map.find(TimKey{3});
  cout << "Printing in order with const iterator starting from key 3 and value " << (*t_c_it).second.data << ": ";
  for(; t_c_it != t_map.end(); t_c_it++) {
    cout << (*t_c_it).second.data << " ";
  } cout << endl;

  t_it = t_map.find(TimKey{7});
  assert((*t_it).second == TimValue{'a' + 7});
  cout << "Modifying t_map[7] = &" << endl;
  t_map[TimKey{7}] = TimValue{'&'};
  cout << "Printing in order with normal iterator starting from key 7 and value " << (*t_it).second.data << ": ";
  for(; t_it != t_map.end(); t_it++) {
    cout << (*t_it).second.data << " ";
  } cout << endl;

  cs540::Map<TimKey, TimValue> t_map_copy;
  t_map_copy = t_map;
  assert(t_map_copy == t_map);

  cout << "Modifying t_map_copy[7] to '?'" << endl;
  t_map_copy[TimKey{7}] = TimValue{'?'};
  assert(!(t_map_copy[TimKey{7}] == t_map[TimKey{7}]));
  assert(t_map_copy != t_map);

  t_map[TimKey{3}] = TimValue{'a'};
  assert(t_map < t_map_copy);
  assert(!(t_map_copy < t_map));
  assert(!(t_map_copy == t_map));

  // Test deletions of all elements but that of key 9 
  t_it = t_map.find(TimKey{9});
  cs540::Map<TimKey, TimValue>::Iterator t_it2 = t_map.begin();
  while(t_map.size() > 1) {
    if(t_it2 != t_it) {
      t_map.erase(t_it2++);
    } else t_it2++;
  }
  assert(t_map.size() == 1);

  // Verify non invalidated iterator
  assert((*t_it).first == TimKey{9});
  assert((*t_it).second == TimValue{'a' + 9});

  t_map.erase(t_it);

  assert(t_map.size() == 0);
  
  return 0;
}
