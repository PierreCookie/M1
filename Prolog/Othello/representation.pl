/* Module de représentations du jeu */

grilleDeDepart(
		[[-,-,-,-,-,-,-,-],
		[-,-,-,-,-,-,-,-],
		[-,-,-,-,-,-,-,-],
		[-,-,-,o,x,-,-,-],
		[-,-,-,x,o,-,-,-],
		[-,-,-,-,-,-,-,-],
		[-,-,-,-,-,-,-,-],
		[-,-,-,-,-,-,-,-]]).

/* Affichage d'une ligne */
afficheLigne([]).
afficheLigne([X|L]) :- write(X),write('|'),afficheLigne(L).

/*
| ?- afficheLigne([-,-,-,-,x,o,-,-]).
-|-|-|-|x|o|-|-|
*/

afficheGrille2([Q],X) :-  nl,write(X),write(' |'),afficheLigne(Q),!.
afficheGrille2([T|Q],X) :- nl,write(X),write(' |'),afficheLigne(T),succNum(X,Y),afficheGrille2(Q,Y).
afficheGrille(X) :-write('   a b c d e f g h'),afficheGrille2(X,1).

/*

grilleDeDepart(X),afficheGrille(X).
   a b c d e f g h
1 |-|-|-|-|-|-|-|-|
2 |-|-|-|-|-|-|-|-|
3 |-|-|-|-|-|-|-|-|
4 |-|-|-|o|x|-|-|-|
5 |-|-|-|x|o|-|-|-|
6 |-|-|-|-|-|-|-|-|
7 |-|-|-|-|-|-|-|-|
8 |-|-|-|-|-|-|-|-|

*/

/* passer d'une ligne a l'autre */
succNum(1,2).
succNum(2,3).
succNum(3,4).
succNum(4,5).
succNum(5,6).
succNum(6,7).
succNum(7,8).


/* passer d'une case a l'autre */
succAlpha(a,b).
succAlpha(b,c).
succAlpha(c,d).
succAlpha(d,e).
succAlpha(e,f).
succAlpha(f,g).
succAlpha(g,h).
