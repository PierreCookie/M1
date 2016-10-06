
xorn(1,1,0).
xorn(1,0,1).
xorn(0,1,1).
xorn(0,0,0).
nand(1,1,0).
nand(1,0,1).
nand(0,1,1).
nand(0,0,1).
non(1,0).
non(0,1).

circuit(X,Y,Z) :- non(X,A),nand(X,Y,B),xorn(A,B,C),non(C,Z).




