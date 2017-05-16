# CS 540 Programming Assignment 4: Metaprogramming

### Due May 19th, 11:59 PM

_(This document was last modified on Tuesday, May 9, 2017 at 11:29:11 AM.)_

* * *

All classes should be in the `cs540` namespace. Your code must work with test classes that are not in the `cs540` namespace, however. Your code should not have any memory errors or leaks as reported by `valgrind`. Your code should compile and run on the `remote.cs.binghamton.edu` cluster. Your code must not have any hard-coded, arbitrary limits or assumptions about maximum sizes, etc. Your code should compile without warning with `-Wall -Wextra -pedantic`. Special exemptions to this may be requested in advance.

These may (or may not) be useful:

```c++
std::tuple
std::remove_const
std::remove_reference
std::enable_if
```

More [here](http://en.cppreference.com/w/cpp/header/type_traits).

Reading about [SFINAE](http://en.cppreference.com/w/cpp/language/sfinae) may also be helpful, though I don't believe that it is necessary.

### Arbitrary Dimension Array Class Template

Implement a array class template named `cs540::Array` that can be instantiated with any number of dimensions. The size of each dimension is given as a sequence of `size_t` constant template arguments. You must not allocate any memory from the heap in your implementation.

Here is an example of how it might be used. The full API is further below.

```c++
#include "Array.hpp"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
int
main() {
    // Define a 2X3X4 array of integers.  Elements are uninitialized.
    cs540::Array<int, 2, 3, 4> a, b;
    cs540::Array<short, 2, 3, 4> c;
    // cs540::Array<int, 0> e1; // This line must cause a compile-time error.
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
}
#pragma GCC diagnostic pop
```

You should throw an object of class type `cs540::OutOfRange` as an exception if there is indexing operation that is out of range. You should provide the definition of this class in your header file.

Your code must not have any fixed limits. You must be able to construct arrays with any number of elements and dimensions, etc.

Your array implementation must be completely contained in `Array.hpp` and `Array.cpp`. Note that `Array.cpp` will likely be empty. Test code for your array is [here](../progs/Array/test_Array.cpp). The correct output is [here](../progs/Array/correct_output.txt). Your code must work with the test code without change. We reserve the right to add addtional tests to this as we see fit, but we will conform to the API used in the provided test code.

### Template

<table frame="border" rules="all">

<thead>

<tr>

<th>Declaration</th>

<th>Description</th>

</tr>

</thead>

<tbody>

<tr>

<td class="prototype">template <typename <var>T</var>, size_t... <var>Dims</var>> class Array;</td>

<td class="description">This declares a multidimensional array containing elements of type <var>T</var> with the given dimensions. If any dimension is zero it should fail to compile.<sup>1</sup> Use `static_assert` to force this behavior.  

<small><sup>1</sup>Clang++ version 3.7.0 will emit an error if a negative number is supplied for an unsigned template parameter. G++ 5.2.0 does not, however.</small></td>

</tr>

</tbody>

</table>

### Type Members

<table frame="border" rules="all">

<thead>

<tr>

<th>Member</th>

<th>Description</th>

</tr>

</thead>

<tbody>

<tr>

<td class="prototype">ValueType</td>

<td class="description">The type of the elements: <var>T</var>.</td>

</tr>

<tr>

<td class="prototype">FirstDimensionMajorIterator</td>

<td class="description">The type of the named iterator.</td>

</tr>

<tr>

<td class="prototype">LastDimensionMajorIterator</td>

<td class="description">The type of the named iterator.</td>

</tr>

</tbody>

</table>

### Public Member Functions

<table frame="border" rules="all">

<thead>

<tr>

<th>Prototype</th>

<th>Description</th>

</tr>

</thead>

<tbody>

<tr>

<td class="prototype">Array();</td>

<td class="description">The default constructor must be defined, either explicitly or implicitly.</td>

</tr>

<tr>

<td class="prototype">Array(const Array &);</td>

<td class="description">The copy constructor must work. The dimensionality of the source array must be the same. Note that a non-template copy constructor must be provided, in addtion to the member template copy constructor.</td>

</tr>

<tr>

<td class="prototype">template <typename <var>U</var>> Array(const Array<<var>U</var>, <var>Dims</var>...> &);</td>

<td class="description">The copy constructor must work. The dimensionality of the source array must be the same.</td>

</tr>

<tr>

<td class="prototype">Array &operator=(const Array &);</td>

<td class="description">The assignment operator must work. The dimensionality of the source array must be the same. Self-assignment must be a no-op. Note that this non-template assignment operator must be provided, in addtion to the member template assignment operator.</td>

</tr>

<tr>

<td class="prototype">template <typename <var>U</var>> Array &operator=(const Array<<var>U</var>, <var>Dims</var>...> &);</td>

<td class="description">The assignment operator must work. The dimensionality of the source array must be the same. Self-assignment must be a no-op.</td>

</tr>

<tr>

<td class="prototype"><var>T</var> &operator[size_t <var>i_1</var>][size_t <var>i_2</var>]...[size_t <var>i_D</var>]; const <var>T</var> &operator[size_t <var>i_1</var>][size_t <var>i_2</var>]...[size_t <var>i_D</var>] const;</td>

<td class="description">This is used to index into the array with range-checking. If any given indices are out-of-range, then an `OutOfRange` exception must be thrown. Note that this is a “conceptual” operator only. Such an operator does not really exist. Instead, you must figure out how to provide this functionality. (Helper classes may be useful.)</td>

</tr>

<tr>

<td class="section" colspan="2">Iterators</td>

</tr>

<tr>

<td class="prototype">FirstDimensionMajorIterator fmbegin();</td>

<td class="description">Returns a `FirstDimensionMajorIterator` (nested classes) object pointing to the first element.</td>

</tr>

<tr>

<td class="prototype">FirstDimensionMajorIterator fmend();</td>

<td class="description">Returns a `FirstDimensionMajorIterator` (nested classes) object pointing one past the last element.</td>

</tr>

<tr>

<td class="prototype">LastDimensionMajorIterator lmbegin();</td>

<td class="description">Returns a `LastDimensionMajorIterator` pointing to the first element.</td>

</tr>

<tr>

<td class="prototype">LastDimensionMajorIterator lmend();</td>

<td class="description">Returns a `LastDimensionMajorIterator` pointing one past the last element.</td>

</tr>

</tbody>

</table>

### Public Member Functions for Nested Class <span class="header_code">FirstDimensionMajorIterator</span>

This iterator is used to iterate through the array in row-major order. This iterator can be be used to read or write from the array.

<table frame="border" rules="all">

<thead>

<tr>

<th>Prototype</th>

<th>Description</th>

</tr>

</thead>

<tbody>

<tr>

<td class="prototype">FirstDimensionMajorIterator();</td>

<td class="description">Must have correct default ctor.</td>

</tr>

<tr>

<td class="prototype">FirstDimensionMajorIterator(const FirstDimensionMajorIterator &);</td>

<td class="description">Must have correct copy constructor. (The implicit one will probably be correct.)</td>

</tr>

<tr>

<td class="prototype">FirstDimensionMajorIterator &operator=(const FirstDimensionMajorIterator &);</td>

<td class="description">Must have correct assignment operator. (The implicit one will probably be correct.)</td>

</tr>

<tr>

<td class="prototype">bool operator==(const FirstDimensionMajorIterator &, const FirstDimensionMajorIterator &);</td>

<td class="description">Compares the two iterators for equality.</td>

</tr>

<tr>

<td class="prototype">bool operator!=(const FirstDimensionMajorIterator &, const FirstDimensionMajorIterator &);</td>

<td class="description">Compares the two iterators for inequality.</td>

</tr>

<tr>

<td class="prototype">FirstDimensionMajorIterator &operator++();</td>

<td class="description">Increments the iterator one element in row-major order, and returns the incremented iterator (preincrement). If the iterator is pointing to the end of the list, the behavior is undefined.</td>

</tr>

<tr>

<td class="prototype">FirstDimensionMajorIterator operator++(int);</td>

<td class="description">Increments the iterator one element in row-major, and returns an iterator pointing to the element prior to incrementing the iterator (postincrement). If the iterator is pointing to the end of the list, the behavior is undefined.</td>

</tr>

<tr>

<td class="prototype"><var>T</var> `&operator*() const;`</td>

<td class="description">Returns a reference to the <var>T</var> at this position in the array. If the iterator is pointing to the end of the list, the behavior is undefined.</td>

</tr>

</tbody>

</table>

### Public Member Functions for Nested Class <span class="header_code">LastDimensionMajorIterator</span>

This iterator is used to iterate through the array in column-major order. This iterator can be only be used to read or write from the array.

<table frame="border" rules="all">

<thead>

<tr>

<th>Prototype</th>

<th>Description</th>

</tr>

</thead>

<tbody>

<tr>

<td class="prototype">LastDimensionMajorIterator();</td>

<td class="description">Must have correct default constructor.</td>

</tr>

<tr>

<td class="prototype">LastDimensionMajorIterator(const LastDimensionMajorIterator &);</td>

<td class="description">Must have correct copy constructor. (The implicit one will probably be correct.)</td>

</tr>

<tr>

<td class="prototype">LastDimensionMajorIterator &operator=(const LastDimensionMajorIterator &);</td>

<td class="description">Must have correct assignment operator. (The implicit one will probably be correct.)</td>

</tr>

<tr>

<td class="prototype">bool operator==(const LastDimensionMajorIterator &, const LastDimensionMajorIterator &);</td>

<td class="description">Compares the two iterators for equality.</td>

</tr>

<tr>

<td class="prototype">bool operator!=(const LastDimensionMajorIterator &, const LastDimensionMajorIterator &);</td>

<td class="description">Compares the two iterators for inequality.</td>

</tr>

<tr>

<td class="prototype">LastDimensionMajorIterator &operator++();</td>

<td class="description">Increments the iterator one element in column-major order, and returns the incremented iterator (preincrement). If the iterator is pointing to the end of the list, the behavior is undefined.</td>

</tr>

<tr>

<td class="prototype">LastDimensionMajorIterator operator++(int);</td>

<td class="description">Increments the iterator one element in column-major order, and returns an iterator pointing to the element prior to incrementing the iterator (postincrement). If the iterator is pointing to the end of the list, the behavior is undefined.</td>

</tr>

<tr>

<td class="prototype"><var>T</var> `&operator*() const;`</td>

<td class="description">Returns a reference to the <var>T</var> at this position in the array. If the iterator is pointing to the end of the list, the behavior is undefined.</td>

</tr>

</tbody>

</table>
