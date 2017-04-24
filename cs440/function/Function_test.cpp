#include "Function.hpp"
#include <iostream>
#include <cassert>

int ret_one_hundred_func() {
  return 100;
}

struct ret_two_hundred_functor_t {
  int operator()(){
    return 200;
  }
};

int sumrange(int a, int b) {
  assert(a<=b);
  return a<b ? a + sumrange(a+1,b) : b;
}

int main(void) {
  auto ret_three_hundred_lambda_func = [](){
    return 300;
  };
  
  {
    //Test default construction
    cs540::Function<int()> default_constructed;
    
    //Test value construction with a free-function
    cs540::Function<int()> ret_one_hundred(ret_one_hundred_func);
    
    //Test value construction with a lambda-function
    cs540::Function<int()> ret_three_hundred_lambda(ret_three_hundred_lambda_func);
    
    //Test value construction with a functor
    cs540::Function<int()> ret_two_hundred_functor(ret_two_hundred_functor_t{});
    
    //Test function operator on default constructed
    int testval = 30;
    try {
      default_constructed();
    } catch(cs540::BadFunctionCall &bfc) {
      //We modify testval here so that we can assert that a change happened later to make sure an exception was caught
      testval += 10;
    }
    assert(testval == 40);
    
    //Test function operator on free-function target, also test that results are correct
    assert(ret_one_hundred() == ret_one_hundred_func());
    
    //Test function operator on functor target, also test that results are correct
    assert(ret_two_hundred_functor() == ret_two_hundred_functor_t{}());
    
    //Test function operator on lambda target, also test that results are correct
    assert(ret_three_hundred_lambda() == ret_three_hundred_lambda_func());
    
    {
      //Test assignment from Function
      cs540::Function<int()> tmp;
      tmp = ret_one_hundred;
      assert(tmp() == ret_one_hundred_func());
    }
    
    {
      //Test assignment from free-function
      cs540::Function<int()> tmp;
      tmp = ret_one_hundred_func;
      assert(tmp() == ret_one_hundred_func());
    }
    
    {
      //Test assignment from Function containing functor
      cs540::Function<int()> tmp;
      tmp = ret_two_hundred_functor;
      assert(tmp() == ret_two_hundred_functor_t{}());
    }
    
    {
      //Test assignment from functor
      cs540::Function<int()> tmp;
      ret_two_hundred_functor_t functor;
      tmp = functor;
      assert(tmp() == ret_two_hundred_functor_t{}());
    }
    
    {
      //Test assignment from Function containing lambda
      cs540::Function<int()> tmp;
      tmp = ret_three_hundred_lambda;
      assert(tmp() == ret_three_hundred_lambda_func());
    }
    
    {
      //Test assignment from lambda
      cs540::Function<int()> tmp;
      tmp = ret_three_hundred_lambda_func;
      assert(tmp() == ret_three_hundred_lambda_func());
    }
    
    {
      //Test equality operators
      assert((!ret_one_hundred.operator bool()) == (ret_one_hundred == nullptr));
      assert((!ret_one_hundred.operator bool()) == (nullptr == ret_one_hundred));
      
      //Test equality operators with a default constructed object
      cs540::Function<void(void)> tmp;
      assert((!tmp.operator bool()) == (tmp == nullptr));
      assert((!tmp.operator bool()) == (nullptr == tmp));
    }
    
    {
      //Test inequality operators
      assert(ret_one_hundred.operator bool() == (ret_one_hundred != nullptr));
      assert(ret_one_hundred.operator bool() == (nullptr != ret_one_hundred));
      
      //Test inequality operators with a default constructed object
      cs540::Function<void()> tmp;
      assert(false == (tmp != nullptr));
      assert(false == (nullptr != tmp));
    }
    
    {
      cs540::Function<int()> tmp(ret_one_hundred);
      assert(ret_one_hundred() == tmp());
      tmp = ret_two_hundred_functor;
      assert(ret_two_hundred_functor() == tmp());
      tmp = ret_three_hundred_lambda;
      assert(ret_three_hundred_lambda() == tmp());
    }
    
    {
      //Testing a function that takes arguments
      cs540::Function<int(int,int)> sum_range(sumrange);
      assert(sumrange(10,15) == sum_range(10,15));
    }
    
    {
      //Testing a recursive lambda that captures a value from the surrounding scope
      const int a = 30;
      cs540::Function<int(int)> sum_range = [a,&sum_range](int b) -> int {
        assert(a<=b);
        return a==b ? b : b + sum_range(b-1);
      };
      
      assert(sum_range(40) == sumrange(30,40));
    }
  }
}



