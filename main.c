#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct maVoiture maVoiture;
struct maVoiture{
    int numero ; //numéro de la voiture
    double S1 ;
    double S2 ;  // temps des secteurs
    double S3 ;
    double tempsTotal; //temps total
    int stand ; // si présent au stand
    int out ; // 0 = abandon

};

double printRandoms()
{
    //srand(time(NULL));
    double random_value = 0;
    for (int i=0;i<20;i++){
        random_value = (double)rand()/RAND_MAX*(20.00)+25.00;//float in range 25 to 45
        printf ( "%f\n", random_value);

    }

    return random_value;
}

int main()
{

    printRandoms();
    rouler();

    return 0;
}

void rouler(){
    maVoiture test;
    /*double start = time().Now();
    printf("%d",lol);
    double t = time.Now()
    double elapsed = t.Sub(start);
*/
    test.S1 =45.00 ,test.S2 =45.00 , test.S3 = 45.00 ;

    //double temps1 = printRandoms();
    if (printRandoms() < test.S1){
        test.S1 = printRandoms() ;
    }
    test.S2 = printRandoms() ;
    test.S3 = printRandoms() ;


    printf ( "%f\n", test.S1);
    printf ( "%f\n", test.S2);
    printf ( "%f\n", test.S3);
}





