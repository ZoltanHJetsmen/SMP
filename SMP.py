class Person:
    
    def __init__(self,name,preference,n):
        #'__' representam uma variavel privada
        self.name = name
        self.currentCouple = '0'
        self.position = 0
        self.preference = preference
        self.invertePreference = list()
        for i in range (0,n):
            self.invertePreference.append(0)
        preference = preference.upper()
        for i in range(0,n):
            self.invertePreference[ord(preference[i])-65] = i
    def addPosition(self):
        self.position += 1

#Read the text file "SMP.txt"
def input():
    file = open('SMP.txt','r')

    n = file.read(1) 
    file.read(2)
    n = int(n)

    mansName = file.readline()
    mansName = mansName.replace(' ','')
    womensName = file.readline()
    womensName = womensName.replace(' ','')

    mans = list()
    womens = list()

    for i in range(2*n):
        preference = file.readline()
        preference = preference.replace(' ','')
        if(i < n):
            man = Person(mansName[i],preference,n)
            mans.append(man)
        else:
            women = Person(womensName[i-n],preference,n)
            womens.append(women)

    file.close()

    return mans,womens,n

def verifyFreeMan(mans):
    for man in mans:
        if man.currentCouple == '0':
            return man

    return -1

def SMP():
    returns = input()

    man = verifyFreeMan(returns[0])
    while man != -1:
        nextWomen = man.preference[man.position]
        for women in returns[1]:
            if nextWomen == women.name:
                break
        if(women.currentCouple == '0'):
            women.currentCouple = man
            man.currentCouple = women
        else:
            if women.invertePreference[ord(man.name)-97] < women.invertePreference[ord(women.currentCouple.name)-97]: 
               women.currentCouple.currentCouple = '0'
               women.currentCouple = man
               man.currentCouple = women
            else:
                man.addPosition()
        man = verifyFreeMan(returns[0])

    for man in returns[0]:
        print("Name: ",man.name," Couple: ",man.currentCouple.name)
    
SMP()