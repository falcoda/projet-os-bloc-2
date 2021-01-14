#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "voiture.h"
#include "circuit.h"




int main(int argc, char**argv){
    int nbreVoiture = 20;
    int nbreTours = 60; //nombre tours
    int longueurTours = 5; //en Km
    int uneHeure = 600 ; //définir combient vaut 1h
    int voitureQualif[20] ;
    int voitureQualifFinale[10] ;


    int numero[20] = {44, 77, 16, 5, 33, 23, 55, 4, 3, 31, 10, 26, 11, 18, 7, 99, 20, 8, 63, 6};
    maVoiture voitureEnMem[20]; //tableau de voitures
    maVoiture qualif1[15];
    maVoiture qualif2[10];

/* ==========================================

Vendredi matin séance d'essais libre d'1h30 (P1)

=============================================*/


    lancerCourse(nbreVoiture,900.0,voitureEnMem,numero);
    creationFichier(nbreVoiture,voitureEnMem,1);
    sleep(1);
    //triDuTableau(voitureEnMem, 20);




/* ==========================================

Vendredi après midi séance d'essais libre d'1h30 (P2)

=============================================*/
    lancerCourse(nbreVoiture,900.0,voitureEnMem,numero);
    creationFichier(nbreVoiture,voitureEnMem,2);
    sleep(1);



/* ==========================================

Samedi matin séance d'essais libre d'1h (P3)

=============================================*/
    lancerCourse(nbreVoiture,600.0,voitureEnMem,numero);
    creationFichier(nbreVoiture,voitureEnMem,3);
    sleep(1);



/* ==========================================

Samedi après midi séance de qualification en 3 parties

Q1, durée 18 minutes => élimine 5 voitures
Q2, durée 15 minutes => élimine 5 voitures
Q3, durée 12 minutes => classe les 10 dernières voitures

=============================================*/

    lancerCourse(nbreVoiture,180.0,voitureEnMem,numero);  //18 minutes => lance la qualification 1
    triDuTableau(voitureEnMem, nbreVoiture);                                        //permet de trier le tableau
    for(int i= 0 ; i< nbreVoiture; i ++){                       //enregistre les numéros de voitures qui sont qualifiés
        voitureQualif[i] = voitureEnMem[i].numero ;
    }
    printf("\nQualification 1\n");
    affichage(nbreVoiture, voitureEnMem);
    nbreVoiture -= 5;                                           //on supprime les 5 dernières voitures (le nombre de voitures est donc de 20-5 = 15)
    sleep(3);
    system("clear");

    lancerCourse(nbreVoiture,150.0,voitureEnMem,voitureQualif);  //15 minutes => lance la qualification 2
    triDuTableau(voitureEnMem, nbreVoiture);

    for(int i= 0 ; i< nbreVoiture; i ++){
        voitureQualifFinale[i] = voitureEnMem[i].numero ;
    }
    printf("\nQualification 2\n");
    affichage(nbreVoiture, voitureEnMem);
    sleep(3);
    system("clear");
    nbreVoiture -= 5;

    lancerCourse(nbreVoiture,120.0,voitureEnMem,voitureQualifFinale);  //12 minutes
    creationFichier(nbreVoiture,voitureEnMem,4);


    printf("\nQualification 3\n");
    affichage(nbreVoiture, voitureEnMem);
    sleep(3);
    system("clear");

/* ==========================================

Dimanche après midi la course en elle même

=============================================*/


    triDuTableau(voitureEnMem, nbreVoiture);
    lancerCourse(nbreVoiture,longueurTours*nbreTours,voitureEnMem,voitureQualifFinale);  //nbre de tours * nbre moyen du tour en minutes
    creationFichier(nbreVoiture,voitureEnMem,5);
    sleep(1);
    printf("\nQualification Finale\n");
    affichage(nbreVoiture, voitureEnMem);


    return 0;
}
