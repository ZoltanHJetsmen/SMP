#ifndef __MAN__
#define __MAN__

class Man{
	
private:
	char name;
	char * preference;
	char * inversePreference;
	char currentCouple;
	int position;

public:
	Man(char name){

		this->name = name;
		this->currentCouple = '\0';
		this->position = 0;
	}

	char getName(){return this->name;}
	
	void setPreference(int n, char * preference){
	
		preference = new char[n];
		for(int i = 0; i < n; i++) this->preference[i] = preference[i];
	}

	char * getPreference(){return preference;}
	
	char getPreference(int n){return preference[n];}

	char getNextPreference(){return preference[position];}
	
	void setCurrentCouple(char women){currentCouple = women;}

	char getCurrentCouple(){return currentCouple;}

	void addPosition(){this->position++;}
};

#endif
