#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "voiture.h"
#include "cirquit.h"




int main(int argc, char**argv){
    //FILE* fichier = NULL;
    int numero[20] = {44, 77, 16, 5, 33, 23, 55, 4, 3, 31, 10, 26, 11, 18, 7, 99, 20, 8, 63, 6};
    maVoiture voitureEnMem[20]; //tableau de voitures
    for(int i = 0; i < 20 ; i++){ //Attribuer les id au tableau de voiture
        voitureEnMem[i].numero = numero[i];
    }
    //fichier = fopen("Essai.txt", "w+");

    //sleep(1);
    lancerCourse(20,1000.0,voitureEnMem);
    creationFichier(20,voitureEnMem);
    sleep(1);
    //fprintf(fichier,"|%.3f\t",voitureEnMem[0]);
    /*
    for(int i = 0 ; i <20 ; i++){
        printf(" la voiture %d a un meilleur temps de %f\n",numero[i],voitureEnMem[i].meilleurTemps);
        usleep(50000);
    }
    */

    sleep(1);




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



    return 0;




}



