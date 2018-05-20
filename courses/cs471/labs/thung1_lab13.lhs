> module Thung1_lab13
>   where

Part A:
You need to do this part inorder to understand the rest of the assignment.

Examples using Haskell   

    \x -> x : is an example of a lambda abstraction which x  is a variable bound by lambda.

1. What is the type of  (\x -> x)?

   \x -> x :: t -> t

2. What is the value of (and verify by typing in hugs/ghci).
   a.  (\x -> x) 3 ? (Verify by typing in hugs/ghci).
       3

   b.  (\x -> x) (+) 5 3 ?
       8

   c.  (\x -> x) (+)?  
       (+)

   d.  What would be a descriptive name for the abstractions in 2.b?
       -- Identity function

3. What is the value of
   a.  (\x -> x +x ) 3 (verify by typing in hugs/ghci).
       6

   b.  What is the expression ? (E)
       3 + 3


4. What is the value of
   a.  (\x -> 3) 6 ? (verify by typing in hugs/ghci).
       3

   b.  What would be a descriptive name for this abstractions?
       returns3



Part C: Factorial
Add the following definition of factorial to your Haskell file and reload into hugs/ghci.

> factorial :: Integral a => a -> a
> factorial n = if n ==0 then 1 else n * (factorial (n - 1))

 Define hFact to be a lambda abstraction such that it takes a function as an argument, and returns another function, similar to hLen.  Write this so that hFact factorial = factorial. What is the type of hFact?  (Follow the model of hLen)

> hFact = (\f n -> if n == 1 then 1 else n * f(n-1))

 Apply hFact to ( ^ 2) -- What is the value of hFact (^ 2) 4?
    36

 What is the value of hFact factorial 5? Is it the same value as factorial 5?
    120
    Yes it is the same value as factorial 5
