#include<iostream>
#include"commissaire.hpp"


void Commissaire::start(){
	for(int i = 0 ;i<Courreur::getNB();i++){
		ilsCourent[i].cours();
	}
}
void Commissaire::inscrire(string nom){
	ilsCourent[Courreur::getNB()] = Courreur(nom);
}

