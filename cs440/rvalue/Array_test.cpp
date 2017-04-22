#include "MyInt.hpp"
#include "Array.hpp"
#include <iostream>
#include <utility>
#include <assert.h>



int main() {

    std::size_t before;

    // Creates an array. The contained element should be of type MyInt.
    cs540::Array a1{1, 2, 3};
    std::cout << a1 << std::endl;

    // Both a2 and a1 should be identical after the copy ctor.
    std::cout << "Testing copy ctor: " << std::endl;
    before = MyInt::count;
    cs540::Array a2{a1};
    // At least 3 MyInt objects should be created.
    assert(MyInt::count - before >= 3);
    std::cout << "    " << a2 << std::endl;
    std::cout << "    " << a1 << std::endl;

    std::cout << "Testing move ctor: " << std::endl;
    before = MyInt::count;
    cs540::Array a3{std::move(a2)};
    // No new MyInt objects should be created here.
    assert(MyInt::count - before == 0);
    std::cout << "    " << a3 << std::endl;

    std::cout << "Testing copy assignment: " << std::endl;
    cs540::Array a4{1};
    before = MyInt::count;
    a4 = a1;
    // At least 3 MyInt objects should be created.
    assert(MyInt::count - before >= 3);
    std::cout << "    " << a4 << std::endl;
    std::cout << "    " << a1 << std::endl;

    std::cout << "Testing move assignment: " << std::endl;
    cs540::Array a5{1};
    before = MyInt::count;
    a5 = std::move(a4);
    // No new MyInt objects should be created here.
    assert(MyInt::count - before == 0);
    std::cout << "    " << a5 << std::endl;

    cs540::Array::move_performance_test();
}
