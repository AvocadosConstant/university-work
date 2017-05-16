#ifndef _ARRAY_HPP_
#define _ARRAY_HPP_

#include <iostream>
#include <stdlib.h>

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

      /** Default Constructor */
      Array();

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
        bool operator==(const FirstDimensionMajorIterator &lhs, const FirstDimensionMajorIterator &rhs);
        bool operator!=(const FirstDimensionMajorIterator &lhs, const FirstDimensionMajorIterator &rhs);
        FirstDimensionMajorIterator &operator++();
        FirstDimensionMajorIterator operator++(int);
        T &operator*() const;
      }; // class FirstDimensionMajorIterator

      class LastDimensionMajorIterator {
        LastDimensionMajorIterator();
        LastDimensionMajorIterator(const LastDimensionMajorIterator &other);
        LastDimensionMajorIterator &operator=(const LastDimensionMajorIterator &other);
        bool operator==(const LastDimensionMajorIterator &lhs, const LastDimensionMajorIterator &rhs);
        bool operator!=(const LastDimensionMajorIterator &lhs, const LastDimensionMajorIterator &rhs);
        LastDimensionMajorIterator &operator++();
        LastDimensionMajorIterator operator++(int);
        T &operator*() const;
      }; // class LastDimensionMajorIterator

  }; // class Array
} // namespace cs540
#endif
