#include "List.hpp"
#include <iostream>

class MyClass {
        friend std::ostream &operator<<(std::ostream &os, const MyClass &obj);
    public:
        MyClass(double v) : val(v) {}
    private:
        double val;
};

std::ostream &
operator<<(std::ostream &os, const MyClass &obj) {
    return os << obj.val;
}

int
main() {

    // Must work with classes with no default ctor.
    List<MyClass> l1;
    std::cout << "Print 1" << std::endl;
    // Range-base for. Need begin(), end(), != and * (deref) for this to work.
    for (const auto &e : l1) {
        std::cout << e << std::endl;
    }

    l1.push_front(MyClass(-1));
    std::cout << "Print 2" << std::endl;
    for (const auto &e : l1) {
        std::cout << e << std::endl;
    }

    l1.erase(l1.begin());
    std::cout << "Print 3" << std::endl;
    for (const auto &e : l1) {
        std::cout << e << std::endl;
    }

    l1.push_back(MyClass{100});
    std::cout << "Print 4" << std::endl;
    for (const auto &e : l1) {
        std::cout << e << std::endl;
    }

    l1.erase(l1.begin());
    std::cout << "Print 5" << std::endl;
    for (const auto &e : l1) {
        std::cout << e << std::endl;
    }

    // Appends.
    l1.push_back(MyClass(1));
    l1.push_back(MyClass(2));
    l1.push_back(MyClass(3));
    l1.push_back(MyClass(4));
    l1.push_back(MyClass(5));
    l1.push_back(MyClass(6));
    l1.push_back(MyClass(7));
    l1.push_back(MyClass(8));
    l1.push_back(MyClass(9));
    auto it1 = l1.begin();
    ++it1;
    ++it1; // Points to the 3rd element.
    // Inserts before the 3rd element.  Returns iterator pointing to inserted element.
    auto it2 = l1.insert(it1, MyClass(2.5));
    *it2++ = 2.9;
    *++it2 = 4.1;

    it2++;

    l1.erase(it2);

    // Make this give a compile-time error.
    // List<MyClass> l2(l1);

    std::cout << "Print 6" << std::endl;
    for (const auto &e : l1) {
        std::cout << e << std::endl;
    }
}
