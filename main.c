#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "voiture.h"
#include "cirquit.h"




int main(int argc, char**argv){
    int nbreVoiture = 20;
    int nbreTours = 60; //nombre tours
    int longueurTours = 5; //en Km
    int uneHeure = 600 ; //définir combient vaut 1h



    int numero[20] = {44, 77, 16, 5, 33, 23, 55, 4, 3, 31, 10, 26, 11, 18, 7, 99, 20, 8, 63, 6};
    maVoiture voitureEnMem[20]; //tableau de voitures
    for(int i = 0; i < 20 ; i++){ //Attribuer les id au tableau de voiture
        voitureEnMem[i].numero = numero[i];
    }
    //fichier = fopen("Essai.txt", "w+");

    //sleep(1);
    lancerCourse(nbreVoiture,900.0,voitureEnMem);
    creationFichier(nbreVoiture,voitureEnMem,1);
    sleep(1);



/* ==========================================

Vendredi matin séance d'essais libre d'1h30 (P1)

=============================================*/


    lancerCourse(nbreVoiture,900.0,voitureEnMem);
    creationFichier(nbreVoiture,voitureEnMem,2);
    sleep(1);
    triDuTableau(voitureEnMem, 20);




/* ==========================================

Vendredi après midi séance d'essais libre d'1h30 (P2)

=============================================*/
    lancerCourse(nbreVoiture,900.0,voitureEnMem);
    creationFichier(nbreVoiture,voitureEnMem,3);
    sleep(1);



/* ==========================================

Samedi matin séance d'essais libre d'1h (P3)

=============================================*/
    lancerCourse(nbreVoiture,600.0,voitureEnMem);
    creationFichier(nbreVoiture,voitureEnMem,4);
    sleep(1);



/* ==========================================

Samedi après midi séance de qualification en 3 parties

Q1, durée 18 minutes => élimine 5 voitures
Q2, durée 15 minutes => élimine 5 voitures
Q3, durée 12 minutes => classe les 10 dernières voitures

=============================================*/


    lancerCourse(nbreVoiture,180.0,voitureEnMem);  //18 minutes
    creationFichier(nbreVoiture,voitureEnMem,5);
    sleep(1);

    //triDuTableau(voitureEnMem, 20);
    nbreVoiture -= 5;
    lancerCourse(nbreVoiture,150.0,voitureEnMem);  //15 minutes
    creationFichier(nbreVoiture,voitureEnMem,5);
    sleep(1);

    //triDuTableau(voitureEnMem, 15);
    nbreVoiture -= 5;
    lancerCourse(nbreVoiture,120.0,voitureEnMem);  //12 minutes
    creationFichier(nbreVoiture,voitureEnMem,5);
    sleep(1);
/* ==========================================

Dimanche après midi la course en elle même

=============================================*/
    triDuTableau(voitureEnMem, 10);
    for(int k = 0 ; k < 20 ; k++){
        printf("%f \n", voitureEnMem[k].meilleurTemps);
        }

    triDuTableau(voitureEnMem, 10);
    lancerCourse(nbreVoiture,longueurTours*nbreTours,voitureEnMem);  //nbre de tours * nbre mayen du tour en minute
    creationFichier(nbreVoiture,voitureEnMem,6);
    sleep(1);


    return 0;




}



