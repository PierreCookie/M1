#include<iostream>
#include"commissaire.hpp"
#include<typeinfo>
Commissaire::Commissaire(){
	_ilsCourent[MAXJOUEURS];
}

void Commissaire::start(){
	for(int i = 0 ;i<Courreur::getNB();i++){
		_ilsCourent[i].cours();
	}
}
void Commissaire::inscrire(char* nom){
	cout<<"Test 1 : "<<endl;
	int i = Courreur::getNB();	
	Courreur cour(nom);
	cout<<"TEST 4 "<<endl;
	_ilsCourent[i] = cour;
	cout<<"Ajout de "<<nom<<" dans la liste des courreur"<<endl;
}

