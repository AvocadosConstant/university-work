#include <iostream>
#include <cstddef>
#include <assert.h>

// 30 points

/* BEGIN YOUR CODE */

// This was really fun!
template<class T>
using PointerTo = T*;

template<class T, std::size_t Size>
using ArrayOf = T[Size];

template<class T>
using ReferenceTo = T&;

template<class RT, typename... Ps>
using Function = RT(Ps...);

/* END YOUR CODE */

int f1(int) {return 1;}

int (*f2(double, int))[7] {
    static int a[7];
    return &a;
}

// Create alias templates so that the below works.

int main() {

    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunused-but-set-variable"
    #pragma GCC diagnostic ignored "-Wunused-variable"

    {
        PointerTo<int> p1;
        int i;
        p1 = &i;

        PointerTo<PointerTo<int>> p2;
        p2 = &p1;
    }

    {
        // The element type, and then the length.
        ArrayOf<int, 5> a = {1, 2, 3, 4, 5};
        assert(a[0] == 1);
        assert(a[1] == 2);
        int (*ap)[5];
        ap = &a;
    }
    {
        int i;
        ReferenceTo<int> r(i);
        assert(&i == &r);
    }

    {
        Function<int, int> f1;
        int i1  = f1(1);
    }

    // Combinations.
    {
        // Array of function pointers.
        void (*a[10])(int, double);
        ArrayOf<PointerTo<Function<void, int, double>>, 10> a2;
        a[0] = a2[1];
    }

    {
        // Function returning an array pointer.
        Function<PointerTo<ArrayOf<double, 7>>, double, int> f2;
        double (*ap)[7];
        ap = f2(1, 3.14);
    }

    {
        // Array of arrays.
        int a1[3][4];
        ArrayOf<ArrayOf<int, 4>, 3> a2;
        // Did it work?
        assert(&a1 != &a2);
    }

    #pragma GCC diagnostic pop
}
