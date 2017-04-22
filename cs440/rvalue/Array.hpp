#ifndef _ARRAY_HPP_
#define _ARRAY_HPP_

#include <initializer_list>
#include <iostream>

namespace cs540 {

  class Array {
    public:
      Array(std::initializer_list<MyInt> init) : size(init.size()), data{new MyInt[size]} {
        int cur = 0;
        for(auto elem : init) {
          data[cur++] = elem;
        }
      }

      void move_performance_test() {
      }

      friend std::ostream& operator<< (std::ostream& stream, const Array& array) {
        for(std::size_t i = 0; i < array.size; i++) {
          stream << array.data[i] << ((i + 1 == array.size) ? "" : ", ");
        }
        return stream;
      }

    private:
      std::size_t size;
      MyInt *data;

  }; // class Array

} // namespace cs540

#endif
