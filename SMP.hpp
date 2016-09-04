#ifndef __SMP__
#define __SMP__

int verifyFreeMan(int n, Man * man){

	for(int i = 0; i < n; i++){
		if(man[i]->CurrentCouple() != '\0') return i;
	}

	return -1;


void SMP(){

	Man * man = new Man('A');
	Women * women = new Women('B');
	int i, a;
	
	while(a = verifyFreeMan() != -1){
		char nextWomen = man[i]->getNextPreference();
		
		for(i = 0; women[i]->getName() != nextWomen; i++);

		if(women[i]->getCurrentCouple() == '\0'){
			int j;
			for(j = 0; j < n; j++){
				if(women[i]->getPreference(j) == man[a].getName()) break;
			women[i]->setCurentCouple(man[a]->getName());
			man[a]->setCurentCouple(women[i]->getName(),j);
		
		}
		else if(){
			int j;
			for(j = 0; j < n; j++){
				if(women[i]->getPreference(j) == man[a].getName()) break;
			if(women[i]->changeCouple(j)){
				for(int q = 0; i < n; q++){
					
				}
				women[i]->setCurentCouple(man[a].getName(),j);
				man[a]->setCurentCouple(women[i]->getName());
			}
			else man[a]->addPosition();
		}		
	}

}

#endif