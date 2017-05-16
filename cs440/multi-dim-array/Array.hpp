#ifndef _ARRAY_HPP_
#define _ARRAY_HPP_

#include <iostream>
#include <stdlib.h>
#include <cstdarg>
#include <vector>
#include <cassert>

namespace cs540 {
  class OutOfRange : public std::exception {
    const char * what () const throw () {
      return "cs540 Out of Range Exception";
    }
  }; // class OutOfRange


  template <typename T, std::size_t... Dims> class Array {
    public:
      typedef T ValueType;
      class FirstDimensionMajorIterator;
      class LastDimensionMajorIterator;

      std::vector<std::size_t> dims;

      /** Default Constructor */
      Array() : dims{Dims...} {
        for(auto dim : dims) {
          assert(dim != 0);
          // TODO: Figure out how to do
          // static_assert(dim != 0, "Dimension cannot be <= 0!");
          std::cerr << dim << " ";
        }
        std::cerr << std::endl;
      }

      /** Copy Constructor */
      Array(const Array&);

      /** Templated Copy Constructor */
      template <typename U>
      Array(const Array<U, Dims...> &);

      /** Assignment Operator */
      Array &operator=(const Array &);

      /** Templated Assignment Operator */
      template <typename U>
      Array &operator=(const Array<U, Dims...> &);

      /** Index Operator */
      /*
      T &operator[size_t i_1][size_t i_2]...[size_t i_D];
      const T &operator[size_t i_1][size_t i_2]...[size_t i_D] const;
      */
      T &operator[](std::size_t i);

      const T &operator[](std::size_t i) const;


      ////////////////
      // ITERATORS //
      //////////////
      FirstDimensionMajorIterator fmbegin();

      FirstDimensionMajorIterator fmend();

      LastDimensionMajorIterator lmbegin();

      LastDimensionMajorIterator lmend();

      class FirstDimensionMajorIterator {
        FirstDimensionMajorIterator();
        FirstDimensionMajorIterator(const FirstDimensionMajorIterator &other);
        FirstDimensionMajorIterator &operator=(const FirstDimensionMajorIterator &other);
        FirstDimensionMajorIterator &operator++();
        FirstDimensionMajorIterator operator++(int);
        T &operator*() const;
      }; // class FirstDimensionMajorIterator

      class LastDimensionMajorIterator {
        LastDimensionMajorIterator();
        LastDimensionMajorIterator(const LastDimensionMajorIterator &other);
        LastDimensionMajorIterator &operator=(const LastDimensionMajorIterator &other);
        LastDimensionMajorIterator &operator++();
        LastDimensionMajorIterator operator++(int);
        T &operator*() const;
      }; // class LastDimensionMajorIterator

  }; // class Array

  /*
  template <typename T, std::size_t... Dims>
  bool operator==(const typename Array<T, Dims>::FirstDimensionMajorIterator &lhs, const Array<T, Dims>::FirstDimensionMajorIterator &rhs);

  template <typename T, std::size_t... Dims>
  bool operator!=(const Array<T, Dims>::FirstDimensionMajorIterator &lhs, const Array<T, Dims>::FirstDimensionMajorIterator &rhs);

  template <typename T, std::size_t... Dims>
  bool operator==(const Array<T, Dims>::LastDimensionMajorIterator &lhs, const Array<T, Dims>::LastDimensionMajorIterator &rhs);

  template <typename T, std::size_t... Dims>
  bool operator!=(const Array<T, Dims>::LastDimensionMajorIterator &lhs, const Array<T, Dims>::LastDimensionMajorIterator &rhs);
  */

} // namespace cs540
#endif
