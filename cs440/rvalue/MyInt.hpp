#ifndef MY_INT_HPP
#define MY_INT_HPP



#include <iostream>

class MyInt {
        friend std::ostream &operator<<(std::ostream &os, const MyInt &mi) {
            return os << mi.i;
        }
    public:
        MyInt() { count++; }
        MyInt(int i) : i(i) { count++; }
    private:
        int i;
    public:
        static std::size_t count;
};



#endif
