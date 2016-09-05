#ifndef __SMP__
#define __SMP__

int verifyFreeMan(int n, Man ** man){

	for(int i = 0; i < n; i++){
		cout << i << endl;
		cout << man[i]->getCurrentCouple() << endl;
		if(man[i]->getCurrentCouple() == '\0') return i;
	}

	return -1;
}


void SMP(int n){

	Man * man[n];
	Women * women[n];

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

	
	
	
	int i, a;
	
	while(a = verifyFreeMan(n,man) != -1){
	//for(int z = 0; z < 1; z++){
		a = verifyFreeMan(n,man);

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
			int j;
			for(j = 0; j < n; j++){
				if(women[i]->getPreference(j) == man[a]->getName()) break;
			}
			if(women[i]->changeCouple(j)){
				for(int q = 0; i < n; q++){
					
				}
				women[i]->setCurrentCouple(man[a]->getName(),j);
				man[a]->setCurrentCouple(women[i]->getName());
			}
			else man[a]->addPosition();
		}		
	}

	for(int i = 0; i < n; i++){
		man[i]->printInformation();
	}
	for(int i = 0; i < n; i++){
		women[i]->printInformation();
	}
}

#endif