#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <pthread.h>
#include <errno.h>
#include "rand.h"
#include "voiture.h"

#define MAXFORK 20







double* rouler(){
    //int tid = pthread_self();


    double temps[2]; // creation du tableau local
    maVoiture test;
    double S1,S2,S3;
    test.S1 = 45.00;
    test.S2 = 45.00;
    test.S3 = 45.00;
    int compteur = 0 ;
    double tourComplet = 45*3;
    double tempsAuStand = 0.0;

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

int main()
{


    maVoiture voitureEnMem[20]; //tableau de voitures
    /* initialisation de la mémoire partagée */

    char* shared_memory;
    struct shmid_ds shmbuffer;

    /* Alloue le segment de mémoire partagée. */
    int segment_id = shmget (IPC_PRIVATE, sizeof(maVoiture)*20,
                       IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);

    /* Attache le segment de mémoire partagée. */

    maVoiture *shm = (maVoiture *) shmat(segment_id, NULL, 0);
    printf ("memoire partagee attachee a l'adresse %p\n", shm);
    /* Écrit une chaîne dans le segment de mémoire partagée. */
    sprintf (shm, "Hello, world.");

    //========================================

   // pthread_t threads[MAXFORK];
    //int sem_init(&semaphore, PTHREAD_PROCESS_SHARED, 1);




    //========================================
    int tableauResult[20][6] ;// creation du tableau global
     //ATTENTION PAS TOUCHER --> la graine du générateur aléatoire doit être en dehors de la fct printRandom()

    int tableau[20] = {44, 77, 16, 5, 33, 23, 55, 4, 3, 31, 10, 26, 11, 18, 7, 99, 20, 8, 63, 6};
    /*création d'un tableau de structure pour les 20 voitures
    Si vous avez une meilleure idée n'hésitez pas à changer*/


    int k = 0;
    for(k=0; k<20; k++)
    {
        maVoiture tableau[k];       //Chaque élément du tableau devient une structure

    }
    //rouler();




    int pid[MAXFORK];
    int i,j;
    int fini;

    for(i=0; i < MAXFORK ; i++){

        pid[i] = fork() ;
        if(pid[i]== -1){
            //printf("Impossible de créer un fils (%d)\n",i);
        }
        else if(pid[i]==0){ // FILS
            //pthread_join(threads[i], NULL);
            //faire les tours
            sleep(1);
            //printf("Fils %2d (PID=%d): Activé\n",i,getpid());
            fflush(stdout);
            rouler();
            exit(0);
            fflush(stdout);
        }
        else {
            //printf("Pere : Activation du fils %2d\n", i);
            fflush(stdout);
        }

    }

    //printf("Père : Fin des activations\nAttente ...\n");
    fflush(stdout);
    fini=0;
    while (!fini) {
    fini=1;
    for(i=0;i<MAXFORK;i++){
      if (pid[i]>0) {
        if (waitpid(pid[i],NULL,WNOHANG)==0) {
          //printf("Père: fin du fils %2d (PID=%d)\n", i, pid[i]);
          fflush(stdout);
          pid[i]=0;
        }
      } else {
        fini=0;
      }
    }
    }


  //printf("Fin de tous les fils.");
  fflush(stdout);



    return 0;
}



