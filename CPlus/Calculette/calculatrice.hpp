#include<std>
#include <vector>
using namespace std;
#include <stack>
using namespace std;

template<typename T>

class Calculatrice<T>{
	public:
	
	T binaires(char*,T,T);
	T unaires(char*,T);
	T constante(char*);

	private:
	stack pile;

};