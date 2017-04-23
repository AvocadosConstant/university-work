#include <iostream>
#include <cassert>
#include "SharedPtr.hpp"

int main() {
  cs540::SharedPtr<int> sp(new int(5));

  //assert(sp.use_count() == 1);
  std::cout << sp.use_count() << std::endl;

  auto sp2 = sp;

  //assert(sp.use_count() == 2);
  std::cout << sp.use_count() << std::endl;

  {
    cs540::SharedPtr<int> sp3(sp2);

    //assert(sp.use_count() == 3);
    std::cout << sp.use_count() << std::endl;
  }

  //assert(sp.use_count() == 2);
  std::cout << sp.use_count() << std::endl;
}
