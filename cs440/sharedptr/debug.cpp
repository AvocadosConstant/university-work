#include <iostream>
#include <cassert>
#include "SharedPtr.hpp"

using namespace cs540;

class Base1 {
  protected:
    Base1() : derived_destructor_called(false) {
      printf("Base1::Base1()\n");
    }
  private:
    Base1(const Base1 &); // Disallow.
    Base1 &operator=(const Base1 &); // Disallow.
  protected:
    ~Base1() {
      printf("Base1::~Base1()\n");
      assert(derived_destructor_called);
    }
  protected:
    bool derived_destructor_called;
};

class Derived : public Base1 {
  friend void inheritance_test();
  private:
  Derived() {}
  Derived(const Derived &); // Disallow.
  Derived &operator=(const Derived &); // Disallow.
  public:
  ~Derived() {
    printf("Derived::~Derived()\n");
    derived_destructor_called = true;
  }
  int value;
};

void inheritance_test() {
  // Test deleting through original class.
  {
    // Base1 created directly with Derived *.
    {
      SharedPtr<Base1> sp(new Derived);
      {
        // Test copy constructor.
        SharedPtr<Base1> sp2(sp);
      }
    }

    // Base1 assigned from SharedPtr<Derived>.
    {
      SharedPtr<Base1> sp2;
      {
        SharedPtr<Derived> sp(new Derived);
        // Test template copy constructor.
        SharedPtr<Base1> sp3(sp);
        sp2 = sp;
        sp2 = sp2;
      }
    }
  }
}

void int_test() {
  SharedPtr<int> sp(new int(5));

  assert(sp.use_count() == 1);
  std::cout << sp.use_count() << std::endl;

  auto sp2 = sp;

  assert(sp.use_count() == 2);
  std::cout << sp.use_count() << std::endl;

  {
    SharedPtr<int> sp3(sp2);

    assert(sp.use_count() == 3);
    std::cout << sp.use_count() << std::endl;
  }

  assert(sp.use_count() == 2);
  std::cout << sp.use_count() << std::endl;

  SharedPtr<int> sp4(std::move(sp2));

  // Move construction shouldn't alter ref count
  assert(sp.use_count() == 2);
  std::cout << sp.use_count() << std::endl;
}

int main() {
  //int_test();
  inheritance_test();
}
