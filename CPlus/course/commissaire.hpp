#include<iostream>
#include"temps.hpp"
#include"courreur.hpp"

class Commissaire{
	public:
		Commissaire();
		void start();
		void inscrire(char*);
		//void arret();
		//bool fini(int);		
	private:
		const static int MAXJOUEURS = 100;
		Temps _temp[];
		int classement[MAXJOUEURS];
		Courreur _ilsCourent[];
};
