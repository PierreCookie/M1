using namespace std;
#include <iostream>
#include <string>



class Courreur{
	public:
		
		void cours();

		Courreur(string);		

		inline string getName(){return nom;};
		inline void setName(string name){this->nom = name;};
		inline int getDossard(){return dossard;};
		inline void setDossard(int dos){this->dossard = dos;};
		static int getNB(){return NombreCoureurs;};
	private:
		const static int KM = 20;
		static int NombreCoureurs;
		int dossard;
		string nom;
};
