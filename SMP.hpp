#ifndef __SMP__
#define __SMP__

int verifyFreeMan(int n, Man ** man){

	for(int i = 0; i < n; i++){
		if(man[i]->getCurrentCouple() == '\0') return i;
	}

	return -1;
}

void input(Man ** man, Women ** women, int n){

	for(int i = 0; i < n; i++){
		char name;
		cin >> name;
		man[i] = new Man(name);
	}
	for(int i = 0; i < n; i++){
		char name;
		cin >> name;
		women[i] = new Women(name);
	}

	for(int i = 0; i < 2*n; i++){
		char * preference = new char[n];
		for(int j = 0; j < n; j++){
			cin >> preference[j];
		}
		if(i < n) man[i]->setPreference(n,preference);
		else women[i-n]->setPreference(n,preference);
		delete preference;
	}
}

void SMP(int n){

	Man * man[n];
	Women * women[n];

	input(man,women,n);
	
	int i, a;
	
	while((a = verifyFreeMan(n,man)) != -1){
		
		char nextWomen = man[a]->getNextPreference();
		
		for(i = 0; women[i]->getName() != nextWomen; i++);

		if(women[i]->getCurrentCouple() == '\0'){
			int j;
			for(j = 0; j < n; j++){
				if(women[i]->getPreference(j) == man[a]->getName()) break;
			}
			women[i]->setCurrentCouple(man[a]->getName(),j);
			man[a]->setCurrentCouple(women[i]->getName());
		
		}
		else{
			int j, q;
			for(j = 0; j < n; j++){
				if(women[i]->getPreference(j) == man[a]->getName()) break;
			}
			if(women[i]->changeCouple(j)){
				char oldCouple = women[i]->getCurrentCouple();
				for(q = 0; man[q]->getName() != oldCouple; q++);
				man[q]->setCurrentCouple('\0');
				man[q]->addPosition();
				women[i]->setCurrentCouple(man[a]->getName(),j);
				man[a]->setCurrentCouple(women[i]->getName());
			}
			else man[a]->addPosition();
		}

		for(int i = 0; i < n; i++){
			man[i]->printInformation(n);
		}
		for(int i = 0; i < n; i++){
			women[i]->printInformation(n);
		}		
	}

	for(int i = 0; i < n; i++) delete man[i]; delete women[i];
}

#endif