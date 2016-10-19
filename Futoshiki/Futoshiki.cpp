/*
 Futoshiki.c - Futoshiki
 Autores:   Zoltán Hirata Jetsmen     Nº USP: 9293272
            Víctor de Aquino Amorim    Nº USP: 9277642
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <list>
#include <iostream>
#include <unordered_map>
using namespace std;

#define BACKTRACKING 0
#define FORWARDCHECKING 1
#define MVR 2

typedef struct table{
    int num; // Número da célula
    int qtdRestrictionsMin; // Quantidade de restrições existentes em que (i, j) < (i', j') 
    int qtdRestrictionsMax; // Quantidade de restrições existentes em que (i, j) > (i', j') 
    int min[4][2]; // Coordenadas em que (i, j) < (i', j')
    int max[4][2]; // Coordenadas em que (i, j) > (i', j')
    int definitive; // Define se o número é definitivo ou não
    list<int> * values; // Lista de possíveis valores para uma determinada coordenada
    list<int> * removedValues; // Guarda os valores removidos
    unordered_multimap<int,int> * coordinates; // Guarda as coordenadas dos valores removidos 
}TABLE;

typedef struct info{
    int dimension; // Dimensão do tabuleiro
    int completed; // Indica se o tabuleiro está completo
    TABLE ** table; // Tabuleiro
}INFO; 

/*
 Função que realiza a coleta da entrada do tabuleiro e de suas restrições
	- Retorno: INFO * info: struct que contém as informações do tabuleiro
*/
INFO * input(){
    
    int i, j;
    
    // Alocação do ponteiro para a struct
    INFO * info = (INFO*) malloc(sizeof(INFO));

    // Dimesão do tabuleiro
    scanf("%d",&info->dimension);

    // Alocação do tabuleiro
    info->table = (TABLE**) malloc(sizeof(TABLE)*info->dimension);
    for(i = 0; i < info->dimension; i++){
        info->table[i] = (TABLE*) malloc(sizeof(TABLE)*info->dimension);
    }    

    // Coleta da quantidade de restrições
    int restrictions;
    scanf("%d",&restrictions);

    // Preenchimento do tabuleiro inicial e alocação dos argumentos da struct
    for(i = 0; i < info->dimension; i++){
        for(j = 0; j < info->dimension; j++){
            scanf("%d",&info->table[i][j].num);
            if(info->table[i][j].num == 0){ // Célula nao definida
                info->table[i][j].definitive = 0;
                info->table[i][j].values = new list<int>; 
                info->table[i][j].coordinates = new unordered_multimap<int,int>;
                for(int n = 0; n < info->dimension; n++) info->table[i][j].values->push_back(n+1);
            }
            else { // Célula definida
                info->table[i][j].definitive = 1; 
                info->table[i][j].values = new list<int>;
                info->table[i][j].coordinates = new unordered_multimap<int,int>;
            }
            info->table[i][j].qtdRestrictionsMin = 0;
            info->table[i][j].qtdRestrictionsMax = 0;
        }
    }

    // Retirada dos valores válidos nas células que possuem células na mesma linha ou coluna com valor já definido
    for(i = 0; i < info->dimension; i++){
        for(j = 0; j < info->dimension; j++){
            if(info->table[i][j].definitive == 1){
                for(int n = 0; n < info->dimension; n++){
                    if(n != i) info->table[n][j].values->remove(info->table[i][j].num);
                    if(n != j) info->table[i][n].values->remove(info->table[i][j].num);
                }
            }
        }
    }

    // Coleta das restrições 
    for(i = 0; i < restrictions; i++){
        int x1, y1, x2, y2;
        // A célula em (x1, y1) tem valor menor que a célula em (x2, y2)
        scanf("%d",&x1); scanf("%d",&y1);
        scanf("%d",&x2); scanf("%d",&y2);
        info->table[x1-1][y1-1].min[info->table[x1-1][y1-1].qtdRestrictionsMin][0] = x2-1;
        info->table[x1-1][y1-1].min[info->table[x1-1][y1-1].qtdRestrictionsMin][1] = y2-1;
        info->table[x2-1][y2-1].max[info->table[x2-1][y2-1].qtdRestrictionsMax][0] = x1-1;
        info->table[x2-1][y2-1].max[info->table[x2-1][y2-1].qtdRestrictionsMax][1] = y1-1;
        // Guarda-se a quantidade de restrição de cada tipo para cada célula
        info->table[x1-1][y1-1].qtdRestrictionsMin++;
        info->table[x2-1][y2-1].qtdRestrictionsMax++;

        if(info->table[x1-1][y1-1].definitive == 1){
            for(int value = 1; value < info->dimension+1; value++){    
                if(value <= info->table[x1-1][y1-1].num){ 
                    info->table[x2-1][y2-1].values->remove(value);
                }
            }
        }
        if(info->table[x2-1][y2-1].definitive == 1){
            for(int value = 1; value < info->dimension+1; value++){    
                if(value >= info->table[x2-1][y2-1].num){ 
                    info->table[x1-1][y1-1].values->remove(value);
                }
            }
        }
    }

    // Criação da lista de valores removidos
    for(i = 0; i < info->dimension; i++){
        for(j = 0; j < info->dimension; j++){
            info->table[i][j].removedValues = new list<int>[info->table[i][j].qtdRestrictionsMin + info->table[i][j].qtdRestrictionsMax];
        }
    }

    return info;
}

/*
 Função para impressão na tela do tabuleiro
    - Parametro: INFO *info: struct que contém o tabuleiro
*/
void printInput(INFO * info){

    int i, j;

    for(i = 0; i < info->dimension; i++){
        for(j = 0; j < info->dimension; j++){
            printf("%d ",info->table[i][j].num);
        }
        printf("\n");
    }
}

/*
 Função que realiza a verificação para que o valor inserido na célula seja válido
    - Parametros:   INFO *info: struct que contém as informações do tabuleiro
                    i: coordenada da linha da célula analisada
                    j: coordenada da coluna da célula analisada
    - Retorno:  flag (0 - válido / 1 - inválido)
*/
int verification(INFO * info, int i, int j){

    int n, flag = 0;

    for (n = 0; n < info->dimension; n++){
        if(n != i){
            if(info->table[n][j].num == info->table[i][j].num) flag = 1;
        }
        if(n != j){
            if(info->table[i][n].num == info->table[i][j].num) flag = 1;
        }
    }

    // Confere se a restrição de ser menor não está sendo estabelecida
    for(n = 0; n < info->table[i][j].qtdRestrictionsMin; n++){
        if(info->table[info->table[i][j].min[n][0]][info->table[i][j].min[n][1]].num != 0){
            if(info->table[i][j].num > info->table[info->table[i][j].min[n][0]][info->table[i][j].min[n][1]].num){
                flag = 1;
            }
        }
    }
    // Confere se a restrição de ser maior não está sendo estabelicida
    for(n = 0; n < info->table[i][j].qtdRestrictionsMax; n++){
        if(info->table[i][j].num < info->table[info->table[i][j].max[n][0]][info->table[i][j].max[n][1]].num){
            flag = 1;
        }
    }

    return flag;
}

/*
 Função que realiza a verificação para que o valor inserido na célula seja válido
 - Retorno: condição de sucesso da verificação (0 - sucesso / 1 - falha)
*/
int FCVerification(INFO * info, int i, int j){

    if(j == 0){ // Verifica-se a linha acima 
        i--;
        j = info->dimension-1;
    } else{ // Verifica-se a coluna anterior
        j--;
    }

    int size, num = info->table[i][j].num;
    for(int n = 0; n < info->dimension; n++){
        if(n != i){ // Trata-se as linhas acima e abaixo
            size = info->table[n][j].values->size();
            info->table[n][j].values->remove(info->table[i][j].num);
            if(size > info->table[n][j].values->size()){ // Caso o valor tenha sido removido
                info->table[i][j].coordinates->insert(pair<int,int>(n,j)); // Guarda-se as cordenadas da célula que o valor foi removido
                if(info->table[n][j].values->empty() && info->table[i][n].num == 0) return 1;
            } 
        }
        if(n != j){ // Trata-se as colunas a direita e a esquerda
            size = info->table[i][n].values->size();
            info->table[i][n].values->remove(info->table[i][j].num);
            if(size > info->table[i][n].values->size()){ // Caso o valor tenha sido removido
                info->table[i][j].coordinates->insert(pair<int,int>(i,n)); // Guarda-se as cordenadas da célula que o valor foi removido
                if(info->table[i][n].values->empty() && info->table[i][n].num == 0) return 1;
            }
        }
    }
    
    // Tratamento das restrições mínimas
    for(int n = 0; n < info->table[i][j].qtdRestrictionsMin; n++){
        for(int value = 1; value < info->dimension+1; value++){    
            if(value <= info->table[i][j].num){
                int size = info->table[info->table[i][j].min[n][0]][info->table[i][j].min[n][1]].values->size(); 
                info->table[info->table[i][j].min[n][0]][info->table[i][j].min[n][1]].values->remove(value);
                if(size > info->table[info->table[i][j].min[n][0]][info->table[i][j].min[n][1]].values->size()){ // Caso o valor tenha sido removido
                     info->table[i][j].removedValues[n].push_back(value);
                     if(info->table[info->table[i][j].min[n][0]][info->table[i][j].min[n][1]].values->empty() && info->table[info->table[i][j].min[n][0]][info->table[i][j].min[n][1]].num == 0) return 1;
                }
            }
        }
    }
   
    // Tratamento das restrições máximas
    for(int n = 0; n < info->table[i][j].qtdRestrictionsMax; n++){
        for(int value = 1; value < info->dimension+1; value++){
            if(value >= info->table[i][j].num){
                int size = info->table[info->table[i][j].max[n][0]][info->table[i][j].max[n][1]].values->size();
                info->table[info->table[i][j].max[n][0]][info->table[i][j].max[n][1]].values->remove(value);
                if(size > info->table[info->table[i][j].max[n][0]][info->table[i][j].max[n][1]].values->size()){ // Caso o valor tenha sido removido
                    info->table[i][j].removedValues[n+info->table[i][j].qtdRestrictionsMin].push_back(value);
                    if(info->table[info->table[i][j].max[n][0]][info->table[i][j].max[n][1]].values->empty() && info->table[info->table[i][j].max[n][0]][info->table[i][j].max[n][1]].num == 0) return 1;
                }
            }
        }
    }

    return 0;
}

/*
 Função para tratamento dos valores que foram removidos em cada célula
 - Parametros:   INFO *info: struct que contém as informações do tabuleiro
                    i: coordenada da linha da célula analisada
                    j: coordenada da coluna da célula analisada
*/
void insertRemoved(INFO * info, int i, int j){

    for(unordered_multimap<int,int>::iterator it = info->table[i][j].coordinates->begin(); it != info->table[i][j].coordinates->end(); it++){
        info->table[it->first][it->second].values->push_back(info->table[i][j].num);     
    }

    info->table[i][j].coordinates->clear();

    // Tratamento das restrições mínimas
    for(int n = 0; n < info->table[i][j].qtdRestrictionsMin; n++){
        if(info->table[i][j].removedValues[n].size() != 0){
            for(list<int>::iterator it = info->table[i][j].removedValues[n].begin(); it != info->table[i][j].removedValues[n].end(); it++){
                info->table[info->table[i][j].min[n][0]][info->table[i][j].min[n][1]].values->push_back(*it);
            }
            info->table[i][j].removedValues[n].clear();
        }
    }

    // Tratamento das restrições máximas
    for(int n = 0; n < info->table[i][j].qtdRestrictionsMax; n++){
        if(info->table[i][j].removedValues[n+info->table[i][j].qtdRestrictionsMin].size() != 0){
            for(list<int>::iterator it = info->table[i][j].removedValues[n+info->table[i][j].qtdRestrictionsMin].begin(); it != info->table[i][j].removedValues[n+info->table[i][j].qtdRestrictionsMin].end(); it++){
                info->table[info->table[i][j].max[n][0]][info->table[i][j].max[n][1]].values->push_back(*it);
            }
            info->table[i][j].removedValues[n+info->table[i][j].qtdRestrictionsMin].clear();
        }
    }
}

/*
 Função que realiza a verificação para que o valor inserido na célula seja válido
 - Retorno: condição de sucesso da verificação (0 - sucesso / 1 - falha)
*/
int MVRVerification(INFO * info, int i, int j){

    int size, num = info->table[i][j].num;
    for(int n = 0; n < info->dimension; n++){
        if(n != i){ // Trata-se as linhas acima e abaixo
            size = info->table[n][j].values->size();
            info->table[n][j].values->remove(info->table[i][j].num);
            if(size > info->table[n][j].values->size()){ // Caso o valor tenha sido removido
                info->table[i][j].coordinates->insert(pair<int,int>(n,j)); // Guarda-se as cordenadas da célula que o valor foi removido
                if(info->table[n][j].values->empty() && info->table[n][j].num == 0) return 1;
            } 
        }
        if(n != j){ // Trata-se as colunas a direita e a esquerda
            size = info->table[i][n].values->size();
            info->table[i][n].values->remove(info->table[i][j].num);
            if(size > info->table[i][n].values->size()){ // Caso o valor tenha sido removido
                info->table[i][j].coordinates->insert(pair<int,int>(i,n)); // Guarda-se as cordenadas da célula que o valor foi removido
                if(info->table[i][n].values->empty() && info->table[i][n].num == 0) return 1;
            }
        }
    }
    
    // Tratamento das restrições mínimas
    for(int n = 0; n < info->table[i][j].qtdRestrictionsMin; n++){
        for(int value = 1; value < info->dimension+1; value++){    
            if(value <= info->table[i][j].num){
                int size = info->table[info->table[i][j].min[n][0]][info->table[i][j].min[n][1]].values->size(); 
                info->table[info->table[i][j].min[n][0]][info->table[i][j].min[n][1]].values->remove(value);
                if(size > info->table[info->table[i][j].min[n][0]][info->table[i][j].min[n][1]].values->size()){ // Caso o valor tenha sido removido
                     info->table[i][j].removedValues[n].push_back(value);
                     if(info->table[info->table[i][j].min[n][0]][info->table[i][j].min[n][1]].values->empty() && info->table[info->table[i][j].min[n][0]][info->table[i][j].min[n][1]].num == 0) return 1;
                }
            }
        }
    }
   
    // Tratamento das restrições máximas
    for(int n = 0; n < info->table[i][j].qtdRestrictionsMax; n++){
        for(int value = 1; value < info->dimension+1; value++){
            if(value >= info->table[i][j].num){
                int size = info->table[info->table[i][j].max[n][0]][info->table[i][j].max[n][1]].values->size();
                info->table[info->table[i][j].max[n][0]][info->table[i][j].max[n][1]].values->remove(value);
                if(size > info->table[info->table[i][j].max[n][0]][info->table[i][j].max[n][1]].values->size()){ // Caso o valor tenha sido removido
                    info->table[i][j].removedValues[n+info->table[i][j].qtdRestrictionsMin].push_back(value);
                    if(info->table[info->table[i][j].max[n][0]][info->table[i][j].max[n][1]].values->empty() && info->table[info->table[i][j].max[n][0]][info->table[i][j].max[n][1]].num == 0) return 1;
                }
            }
        }
    }

    return 0;
}

/*
 Função que realiza o backtracking simples para o preenchimento do tabuleiro
    - Parametros:   INFO *info: struct que contém as informações do tabuleiro
                    i: coordenada da linha da célula analisada
                    j: coordenada da coluna da célula analisada
    - Retorno: condição de sucesso do backtracking
*/
int backtraking(INFO * info, int i, int j, long int * counter){

    if(*counter > 1000000){
        printf("Number of operations overshot.\n");
        info->completed = 1;
        return 0;
    }

    int flag = -1;

    if(i == info->dimension){
        info->completed = 0;
        return 0;
    }

    if(info->table[i][j].definitive == 0){ // Caso a célula não tenha sido definida
        while(info->table[i][j].num < info->dimension){
            (*counter)++;
            info->table[i][j].num++; // Tenta-se os valores em ordem crescente
            if(verification(info,i,j) == 0){ // Tentativa válida
                if((j + 1) < info->dimension) flag = backtraking(info,i,j+1,counter); // Caso a próxima coluna ainda esteja nos limites
                else flag = backtraking(info,i+1,0,counter); // Pula para a próxima linha
            } 
            if(flag == 0) return 0;
        }
        info->table[i][j].num = 0;
        return 1;
    }
    else{ // Caso o valor já tenha sido preenchido
        if((j + 1) < info->dimension) flag = backtraking(info,i,j+1,counter); // Caso a próxima coluna ainda esteja nos limites
        else flag = backtraking(info,i+1,0,counter); // Pula para a próxima linha
        return flag;
    }
}

/*
 Função que realiza da verificação adiante para preenchimento do tabuleiro
  - Parametros:   INFO *info: struct que contém as informações do tabuleiro
                    i: coordenada da linha da célula analisada
                    j: coordenada da coluna da célula analisada
    - Retorno: condição de sucesso
*/
int forwardChecking(INFO * info, int i, int j, long int * counter){

    if(*counter > 1000000){
        printf("Number of operations overshot.\n");
        info->completed = 1;
        return 0;
    }

    int flag = -1;

    if(i == info->dimension){
        info->completed = 0;
        return 0;
    }
    if(i != 0 || j != 0){
        if(FCVerification(info,i,j) == 1) return 1;
    }

    if(info->table[i][j].definitive == 0){ // Valor não definido ainda
        list<int> * auxiliar = new list<int>;
        // Percorre a lista de valores possíveis utilizando o iterator e atribuindo a lista auxilar
        for(list<int>::iterator it = info->table[i][j].values->begin(); it != info->table[i][j].values->end(); it++){
            auxiliar->push_back(*it);
        }
        while(!(info->table[i][j].values->empty())){
            (*counter)++;
            info->table[i][j].num = info->table[i][j].values->front(); // É atribuido o primeiro valor da lista à célula
            info->table[i][j].values->pop_front(); // É retirado o primeiro valor da lista
            if((j + 1) < info->dimension){ // Dentro dos limites do tabuleiro
                flag = forwardChecking(info,i,j+1,counter); // Chamada para a próxima célula da linha
            } else{ // Dentro dos limites do tabuleiro
                flag = forwardChecking(info,i+1,0,counter); // Chamada para a próxima célula da coluna
            } 
            if(flag == 0){
                delete auxiliar;
                return 0;
            } 
            insertRemoved(info,i,j);
        }
        delete info->table[i][j].values;
        info->table[i][j].values = auxiliar;
        info->table[i][j].num = 0;
        return 1;
    }
    else{ // Valor definido, passa-se para a a próxima célula
        if((j + 1) < info->dimension){ // Dentro dos limites do tabuleiro
            flag = forwardChecking(info,i,j+1,counter); // Chamada para a próxima célula da linha
        } else{ // Dentro dos limites do tabuleiro
            flag = forwardChecking(info,i+1,0,counter); // Chamada para a próxima célula da coluna
        }
        return flag;
    }
}

/*
 Função que realiza MRV para preenchimento do tabuleiro
  - Parametros:   INFO *info: struct que contém as informações do tabuleiro
                    i: coordenada da linha da célula analisada
                    j: coordenada da coluna da célula analisada
    - Retorno: condição de sucesso
*/
int f_MVR(INFO * info, int a, int b, long int * counter){
    
    if(*counter > 1000000){
        printf("Number of operations overshot.\n");
        info->completed = 1;
        return 0;
    }

    int min = info->dimension + 1;
    int i = -1, j = -1, flag = -1;

    if(a != -1 && b != -1){ // Na primeira chamada da função não verifica-se os valores já retirados
        if(MVRVerification(info,a,b) == 1) return 1;
    }

    // Busca pela célula MVR
    for(int x = 0; x < info->dimension; x++){
        for(int y = 0; y < info->dimension; y++){
            if(info->table[x][y].num == 0){
                int size = info->table[x][y].values->size();
                if(size < min){
                    min = size;
                    i = x;
                    j = y;
                }
            }
        }
    }

    if(i == -1 && j == -1){ // O tabuleiro já foi preenchido por completo
        info->completed = 0;
        return 0; 
    }

    list<int> * auxiliar = new list<int>;
    // Percorre a lista de valores possíveis utilizando o iterator e atribuindo a lista auxilar
    for(list<int>::iterator it = info->table[i][j].values->begin(); it != info->table[i][j].values->end(); it++){
        auxiliar->push_back(*it);
    }
    while(!(info->table[i][j].values->empty())){
        (*counter)++;
        info->table[i][j].num = info->table[i][j].values->front(); // É atribuido o primeiro valor da lista à célula
        info->table[i][j].values->pop_front(); // É retirado o primeiro valor da lista
        flag = f_MVR(info,i,j,counter); // Chamada para a próxima célula
        if(flag == 0){
            delete auxiliar;
            return 0;
        } 
        insertRemoved(info,i,j);
    }
    delete info->table[i][j].values;
    info->table[i][j].values = auxiliar;
    info->table[i][j].num = 0;
    return 1;
}

/*
 Função que realiza a chamada para o preenchimento do tabuleiro
    - Parametros:   INFO *info: struct que contém as informações do tabuleiro
*/
void futoshiki(INFO * info, int opt){

    clock_t t = clock();
    long int counter = 0;

    switch(opt){
        case BACKTRACKING: backtraking(info,0,0,&counter);
            break;
        case FORWARDCHECKING: forwardChecking(info,0,0,&counter);
            break;
        case MVR:f_MVR(info,-1,-1,&counter);
            break;
    }

    t = clock() - t;
    if(info->completed == 0) printf("Time: %lf\nOperations: %ld\n",((double)t)/CLOCKS_PER_SEC,counter);
}

/*
 Função que realiza a liberação de memoria alocada
*/
void freeMemory(INFO * info){

    for(int i = 0; i < info->dimension; i++){
        for(int j = 0; j < info->dimension; j++){
            delete info->table[i][j].values;
            delete info->table[i][j].coordinates;
        }
    }

    for(int i = 0; i < info->dimension; i++) free(info->table[i]);
    free(info->table); free(info);
}

int main(int argc, char ** argv){

    int tests, aux; // Quantidade de tabuleiros
    scanf("%d",&tests);
    aux = tests;

    INFO ** info = (INFO**) malloc(sizeof(INFO*)*tests);

    while(tests != 0){
        info[aux-tests] = input();
        tests--;
    }
    for(int i = 0; i < aux; i++){
        
        // Chamada para Backtracking
        //printf("\nBacktracking\n");
        //futoshiki(info[i], BACKTRACKING);
        
        // Chamada para Forward Checking
        //printf("\nForward Checking\n");
        //futoshiki(info[i], FORWARDCHECKING);
        
        // Chamada para MVR
        printf("\nMVR\n");
        futoshiki(info[i], MVR);
        
        if(info[i]->completed == 0) printInput(info[i]);
        freeMemory(info[i]);
        
    }

    free(info);

    return 0;
}