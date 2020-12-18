#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <sys/sem.h>

#include "voiture.h"
#include "rand.h"

#define MAXFORK 20



/*=======================================================
Création des fonctions Sémaphore
=========================================================*/
void lancerCourse(int nbreVoiture,double raceTime,struct maVoiture pilotes[20]){
    int i ;
    int shmId;
    int semId;
    struct sembuf shmbuffer;
    int nbLecteur;
    key_t key = ftok("/dev/null", 7);
    struct maVoiture *circuit; //Structure qui stocke les voitures actuellement en piste
    shmId = shmget(key, 20*sizeof(struct maVoiture), IPC_CREAT|0666);
    if(shmId == -1){ //Erreur lors de la creation de la memoire partagee
        printf("Erreur shmId = -1 ");
        exit(1);
    }
    circuit = shmat(shmId,0,0);
    if(circuit == (struct voiture*)-1){ //Erreur lors de l'acces a la memoire partagee
        printf("Erreur shmat = -1");
        exit(1);
    }
    void initSem(){ //Initalise le semaphore
        key_t semCle;
        semId = semget(semCle, 2, IPC_CREAT | 0666);
        if( semCle < 0 )
        {
            printf("Erreur semid\n");
            exit(0);
        }
        semctl(semId, 0, SETVAL, 1);
        semctl(semId, 1, SETVAL, 1);
    }
    void wait(int i){ //Met le thread en attente
        shmbuffer.sem_num = i;
        shmbuffer.sem_op = -1;
        shmbuffer.sem_flg = SEM_UNDO;
        semop(semId, &shmbuffer, 1);
    }
    void post(int i){ //Enleve le thread de la liste d'attente
        shmbuffer.sem_num = i;
        shmbuffer.sem_op = 1;
        shmbuffer.sem_flg = SEM_UNDO;
        semop(semId, &shmbuffer, 1);
    }
    initSem();







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
    rouler();



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

    int pid[MAXFORK];
    int k,l;
    int fini;
    int compteur = 0 ;
    double tourComplet = 45*3;
    double tempsAuStand ;
    double S1,S2,S3;


    for(k=0; k < MAXFORK ; k++){

        pid[k] = fork() ;
        if(pid[k]== -1){
            printf("Impossible de créer un fils (%d)\n",k);
        }
        else if(pid[k]==0){ // FILS
            circuit = shmat(shmId,0,0);

            if(circuit == (struct maVoiture*)-1){ //Erreur lors de l'acces a la memoire partagee
                printf("Erreur shmat = -1");
                exit(1);
            }
//=================================================TEST!!!!!=====================

            for(int i=0; i<NBR_TOURS; i++){  //permet de lancer la fonction x fois



            if(stand(TAUX_DE_STAND)==1){
                tempsAuStand =tempsStand();
                //printf("temps au stand %f \n",tempsAuStand);
            }
            else if (out(CRASH)==1){

                circuit[k].tempsTotal = tourComplet;
              /*  temps[0]=circuit[k].S1 ;
                temps[1]=circuit[k].S2 ;
                temps[2]=circuit[k].S3 ;*/
                printf("out \n");
                return "out" ;
            }
            S1 = printRandoms() ;
            //printf("%f \n",S1);

            if (circuit[k].S1 > S1) {     //permet d'enregistrer le meilleur temps
                circuit[k].S1 = S1;


            }

            S2 = printRandoms() ;
            if (circuit[k].S2 > S2) {
                circuit[k].S2 = S2;
            }

            S3 = printRandoms() ;
            if (circuit[k].S3 > S3) {
                circuit[k].S3 = S3;
            }

            if((S1+S2+S3) < tourComplet){
                tourComplet = S1+S2+S3 +tempsAuStand;
            }

        }



        circuit[k].tempsTotal = tourComplet;






//=================================TEST!!!!


            circuit[i].tempsTotal;
            printf("Temps total : %d\n",circuit[i].tempsTotal);
            //printf("Fils %2d (PID=%d): Active\n",k,getpid());
            sleep(2);


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


}
