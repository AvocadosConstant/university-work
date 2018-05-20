#include <iostream>
#include <stdlib.h>
#include <array>
#include <typeinfo>
#include <type_traits>

// 70 points

/* BEGIN YOUR CODE */

template <std::size_t I> struct Index {
  Index() {}
  int get(std::array<int, 5> a) {
    return a[I];
  }
};

// SUM() 
// Base case
template <typename T>
constexpr T sum(std::array<int, 5>, T first) {
  return first;
}

// Index + Index
template <typename T, std::size_t I1, std::size_t I2, typename... Ps>
constexpr T sum(std::array<int, 5> a, Index<I1> first, Index<I2> second, Ps... rest) {
  return sum(a, first.get(a) + second.get(a), rest...);
}

// T + Index
template <typename T, std::size_t I, typename... Ps>
constexpr T sum(std::array<int, 5> a, T first, Index<I> index, Ps... rest) {
  return sum(a, first + index.get(a), rest...);
}

// Index + T
template <typename T, std::size_t I, typename... Ps>
constexpr T sum(std::array<int, 5> a, Index<I> index, T second, Ps... rest) {
  return sum(a, index.get(a) + second, rest...);
}

template <typename T, typename U, typename... Ps>
constexpr T sum(std::array<int, 5> a, T first, U second, Ps... rest) {
  return sum(a, first + second, rest...);
}

template <typename T, typename U>
constexpr T sum(std::array<int, 5>, T first, U second) {
  return first + second;
}



// SUM 2
// Base case
template <typename T>
constexpr T sum2(std::array<int, 5>, T first) {
  return first;
}

// Index + Index
template <typename T, std::size_t I1, std::size_t I2, typename... Ps>
constexpr T sum2(std::array<int, 5> a, Index<I1> first, Index<I2> second, Ps... rest) {
  return sum2(a, first.get(a) + second.get(a), rest...);
}

// T + Index
template <typename T, std::size_t I, typename... Ps>
constexpr T sum2(std::array<int, 5> a, T first, Index<I> index, Ps... rest) {
  return sum2(a, first + index.get(a), rest...);
}

// Index + T
template <typename T, std::size_t I, typename... Ps>
constexpr T sum2(std::array<int, 5> a, Index<I> index, T second, Ps... rest) {
  return sum2(a, index.get(a) + second, rest...);
}

template <typename T, typename U, typename... Ps>
constexpr T sum2(std::array<int, 5> a, T first, U second, Ps... rest) {
  return sum2(a, first + second, rest...);
}

template <typename T, typename U>
constexpr T sum2(std::array<int, 5>, T first, U second) {
  return first + second;
}


/* END YOUR CODE */

class A {};
class B {};
class C {};

A operator+(const A &, const B &) { return A(); }
A operator+(const A &, const C &) { return A(); }

void operator+(const B &, const C &) {}

std::ostream &operator<<(std::ostream &os, const A &) {
    return os << "A";
}

int
main() {

    // Defines an array of 5 ints.
    std::array<int, 5> a{{1, 2, 3, 4, 5}};
    //std::cout << sum(a, Index<4>(), Index<3>(), 2) << std::endl;

    // Sum 1 and 2, a is unused.
    std::cout << sum(a, 1, 2) << std::endl;

    // Sum 1 and 2 and the 3rd element of a.
    std::cout << sum(a, 1, 2, Index<2>()) << std::endl;

    // Sum 4th element of a and 1 and 2 and the 3rd element of a.
    std::cout << sum(a, Index<3>(), 1, 2, Index<2>()) << std::endl;

    // Should work for arbitrary.

    // For 15 points extra credit, make the associativity correct.
    std::cout << sum2(a, A()) << std::endl;
    std::cout << sum2(a, A(), B()) << std::endl;
    std::cout << sum2(a, A(), B(), C()) << std::endl;
    std::cout << sum2(a, A(), B(), B(), C()) << std::endl;
    
    std::cout << sum2(a, 1, 2, Index<1>()) << std::endl;
}
