#include<iostream>
#include"courreur.hpp"
int Courreur::NombreCoureurs = 0;	

Courreur::Courreur(char* name){

	nom = name;
	dossard=NombreCoureurs;
	NombreCoureurs++;
	cout<<"TEST3 "<<endl;
}

void Courreur::cours(){
	for(int i =0; i<KM;i++){
		cout<<this->dossard<<" "<<this->nom<<endl;
	}	
}

