#ifndef _ARRAY_HPP_
#define _ARRAY_HPP_

#include <iostream>
#include <stdlib.h>
#include <cstdarg>
#include <array>
#include <cassert>

namespace cs540 {
  class OutOfRange : public std::exception {
    const char * what () const throw () {
      return "cs540 Out of Range Exception";
    }
  }; // class OutOfRange


  template <typename T, std::size_t Dim1, std::size_t... Dims> class Array {
    public:
      typedef T ValueType;
      class FirstDimensionMajorIterator;
      class LastDimensionMajorIterator;
      using NextLevel = Array<T, Dims...>;

      std::array<NextLevel, Dim1> elems;

      /** Default Constructor */
      Array() {
        static_assert(Dim1 != 0, "Dimension cannot be 0!");
      }

      ///** Copy Constructor */
      //Array(const Array&);

      ///** Templated Copy Constructor */
      //template <typename U>
      //Array(const Array<U, Dims...> &);

      ///** Assignment Operator */
      //Array &operator=(const Array &);

      ///** Templated Assignment Operator */
      //template <typename U>
      //Array &operator=(const Array<U, Dims...> &);

      /** Index Operator */
      /*
      T &operator[size_t i_1][size_t i_2]...[size_t i_D];
      const T &operator[size_t i_1][size_t i_2]...[size_t i_D] const;
      */
      NextLevel &operator[](std::size_t i) {
        if(i < 0 || i >= Dim1) {
          throw cs540::OutOfRange();
        }
        return elems[i];
      }

      const NextLevel &operator[](std::size_t i) const {
        if(i < 0 || i >= Dim1) {
          throw cs540::OutOfRange();
        }
        return elems[i];
      }


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

  template <typename T, std::size_t Dim1> class Array<T, Dim1> {
    public:
      std::array<T, Dim1> elems;

      Array() {
        static_assert(Dim1 != 0, "Dimension cannot be 0!");
      }

      T &operator[](std::size_t i) {
        return elems[i];
      }

      const T &operator[](std::size_t i) const {
        return elems[i];
      }
  };


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
