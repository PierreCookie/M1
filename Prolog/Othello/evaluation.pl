/* modules d'évaluations d'une position */
:- include(representation).

/* Check si la ligne est dans la grille a la position NumLigne */
recupNumLigne([Ligne|_],Ligne,NumLigne,NumLigne):- !.
recupNumLigne([_|Q],Ligne,NumLigne,Inc) :- succNum(Inc,Inc1),recupNumLigne(Q,Ligne,NumLigne,Inc1).
ligneDansGrille(NumLigne,Grille,Ligne) :- recupNumLigne(Grille,Ligne,NumLigne,1).

/* Check si la valeur est dans la Ligne a la position Col*/
recupValeurCase([Valeur|_],Valeur,Col,Col) :- !.
recupValeurCase([_|Q],Valeur,Col,Inc) :- succAlpha(Inc,Inc1),recupValeurCase(Q,Valeur,Col,Inc1).
caseDansLigne(Col,Liste,Valeur):- recupValeurCase(Liste,Valeur,Col,a).

/*
grilleDeDepart(X),ligneDansGrille(4,X,Ligne),caseDansLigne(c,Ligne,Valeur).
Ligne = [-, -, -, o, x, -, -, -],
Valeur = (-).

*/


/* donne la valeur d'une case si elle existe */
donneValeurDeCase(Grille,NumLigne,NumColonne,Valeur):- recupNumLigne(Grille,Ligne,NumLigne,1), recupValeurCase(Ligne,Valeur,NumColonne,a).

/*
grilleDeDepart(X),donneValeurDeCase(X,4,d,Valeur).

Valeur = o.
*/

/* isVide? */
equal(X,X).
caseVide(Grille,NumLigne,NumColonne) :- donneValeurDeCase(Grille,NumLigne,NumColonne,Valeur),equal(Valeur,-).

/* implementation des directions */

dir(east).
dir(west).
dir(north).
dir(south).
dir(southWest).
dir(northWest).
dir(southEast).
dir(northEast).

direction(east,0,1).
direction(west,0,-1).
direction(north,-1,0).
direction(south,1,0).
direction(northEast,-1,1).
direction(northWest,-1,-1).
direction(southEast,1,1).
direction(southWest,1,-1).

getLigne(X,1,Z):- succNum(X,Z).
getLigne(X,-1,Z) :- succNum(Z,X).
getLigne(X,0,X).

getColumn(X,1,Z):- succAlpha(X,Z).
getColumn(X,-1,Z) :- succAlpha(Z,X).
getColumn(X,0,X).

donneListeCasesDansDirection(Dir,Grille,NumLig,NumCol,[Valeur|Q]):-
        direction(Dir,X,Y),
        getLigne(NumLig,X,NumLig1),
        getColumn(NumCol,Y,NumCol1),
        donneValeurDeCase(Grille,NumLig1,NumCol1,Valeur),
        donneListeCasesDansDirection(Dir,Grille,NumLig1,NumCol1,Q),!.

donneListeCasesDansDirection(_,_,1,_,[]) :- !.
donneListeCasesDansDirection(_,_,8,_,[]) :- !.
donneListeCasesDansDirection(_,_,_,a,[]) :- !.
donneListeCasesDansDirection(_,_,_,h,[]) :- !.

/*

grilleDeDepart(X), donneListeCasesDansDirection(north,X,8,d,Z).
X = [[-, -, -, -, -, -, -, -], [-, -, -, -, -, -, -|...], [-, -, -, -, -, -|...], [-, -, -, o, x|...], [-, -, -, x|...], [-, -, -|...], [-, -|...], [-|...]],
Z = [-, -, x, o, -, -, -].

grilleDeDepart(X), donneListeCasesDansDirection(northEast,X,8,a,Z).
X = [[-, -, -, -, -, -, -, -], [-, -, -, -, -, -, -|...], [-, -, -, -, -, -|...], [-, -, -, o, x|...], [-, -, -, x|...], [-, -, -|...], [-, -|...], [-|...]],
Z = [-, -, x, x, -, -, -].
*/

oppose(x,o).
oppose(o,x).

faitPrise(Camp,[O,Camp|_]) :- oppose(Camp,O).
faitPrise(Camp,[X|T]) :- oppose(Camp,X),faitPrise(Camp,T).

/*

| ?- grilleDeDepart(X),donneListeCasesDansDirection(northEast,X,6,c,Z),faitPrise(o,Z).

X = [[-,-,-,-,-,-,-,-],[-,-,-,-,-,-,-,-],[-,-,-,-,-,-,-,-],[-,-,-,o,x,-,-,-],[-,-,-,x,o,-,-,-],[-,-,-,-,-,-,-,-],[-,-,-,-,-,-,-,-],[-,-,-,-,-,-,-,-]]
Z = [x,x,-,-,-]

yes
*/

leCoupEstValide(Grille,Camp,NumLigne,NumCol) :- caseVide(X,NumLigne,NumCol),donneListeCasesDansDirection(Dir,Grille,NumLigne,NumCol,Retour),faitPrise(Camp,Retour).

retournePionDansDirection(Dir,Depart,NumLig,NumCol,Valeur