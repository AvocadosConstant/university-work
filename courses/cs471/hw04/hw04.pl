/* Tim Hung (thung1)
   Homework Assignment 4 - Prolog 2
   Programming Languages
   CS471, Spring 2017
   Binghamton University */

/* Homework 4 */

/* Due: , 2/13 at 11:59 PM */

/* Purpose: To get comfortable with backtracking, recursion,
   become familar with reflective mechanism of Prolog,
   and Prolog as a symbolic programming language.
*/


/* Problem 1:
   Write a predicate sumlist(List,Sum) which succeeds if Sum is the total value
   of all the elements of List. This will be a top down recursion.
   The recursion clause will add the current value to the result of the sum
   of the rest of the list.
   We have already provided the base case for this predicate underneath
   'Problem 1 Answer'. You just need to add the recursive clause.
*/

/* Problem 1 Answer */

sumlist([], 0).
sumlist([Cur|Tail], Sum) :- sumlist(Tail, TailSum),
                            Sum is TailSum + Cur.

/* Problem 1 Test */
/* There should be no warnings when compiling,
   tests which are supposed to fail are written as such */

%%:- sumlist([], 0).
%%:- sumlist([], 1) -> fail ; true.
%%:- sumlist([1,2,3,4], 10).
%%:- sumlist([1], 1).

/* Problem 2:
   Write the predicate sumlist2(List,Sum) which succeeds if Sum is the sum total
   of all the elements of List. Instead of adding the current value to the
   result of the sum of the tail, you will calculate the partial sum of the all
   the elements you have reached so far. You will need an extra argument to
   store the partial sum, so you will write an auxilitary predicate sumlist2/3
   to handle the extra argument.

   Underneath 'Problem 2 Answer' we have provided sumlist2/2, which calls the
   auxiliary predicate sumlist2/3. We have also provided the base case for the
   auxiliary predicate. You just need to add the recursive clause for
   sumlist2/3.

*/

/* Problem 2 Answer */

sumlist2(List,Sum) :- sumlist2(List, 0, Sum).
sumlist2([], Sum, Sum).
sumlist2([Cur|Tail], Partial, Sum) :- NewPartial is Partial + Cur,
                                      sumlist2(Tail, NewPartial, Sum).

/* Problem 2 Test */

%%:- sumlist2([], 0).
%%:- sumlist2([], 1) -> fail ; true.
%%:- sumlist2([1,2,3,4], 10).
%%:- sumlist2([1], 1).

/* Problem 3:
   Write the predicate sumPartialR(N, SumLst), which succeeds as follows:
   given a number N, SumLst is a sequence of sums such that first number in
   S is the sum of all the numbers from N to 1, the second number in S the sum
   of all the numbers from N-1 down to 1, and so on.
   In other words, SumLst = [N+(N-1)+..+1, (N-1)+(N-2)+..+1, ..., 1].
   For example:

     ?- sumPartialR(6,S).
     S = [21, 15, 10, 6, 3, 1] .

   This problem can be solved in 2 clauses.
*/


/* Problem 3 Answer */

sumPartialR(1, [1]).
sumPartialR(N, [Partial|Tail]) :- M is N - 1,
                                  [First|_] = Tail,
                                  Partial is N + First,
                                  sumPartialR(M, Tail).


/* Problem 3 Test */

%%:- sumPartialR(1, [1]).
%%:- sumPartialR(1, []) -> fail ; true.
%%:- sumPartialR(2, [3, 1]).
%%:- sumPartialR(6, [21, 15, 10, 6, 3, 1]).



/* Problem 4:
   Write the predicate sumPartialL(N, SumLst). This problem is very similar to
   Problem 3, but has one key difference. The sum totals accumulate from left
   to right, so the SumLst generated will be different. For example, the first
   value in S will be N, the second value will be N + (N-1), and so on.
   In other words, SumLst = [N, N+(N-1), ..., N+(N-1)+(N-2)+...+1].
   For example,

     ?- sumPartialL(6,S).
     S = [6, 11, 15, 18, 20, 21]

   It would be helpful to follow the idea used in problem 2. So your first
   clause should be:

       sumPartialL(N,Lst):-sumPartialL(N,N,Lst).

   You need to add 2 additional clauses.*/

/* Problem 4 Answer */


sumPartialL(N, Lst) :- sumPartialL(N, N, Lst).
sumPartialL(1, P, [P]).
sumPartialL(N, Partial, [Partial|Tail]) :- M is N - 1,
                                           NewPartial is Partial + M,
                                           sumPartialL(M, NewPartial, Tail).

/* Problem 4 Test */

%%:- sumPartialL(1, [1]).
%%:- sumPartialL(1, []) -> fail ; true.
%%:- sumPartialL(6, [6, 11, 15, 18, 20, 21]).


/* Problem 5:
   A) Give a formal mathematical definition of:
     a) a relation
     b) a function 
   B) Is every function a relation? If false, give a counter example.
   C) Is every relation a function? If false, give a counter example. */

/* Problem 5 Answer: */

/*
    A)
        a) A relation between two sets is any subset of the cartesian product of those sets.
        b) A function is a binary relation where each input has exactly one output.
    B) Yes.
    C) No. \[ y = \sqrt(x) \] is not a function, because for each x, there are two outputs.
*/




/* Problem 6:
   We will use a predicate edge(X,Y) to encode a graph.
   edge(X,Y) is true if there is a directed edge from X to Y.
   The following is a mini graph encoded in Prolog. */

edge(a,b).
edge(a,e).
edge(a,c).
edge(b,a).
edge(b,c).
edge(b,d).
edge(c,e).
edge(f,e).

/* Using your knowledge of backtracking and the findall predicate, write
   predicates outgoing/2 and incoming/2.

   outgoing(X,Y) should succeed if Y is a list of all immediate vertices
   reached from X's outgoing edges. incoming(X,Y) should succeed if Y is a
   list of all vertices that have outgoing edges to X.
*/

/* Problem 6 Answer */

outgoing(X, Y) :- findall(Z, edge(X, Z), Y).
incoming(X, Y) :- findall(Z, edge(Z, X), Y).


/* Problem 6 Test */
%%:- outgoing(a,X), X = [b,e,c].
%%:- outgoing(e,X), X = [].
%%:- outgoing(a,X), X = [b,e,c].
%%:- incoming(a,X), X = [b].
%%:- incoming(f,X), X = [].
%%
%%:- outgoing(e,X), X = [a] -> fail ; true.
%%:- incoming(e,X), X = [] -> fail ; true.



/* Problem 7:
   Define homoiconic.
   Is Prolog homoiconic?
   What does it mean to say a language is fully reflective?
   Is Prolog fully reflective?

   (See page 584 and Chapter 12)
*/

/* Problem 7 Answer: */

/*

    Homoiconic programming languages have program structure similar to their syntax,
        and therefore their program's internal representations can be inferred by reading the code.
    Yes.
    A fully reflective programming language has the ability to examine,
        introspect, and modify its own structure and behavior at runtime.
    No.



*/


/* Problem 8:
   Write a predicate computeS/4. computeS(Op, Arg1, Arg2, Result) succeeds if
   Result is the value after computing Arg1 Op Arg2. Use the insight you gained
   in Problem 0B. Op must be a builtin Prolog operator.
*/

/* Problem 8 Answer: */

computeS(Op, Arg1, Arg2, Result) :- Terms =.. [Op, Arg1, Arg2],
                                    Result is Terms.


/* Problem 8 Test: */
%%:- computeS(-, 19, 7, 12).
%%:- computeS(div, 19, 7, 2).
%%:- computeS(div, 19, 7, R), R = 2.
%%
%%:- computeS(/, 19, 7, 2) -> fail ; true.
%%:- catch((computeS(sin, 90, 1, _), fail), error(_Err, _Context), true).




/* Problem 9:
   In class we discussed the 'is' predicate for evaluating expressions. Write a
   predicate results/2.
   result(Elst,RLst) succeeds if Rlst is unifies with the values computed from
   the list of expressions, Elst.
*/

/* Problem 9 Answer: */

result([], []).
result([Expr|ETail], [Result|RTail]) :- Result is Expr,
                                        result(ETail, RTail).


/* Problem 9 Test */
%%:- result([],[]).
%%:- result([+(3,7), mod(104,7),-(5)],[10, 6, -5]).
%%:- result([+(3,7), +(15, -(3,11))],X), X = [10, 7].
%%
%%:- result([+(3,7), mod(104,7)],[10,13]) -> fail ; true.



/* Problem 10:
   A good example of symbolic computation is symbolic differentiation. Below
   are the rules for symbolic differentiation where U, V are mathematical
   expressions, C is a number constant, N is an integer constant and x is a
   variable:

        dx/dx = 1
        d(C)/dx = 0.
        d(Cx)/dx = C
        d(-U)/dx = -(dU/dx)
        d(U+V)/dx = dU/dx + dV/dx
        d(U-V)/dx = dU/dx - dV/dx
        d(U*V)/dx = U*(dV/dx) + V*(dU/dx)
        d(U^N)/dx = N*U^(N-1)*(dU/dx)

   Translate these rules into Prolog. (Please keep the order of the rules the
   same for testing purposes).
*/

/* Problem 10 Answer: */

d(x,x,1).
d(C,x,0) :- number(C).
d(C*x,x,C) :- number(C).
d(-U, X, -DU) :- d(U, X, DU).
d( U + V, x, RU + RV ) :- d(U, x, RU), d(V, x, RV).
d( U - V, x, RU - RV ) :- d(U, x, RU), d(V, x, RV).
d(U * V,x, U * DV + V * DU):- d(U, x, DU), d(V, x, DV).
d(U ^ N, x, N * (U^M) * DU) :- M is N-1, d(U, x, DU).



/* Problem 10 Test: */
%%:- d(x,x,R), R = 1 .
%%:- d(7,x,R), R = 0 .
%%:- d(7*x,x,R), R = 7 .
%%:- d(x +2*(x^3 + x*x),x,Result), Result = 1+ (2* (3*x^2*1+ (x*1+x*1))+ (x^3+x*x)*0) .
%%:- d(-(1.24*x -x^3),x,Result), Result = - (1.24-3*x^2*1) .
%%:- d(-(1.24*x -2*x^3),x,Result), Result = - (1.24- (2* (3*x^2*1)+x^3*0)) .

% Pay careful attention to why this fails.
% :- d(x +2*(x^3 + x*x),x,Result), Result = 1+ (2* (3*x^(3-1)*1+ (x*1+x*1))+ (x^3+x*x)*0) -> fail ; true.



/* Problem 11:
   (Exercise 3.5 from Learn Prolog Now!)
   Binary trees are trees where all internal nodes have exactly two children.
   The smallest binary trees consist of only one leaf node. We will represent
   leaf nodes as leaf(Label) . For instance, leaf(3) and leaf(7) are leaf
   nodes, and therefore small binary trees.

   Given two binary trees B1 and B2 we can combine them into one binary tree
   using the functor tree/2 as follows: tree(B1,B2) .
   So, from the leaves leaf(1) and leaf(2) we can build the binary tree
   tree(leaf(1),leaf(2)) .
   From the binary trees tree(leaf(1),leaf(2)) and leaf(4) we can build
   tree( tree(leaf(1), leaf(2)), leaf(4)) .

   Define a predicate swap/2 , which produces the mirror image of the binary
   tree that is its first argument. For example:

   ?-  swap( tree( tree(leaf(1), leaf(2)), leaf(4)), T).
   T  =  tree( leaf(4), tree(leaf(2), leaf(1))).
*/

/* Problem 11 Answer: */

swap(leaf(A), leaf(A)).
swap(tree(A, B), tree(SB, SA)) :- swap(B, SB), swap(A, SA).


/* Problem 11 Test: */
%%:- swap( tree( tree(leaf(1), leaf(2)), leaf(4)), T), T  =  tree( leaf(4), tree(leaf(2), leaf(1))).
%%:- swap(leaf(1), leaf(1)).
%%:- swap(tree(leaf(1), leaf(2)), tree(leaf(1), leaf(2))) -> fail ; true.



