using namespace std;
#include <iostream>
#include <string.h>


class Courreur{
	public:
		
		void cours();

		Courreur(char*);		
		Courreur& operator=(const Courreur &c){
			cout<<"TEST6"<<endl;
			nom = new char();			
			strcpy(nom,c.getName());
			cout<<"TEST7"<<endl;
			dossard = c.getDossard();
		}
		inline char* getName()const{return this->nom;};
		inline void setName(char* name){this->nom = name;};
		inline int getDossard()const{return this->dossard;};
		inline void setDossard(int dos){this->dossard = dos;};
		static int getNB(){return NombreCoureurs;};
	private:
		const static int KM = 20;
		static int NombreCoureurs;
		int dossard;
		char* nom;
};
