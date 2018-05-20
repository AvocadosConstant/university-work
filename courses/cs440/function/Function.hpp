#ifndef _FUNCTION_HPP_
#define _FUNCTION_HPP_

#include <iostream>
#include <memory>

namespace cs540 {

  struct BadFunctionCall : public std::exception {
    const char * what () const throw () {
      return "cs540 Bad Function Call Exception";
    }
  };

  template <typename T> struct Callable;
  template <typename ResultType, typename ... ArgumentTypes> 
  struct Callable<ResultType(ArgumentTypes...)> {

    typedef ResultType (*Func)(ArgumentTypes ...);

    Func* func;

    Callable() : func{nullptr} {}

    Callable(Func a) : func{a} {}

    virtual ~Callable() { delete func; }

    virtual ResultType operator()(ArgumentTypes... args) const {
      if(func == nullptr)
        throw BadFunctionCall("Bad Function Call Exception");
      return ((*func)(std::forward<ArgumentTypes>(args)...));
    }
  };

  template <typename> class Function;
  template <typename ResultType, typename ... ArgumentTypes>
  class Function<ResultType(ArgumentTypes ...)> {

    public:

      Callable<ResultType(ArgumentTypes...)>* fn;

      /** Default Constructor */
      Function() : fn{nullptr} {} 

      template <typename FunctionType> Function(FunctionType f)
        : fn{new Callable<ResultType(ArgumentTypes...)>(f)} {}

      /** Copy Constructor */
      Function(const Function &that) {
        delete fn;
        fn = that.fn;
      } 

      /** Assignment Operator */
      Function &operator=(const Function &that) { 
        if(&that == this) return *this;
        delete fn;
        fn = that.fn;
        return *this;
      }

      ~Function() {
        if(fn != nullptr) delete fn;
      }


      // Call the wrapped function target with the given arguments and return its result.
      // Throw cs540::BadFunctionCall if a callable function is not stored.
      ResultType operator()(ArgumentTypes... args) {
        if(fn == nullptr) throw BadFunctionCall();
        return ((*fn)(std::forward<ArgumentTypes>(args)...));
      }

      // Return true if the cs540::Function 
      // references a callable function
      explicit operator bool() const {
        return fn != nullptr;
      }

  }; // class Function

  template <typename ResultType, typename... ArgumentTypes>
  bool operator==(const Function<ResultType(ArgumentTypes...)> &f, std::nullptr_t) {
    return !f;
  }

  template <typename ResultType, typename... ArgumentTypes>
  bool operator==(std::nullptr_t, const Function<ResultType(ArgumentTypes...)> & f) {
    return !f;
  }

  template <typename ResultType, typename... ArgumentTypes>
  bool operator!=(const Function<ResultType(ArgumentTypes...)> &f, std::nullptr_t) {
    return bool(f);
  }

  template <typename ResultType, typename... ArgumentTypes>
  bool operator!=(std::nullptr_t, const Function<ResultType(ArgumentTypes...)> & f) {
    return bool(f);
  }

} // namespace cs540

#endif
