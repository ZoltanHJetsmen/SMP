/*
1 - Confirmando a traca de casais, fazer com que o homem velho fique sem esposa, setando barra zero em 'currentCouple' e aumentando em 
um na posição
2 - Fazer a impressão do resultado final
3 - Fazer a main correta(cirar man e women como vetores e etc)
*/

#include <iostream>
using namespace std;

#include "Man.hpp"
#include "Women.hpp"
#include "SMP.hpp"

int main(int argc, char ** argv){

	int n;
	cin >> n;

	SMP(n);

	return 0;

}
