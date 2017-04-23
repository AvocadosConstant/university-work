#ifndef _SHARED_PTR_HPP_
#define _SHARED_PTR_HPP_

#include <iostream>
#include <cstddef>

namespace cs540 {

  struct Ref_Count_Base {

    Ref_Count_Base() : count(1) {}
    virtual ~Ref_Count_Base() {}

    void inc_ref() { ++count; }
    void dec_ref() {
      --count;
      if(count <= 0) delete this;
    }

    std::size_t count;
  };

  template<class U> struct Ref_Count : public Ref_Count_Base {
    Ref_Count(U *p) : ptr(p) {}
    ~Ref_Count() { delete ptr; }

    private:
      U* ptr;
  };

  template <typename T> class SharedPtr {

    T *ptr;
    Ref_Count_Base *ref;

    public:
      //////////////////////////////
      // CTORS, ASSIGN OPS, DTOR //
      ////////////////////////////

      /** Default Constructor */
      SharedPtr() : ptr(nullptr), ref(nullptr) {}

      template <typename U>
        explicit SharedPtr(U *p) : ptr{p}, ref{new Ref_Count<U>(p)} {}

      SharedPtr(const SharedPtr &p) : ptr{p.ptr}, ref{p.ref} {
        std::cout << "Constructing with SharedPtr(const SharedPtr &p)" << std::endl;
        if(ref != nullptr) ref->inc_ref();
      }

      template <typename U>
        SharedPtr(const SharedPtr<U> &p);

      SharedPtr(SharedPtr &&p);

      template <typename U>
        SharedPtr(SharedPtr<U> &&p);

      SharedPtr &operator=(const SharedPtr &);

      template <typename U>
        SharedPtr<T> &operator=(const SharedPtr<U> &);

      SharedPtr &operator=(SharedPtr &&p);

      template <typename U>
        SharedPtr &operator=(SharedPtr<U> &&p);

      ~SharedPtr() {
        std::cerr << "Destructing a SharedPtr" << std::endl;
        if(ref != nullptr) ref->dec_ref();
      }

      ////////////////
      // MODIFIERS //
      //////////////

      void reset();

      template <typename U>
        void reset(U *p);

      ////////////////
      // OBSERVERS //
      //////////////

      T *get() const;

      T &operator*() const;

      T *operator->() const;

      explicit operator bool() const;

      // Not required for project
      std::size_t use_count() {
        return ref->count;
      }

  }; // class SharedPtr

  ///////////////////////////////
  // COMPARISON OPS AND CASTS //
  /////////////////////////////

  template <typename T1, typename T2>
    bool operator==(const SharedPtr<T1> &, const SharedPtr<T2> &);
  
  template <typename T>
    bool operator==(const SharedPtr<T> &, std::nullptr_t);

  template <typename T>
    bool operator==(std::nullptr_t, const SharedPtr<T> &);

  template <typename T1, typename T2>
    bool operator!=(const SharedPtr<T1> &, const SharedPtr<T2> &);
  
  template <typename T>
    bool operator!=(const SharedPtr<T> &, std::nullptr_t);

  template <typename T>
    bool operator!=(std::nullptr_t, const SharedPtr<T> &);

  template <typename T, typename U>
    SharedPtr<T> static_pointer_cast(const SharedPtr<U> &sp);

  template <typename T, typename U>
    SharedPtr<T> dynamic_pointer_cast(const SharedPtr<U> &sp);

} // namespace cs540

#endif
