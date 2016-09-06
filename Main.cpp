//Progam which resolve the Stable Marrige Problem with the algorith Gale-Shapley

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
