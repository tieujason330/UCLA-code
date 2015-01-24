flip(west,east).
flip(east,west).
move([P,W,G,C], wolf, [O,O,G,C]) :-
	P=W, G\=C, flip(P,O).
move([P,W,G,C], goat, [O,W,O,C]) :-
	P=G, flip(P,O).
move([P,W,G,C], cabbage, [O,W,G,O]) :-
	P=C, W\=G, flip(P,O).
move([P,W,G,C], nothing, [O,W,G,C]) :-
	W\=G, G\=C, flip(P,O).

moves(Start, [], Start). % base case
moves(Start, [A|As], End) :-
	move(Start, A, Mid), moves(Mid, As, End).
