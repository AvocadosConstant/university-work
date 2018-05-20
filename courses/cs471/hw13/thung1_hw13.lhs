> module Thung1_hw13
>   where

Part B: What do we do with the ??? ?

Type (Do NOT cut and paste) the following non-recursive definition in a Haskell file and load into hugs/ghci:

> hLen :: (Num u, Eq t) => ([t] -> u) -> [t] -> u 
> hLen = (\f x -> if x == [] then 0 else 1 + (f (tail x)))
> myLength ls = if ls == [] then 0 else 1 + myLength (tail ls)

Look at hLen --

Why is hLen recursive?  Why or why not.

    No. It doesn't call itself.


Why is hLen a HOF (higher order function)? Why or why not?

    It is a HOF because it takes in a function as its first parameter.


What is the value of 
    hLen sum [4,5,6] ?
        12

    hLen head [4,5,6] ?
        6

Does hLen have anything to do with sum or head?
    No. Those functions just happen to be the arguments in those examples.


What is the value of 
    hLen myLength [4,5,6] ?
        3

What is the relationship  between hLen and myLength?
    If hLen is called with myLength, it will return the length of the list.




> factorial :: Integral a => a -> a
> factorial n = if n ==0 then 1 else n * (factorial (n - 1))
> hFact = (\f n -> if n == 1 then 1 else n * f(n-1))


Part D: General definition

Here hFact factorial is factorial, i.e. the factorial function is the “smallest” fixed point of hFact

In general, to give meaning to the recursive function

    f = (\ x. if (cond(x)) then val(x) else expr(f, x))

(which cannot be expressed in lambda-notation), we define

    Hf =(\ F -> \ x -> if (cond(x)) then val(x) else expr(F, x))

Fix ( Haskell's version of the fixed-point combinator)

Add the following definition of factorial' to your haskell file:

> factorial' = hFact factorial'

Remember that if x = f x then x is the fix point of f  so hFact factorial' equals factorial'

What is the type of factorial' ?

> factorial' :: Integer -> Integer


Now we can define our fix point operator ( Haskells equivalent Y combinator)

> fix f = f (fix f )

What is the type of fix?

> fix :: (t -> t) -> t

What is the difference between the code below?

fix f = f (fix f )

    and

fix f = f fix f

    In the first definition, f will take the result of (fix f). In the second, it will take fix and f as 2 parameters.    


[10pts]
Combining all we have done -- What is the value of 

factorial 6   (definition given in part C)
    720

hFact factorial 6 ( you defined in part C #2 )
    720

factorial' 6 (definition given in part D #1)
    720

fix hFact 6 (definition given in part D)
    720


Hope these examples pique your curiousity.
    ok
