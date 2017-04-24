#ifndef _FUNCTION_HPP_
#define _FUNCTION_HPP_

namespace cs540 {

  template <typename> class Function;

  template <typename ResultType, typename ... ArgumentTypes>
  class Function<ResultType(ArgumentTypes ...)> {

  }; // class Function

} // namespace cs540

#endif
