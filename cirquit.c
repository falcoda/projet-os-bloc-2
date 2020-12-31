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

int mesVoitures[20] = {44, 77, 16, 5, 33, 23, 55, 4, 3, 31, 10, 26, 11, 18, 7, 99, 20, 8, 63, 6};

/*=======================================================
Création des fonctions Sémaphore
=========================================================*/
void lancerCourse(int nbreVoiture,double raceTime,struct maVoiture pilotes[20]){
    int i ;
    int shmId;
    int semId;
    struct sembuf shmbuffer;
    int nbLecteur;
    //key_t key = ftok("/dev/null", 7);
    struct maVoiture *circuit; //Structure qui stocke les voitures actuellement en piste
    shmId = shmget(IPC_PRIVATE, 20*sizeof(struct maVoiture), IPC_CREAT|0666);
    if(shmId == -1){ //Erreur lors de la creation de la memoire partagee
        printf("Erreur lors de la creation de la mem partagee shmId = -1 ");
        exit(1);
    }
    circuit = shmat(shmId,0,0);
    if(circuit == (struct voiture*)-1){ //Erreur lors de l'acces a la memoire partagee
        printf("Erreur shmat = -1");
        exit(1);
    }
    void initSem(){ //Initalise le semaphore
        //key_t semCle;
        semId = semget(IPC_PRIVATE, 2, IPC_CREAT | 0666);
      /*  if( semCle < 0 )
        {
            printf("Erreur semid\n");
            exit(0);
        }*/
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

    //==============Sémaphores + mémoire partagée + fork

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
            srand(time(NULL)^ getpid());                // graine, elle doit être dans les fils afin de prendre leurs pid unique à chaques fois

            circuit = shmat(shmId,0,0);

            if(circuit == (struct maVoiture*)-1){       //Erreur lors de l'acces a la memoire partagee
                printf("Erreur shmat = -1");
                exit(1);
            }
//=================================================TEST!!!!!=====================
            circuit[k].out = 0;                         // met la voiture à crash = 0 => elle roule
            circuit[k].numero = mesVoitures[k];      // donne l'id de la voiture
            circuit[k].stand = 0;                       //met la voiture à stand = 0 => pas au stand
            circuit[k].tempsTotal = 0;                  //défini le temps total à 0 (début de la course)
            circuit[k].meilleurTemps = 0.0;             //défini le meilleur temps d'un tour à 0
            circuit[k].meilleurTemps = 999.9;           // défini le meilleur temps à 999

            while(circuit[k].out == 0 && circuit[k].tempsTotal < raceTime){
                tempsAuStand = 0.0 ;                    // défini le temps passé au stand à 0

                S1 = printRandoms() ;
                S2 = printRandoms() ;                   //enregistre un temps de secteur
                S3 = printRandoms() ;

                if(stand(TAUX_DE_STAND)==1){            //génère un nombre alléatoire, si il est = à 1, la voiture va au stand
                    tempsAuStand =tempsStand();         //gènère le temps en secondes que la voiture va passer au stand
                    //printf("temps au stand %f \n",tempsAuStand);
                }
                else if (out(CRASH)==1){                //génère un nombre aléatoire, si il est = à 1, la voiture se crash
                    wait(1);
                    circuit[k].out = 1 ;
                    circuit[k].S1 = S1;
                    circuit[k].S2 = S2 ;
                    circuit[k].S3 = S3;
                    printf("La voiture %d est out.\n",circuit[k].numero);
                    post(1);
                    exit(0);
                }

               // printf("random ?%f \n",S1);

                circuit[k].tempsTotal +=S1+S2+S3 ;                   // calcule le temps total, temps depuis le quel les voitures roules
                //printf("Temps total : %f\n",circuit[k].tempsTotal);
                if((S1+S2+S3) < circuit[k].meilleurTemps){          // si le dernier tour est le tour le plus rapide
                    wait(2);
                    circuit[k].meilleurTemps = S1+S2+S3 +tempsAuStand;     //enregistre le meilleur temps
                    circuit[k].S1 = S1;
                    circuit[k].S2 = S2;                                     //enregistre les données de S1, S2, S3
                    //printf("temps au S2 %f \n",circuit[k].S2);
                    circuit[k].S3 = S3;

                    post(2);

                }
                //printf("Fils %2d (PID=%d): Active\n",k,getpid());

            }
            //printf("Temps total : %f\n",circuit[k].tempsTotal);
            printf("Meilleur Temps de %d: %f\n",circuit[k].numero,circuit[k].meilleurTemps);

            exit(0);
            fflush(stdout);
        }

        else {
            //printf("Pere : Activation du fils %2d\n", i);
            fflush(stdout);
           //printf("Père : Fin des activations\nAttente ...\n");

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



          //printf("Fin de tous les fils.");
          fflush(stdout);
                }




    }
    memcpy(pilotes,circuit,20*sizeof(struct maVoiture)); //permet de faire la copie en mémoire

}


//Creer les fichiers pour sauvegarder les resultats
int creationFichier(int nbreVoiture,struct maVoiture pilotes[20]){
    FILE* fichier = NULL;

    fichier = fopen("Essai.txt", "w+");
    if (fichier != NULL){
        //Debut de l'ecriture
        fprintf(fichier,"|N°\t|S1\t\t|S2\t\t|S3\t\t|Tour\t\t|Best\t\t|PIT\t|OUT\t|\n");
        fprintf(fichier,"\n");

        for(int j = 0; j < nbreVoiture ; j++){
            fprintf(fichier,"|%d\t",pilotes[j].numero); //Imprimme le N°
            if (pilotes[j].S1 == 0){ //Imprime le temps S1
                fprintf(fichier,"|NULL\n");
            }
            else{
                fprintf(fichier,"|%.3f\n",pilotes[j].S1);
            }
            if (pilotes[j].S2 == 0){ //Imprime le temps S2
                fprintf(fichier,"|NULL\n");
            }
            else{
                fprintf(fichier,"|%.3f\n",pilotes[j].S2);
            }
            if (pilotes[j].S3 == 0){ //Imprime le temps S3
                fprintf(fichier,"|NULL\n");
            }
            else{
                fprintf(fichier,"|%.3f\t",pilotes[j].S3);
            }
            if (pilotes[j].tempsTotal == 0){ //Imprime le temps du tour
                fprintf(fichier,"|NULL\t\t");
            }
            else if(pilotes[j].tempsTotal<100.000){
                fprintf(fichier,"|%.3f\t\t",pilotes[j].tempsTotal);
            }
            else{
                fprintf(fichier,"|%.3f\t",pilotes[j].tempsTotal);
            }
            if (pilotes[j].meilleurTemps < 100.000){ //Imprimme le meilleur temps
                fprintf(fichier,"|%.3f\t\t",pilotes[j].meilleurTemps);
            }
            else{
                fprintf(fichier,"|%.3f\t",pilotes[j].meilleurTemps);
            }
            if(pilotes[j].stand != 0){ //Imprime le nombre de pit du pilote
                fprintf(fichier,"|%d\t",pilotes[j].stand);
            }
            else{
                fprintf(fichier,"|0\t");
            }
            if(pilotes[j].out == 1){ //Imprimme si le pilote est out
                fprintf(fichier,"|X\t|\n");
            }
            else{
            fprintf(fichier,"|\t|\n");
            }
        }//Fin ecriture
    fclose(fichier);
    }
    return 0;
}
