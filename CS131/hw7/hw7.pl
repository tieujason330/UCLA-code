/**
* Name: Jason Tieu
* ID: 304047667
* resources used:
  www.piazza.com
  www.stackoverflow.com
  www.google.com
  www.gprolog.org/manual/gprolog.html
*/

/** 
1. Implement a predicate duplist(X,Y) which succeeds whenever list Y is simply list X with each element duplicated. For example:

| ?- duplist([1,2,3], Y).

Y = [1,1,2,2,3,3]

Make sure that your predicate works in both directions. For example, you should be able to ask the query duplist(X, [1,1,2,2,3,3]) and get the right value of X.
*/
% base case %
duplist([], []). 
duplist([H|Rest1],[H1,H2|Rest2]) :- H=H1, H=H2, duplist(Rest1, Rest2).

/** 
Let's implement everyone's favorite data structure...wait for it...dictionaries! We'll represent a dictionary as a list of pairs, where each pair is represented as a two-element list. Implement a predicate put(K,V,D1,D2) that succeeds if D2 is the dictionary that results from mapping key K to value V in dictionary D1. If the key K is already mapped in D1, then put should (conceptually) replace it with the new key. In other words, D2 should only ever have at most one entry for a given key (assuming D1 satisfies this property). Also implement a predicate get(K,D,V) that succeeds if K is mapped to V in D. This being Prolog, these predicates are much cooler than the versions we implemented in other languages, because they can be used to answer a wide variety of queries. For example, get can not only get the value associated with a key but also get all keys associated with a particular value and also iterate over all key-value pairs in the dictionary. Some examples:

| ?- put(1,hello,[[2,two]],D).

D = [[2,two],[1,hello]] ? 

| ?- put(1,hello,[[1,one],[2,two]],D).

D = [[1,hello],[2,two]] ? 

| ?- put(1,hello,D,[[2,two],[1,hello]]).

D = [[2,two]] ? ;

D = [[2,two],[1,_]] ? ;

| ?- get(1,[[2,two],[1,hello]],V).      

V = hello ? 

| ?- get(K,[[2,hello],[1,hello]],hello). 

K = 2 ? ;

K = 1 ? ;

| ?- get(K,[[2,two],[1,hello]],V).      

K = 2
V = two ? ;

K = 1
V = hello ? ;
*/
% base case %
put(K,V,[],[[K,V]]). 
% K is in dict already %
put(K,V,[[K,_]|Rest], [[K,V]|Rest]). 
% K is not in dict %
put(K,V,[[A,B]|Rest1], [[A,B]|Rest2]) :- \+(K=A), put(K,V,Rest1,Rest2). 

% get %
get(K,[[K,V]|_], V).
get(K,[[_,_]|D], V) :- get(K,D,V).

/**
Problem 3
E ::= intconst(I) | boolconst(B) | var(X) | geq(E1,E2) | if(E1, E2, E3) | function(X, E) | funcall(E1, E2)

V ::= intval(I) | boolval(B) | funval(X,E,Env)
*/
% intconst(I)
eval(intconst(I), _, intval(I)).  

% boolconst(B)
eval(boolconst(B), _, boolval(B)).

% var(X)
eval(var(X), Env, V) :- get(X, Env, V). 

% geq(E1,E2)
eval(geq(E1,E2), Env, V) :- 
	eval(E1, Env, intval(I1)),
	eval(E2, Env, intval(I2)),
	((I1 >= I2) -> V=boolval(true) ; V=boolval(false)).

% if(E1,E2,E3)
eval(if(E1,E2,E3), Env, V) :-
	eval(E1, Env, boolval(B)), %first get a bool to use for if statement
	% if statement format: if -> then ; else	
	(
	(B=true)
	-> (eval(E2, Env, V2), V=V2)
	; (eval(E3, Env, V3), V=V3)
	).

% function(X,E)
eval(function(X,E), Env, funval(X,E,Env)).
	

% funcall(E1,E2)
eval(funcall(E1,E2), Env1, V) :-
	eval(E1, Env1, funval(X,E,Env2)), % First get the function being used
	eval(E2, Env1, A),                % get the argument
	append(Env1,Env2,Env3),           % append the environments from the function and call
	put(X,A,Env3, Env4),              % add the variable and argument to environment
	eval(E,Env4,V).
	

/**
You have a robot that knows how to do two kinds of actions. 
First, the robot can pick up the top block from a stack, as long as that stack is nonempty and the robot's mechanical hand is free. 
Second, the robot can place a block from its hand on to a stack.

three arguments: a start world, a list of actions, and a goal world.

world that has four components: three lists to represent the three stacks, and a component to represent the contents of the mechanical hand. That last component either contains a single block or the constant none, to represent the fact that the hand is empty. 
	For example, world([a,b,c],[],[d],none) represents a world in which there are four blocks, with three of them in the first stack (in the specified order, with a at the top of the stack), the fourth block on the third stack, and nothing in the mechanical hand.

We will use the constants stack1, stack2, and stack3 to represent the three stacks. There are two kinds of actions, pickup(S) and putdown(S), corresponding to the two robot actions described above, where S is one of the three stack constants.

For example, here is a sample query:

| ?- length(Actions,L), blocksworld(world([a,b,c],[],[],none), Actions, world([],[],[a,b,c],none)). 

Actions = [pickup(stack1),putdown(stack2),pickup(stack1),putdown(stack2),pickup(stack1),putdown(stack3),pickup(stack2),putdown(stack3),pickup(stack2),putdown(stack3)]
L = 10 ?

Notice how I use length to limit the size of the resulting list of actions. This is necessary to do when you test your code, in order to prevent Prolog from getting stuck down infinite paths (e.g., continually picking up and putting down the same block). 
*/
/**
% world(stack1, stack2, stack3, hand). %
testhand(H, L) :- H\=none.
teststack(S, S) :- S=[].
% stack1 actions %
act(world([stack1|rest1],stack2,stack3,hand), pickup(stack1), world(rest1, stack2, stack3, stack1)) :- 
	hand=none, [stack1|rest1]\=[].
act(world(rest1,stack2,stack3,hand), putdown(stack1), world(rest1, stack2, stack3, none)) :- 
	hand\=none, append([hand], rest1, rest1).

% stack2 actions %
act(world(stack1,[stack2|rest2],stack3,hand), pickup(stack2), world(stack1, rest2, stack3, stack1)) :- 
	hand=none, [stack2|rest2]\=[].
act(world(stack1,rest2,stack3,hand), putdown(stack2), world(stack1, rest2, stack3, none)) :- 
	hand\=none, append([hand], rest2, rest2).

% stack3 actions %
act(world(stack1,stack2,[stack3|rest3],hand), pickup(stack3), world(stack1, stack2, rest3, stack1)) :- 
	hand=none, [stack3|rest3]\=[].
act(world(stack1,stack2,rest3,hand), putdown(stack3), world(stack1, stack2, rest3, none)) :- 
	hand\=none, append([hand], rest3, rest3).


blocksworld(Start, [], Start).
blocksworld(Start, [A|As], End) :-
	act(Start, A, Mid), blocksworld(Mid, As, End).
*/

act(world([Block1|Rest1], Stack2, Stack3, none), pickup(stack1), world(Rest1, Stack2, Stack3, Block1)).
act(world(Stack1, [Block2|Rest2], Stack3, none), pickup(stack2), world(Stack1, Rest2, Stack3, Block2)).
act(world(Stack1, Stack2, [Block3|Rest3], none), pickup(stack3), world(Stack1, Stack2, Rest3, Block3)).

act(world(Stack1, Stack2, Stack3, Block), putdown(stack1), world([Block|Stack1], Stack2, Stack3, none)).
act(world(Stack1, Stack2, Stack3, Block), putdown(stack2), world(Stack1, [Block|Stack2], Stack3, none)).
act(world(Stack1, Stack2, Stack3, Block), putdown(stack3), world(Stack1, Stack2, [Block|Stack3], none)).

moves(End, End, []). %base case

moves(Start, End, [A|As]) :-
act(Start, A, Mid), moves(Mid, End, As).

% blocksworld(Start, [], Start).
blocksworld(Start, Actions, End) :- moves(Start, End, Actions).

