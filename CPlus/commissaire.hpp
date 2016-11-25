#include<iostream>
#include"temps.hpp"
#include"courreur.hpp"
#include<string>

class Commissaire{
	public:
		void start();
		void inscrire(string);
		//void arret();
		//bool fini(int);		
	private:
		const static int MAXJOUEURS = 100;
		Temps _temp[MAXJOUEURS];
		int classement[MAXJOUEURS];
		Courreur ilsCourent[MAXJOUEURS];
};
