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

  //cs540::Map<int, char>::SkipList::Iterator it = skip.end();
  skip.end();
  cs540::Map<int, char>::SkipList::Iterator it = skip.end();
  return 0;
}
