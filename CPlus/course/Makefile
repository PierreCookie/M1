ski: ski.o commissaire.o courreur.o temps.o
	g++ ski.o commissaire.o courreur.o temps.o -o ski

ski.o: ski.cpp
	g++ -c ski.cpp -o ski.o

commissaire.o: commissaire.cpp
	g++ -c commissaire.cpp -o commissaire.o
courreur.o: courreur.cpp
	g++ -c courreur.cpp -o courreur.o
temps.o: temps.cpp
	g++ -c temps.cpp -o temps.o

clean:
	rm -rf *.o
