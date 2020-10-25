#include <stdio.h>
#include <stdlib.h>
#include <time.h>


void printRandoms()
{
    srand(time(NULL));
    double random_value;
    for (int i=0;i<20;i++){
        random_value = (double)rand()/RAND_MAX*(20.00)+25.00;//float in range -1 to 1
        printf ( "%f\n", random_value);

    }

    return 0;
}

int main()
{

    printRandoms();
    rouler();

    return 0;
}

void rouler(){
    double start = time().Now();
    printf("%d","lol);
    double t = time.Now()
    double elapsed = t.Sub(start);

}


struct maVoiture{
    int numero ; //numéro de la voiture
    double S1 ;
    double S2 ;  // temps des secteurs
    double S3 ;
    double tempsTotal; //temps total
    bool stand ; // si présent au stand
    bool out ; // true = abandon

};

