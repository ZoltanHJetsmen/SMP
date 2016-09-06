#ifndef __WOMEN__
#define __WOMEN__

class Women{

private:
    char name;
	char * preference;
	char * inversePreference;
	char currentCouple;
    int positionCurrentCouple;

public:
	Women(char name){

		this->name = name;
		this->currentCouple = '\0';
        this->positionCurrentCouple = -1;
    }

	char getName(){return this->name;}
	
	void setPreference(int n, char * preference){
	
		this->preference = new char[n];
		for(int i = 0; i < n; i++) this->preference[i] = preference[i];
	}

	char * getPreference(){return preference;}
	
	char getPreference(int n){return preference[n];}
	
	void setCurrentCouple(char man, int n){
        currentCouple = man;
        positionCurrentCouple = n;
    }

	char getCurrentCouple(){return currentCouple;}

    bool changeCouple(int n){
        if(n > positionCurrentCouple) return false;
        
        return true;
    }

	void printInformation(int n){

		cout << "Name: " << name << endl;
		cout << "Couple: " << currentCouple << endl;
		for(int i = 0; i < n; i++) cout << "Preference: " << preference[i] << endl;
	}
};

#endif