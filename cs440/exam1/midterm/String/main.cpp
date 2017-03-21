#include "String.hpp"
#include <iostream>



// Must have no leaks, or other memory errors.



int
main() {

    String s1("hello");
    String s2{s1};

    std::cout << s2 << std::endl;

    s2 = "goodbye";

    std::cout << s2 << std::endl;

    s2 = s1 + " goodbye";

    std::cout << s2 << std::endl;
}
