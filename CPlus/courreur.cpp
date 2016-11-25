#include<iostream>
#include"courreur.hpp"
#include<string>
int Courreur::NombreCoureurs = 0;	

Courreur::Courreur(string name){
	nom = name;
	NombreCoureurs++;
}

void Courreur::cours(){
	for(int i =0; i<KM;i++){
		cout<<this->dossard<<" "<<this->nom<<endl;
	}	
}

