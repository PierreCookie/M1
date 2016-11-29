#include<iostream>
#include"calculatrice.hpp"
#include<math.h>
T Calculatrice<T>::binaires(char* op,T a,T b){
	switch(op){
		case '/':
			return (T)a/b;
		case '*':
			return (T)a*b;
		case '+':
			return (T)a+b;
		case '-':
			return (T)a-b;
		case '^':
			return (T)math.pow(a,b);
		default:
			printf("IMPOSSIBLE DE DETERMINER %s",op);
	}
	return 0;
}
T Calculatrice<T>::unaires(char* op,T a){
	switch(op){
		case("sin"):
			return (T)sin(a);
		case("cos"):
			return (T)cos(a);
		case("exp"):
			return (T)exp(a);
		case("ln"):
			return (T)log(a);
		default:
			printf("IMPOSSIBLE DE DETERMINER %s",op);
	}
	return 0;
}
T Calculatrice<T>::constante(char*)