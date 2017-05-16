#include "Array.hpp"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
int main() {
  //Define a 2X3X4 array of integers.  Elements are uninitialized.

  cs540::Array<int, 2, 3, 4> a, b;
  cs540::Array<short, 2, 3, 4> c;

  //cs540::Array<int, 0> e1; // This line must cause a compile-time error.
  // Initialize.
  int counter = 0;
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 3; j++) {
      for (int k = 0; k < 4; k++) {
        a[i][j][k] = b[i][j][k] = counter;
        c[i][j][k] = counter;
        counter++;
      }
    }
  }

  // Range-checked indexing.
  a[0][0][0] = 1234;
  a[1][1][1] = a[0][0][0];
  a[0][2][3] = 5678; // Set the element in the first plane, 3rd row, and 4th column.
  try {
    a[0][3][0] = 1; // Out of range, throws.
    assert(false);
  } catch (cs540::OutOfRange) {
  }

  /*

  a = a; // Self-assignment must be a no-op.
  b = a; // Okay, as long as dimensions and type match exactly.
  a = c; // Member template constructor.

  const cs540::Array<int, 2, 3, 4> &const_ref(a);

  int i = const_ref[0][0][0]; // Okay.
  // const_ref[0][0][0] = 1; // Syntax error.

  // Iterate through in plane major order.  (Note: Normally, you would use
  // auto to simplify.)
  std::cout << "First dimension major (row-major in 2-D): " << std::endl;
  for (cs540::Array<int, 2, 3, 4>::FirstDimensionMajorIterator it = a.fmbegin(); it != a.fmend(); ++it) {
    std::cout << *it << std::endl;
  }

  // Iterate through in column major order.
  std::cout << "Last dimension major (column-major in 2-D): " << std::endl;
  for (cs540::Array<int, 2, 3, 4>::LastDimensionMajorIterator it = a.lmbegin(); it != a.lmend(); ++it) {
    std::cout << *it << std::endl;
  }

  */
}
#pragma GCC diagnostic pop
