#ifndef _ARRAY_HPP_
#define _ARRAY_HPP_

#include <iostream>
#include <stdlib.h>
#include <cstdarg>
#include <array>
#include <cassert>

#define NumDims sizeof...(Dims) + 1

namespace cs540 {
  class OutOfRange : public std::exception {
    const char * what () const throw () {
      return "cs540 Out of Range Exception";
    }
  }; // class OutOfRange


  template <typename T, std::size_t Dim1, std::size_t... Dims> class Array {
    public:
      using ValueType = T;
      class FirstDimensionMajorIterator;
      class LastDimensionMajorIterator;
      using NextLevel = Array<T, Dims...>;

      std::array<std::size_t, NumDims> dims_;
      std::array<NextLevel, Dim1> elems_;

      /** Default Constructor */
      Array() : dims_({Dim1, Dims...}) {
        static_assert(Dim1 != 0, "Dimension cannot be 0!");
      }

      /** Copy Constructor */
      Array(const Array &other) {
        // Check that dims_ == other.dims_;


        // Copy elems_
        for(std::size_t i = 0; i < Dim1; i++) {
          elems_[i] = other.elems_[i];
        }
      }

      /** Templated Copy Constructor */
      template <typename U>
      Array(const Array<U, Dim1, Dims...> &other) {
        dims_ = other.dims_;
        elems_ = other.elems_;
      }

      /** Assignment Operator */
      Array &operator=(const Array &) {
        std::cerr << "Assignment Op" << std::endl;
        return *this;
      }

      /** Templated Assignment Operator */
      template <typename U>
      Array &operator=(const Array<U, Dim1, Dims...> &) {
        std::cerr << "Templated Assignment Op" << std::endl;
        return *this;
      }

      /** Index Operator */
      NextLevel &operator[](std::size_t i) {
        if(i < 0 || i >= Dim1) {
          throw cs540::OutOfRange();
        }
        return elems_[i];
      }

      const NextLevel &operator[](std::size_t i) const {
        if(i < 0 || i >= Dim1) {
          throw cs540::OutOfRange();
        }
        return elems_[i];
      }

      T &get(std::array<std::size_t, NumDims> indices) {
        if(indices[0] < 0 || indices[0] >= Dim1) {
          throw cs540::OutOfRange();
        }
        std::array<std::size_t, NumDims - 1> new_indices;
        for(std::size_t i = 0; i < new_indices.size(); i++) {
          new_indices[i] = indices[i + 1];
        }
        return elems_[indices[0]].get(new_indices);
      }

      const T get(std::array<std::size_t, NumDims> indices) const {
        if(indices[0] < 0 || indices[0] >= Dim1) {
          throw cs540::OutOfRange();
        }
        std::array<std::size_t, NumDims - 1> new_indices;
        for(std::size_t i = 0; i < new_indices.size(); i++) {
          new_indices[i] = indices[i + 1];
        }
        return elems_[indices[0]].get(new_indices);
      }

      ////////////////
      // ITERATORS //
      //////////////
      FirstDimensionMajorIterator fmbegin() {
        return FirstDimensionMajorIterator(dims_, std::array<std::size_t, NumDims>(), this);
      }

      FirstDimensionMajorIterator fmend() {
        auto indices = dims_;
        for(auto &i : indices) {
          --i;
        }
        indices[0]++;
        return FirstDimensionMajorIterator(dims_, indices, this);
      }

      LastDimensionMajorIterator lmbegin() {
        return LastDimensionMajorIterator(dims_, std::array<std::size_t, NumDims>(), this);
      }

      LastDimensionMajorIterator lmend() {
        auto indices = dims_;
        for(auto &i : indices) {
          --i;
        }
        indices[dims_.size() - 1]++;
        return LastDimensionMajorIterator(dims_, indices, this);
      }

      struct Iterator {
        std::array<std::size_t, NumDims> dims_;
        std::array<std::size_t, NumDims> indices_;
        Array<T, Dim1, Dims...> *array_;

        Iterator() {}

        Iterator(
            std::array<std::size_t, NumDims> dims,
            std::array<std::size_t, NumDims> indices,
            Array<T, Dim1, Dims...> *array)
          : dims_(dims), indices_(indices), array_(array) {}

        Iterator(const FirstDimensionMajorIterator &other) : indices_(other.indices_) {}

        Iterator &operator=(const FirstDimensionMajorIterator &other) {
          indices_ = other.indices_;
        }

        T &operator*() const { return array_->get(indices_); }

        void print() {
          std::cerr << "Iter at ";
          for(auto i : indices_) {
            std::cerr << "[" << i << "]";
          }
          std::cerr << std::endl;
        }
      }; // struct Iterator

      struct FirstDimensionMajorIterator : public Iterator{
        FirstDimensionMajorIterator(
            std::array<std::size_t, NumDims> dims,
            std::array<std::size_t, NumDims> indices,
            Array<T, Dim1, Dims...> *array)
          : Iterator(dims, indices, array) {}

        friend bool operator==(const FirstDimensionMajorIterator &lhs, const FirstDimensionMajorIterator &rhs) {
          return lhs.indices_ == rhs.indices_;
        }

        friend bool operator!=(const FirstDimensionMajorIterator &lhs, const FirstDimensionMajorIterator &rhs) {
          return !(lhs == rhs);
        }

        FirstDimensionMajorIterator &operator++() {
          for(std::size_t i = this->indices_.size() - 1; i >= 0; i--) {
            if(this->indices_[i] < this->dims_[i] - 1) {
              this->indices_[i]++;
              return *this;
            } else {
              if(i != 0) this->indices_[i] = 0;
              else {
                // Incremented to end
                this->indices_ = this->dims_;
                for(std::size_t &i : this->indices_) {
                  --i;
                }
                this->indices_[0]++;
                return *this;
              }
            }
          }
          return *this;
        }

        FirstDimensionMajorIterator operator++(int) {
          FirstDimensionMajorIterator tmp(*this);
          operator++();
          return tmp;
        }
      }; // struct FirstDimensionMajorIterator

      struct LastDimensionMajorIterator : public Iterator{
        LastDimensionMajorIterator(
            std::array<std::size_t, NumDims> dims,
            std::array<std::size_t, NumDims> indices,
            Array<T, Dim1, Dims...> *array)
          : Iterator(dims, indices, array) {}

        friend bool operator==(const LastDimensionMajorIterator &lhs, const LastDimensionMajorIterator &rhs) {
          return lhs.indices_ == rhs.indices_;
        }

        friend bool operator!=(const LastDimensionMajorIterator &lhs, const LastDimensionMajorIterator &rhs) {
          return !(lhs == rhs);
        }

        LastDimensionMajorIterator &operator++() {
          for(std::size_t i = 0; i < this->indices_.size(); i++) {
            if(this->indices_[i] < this->dims_[i] - 1) {
              this->indices_[i]++;
              return *this;
            } else {
              if(i != this->indices_.size() - 1) this->indices_[i] = 0;
              else {
                // Incremented to end
                this->indices_ = this->dims_;
                for(std::size_t &i : this->indices_) {
                  --i;
                }
                this->indices_[this->indices_.size() - 1]++;
                return *this;
              }
            }
          }
          return *this;
        }

        LastDimensionMajorIterator operator++(int) {
          LastDimensionMajorIterator tmp(*this);
          operator++();
          return tmp;
        }
      }; // struct LastDimensionMajorIterator

  }; // class Array

  template <typename T, std::size_t Dim1> class Array<T, Dim1> {
    public:
      using ValueType = T;
      std::array<ValueType, Dim1> elems_;
      using FirstDimensionMajorIterator = typename std::array<ValueType, Dim1>::iterator;
      using LastDimensionMajorIterator = typename std::array<ValueType, Dim1>::iterator;

      Array() {
        static_assert(Dim1 != 0, "Dimension cannot be 0!");
      }

      /** Copy Constructor */
      Array(const Array &other) {
        elems_ = other.elems_;
      }

      /** Templated Copy Constructor */
      template <typename U>
      Array(const Array<U, Dim1> &other) {
        elems_ = other.elems_;
      }

      /** Assignment Operator */
      Array &operator=(const Array &) {
        std::cerr << "Assignment Op" << std::endl;
        return *this;
      }

      /** Templated Assignment Operator */
      template <typename U>
      Array &operator=(const Array<U, Dim1> &other) {
        std::cerr << "Templated Assignment Op" << std::endl;

        this->ValueType = other.ValueType;

        this->elems_ = other.elems_;

        return *this;
      }

      ValueType &operator[](std::size_t i) {
        if(i < 0 || i >= Dim1) throw cs540::OutOfRange();
        return elems_[i];
      }

      const ValueType &operator[](std::size_t i) const {
        if(i < 0 || i >= Dim1) throw cs540::OutOfRange();
        return elems_[i];
      }

      ValueType &get(std::array<std::size_t, 1> index) {
        if(index[0] < 0 || index[0] >= Dim1) {
          throw cs540::OutOfRange();
        }
        return elems_[index[0]];
      }

      const ValueType get(std::array<std::size_t, 1> index) const {
        if(index[0] < 0 || index[0] >= Dim1) {
          throw cs540::OutOfRange();
        }
        return elems_[index[0]];
      }

      FirstDimensionMajorIterator &fmbegin() {
        return elems_.begin();
      }

      FirstDimensionMajorIterator &fmend() {
        return elems_.end();
      }

      LastDimensionMajorIterator &lmbegin() {
        return elems_.begin();
      }

      LastDimensionMajorIterator &lmend() {
        return elems_.end();
      }
  };
} // namespace cs540
#endif
