/*
 * Don't use auto in for loops so as to test
 * nested FDMIt type, etc.
 */

#include "Array.hpp"
#include <iostream>
#include <assert.h>
#include <cstddef>
#include <typeinfo>

using namespace std;
using cs540::Array;
using cs540::OutOfRange;



template <typename T1, typename T2, size_t N>
void assert_equal(const Array<T1, N> &a1, const T2 (&a2)[N]) {
    for (size_t i = 0; i < N; i++) {
        // Prevent false okay caused by funky return from
        // indexing op.
        T1 t = a1[i];
        assert(t == a2[i]);
    }
}

template <typename T1, typename T2, size_t N1, size_t N2, size_t N3, size_t N4, size_t N5, size_t N6, size_t N7, size_t N8, size_t N9, size_t N10>
void assert_equal(const Array<T1, N1, N2, N3, N4, N5, N6, N7, N8, N9, N10> &a1, const T2 (&a2)[N1][N2][N3][N4][N5][N6][N7][N8][N9][N10]) {
    for (size_t i = 0; i < N1; i++) {
        for (size_t j = 0; j < N2; j++) {
            for (size_t k = 0; k < N3; k++) {
                for (size_t l = 0; l < N4; l++) {
                    for (size_t m = 0; m < N5; m++) {
                        for (size_t n = 0; n < N6; n++) {
                            for (size_t o = 0; o < N7; o++) {
                                for (size_t p = 0; p < N8; p++) {
                                    for (size_t q = 0; q < N9; q++) {
                                        for (size_t r = 0; r < N10; r++) {
                                            // Prevent false okay caused by funky return from
                                            // indexing op.
                                            T1 t1 = a1[i][j][k][l][m][n][o][p][q][r];
                                            T2 t2 = a2[i][j][k][l][m][n][o][p][q][r];
                                            assert(t1 == t2);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

int main() {

    /*
     * Test 1-D functionality.
     */

    {
        using array_t = Array<int, 3>;
        array_t a1;
        int a[] = {2, 11, 7};

        // Has ValueType.
        assert(typeid(array_t::ValueType) == typeid(int));
        // Other type members tested implicitly below.

        // Test assignment.
        for (size_t i = 0; i < 3; i++) {
            a1[i] = a[i];
        }
        assert_equal(a1, a);

        // Copy ctor.
        array_t a2(a1);
        assert_equal(a2, a);
        // Templated copy ctor.
        Array<double, 3> a4(a2);
        assert_equal(a4, a);

        // Assignment op.
        array_t a3;
        a3 = a2;
        assert_equal(a3, a);
        // Templated assignment op.
        a2[1] = 17;
        a4 = a2;
        assert(a4[1] == 17);

        // Test exception.
        try {
            a4[3] = 1;
            assert(false);
        } catch (OutOfRange &) {
            // Okay.
        }

        // Test const ref.
        const array_t &a1_cr(a1);
        assert_equal(a1_cr, a);
        // Below must give syntax error.
        #ifdef TEST_CR_ASSGN
            a1_cr[0] = 1;
        #endif

        /*
         * Test FDM iterator.
         */

        {
            // Can iterate and deref.
            int i = 0;
            for (array_t::FirstDimensionMajorIterator it = a1.fmbegin(); it != a1.fmend(); ++it) {
                int t = *it;
                assert(t == a[i++]);
            }
            // Make sure it actually did it.
            assert(i == 3);

            // Can compare.
            assert(a1.fmbegin() == a1.fmbegin());
            assert(a1.fmbegin() != ++a1.fmbegin());

            // Can assign.
            array_t::FirstDimensionMajorIterator it = a1.fmbegin(), it2 = it;
            it = a1.fmbegin();
            assert(*it == a[0]);

            // Test post-increment.
            it2 = a1.fmbegin();
            it = it2++;
            assert(*it == a[0]);
            assert(*it2 == a[1]);

            // Test pre-increment.
            it2 = a1.fmbegin();
            it = ++it2;
            assert(*it == a[1]);
            assert(*it2 == a[1]);

            // Can assign to pointee.
            *a1.fmbegin() = 12;
            a[0] = 12; // Change test array too.
            assert_equal(a1, a);
        }

        /*
         * Test LDM iterator.
         */

        {
            // Can iterate and deref.
            int i = 0;
            for (array_t::LastDimensionMajorIterator it = a1.lmbegin(); it != a1.lmend(); ++it) {
                int t = *it;
                assert(t == a[i++]);
            }
            assert(i == 3);

            // Can compare.
            assert(a1.lmbegin() == a1.lmbegin());
            assert(a1.lmbegin() != ++a1.lmbegin());

            // Can assign.
            array_t::LastDimensionMajorIterator it = a1.lmbegin(), it2 = it;
            it = a1.lmbegin();
            assert(*it == a[0]);

            // Test post-increment.
            it2 = a1.lmbegin();
            it = it2++;
            assert(*it == a[0]);
            assert(*it2 == a[1]);

            // Test pre-increment.
            it2 = a1.lmbegin();
            it = ++it2;
            assert(*it == a[1]);
            assert(*it2 == a[1]);

            // Can assign to pointee.
            *a1.lmbegin() = 12;
            a[0] = 12; // Change test array too.
            assert_equal(a1, a);
        }

        /*
         * Test const FDM iterator.
         */

        #if 0
        {
            int i = 0;
            for (array_t::ConstFirstDimensionMajorIterator it = a1_cr.fmbegin(); it != a1_cr.fmend(); ++it) {
                int t = *it;
                assert(t == a[i++]);
            }
            assert(i == 3);

            // Can compare.
            assert(a1_cr.fmbegin() == a1_cr.fmbegin());
            assert(a1_cr.fmbegin() != ++a1_cr.fmbegin());

            // Can assign.
            array_t::ConstFirstDimensionMajorIterator it = a1_cr.fmbegin(), it2 = it;
            it = a1_cr.fmbegin();
            assert(*it == a[0]);

            // Test post-increment.
            it2 = a1_cr.fmbegin();
            it = it2++;
            assert(*it == a[0]);
            assert(*it2 == a[1]);

            // Test pre-increment.
            it2 = a1_cr.fmbegin();
            it = ++it2;
            assert(*it == a[1]);
            assert(*it2 == a[1]);

            // Cannot assign to pointee.
            #ifdef TEST_CI_ASSIGN
                *a1_cr.fmbegin() = 1;
            #endif
        }
        #endif
    }

    /*
     * 1-D unit functionality.
     */
    {
        using array_t = Array<int, 1>;
        array_t a1;
        int a[] = {787};

        // Has ValueType.
        assert(typeid(array_t::ValueType) == typeid(int));
        // Other type members tested implicitly below.

        // Test assignment.
        for (size_t i = 0; i < 1; i++) {
            a1[i] = a[i];
        }
        assert_equal(a1, a);

        // Copy ctor.
        array_t a2(a1);
        assert_equal(a2, a);
        // Templated copy ctor.
        Array<double, 1> a4(a2);
        assert_equal(a4, a);

        // Assignment op.
        array_t a3;
        a3 = a2;
        assert_equal(a3, a);
        // Templated assignment op.
        a2[0] = 17;
        a4 = a2;
        assert(a4[0] == 17);

        // Test exception.
        try {
            a4[1] = 1;
            assert(false);
        } catch (OutOfRange &) {
            // Okay.
        }

        // Test const ref.
        const array_t &a1_cr(a1);
        assert_equal(a1_cr, a);
        // Below must give syntax error.
        #ifdef TEST_CR_ASSGN
            a1_cr[0] = 1;
        #endif

        /*
         * Test FDM iterator.
         */

        {
            // Can iterate and deref.
            int i = 0;
            for (array_t::FirstDimensionMajorIterator it = a1.fmbegin(); it != a1.fmend(); ++it) {
                int t = *it;
                assert(t == a[i++]);
            }
            // Make sure it actually did it.
            assert(i == 1);

            // Can compare.
            assert(a1.fmbegin() == a1.fmbegin());
            assert(a1.fmbegin() != ++a1.fmbegin());

            // Can assign.
            array_t::FirstDimensionMajorIterator it = a1.fmbegin(), it2 = it;
            it = a1.fmbegin();
            assert(*it == a[0]);

            // Test post-increment.
            it2 = a1.fmbegin();
            it = it2++;
            assert(*it == a[0]);
            assert(it2 == a1.fmend());

            // Test pre-increment.
            it2 = a1.fmbegin();
            it = ++it2;
            assert(it == a1.fmend());
            assert(it2 == a1.fmend());

            // Can assign to pointee.
            *a1.fmbegin() = 983;
            a[0] = 983; // Change test array too.
            assert_equal(a1, a);
        }

        /*
         * Test LDM iterator.
         */

        {
            // Can iterate and deref.
            int i = 0;
            for (array_t::LastDimensionMajorIterator it = a1.lmbegin(); it != a1.lmend(); ++it) {
                int t = *it;
                assert(t == a[i++]);
            }
            assert(i == 1);

            // Can compare.
            assert(a1.lmbegin() == a1.lmbegin());
            assert(a1.lmbegin() != ++a1.lmbegin());

            // Can assign.
            array_t::LastDimensionMajorIterator it = a1.lmbegin(), it2 = it;
            it = a1.lmbegin();
            assert(*it == a[0]);

            // Test post-increment.
            it2 = a1.lmbegin();
            it = it2++;
            assert(*it == a[0]);
            assert(it2 == a1.lmend());

            // Test pre-increment.
            it2 = a1.lmbegin();
            it = ++it2;
            assert(it == a1.lmend());
            assert(it2 == a1.lmend());

            // Can assign to pointee.
            *a1.lmbegin() = 44;
            a[0] = 44; // Change test array too.
            assert_equal(a1, a);
        }

        /*
         * Test const FDM iterator.
         */

        #if 0
        {
            int i = 0;
            for (array_t::ConstFirstDimensionMajorIterator it = a1_cr.fmbegin(); it != a1_cr.fmend(); ++it) {
                int t = *it;
                assert(t == a[i++]);
            }
            assert(i == 1);

            // Can compare.
            assert(a1_cr.fmbegin() == a1_cr.fmbegin());
            assert(a1_cr.fmbegin() != ++a1_cr.fmbegin());

            // Can assign.
            array_t::ConstFirstDimensionMajorIterator it = a1_cr.fmbegin(), it2 = it;
            it = a1_cr.fmbegin();
            assert(*it == a[0]);

            // Test post-increment.
            it2 = a1_cr.fmbegin();
            it = it2++;
            assert(*it == a[0]);
            assert(it2 == ++a1_cr.fmbegin());

            // Test pre-increment.
            it2 = a1_cr.fmbegin();
            it = ++it2;
            assert(it == ++a1_cr.fmbegin());
            assert(it2 == a1_cr.fmend());

            // Cannot assign to pointee.
            #ifdef TEST_CI_ASSIGN
                *a1_cr.fmbegin() = 1;
            #endif
        }
        #endif

    }

    /*
     * Test 10-D functionality.
     */

    {
        using array_t = Array<int, 1, 2, 3, 1, 5, 6, 7, 4, 2, 1>;
        array_t a1;
        int a[1][2][3][1][5][6][7][4][2][1];
        constexpr size_t N = sizeof(a)/sizeof(int);
        // Cheat.
        int *ap = (int *) a;
        for (size_t i = 0; i < N; i++) {
            ap[i] = drand48()*100;
        }

        // Has ValueType.
        assert(typeid(array_t::ValueType) == typeid(int));
        // Other type members tested implicitly below.

        // Test assignment.
        for (size_t i = 0; i < 1; i++) {
            for (size_t j = 0; j < 2; j++) {
                for (size_t k = 0; k < 3; k++) {
                    for (size_t l = 0; l < 1; l++) {
                        for (size_t m = 0; m < 5; m++) {
                            for (size_t n = 0; n < 6; n++) {
                                for (size_t o = 0; o < 7; o++) {
                                    for (size_t p = 0; p < 4; p++) {
                                        for (size_t q = 0; q < 2; q++) {
                                            for (size_t r = 0; r < 1; r++) {
                                                a1[i][j][k][l][m][n][o][p][q][r] = a[i][j][k][l][m][n][o][p][q][r];
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        assert_equal(a1, a);

        // Copy ctor.
        array_t a2(a1);
        assert_equal(a2, a);
        // Templated copy ctor.
        Array<double, 1, 2, 3, 1, 5, 6, 7, 4, 2, 1> a4(a2);
        assert_equal(a4, a);

        // Assignment op.
        array_t a3;
        a3 = a2;
        assert_equal(a3, a);
        // Templated assignment op.
        // int a[1][2][3][1][5][6][7][4][2][1];
              a2[0][1][2][0][3][4][0][2][1][0] = 17;
        a4 = a2;
        assert(a4[0][1][2][0][3][4][0][2][1][0] == 17);

        // Test exception.
        try {
            a4[1][1][2][0][3][4][0][2][1][0] = 17;
            assert(false);
        } catch (OutOfRange &) {
            // Okay.
        }
        try {
            a4[0][1][2][0][3][4][0][2][1][1] = 17;
            assert(false);
        } catch (OutOfRange &) {
            // Okay.
        }
        try {
            a4[0][1][2][0][10][4][0][2][1][0] = 17;
            assert(false);
        } catch (OutOfRange &) {
            // Okay.
        }

        // Test const ref.
        const array_t &a1_cr(a1);
        assert_equal(a1_cr, a);
        // Below must give syntax error.
        #ifdef TEST_CR_ASSGN
            a1_cr[0][1][2][0][3][4][0][2][1][0] = 17;
        #endif

        /*
         * Test FDM iterator.
         */

        {

            // Can iterate and deref.
            size_t i = 0;
            // Cheat.
            int *ap = (int *) a;
            for (array_t::FirstDimensionMajorIterator it = a1.fmbegin(); it != a1.fmend(); ++it) {
                int t = *it;
                assert(t == ap[i++]);
            }
            // Make sure it actually did it.
            assert(i == N);

            // Can compare.
            assert(a1.fmbegin() == a1.fmbegin());
            assert(a1.fmbegin() != ++a1.fmbegin());

            // Can assign.
            array_t::FirstDimensionMajorIterator it = a1.fmbegin(), it2 = it;
            it = ++a1.fmbegin();
            assert(it == ++a1.fmbegin());

            // Test post-increment.
            it2 = a1.fmbegin();
            it = it2++;
            assert(it == a1.fmbegin());
            assert(it2 == ++a1.fmbegin());

            // Test pre-increment.
            it2 = a1.fmbegin();
            it = ++it2;
            assert(it == it2);
            assert(it2 == ++a1.fmbegin());

            // Can assign to pointee.
            *a1.fmbegin() = 12;
            a[0][0][0][0][0][0][0][0][0][0] = 12; // Change test array too.
            assert_equal(a1, a);
        }

        /*
         * Test LDM iterator.
         */

        {
            array_t::LastDimensionMajorIterator it = a1.lmbegin();
            for (size_t r = 0; r < 1; r++) {
                for (size_t q = 0; q < 2; q++) {
                    for (size_t p = 0; p < 4; p++) {
                        for (size_t o = 0; o < 7; o++) {
                            for (size_t n = 0; n < 6; n++) {
                                for (size_t m = 0; m < 5; m++) {
                                    for (size_t l = 0; l < 1; l++) {
                                        for (size_t k = 0; k < 3; k++) {
                                            for (size_t j = 0; j < 2; j++) {
                                                for (size_t i = 0; i < 1; i++) {
                                                    assert(it != a1.lmend());
                                                    int t = *it;
                                                    assert(t == a[i][j][k][l][m][n][o][p][q][r]);
                                                    it++;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            assert(it == a1.lmend());

            // Can compare.
            assert(a1.lmbegin() == a1.lmbegin());
            assert(a1.lmbegin() != ++a1.lmbegin());

            // Can assign.
            array_t::LastDimensionMajorIterator it2 = it;
            it = ++a1.lmbegin();
            assert(it == ++a1.lmbegin());

            // Test post-increment.
            it2 = a1.lmbegin();
            it = it2++;
            assert(it == a1.lmbegin());
            assert(it2 == ++a1.lmbegin());

            // Test pre-increment.
            it2 = a1.lmbegin();
            it = ++it2;
            assert(it == it2);
            assert(it2 == ++a1.lmbegin());

            // Can assign to pointee.
            *a1.lmbegin() = 12;
            a[0][0][0][0][0][0][0][0][0][0] = 12; // Change test array too.
            assert_equal(a1, a);
        }

        /*
         * Test const FDM iterator.
         */

        #if 0
        {
            size_t i = 0;
            // Cheat.
            int *ap = (int *) a;
            for (array_t::ConstFirstDimensionMajorIterator it = a1_cr.fmbegin(); it != a1_cr.fmend(); ++it) {
                int t = *it;
                assert(t == ap[i++]);
            }
            // Make sure it actually did it.
            assert(i == N);

            // Can compare.
            assert(a1_cr.fmbegin() == a1_cr.fmbegin());
            assert(a1_cr.fmbegin() != ++a1_cr.fmbegin());

            // Can assign.
            array_t::ConstFirstDimensionMajorIterator it = a1_cr.fmbegin(), it2 = it;
            it = ++a1_cr.fmbegin();
            assert(it == ++a1_cr.fmbegin());

            // Test post-increment.
            it2 = a1_cr.fmbegin();
            it = it2++;
            assert(it == a1_cr.fmbegin());
            assert(it2 == ++a1_cr.fmbegin());

            // Test pre-increment.
            it2 = a1_cr.fmbegin();
            it = ++it2;
            assert(it == it2);
            assert(it2 == ++a1_cr.fmbegin());

            // Can assign to pointee.
            #ifdef TEST_CI_ASSIGN
                *a1_cr.fmbegin() = 12;
            #endif
        }
        #endif
    }
    
    /*
     * Tests from the web page.
     */

     {
        // Define a 2X3X4 array of integers.  Elements are uninitialized.
        Array<int, 2, 3, 4> a;
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 3; j++) {
                for (int k = 0; k < 4; k++) {
                    a[i][j][k] = 0;
                }
            }
        }
        // This line must cause a compile-time error if uncommented.
        // Array<int, 0> e1;

        // Range-checked indexing.
        a[0][0][0] = 1234;
        a[1][1][1] = a[0][0][0];
        a[0][2][3] = 5678; // Set the element in the first plane, 3rd row, and 4th column.
        try {
            a[0][3][0] = 1; // Out of range, throws.
            assert(false);
        } catch (OutOfRange &) {
            // Okay.
        }

        a = a; // Self-assignment must be a no-op.

        const Array<int, 2, 3, 4> &const_ref(a);

        int i = const_ref[0][0][0]; // Okay.
        cout << i << endl;
        // Below should give syntax error.
        // const_ref[0][0][0] = 1;

        // Iterate through in plane major order.  (Note: Normally, you would use
        // auto to simplify.)
        for (Array<int, 2, 3, 4>::FirstDimensionMajorIterator it = a.fmbegin(); it != a.fmend(); ++it) {
            cout << *it << endl;
        }

        // Iterate through in column major order.
        for (Array<int, 2, 3, 4>::LastDimensionMajorIterator it = a.lmbegin(); it != a.lmend(); ++it) {
            cout << *it << endl;
        }
    }

    /*
     * Some miscellaneous tests.
     */

    {
        Array<int, 2, 3> a;

        int n = 0;
        for (size_t i = 0; i < 2; i++) {
            for (size_t j = 0; j < 3; j++) {
                a[i][j] = n++;
            }
        }
    }


    {
        Array<int, 3> a;
        for (int i = 0; i < 3; i++) {
            a[i] = i;
        }

        cout << "FDM by loops" << endl;
        for (int i = 0; i < 3; i++) {
            cout << a[i] << endl;
        }

        cout << "FDM by iterator" << endl;
        for (auto it = a.fmbegin(); it != a.fmend(); ++it) {
            cout << *it << endl;
        }

        cout << "LDM by iterator" << endl;
        for (auto it = a.lmbegin(); it != a.lmend(); ++it) {
            cout << *it << endl;
        }
    }

    Array<int, 3> a1;
    Array<int, 4, 5> a2;
    Array<int, 2, 3, 4> a3;
    Array<int, 3, 2, 4, 5> a4;

    a1[1] = 3;
    a2[1][1] = 4;

    int n = 0;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 4; k++) {
                a3[i][j][k] = n++;
            }
        }
    }

    cout << "FDM by loops" << endl;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 4; k++) {
                cout << a3[i][j][k] << endl;
            }
        }
    }

    cout << "FDM by iterator" << endl;
    for (auto it = a3.fmbegin(); it != a3.fmend(); ++it) {
        cout << *it << endl;
    }

    cout << "LDM by loops" << endl;
    for (int k = 0; k < 4; k++) {
        for (int j = 0; j < 3; j++) {
            for (int i = 0; i < 2; i++) {
                cout << a3[i][j][k] << endl;
            }
        }
    }
    cout << "LDM by iterator" << endl;
    for (auto it = a3.lmbegin(); it != a3.lmend(); ++it) {
        cout << *it << endl;
    }

    #if 0
    {
        const Array<int, 2, 3, 4> &const_a3(a3);
        cout << "FDM by const iterator" << endl;
        for (auto it = const_a3.fmbegin(); it != const_a3.fmend(); ++it) {
            cout << *it << endl;
        }
    }
    #endif

    Array<double, 2, 3, 4> da(a3);
    da = a3;
}
