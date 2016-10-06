couleur(vert).
couleur(jaune).
couleur(rouge).

check(X,Y) :- X \== Y.

coloriageMachine(C1,C2,C3,C4) :-couleur(C1),couleur(C2),check(C1,C2),
				couleur(C3),check(C1,C3),check(C2,C3),
				couleur(C4),check(C1,C4),check(C3,C4).

coloriageC1(C1,C2,C3,C4) :-	couleur(C2),check(C1,C2),
				couleur(C3),check(C1,C3),check(C2,C3),
				couleur(C4),check(C1,C4),check(C3,C4).

coloriageC2(C1,C2,C3,C4) :-	couleur(C1),check(C1,C2),
				couleur(C3),check(C1,C3),check(C2,C3),
				couleur(C4),check(C1,C4),check(C3,C4).

coloriageC3(C1,C2,C3,C4) :-	couleur(C1),check(C1,C3),
				couleur(C2),check(C1,C2),check(C2,C3),
				couleur(C4),check(C1,C4),check(C3,C4).

coloriageC4(C1,C2,C3,C4) :-	couleur(C1),check(C1,C4),
				couleur(C2),check(C1,C2),
				couleur(C3),check(C1,C3),check(C2,C3),check(C3,C4).

coloriage(C1,C2,C3,C4):-write('Commencer par \n'),
			write('1, C1\n'),
			write('2, C2\n'),
			write('3, C3\n'),
			write('4, C4\n'),
			write('0, machine\n'),
			read(X),
			write('Couleur ?\n'),
			write('vert\n'),
			write('jaune\n'),
			write('rouge\n'),
			read(Y),
			colorie(X,Y,C1,C2,C3,C4).

colorie(1,Y,C1,C2,C3,C4) :-coloriageC1(Y,C2,C3,C4).
colorie(2,Y,C1,C2,C3,C4) :-coloriageC2(C1,Y,C3,C4).
colorie(3,Y,C1,C2,C3,C4) :-coloriageC3(C1,C2,Y,C4).
colorie(4,Y,C1,C2,C3,C4) :-coloriageC4(C1,C2,C3,Y).
colorie(0,Y,C1,C2,C3,C4) :-coloriageMachine(C1,C2,C3,C4).