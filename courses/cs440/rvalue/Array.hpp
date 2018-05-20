#ifndef _ARRAY_HPP_
#define _ARRAY_HPP_

#include <initializer_list>
#include <iostream>
#include <chrono>
#include <functional>
#include <cassert>

namespace cs540 {

  /** Handles timing of functions for performance testing */
  template<typename> struct Decorator;
  template <typename R, typename... Args>
    struct Decorator<R(Args...)> {
      Decorator(std::function<R(Args...)> f) : f_(f) {}

      /** Returns elapsed time of function in nanoseconds */
      std::chrono::nanoseconds operator()(Args... args) {
        std::chrono::time_point<std::chrono::system_clock> start, end;
        start = std::chrono::system_clock::now();
        f_(args...);
        end = std::chrono::system_clock::now();
        std::chrono::nanoseconds elapsed_nanos =
          std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        std::cerr << "\t> Executed in " << elapsed_nanos.count()
          << " nanoseconds." << std::endl;
        return elapsed_nanos;
      }
      std::function<R(Args...)> f_;
    };
  template <typename R, typename... Args>
  Decorator<R(Args...)> makeDecorator(R (*f)(Args...)) {
    return Decorator<R(Args...)>(std::function<R(Args...)>(f));
  }

  class Array {
    public:
      Array(std::initializer_list<MyInt> init) : size(init.size()), data{new MyInt[size]} {
        int cur = 0;
        for(auto elem : init) {
          data[cur++] = elem;
        }
      }

      /** Copy Constructor */
      Array(const Array& that) {
        size = that.size;
        data = new MyInt[size];

        for(std::size_t i = 0; i < that.size; i++) {
          data[i] = MyInt{that.data[i]};
        }
      }

      /** Move Constructor */
      Array(Array &&that) : size(that.size), data(that.data) {
        that.data = nullptr;
      }

      /** Destructor */
      ~Array() { if(data) delete[] data; }

      /** Copy assignment operator */
      Array& operator= (const Array& that) {
        if(this == &that) return *this;
        size = that.size;
        delete[] data;
        data = new MyInt[size];

        for(std::size_t i = 0; i < that.size; i++) {
          data[i] = MyInt{that.data[i]};
        }
        return *this;
      }

      /** Move assignment operator */
      Array& operator= (Array&& that) {
        if(this == &that) return *this;
        size = that.size;
        delete[] data;
        data = that.data;
        that.data = nullptr;
        return *this;
      }

      friend std::ostream& operator<< (std::ostream& stream, const Array& array) {
        for(std::size_t i = 0; i < array.size; i++) {
          stream << array.data[i] << ((i + 1 == array.size) ? "" : ", ");
        }
        return stream;
      }

      /** Verifies performance of move operations */
      static void move_performance_test() {
        std::cerr << std::endl << "____________________________"
          << std::endl << "RUNNING PERFORMANCE TESTS..." << std::endl << std::endl;

        Array seed{
          0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
          0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
          0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
          0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
          0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
          0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
          0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
          0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
          0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
          0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        Array seed2{seed};
        int times = 100000;

        std::chrono::nanoseconds copy_ctor_elapsed = makeDecorator(copy_ctor_test)(seed, times);
        std::chrono::nanoseconds move_ctor_elapsed = makeDecorator(move_ctor_test)(seed, times);
        // Assert that move performs better
        assert(copy_ctor_elapsed.count() > move_ctor_elapsed.count());
        std::cerr << "Move constructor is faster than copy constructor!" << std::endl << std::endl;

        std::chrono::nanoseconds copy_assign_elapsed = makeDecorator(copy_assign_test)(seed2, times);
        std::chrono::nanoseconds move_assign_elapsed = makeDecorator(move_assign_test)(seed2, times);
        // Assert that move performs better
        assert(copy_assign_elapsed.count() > move_assign_elapsed.count());
        std::cerr << "Move assignment is faster than copy assignment!" << std::endl;

        std::cerr << std::endl << "END OF PERFORMANCE TEST"
          << std::endl << "____________________________" << std::endl;
      }

      /** Tests the copy constructor <times> times */
      static void copy_ctor_test(Array &seed, int times) {
        std::cerr << "COPY CTOR TEST" << std::endl;
        for(int i = 0; i < times; i++) Array a1{seed};
      }

      /** Tests the move constructor <times> times */
      static void move_ctor_test(Array &seed, int times) {
        std::cerr << "MOVE CTOR TEST" << std::endl;
        for(int i = 0; i < times; i++) Array a1{std::move(seed)};
      }

      /** Tests copy assignment <times> times */
      static void copy_assign_test(Array &seed, int times) {
        std::cerr << "COPY CTOR TEST" << std::endl;
        for(int i = 0; i < times; i++) {
          Array a1{1};
          a1 = seed;
        }
      }

      /** Tests move assignment <times> times */
      static void move_assign_test(Array &seed, int times) {
        std::cerr << "MOVE CTOR TEST" << std::endl;
        for(int i = 0; i < times; i++) {
          Array a1{1};
          a1 = std::move(seed);
        }
      }

    private:
      std::size_t size;
      MyInt *data;

  }; // class Array

} // namespace cs540

#endif
