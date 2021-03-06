#ifndef _SHARED_PTR_HPP_
#define _SHARED_PTR_HPP_

#include <iostream>
#include <cstddef>
#include <mutex>

namespace cs540 {

  std::mutex mtx;

  /** Reference Counting Control Block Base Class */
  struct Ref_Count_Base {

    Ref_Count_Base() : count{1} {}
    virtual ~Ref_Count_Base() {}

    void inc_ref() { 
      std::unique_lock<std::mutex> lock(mtx);
      ++count; 
    }
    void dec_ref() {
      std::unique_lock<std::mutex> lock(mtx);
      --count;
      if(count == 0) delete this;
    }

    std::size_t count;
  };

  /** Reference Counting Control Block Derived Class */
  template<class U> struct Ref_Count : public Ref_Count_Base {
    Ref_Count(U *p) : Ref_Count_Base{}, ptr(p) {}
    ~Ref_Count() { delete ptr; }

    private:
      U* ptr;
  };

  template <typename T> class SharedPtr {

    public:

      T *ptr;
      Ref_Count_Base *ref;

      //////////////////////////////
      // CTORS, ASSIGN OPS, DTOR //
      ////////////////////////////

      /** Default Constructor */
      SharedPtr() : ptr(nullptr), ref(nullptr) {}

      template <typename U>
        explicit SharedPtr(U *p) : ptr{p}, ref{new Ref_Count<U>(p)} {}

      /** Copy Constructor */
      // Can be replaced by just the templated version?
      SharedPtr(const SharedPtr &that) : ptr{that.ptr}, ref{that.ref} {
        if(ref) ref->inc_ref();
      }

      /** Templated Copy Constructor */
      template <typename U>
      SharedPtr(const SharedPtr<U> &that) : ptr{static_cast<T *>(that.ptr)}, ref{that.ref} {
        if(ref) ref->inc_ref();
      }

      /** Move Constructor */
      SharedPtr(SharedPtr &&that) : ptr{that.ptr}, ref{that.ref} {
        that.ptr = nullptr;
        that.ref = nullptr;
      }

      /** Templated Move Constructor */
      template <typename U>
      SharedPtr(SharedPtr<U> &&that) : ptr{static_cast<T *>(that.ptr)}, ref{that.ref} {
        that.ptr = nullptr;
        that.ref = nullptr;
      }

      /** Custom Constructor for Casts */
      template <typename U>
      SharedPtr(const SharedPtr<U> &sp, T *p) : ptr{p}, ref{sp.ref} {
        if(ref) ref->inc_ref();
      }

      /** Assignment Operator */
      SharedPtr &operator=(const SharedPtr &that) {
        if(that == *this) return *this;
        if(ref) ref->dec_ref();
        ptr = that.ptr;
        ref = that.ref;
        if(ref) ref->inc_ref();
        return *this;
      }

      /** Templated Assignment Operator */
      template <typename U>
      SharedPtr<T> &operator=(const SharedPtr<U> &that) {
        if(that == *this) return *this;
        if(ref) ref->dec_ref();
        ptr = that.ptr;
        ref = that.ref;
        if(ref) ref->inc_ref();
        return *this;
      }

      /** Move Assignment Operator */
      SharedPtr &operator=(SharedPtr &&that) {
        ptr = that.ptr;
        ref = that.ref;
        that.ptr = nullptr;
        that.ref = nullptr;
        return *this;
      }

      /** Templated Move Assignment Operator */
      template <typename U>
      SharedPtr &operator=(SharedPtr<U> &&that) {
        ptr = that.ptr;
        ref = that.ref;
        that.ptr = nullptr;
        that.ref = nullptr;
        return *this;
      }

      ~SharedPtr() {
        if(ref) ref->dec_ref();
      }

      ////////////////
      // MODIFIERS //
      //////////////

      void reset() {
        if(ref) ref->dec_ref();
        ptr = nullptr;
        ref = nullptr;
      }

      template <typename U> void reset(U *p) {
        if(ref) ref->dec_ref();
        ptr = p;
        ref = new Ref_Count<U>(p);
      }

      ////////////////
      // OBSERVERS //
      //////////////

      T *get() const { return ptr; }

      T &operator*() const { return *this->get(); }

      T *operator->() const { return this->get(); }

      explicit operator bool() const { return ptr != nullptr; }

      // Not required for project
      std::size_t use_count() {
        if(ref) return ref->count;
        return 0;
      }

  }; // class SharedPtr

  ///////////////////////////////
  // COMPARISON OPS AND CASTS //
  /////////////////////////////

  template <typename T1, typename T2>
  bool operator==(const SharedPtr<T1> &lhs, const SharedPtr<T2> &rhs) {
    return lhs.get() == rhs.get();
  }
  
  template <typename T>
  bool operator==(const SharedPtr<T> &lhs, std::nullptr_t) {
    return !lhs;
  }

  template <typename T>
  bool operator==(std::nullptr_t, const SharedPtr<T> &rhs) {
    return !rhs;
  }

  template <typename T1, typename T2>
  bool operator!=(const SharedPtr<T1> &lhs, const SharedPtr<T2> &rhs) {
    return !(lhs == rhs);
  }
  
  template <typename T>
  bool operator!=(const SharedPtr<T> &lhs, std::nullptr_t) {
    return lhs;
  }

  template <typename T>
  bool operator!=(std::nullptr_t, const SharedPtr<T> &rhs) {
    return rhs;
  }

  template <typename T, typename U>
  SharedPtr<T> static_pointer_cast(const SharedPtr<U> &sp) {
    return SharedPtr<T>(sp, static_cast<T *>(sp.get()));
  }

  template <typename T, typename U>
  SharedPtr<T> dynamic_pointer_cast(const SharedPtr<U> &sp) {
    return SharedPtr<T>(sp, dynamic_cast<T *>(sp.get()));
  }

} // namespace cs540

#endif
