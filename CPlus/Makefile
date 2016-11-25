ski: ski.o commissaire.o
	g++ ski.o -o ski

ski.o: ski.cpp
	g++ -c ski.cpp -o ski.o

courreur.o: courreur.cpp
	g++ -c courreur.cpp -o courreur.o
commissaire.o: commissaire.cpp courreur.o temps.o
	g++ -c commissaire.cpp -o commissaire.o
temps.o: temps.cpp
	g++ -c temps.cpp -o temps.o

clean:
	rm -rf *.o
