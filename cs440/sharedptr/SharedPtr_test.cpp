/*
 * Usage: -t secs
 *   where secs is how long to run the thread test for.  Defaults
 *   to 15 seconds.
 */


// NOTE compile with -pthread
#include "SharedPtr.hpp"
#include <new>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <iostream>
#include <algorithm>
#include <random>
#include <errno.h>
#include <assert.h>



using namespace std;
using namespace cs540;



class Random {
    public:
        Random(unsigned int s = 0);
        // Generate random int in range [l,h].
        int operator()(int l, int h) {
            return std::uniform_int_distribution<int>(l, h)(gen);
        }
        Random(const Random &) = delete;
        Random &operator=(const Random &) = delete;
    private:
        std::default_random_engine gen;
};

Random::Random(unsigned int seed) : gen(seed) {}



void basic_tests_1();
void basic_tests_2();
// RunSecs needs to be here so that it can be set via command-line arg.
int RunSecs = 15;
void threaded_test();
size_t AllocatedSpace;



void
usage() {
    fprintf(stderr, "Bad args, usage: ./a.out [ -t secs ]\n");
    exit(1);
}



int
main(int argc, char *argv[]) {

    int c;

    setlinebuf(stdout);

    {
        // Force initial iostreams allocation so that memory-leak detecting
        // will work right.  Tabs and pointer are to get locale stuff
        // allocated.
        int *p = (int *) 0x1234;
        cout << "\tForce initial allocation on cout: " << p << endl;
        cerr << "\tForce initial allocation on cerr: " << p << endl;
        clog << "\tForce initial allocation on clog: " << p << endl;
    }

    while ((c = getopt(argc, argv, "t:")) != -1) {
        switch (c) {
            case 't':
                RunSecs = atoi(optarg);
		assert(1 <= RunSecs && RunSecs <= 10000);
                break;
            case '?':
                usage();
                break;
            default:
                assert(false);
                abort();
        }
    }

    // Catch any command-line args without -.
    if (optind < argc) {
        usage();
    }

    basic_tests_1();
    basic_tests_2();
    threaded_test();
}

void *operator new(size_t sz) {
    char *p = (char *) malloc(sz + 8);
    *((size_t *) p) = sz;
    __sync_add_and_fetch(&AllocatedSpace, sz);
    return p + 8;
}

void operator delete(void *vp) noexcept {

    if (vp == 0) {
        return;
    }

    char *p = (char *) vp;
    size_t sz = *((size_t *) (p - 8));
    __sync_sub_and_fetch(&AllocatedSpace, sz);
    // Zero out memory to help catch bugs.
    memset(p - 8, 0xff, sz + 8);
    free(p - 8);
}

/* Basic Tests 1 ================================================================================ */

class Base1 {
    protected:
        Base1() : derived_destructor_called(false) {
            printf("Base1::Base1()\n");
        }
    private:
        Base1(const Base1 &); // Disallow.
        Base1 &operator=(const Base1 &); // Disallow.
    protected:
        ~Base1() {
            printf("Base1::~Base1()\n");
            assert(derived_destructor_called);
        }
    protected:
        bool derived_destructor_called;
};

class Derived : public Base1 {
        friend void basic_tests_1();
    private:
        Derived() {}
        Derived(const Derived &); // Disallow.
        Derived &operator=(const Derived &); // Disallow.
    public:
        ~Derived() {
            printf("Derived::~Derived()\n");
            derived_destructor_called = true;
        }
        int value;
};

class Base_polymorphic {
    protected:
        Base_polymorphic() {
            printf("Base_polymorphic::Base_polymorphic()\n");
        }
    private:
        Base_polymorphic(const Base_polymorphic &); // Disallow.
        Base_polymorphic &operator=(const Base_polymorphic &); // Disallow.
    protected:
        virtual ~Base_polymorphic() {
            printf("Base_polymorphic::~Base_polymorphic()\n");
        }
};

class Derived_polymorphic : public Base_polymorphic {
        friend void basic_tests_1();
    private:
        Derived_polymorphic() {}
        Derived_polymorphic(const Derived_polymorphic &); // Disallow.
        Derived_polymorphic &operator=(const Derived_polymorphic &); // Disallow.
};

class Derived2_polymorphic : public Base_polymorphic {
    private:
        Derived2_polymorphic() {}
        Derived2_polymorphic(const Derived2_polymorphic &); // Disallow.
        Derived2_polymorphic &operator=(const Derived2_polymorphic &); // Disallow.
};

class Base2 {
    protected:
        Base2() : derived_destructor_called(false) {
            printf("Base2::Base2()\n");
        }
    private:
        Base2(const Base2 &); // Disallow.
        Base2 &operator=(const Base2 &); // Disallow.
    protected:
        ~Base2() {
            printf("Base2::~Base2()\n");
            assert(derived_destructor_called);
        }
    protected:
        bool derived_destructor_called;
};

class Derived_mi : public Base1, public Base2 {
        friend void basic_tests_1();
    private:
        Derived_mi() {}
        Derived_mi(const Derived_mi &); // Disallow.
        Derived_mi &operator=(const Derived_mi &); // Disallow.
    public:
        ~Derived_mi() {
            printf("Derived_mi::~Derived_mi()\n");
            Base1::derived_destructor_called = true;
            Base2::derived_destructor_called = true;
        }
        int value;
};

class Base1_vi {
    protected:
        Base1_vi() : derived_destructor_called(false) {
            printf("Base1_vi::Base1_vi()\n");
        }
    private:
        Base1_vi(const Base1_vi &); // Disallow.
        Base1_vi &operator=(const Base1_vi &); // Disallow.
    protected:
        ~Base1_vi() {
            printf("Base1_vi::~Base1_vi()\n");
            assert(derived_destructor_called);
        }
    protected:
        bool derived_destructor_called;
};

class Base2_vi : public virtual Base1_vi {
    protected:
        Base2_vi() {
            printf("Base2_vi::Base2_vi()\n");
        }
    private:
        Base2_vi(const Base2_vi &); // Disallow.
        Base2_vi &operator=(const Base2_vi &); // Disallow.
    protected:
        ~Base2_vi() {
            printf("Base2_vi::~Base2_vi()\n");
            assert(derived_destructor_called);
        }
};

class Derived_vi : public virtual Base1_vi, public Base2_vi {
        friend void basic_tests_1();
    private:
        Derived_vi() {}
        Derived_vi(const Derived_vi &); // Disallow.
        Derived_vi &operator=(const Derived_vi &); // Disallow.
    public:
        ~Derived_vi() {
            printf("Derived_vi::~Derived_vi()\n");
            derived_destructor_called = true;
        }
        int value;
};

void basic_tests_1() {

    size_t base = AllocatedSpace;

    // Test deleting through original class.
    {
        // Base1 created directly with Derived *.
        {
            SharedPtr<Base1> sp(new Derived);
            {
                // Test copy constructor.
                SharedPtr<Base1> sp2(sp);
            }
        }
        // Base1 assigned from SharedPtr<Derived>.
        {
            SharedPtr<Base1> sp2;
            {
                SharedPtr<Derived> sp(new Derived);
                // Test template copy constructor.
                SharedPtr<Base1> sp3(sp);
                sp2 = sp;
                sp2 = sp2;
            }
        }
    }

    // Test copy constructor and template copy constructor.
    {
        {
            SharedPtr<Derived> sp(new Derived);
            SharedPtr<Derived> sp2(sp);
        }
        {
            SharedPtr<Derived> sp(new Derived);
            SharedPtr<Base1> sp2(sp);
        }
    }

    // Test assignment operator and template assignment operator.
    {
        {
            SharedPtr<Derived> sp(new Derived);
            SharedPtr<Derived> sp2;
            sp2 = sp;
            sp = sp; // Self assignment.
        }
        {
            SharedPtr<Derived> sp(new Derived);
            SharedPtr<Base1> sp2;
            sp2 = sp;
            sp2 = sp2; // Self assignment.
            sp2 = sp;
            sp = sp;
        }
    }

    // Test reset.
    {
        {
            SharedPtr<Derived> sp(new Derived);
            SharedPtr<Base1> sp2;
            sp2 = sp;
            sp2 = sp2;
            sp.reset();
            sp.reset(new Derived);
            SharedPtr<Base1> sp3(sp2);
        }
        // Need to make sure that it's the reset that
        // is forcing the object to be deleted, and
        // not the smart pointer destructor.
        {
            SharedPtr<Derived> sp(new Derived);
            char *buf = (char *) ::operator new(sizeof(SharedPtr<Base1>));
            SharedPtr<Base1> &sp2 = *(new (buf) SharedPtr<Base1>());
            sp2 = sp;
            sp2 = sp2;
            sp.reset();
            sp2.reset();
            ::operator delete(buf);
        }
    }

    // Test *.
    {
        SharedPtr<Derived> sp(new Derived);
        (*sp).value = 1234;

        SharedPtr<const Derived> sp2(sp);
        int i = (*sp2).value;
        assert(i == 1234);
        // (*sp2).value = 567; // Should give a syntax error if uncommented.
    }

    // Test ->.
    {
        SharedPtr<Derived> sp(new Derived);
        sp->value = 1234;

        SharedPtr<const Derived> sp2(sp);
        int i = sp2->value;
        assert(i == 1234);
        // sp2->value = 567; // Should give a syntax error if uncommented.
    }

    // Test get().
    {
        SharedPtr<Derived> sp(new Derived);
        sp.get()->value = 1234;

        SharedPtr<const Derived> sp2(sp);
        int i = sp2.get()->value;
        assert(i == 1234);
    }

    // Test bool.
    {
        SharedPtr<Derived> sp(new Derived);
        assert(sp);
        sp.reset();
        assert(!sp);

        SharedPtr<Derived> sp2;

        // int i = sp; // Must give syntax error.
        // delete sp; // Must give syntax error.
    }

    // Test ==.
    {
        SharedPtr<Derived> sp(new Derived);
        SharedPtr<Derived> sp2(sp);
        SharedPtr<Derived> sp3;

        assert(sp2 == sp);
        assert(!(sp2 == sp3));
        sp3.reset(new Derived);
        assert(!(sp2 == sp3));
    }

    // Test static_pointer_cast.
    {
        SharedPtr<Derived> sp(new Derived);
        SharedPtr<Base1> sp2(sp);

        // SharedPtr<Derived> sp3(sp2); // Should give a syntax error.
        SharedPtr<Derived> sp3(static_pointer_cast<Derived>(sp2));
        // SharedPtr<Derived> sp4(dynamic_pointer_cast<Derived>(sp2)); // Should give syntax error about polymorphism.
    }

    // Test dynamic_pointer_cast.
    {
        SharedPtr<Derived_polymorphic> sp(new Derived_polymorphic);
        SharedPtr<Base_polymorphic> sp2(sp);

        // SharedPtr<Derived_polymorphic> sp3(sp2); // Should give a syntax error.
        SharedPtr<Derived_polymorphic> sp3(dynamic_pointer_cast<Derived_polymorphic>(sp2));
        SharedPtr<Derived_polymorphic> sp4(static_pointer_cast<Derived_polymorphic>(sp2));
        SharedPtr<Derived2_polymorphic> sp5(dynamic_pointer_cast<Derived2_polymorphic>(sp2));
        assert(!sp5);
    }

    // Test to make sure works with MI.
    {
        SharedPtr<Base2> sp2;
        {
            SharedPtr<Base1> sp1;
            {
                SharedPtr<Derived_mi> sp(new Derived_mi);
                sp1 = sp;
                sp2 = static_pointer_cast<Base2>(static_pointer_cast<Derived_mi>(sp1));
            } // Destructor for sp called.
        } // Destructor for sp1 called.
    } // Destructor for sp2 called.

    // Test with MI to make sure not doing invalid casts elsewhere.
    {
        {
            SharedPtr<Base1> sp1;
            {
                SharedPtr<Derived_mi> sp(new Derived_mi);
                sp1 = static_pointer_cast<Base1>(static_pointer_cast<Derived_mi>(sp));
            } // Destructor for sp called.
        } // Destructor for sp1 called.
        {
            SharedPtr<Base2> sp2;
            {
                SharedPtr<Derived_mi> sp(new Derived_mi);
                sp2 = static_pointer_cast<Base2>(static_pointer_cast<Derived_mi>(sp));
            } // Destructor for sp called.
        } // Destructor for sp2 called.
    }

    // Should add test here to make sure not doing cast like from Helper<T> * to
    // Helper<U> *.

    // Test to make sure works with virtual inheritance.
    {
        SharedPtr<Base2_vi> sp2;
        {
            SharedPtr<Base1_vi> sp1;
            {
                SharedPtr<Derived_vi> sp(new Derived_vi);
                sp1 = sp;
                sp2 = sp;
            } // Destructor for sp called.
        } // Destructor for sp1 called.
    } // Destructor for sp2 called.

    // Test const assignment.
    {
        SharedPtr <const Derived> sp_const(new Derived);
        SharedPtr <Derived> sp(new Derived);
        sp_const = sp;
        // sp = sp_const; // Syntax error if uncommented.
    }

    if (base != AllocatedSpace) {
        printf("Leaked %zu bytes in basic tests 1.\n", AllocatedSpace - base);
        abort();
    }

    printf("Basic tests 1 passed.\n");
}

/* Basic Tests 2 ================================================================================ */

class A {
    public:
	virtual ~A() {
	    //printf("%p deleted\n", (Obj *) this);
	}
};

class B : public A {
    public:
	virtual ~B() {}
};

class C : public A {
    public:
	virtual ~C() {}
};

// These tests overlap a lot with the ones in basic tests 1.
void
basic_tests_2() {

    size_t base = AllocatedSpace;
    {
        // Test default constructor.
        {
            SharedPtr<A> np;
            assert(!np);
        }

        // Test construction from object.
        {
            A *ap = new A;
            SharedPtr<A> a(ap);
            assert(a.get() == ap);
        }

        // Test construction from another SharedPtr of the same type.
        {
            SharedPtr<A> a(new A);
            SharedPtr<A> a2(a);
        }

        // Test construction from another SharedPtr of a derived type.
        {
            SharedPtr<B> b(new B);
            SharedPtr<A> a(b);
        }

        // Test assignment operator.
        {
            // Same type.
            SharedPtr<A> a1, a2(new A);
            a1 = a2;

            // Derived to base.
            SharedPtr<B> b(new B);
            a1 = b;

            // Object ptr.
            a1.reset(new A);

            // To Null.
            a1.reset();
        }

        // More misc tests.
        {
            SharedPtr<B> b(new B);
            SharedPtr<C> c(new C);

            /*
            printf("new B: %p\n", static_cast<Body *>(b.ptr()));
            printf("new C: %p\n", static_cast<Body *>(c.ptr()));

            printf("b: %p\n", &b);
            printf("c: %p\n", &c);
            */

            // Initialization from base should not require cast.
            SharedPtr<A> a_base(b);

            // Default initialization and cast to base.
            SharedPtr<A> a_dc(b);
            // Note that this will use the templated constructor to do a conversion.
            // if a templated assignment does not exist.
            a_dc = b;

            // Static cast to base.
            SharedPtr<A> a_b = b;
            SharedPtr<A> a_c = c;
            /*
            printf("a_b: %p\n", &a_b);
            printf("a_c: %p\n", &a_c);
            */

            // Dynamic downcast.
            SharedPtr<B> b2 = dynamic_pointer_cast<B>(a_b);
            // If the below is uncommented, we should get an error in the templated
            // assignment operator.  This will verify that that is being used rather
            // than templated constructors, etc.
            //b2 = a_b;
            assert(b2);
            SharedPtr<C> c2 = dynamic_pointer_cast<C>(a_b);
            assert(!c2);
            /*
            printf("b2: %p\n", &b2);
            printf("c2: %p\n", &c2);
            */

            // Dynamic downcast.
            SharedPtr<B> b3 = dynamic_pointer_cast<B>(a_c);
            assert(!b3);
            SharedPtr<C> c3 = dynamic_pointer_cast<C>(a_c);
            assert(c3);
            /*
            printf("b3: %p\n", &b3);
            printf("c3: %p\n", &c3);
            */
        }
    }
    if (base != AllocatedSpace) {
        printf("Leaked %zu bytes in basic tests 2.\n", AllocatedSpace - base);
        abort();
    }

    printf("Basic tests 2 passed.\n");
}

/* Threaded Test * ============================================================================== */

// These need to be global so the threads can access it.
time_t StartTime;
const int TABLE_SIZE = 100;

class TestObj {
    public:
	TestObj() {}
	TestObj(int i) : a(i) {}
        int a;
};

struct TableEntry {
    TableEntry();
    ~TableEntry();
    pthread_mutex_t lock;
    SharedPtr<TestObj> *ptr;
} *Table;

TableEntry::TableEntry() : ptr(0) {
    int ec;
    ec = pthread_mutex_init(&lock, 0);
    assert(ec == 0);
}
TableEntry::~TableEntry() {
    int ec;
    ec = pthread_mutex_destroy(&lock);
    assert(ec == 0);
    if (ptr != 0) {
        delete ptr;
        ptr = 0;
    }
}

void *
run(void *vp) {

    Random rand((unsigned int) (unsigned long) vp);
    
    pthread_t tid = pthread_self();
    int ec;
    struct Counters {
        unsigned long assignment_new, assignment, reset, new_default, new_copy, delet;
    } counters = {0, 0, 0, 0, 0, 0};

    while (time(NULL) < StartTime + RunSecs) {

        int act = rand(0, 4);
        switch (act) {
            case 0:
                // Assign ptr new TestObj;
                {
                    int i = rand(0, TABLE_SIZE - 1);
                    // printf("%d: new %d start\n", (int)tid, i);
                    ec = pthread_mutex_lock(&Table[i].lock); assert(ec == 0);
                    if (Table[i].ptr != 0) {
                        Table[i].ptr->reset(new TestObj); // fix
                        counters.assignment_new++;
                    }
                    ec = pthread_mutex_unlock(&Table[i].lock); assert(ec == 0);
                    // printf("%d: new %d done\n", (int) tid, i);
                }
                break;
            case 1:
                // Set ptr to NULL
                {
                    int i = rand(0, TABLE_SIZE - 1);
                    // printf("%d: clear %d start\n", (int) tid, i);
                    ec = pthread_mutex_lock(&Table[i].lock); assert(ec == 0);
                    if (Table[i].ptr != 0) {
                        Table[i].ptr->reset();
                        counters.reset++;
                    }
                    ec = pthread_mutex_unlock(&Table[i].lock); assert(ec == 0);
                    // printf("%d: clear %d done\n", (int) tid, i);
                }
                break;
            case 2:
                // Assign ptr from another ptr
                {
                    int i = rand(0, TABLE_SIZE - 1);
                    int j = rand(0, TABLE_SIZE - 1);

                    // printf("%d: assign %d=%d start\n", (int) tid, i, j);

                    // Order to avoid deadlock.
                    if (i <= j) {
                        ec = pthread_mutex_lock(&Table[i].lock); assert(ec == 0);
                        if (i != j) {
                            ec = pthread_mutex_lock(&Table[j].lock); assert(ec == 0);
                        }
                    } else {
                        ec = pthread_mutex_lock(&Table[j].lock); assert(ec == 0);
                        ec = pthread_mutex_lock(&Table[i].lock); assert(ec == 0);
                    }

                    if (Table[i].ptr && Table[j].ptr) {
                        *Table[i].ptr = *Table[j].ptr;
                        counters.assignment++;
                    }

                    ec = pthread_mutex_unlock(&Table[i].lock); assert(ec == 0);
                    if (i != j) {
                        ec = pthread_mutex_unlock(&Table[j].lock); assert(ec == 0);
                    }
                    // printf("%d: assign %d=%d done\n", (int) tid, i, j);
                }
                break;
            case 3:
                // Create new sptr.
                {
                    // Create with default.
                    if (rand(0, 1) == 0) {
                        int i = rand(0, TABLE_SIZE - 1);
                        ec = pthread_mutex_lock(&Table[i].lock); assert(ec == 0);
                        if (!Table[i].ptr) {
                            Table[i].ptr = new SharedPtr<TestObj>;
                            counters.new_default++;
                        }
                        ec = pthread_mutex_unlock(&Table[i].lock); assert(ec == 0);
                    // Create with copy constructor.
                    } else {
                        int i = rand(0, TABLE_SIZE - 1), j;
                        do {
                            j = rand(0, TABLE_SIZE - 1);
                        } while (i == j);

                        // Order to avoid deadlock.
                        if (i < j) {
                            ec = pthread_mutex_lock(&Table[i].lock); assert(ec == 0);
                            ec = pthread_mutex_lock(&Table[j].lock); assert(ec == 0);
                        } else {
                            ec = pthread_mutex_lock(&Table[j].lock); assert(ec == 0);
                            ec = pthread_mutex_lock(&Table[i].lock); assert(ec == 0);
                        }

                        if (!Table[i].ptr && Table[j].ptr) {
                            Table[i].ptr = new SharedPtr<TestObj>(*Table[j].ptr);
                            counters.new_copy++;
                        }

                        ec = pthread_mutex_unlock(&Table[i].lock); assert(ec == 0);
                        ec = pthread_mutex_unlock(&Table[j].lock); assert(ec == 0);
                        // printf("%d: assign %d=%d done\n", (int) tid, i, j);
                    }
                }
                break;
            case 4:
                // Delete
                {
                    int i = rand(0, TABLE_SIZE - 1);
                    ec = pthread_mutex_lock(&Table[i].lock); assert(ec == 0);
                    if (Table[i].ptr) {
                        delete Table[i].ptr;
                        Table[i].ptr = 0;
                        counters.delet++;
                    }
                    ec = pthread_mutex_unlock(&Table[i].lock); assert(ec == 0);
                }
                break;
            default:
                fprintf(stderr, "Bad case: %d\n", act);
                abort();
                break;
        }
    }

    double rs = RunSecs;
    printf("%10lu: assignment_new=%lu ops, assignment=%lu ops, reset=%lu ops, new_default=%lu ops, new_copy=%lu ops, delete=%lu ops\n"
     "    rates: assignment_new=%.0f ops/sec, assignment=%.0f ops/sec, reset=%.0f ops/sec, new_default=%.0f ops/sec, new_copy=%.0f ops/sec, delete=%.0f ops/sec\n",
     (unsigned long) tid,
     counters.assignment_new, counters.assignment, counters.reset, counters.new_default, counters.new_copy, counters.delet,
     double(counters.assignment_new)/rs, double(counters.assignment)/rs, double(counters.reset)/rs, double(counters.new_default)/rs, double(counters.new_copy)/rs, double(counters.delet)/rs);

    return NULL;
}

void
threaded_test() {

    int ec;

    size_t base = AllocatedSpace;

    Table = new TableEntry[TABLE_SIZE];

    printf("Running threaded test for %d seconds.\n", RunSecs);

    StartTime = time(NULL);
    pthread_t tid1, tid2, tid3, tid4;
    ec = pthread_create(&tid1, 0, run, (void *) 1); assert(ec == 0);
    ec = pthread_create(&tid2, 0, run, (void *) 2); assert(ec == 0);
    ec = pthread_create(&tid3, 0, run, (void *) 3); assert(ec == 0);
    ec = pthread_create(&tid4, 0, run, (void *) 3); assert(ec == 0);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);
    pthread_join(tid4, NULL);

    delete [] Table;

    if (base != AllocatedSpace) {
        printf("Leaked %zu bytes in threaded test.\n", AllocatedSpace - base);
        abort();
    }
}



/* Local Variables: */
/* c-basic-offset: 4 */
/* End: */
/* vim: set filetype=cpp tabstop=8 shiftwidth=4 softtabstop=4: */
