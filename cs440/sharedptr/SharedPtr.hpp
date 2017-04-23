#ifndef _SHARED_PTR_HPP_
#define _SHARED_PTR_HPP_

#include <cstddef>

namespace cs540 {

  template <typename T> class SharedPtr {

    public:
      //////////////////////////////
      // CTORS, ASSIGN OPS, DTOR //
      ////////////////////////////
      SharedPtr();

      template <typename U>
        explicit SharedPtr(U *);

      SharedPtr(const SharedPtr &p);

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

      ~SharedPtr();

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
