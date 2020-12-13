#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "voiture.h"
#include "rand.h"


double* rouler(){
    //int tid = pthread_self();
    srand(time(NULL)^ getpid()); // graine, elle doit être dans les fils afin de prendre leurs pid unique à chaques fois

    double temps[2]; // creation du tableau local
    maVoiture test;
    double S1,S2,S3;
    test.S1 = 45.00;
    test.S2 = 45.00;
    test.S3 = 45.00;
    int compteur = 0 ;
    double tourComplet = 45*3;
    double tempsAuStand ;

    for(int i=0; i<NBR_TOURS; i++){  //permet de lancer la fonction x fois



        if(stand(TAUX_DE_STAND)==1){
            tempsAuStand =tempsStand();
            //printf("temps au stand %f \n",tempsAuStand);
        }
        else if (out(CRASH)==1){

            test.tempsTotal = tourComplet;
            temps[0]=test.S1 ;
            temps[1]=test.S2 ;
            temps[2]=test.S3 ;
            printf("out \n");
            return temps ;
        }
        S1 = printRandoms() ;
       // printf("%f \n",S1);

        if (test.S1 > S1) {     //permet d'enregistrer le meilleur temps
            test.S1 = S1;


        }

        S2 = printRandoms() ;
        if (test.S2 > S2) {
            test.S2 = S2;
        }

        S3 = printRandoms() ;
        if (test.S3 > S3) {
            test.S3 = S3;
        }

        if((S1+S2+S3) < tourComplet){
            tourComplet = S1+S2+S3 +tempsAuStand;
        }

    }



    test.tempsTotal = tourComplet;
    temps[0]=test.S1 ;
    temps[1]=test.S2 ;
    temps[2]=test.S3 ;
    printf ( "meilleur tour %f\n", test.tempsTotal);
    //sem_wait(&semaphore);
   // printf ( "secteur 1: %f\n", test.S1);
    /*sleep(1);
    sem_post(&semaphore);*/
    //printf ( "secteur 2: %f\n", test.S2);
    //printf ( "secteur 3: %f\n", test.S3);
    //printf ( "le temps total est de : %f secondes\n", test.tempsTotal);
    return temps;
}




/* ==========================================

Vendredi matin séance d'essais libre d'1h30 (P1)

=============================================*/





/* ==========================================

Vendredi après midi séance d'essais libre d'1h30 (P2)

=============================================*/




/* ==========================================

Samedi matin séance d'essais libre d'1h (P3)

=============================================*/




/* ==========================================

Samedi après midi séance de qualification en 3 parties

Q1, durée 18 minutes => élimine 5 voitures
Q1, durée 15 minutes => élimine 5 voitures
Q3, durée 12 minutes => classe les 10 dernières voitures

=============================================*/


/* ==========================================

Dimanche après midi la course en elle même

=============================================*/

