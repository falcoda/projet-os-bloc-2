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
void lancerCourse(int nbreVoiture,double raceTime,struct maVoiture pilotes[20],int mesVoitures[20]){
    int i ;
    int shmId;
    int semId;
    struct sembuf shmbuffer;
    int nbLecteur;

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

        semId = semget(IPC_PRIVATE, 2, IPC_CREAT | 0666);
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
    void commencerLecture(){ //Debut section critique
        wait(0);
        nbLecteur++;
        if (nbLecteur == 1) {
            wait(1);
        }
        post(0);
    }
    void arreterLecture(){ //Fin section critique
        wait(0);
        nbLecteur--;
        if (nbLecteur == 0) {
            post(1);
        }
        post(0);
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


    for(k=0; k < MAXFORK+1 ; k++){

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

            circuit[k].out = 0;                         // met la voiture à crash = 0 => elle roule
            circuit[k].numero = mesVoitures[k];         // donne l'id de la voiture

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
                    circuit[k].stand += 1;            //on ajoute le passage au pit
                    circuit[k].tempsTotal += tempsAuStand ;
                }
                else if (out(CRASH)==1){                //génère un nombre aléatoire, si il est = à 1, la voiture se crash
                    wait(1);
                    circuit[k].out = 1 ;
                    circuit[k].S1 = S1;
                    circuit[k].S2 = S2 ;
                    circuit[k].S3 = S3;
                    post(1);
                    exit(0);
                }


                circuit[k].tempsTotal +=S1+S2+S3 ;                   // calcule le temps total, temps depuis le quel les voitures roules
                if((S1+S2+S3) < circuit[k].meilleurTemps){          // si le dernier tour est le tour le plus rapide
                    wait(2);
                    circuit[k].meilleurTemps = S1+S2+S3 +tempsAuStand;     //enregistre le meilleur temps
                    circuit[k].S1 = S1;
                    circuit[k].S2 = S2;                                     //enregistre les données de S1, S2, S3
                    circuit[k].S3 = S3;
                    post(2);

                }

            }

            exit(0);

        }

        else { //fermeture des fils

        usleep(5);
        commencerLecture();
        memcpy(pilotes,circuit,nbreVoiture*sizeof(struct maVoiture)); //permet de faire la copie en mémoire
        arreterLecture();
        fflush(stdout);

        for(i=0;i<MAXFORK;i++){
          if (pid[i]>0) {
            if (waitpid(pid[i],NULL,WNOHANG)==0) {
              fflush(stdout);
              pid[i]=0;
            }
          } else {
            fini=0;
          }
        }
      fflush(stdout);
        }




    }

}


//Creer les fichiers pour sauvegarder les resultats
int creationFichier(int nbreVoiture,struct maVoiture pilotes[20],int numeroCourse){
    triDuTableau(pilotes, nbreVoiture); //tri a bulles
    FILE* fichier = NULL;
    if (numeroCourse == 1){
        fichier = fopen("courses/CourseP1.txt", "w+");
    }
    else if (numeroCourse == 2){
        fichier = fopen("courses/CourseP2.txt", "w+");
    }
    else if (numeroCourse == 3){
        fichier = fopen("courses/CourseP3.txt", "w+");
    }
    else if (numeroCourse == 4){
        fichier = fopen("courses/CourseQualif.txt", "w+");
    }
    else if (numeroCourse == 5){
        fichier = fopen("courses/CourseFinale.txt", "w+");
    }
    if (fichier != NULL){
        //Debut de l'ecriture
        fprintf(fichier,"N°\tS1\t\tS2\t\tS3\t\tTemps en piste\tBest\t\tPIT\tOUT\t\n");
        fprintf(fichier,"\n");

        for(int j = 0; j < nbreVoiture ; j++){
            fprintf(fichier,"%d\t",pilotes[j].numero); //Imprimme le N°
            if (pilotes[j].S1 == 0){ //Imprime le temps S1
                fprintf(fichier,"NULL|\n");
            }
            else{
                fprintf(fichier,"%.3f|",pilotes[j].S1);
            }
            if (pilotes[j].S2 == 0){ //Imprime le temps S2
                fprintf(fichier,"NULL|");
            }
            else{
                fprintf(fichier,"%.3f|",pilotes[j].S2);
            }
            if (pilotes[j].S3 == 0){ //Imprime le temps S3
                fprintf(fichier,"NULL\t");
            }
            else{
                fprintf(fichier,"%.3f\t",pilotes[j].S3);
            }
            if (pilotes[j].tempsTotal == 0){ //Imprime le temps du tour
                fprintf(fichier,"NULL\t\t\t");
            }
            else if(pilotes[j].tempsTotal<100.000){
                fprintf(fichier,"%.3f\t\t\t",pilotes[j].tempsTotal);
            }
            else if(pilotes[j].tempsTotal>1000.000){
                fprintf(fichier,"%.3f\t\t",pilotes[j].tempsTotal);
            }
            else{
                fprintf(fichier,"%.3f\t\t\t",pilotes[j].tempsTotal);
            }
            if (pilotes[j].meilleurTemps < 100.000){ //Imprimme le meilleur temps
                fprintf(fichier,"%.3f\t\t",pilotes[j].meilleurTemps);
            }
            else{
                fprintf(fichier,"%.3f\t\t",pilotes[j].meilleurTemps);
            }
            if(pilotes[j].stand != 0){ //Imprime le nombre de pit du pilote
                fprintf(fichier,"%d\t",pilotes[j].stand);
            }
            else{
                fprintf(fichier,"0\t");
            }
            if(pilotes[j].out == 1){ //Imprimme si le pilote est out
                fprintf(fichier,"X\t\n");
            }
            else{
            fprintf(fichier,"\t\n");
            }
        }//Fin ecriture

    fclose(fichier);
    }

    return 0;
}

int triDuTableau (struct maVoiture tab2[20],int tailleDuTableau){  // tri a bulles
    int i,j,k;
    maVoiture memoireTbl;
    for(j = 1; j <= tailleDuTableau ; j++){
        for(i = 0 ; i < tailleDuTableau - 1 ; i++){
            if (tab2[i].meilleurTemps > tab2[i+1].meilleurTemps ) {  //compare deux casse successive pour les échangés de place si l'ordre n'est pas le bon
                memoireTbl = tab2[i]; //sauvegarde la case qui vas être écrasée par la valeur plus petite
                tab2[i] = tab2[i+1];
                tab2[i+1] = memoireTbl;
            }
        }
    }
    return tab2;

}

int affichage(int nbreVoiture,maVoiture pilotes[20]){
    printf("N°\tS1\tS2\tS3\tTemps en piste\tBest\t\tPIT\tOUT\t\n");
    printf("\n");
    for(int j = 0; j < nbreVoiture ; j++){
            printf("%d\t",pilotes[j].numero); //Imprimme le N°
            if (pilotes[j].S1 == 0){ //Imprime le temps S1
                printf("NULL|\n");
            }
            else{
                printf("%.3f|",pilotes[j].S1);
            }
            if (pilotes[j].S2 == 0){ //Imprime le temps S2
                printf("NULL|");
            }
            else{
                printf("%.3f|",pilotes[j].S2);
            }
            if (pilotes[j].S3 == 0){ //Imprime le temps S3
                printf("NULL\t");
            }
            else{
                printf("%.3f\t",pilotes[j].S3);
            }
            if (pilotes[j].tempsTotal == 0){ //Imprime le temps du tour
                printf("NULL\t");
            }
            else if(pilotes[j].tempsTotal<100.000){
                printf("%.3f\t",pilotes[j].tempsTotal);
            }
            else{
                printf("%.3f\t",pilotes[j].tempsTotal);
            }
            if (pilotes[j].meilleurTemps < 100.000){ //Imprimme le meilleur temps
                printf("\t%.3f\t\t",pilotes[j].meilleurTemps);
            }
            else{
                printf("\t%.3f\t\t",pilotes[j].meilleurTemps);
            }
            if(pilotes[j].stand != 0){ //Imprime le nombre de pit du pilote
                printf("%d\t",pilotes[j].stand);
            }
            else{
                printf("0\t");
            }
            if(pilotes[j].out == 1){ //Imprimme si le pilote est out
                printf("X\t\n");
            }
            else{
            printf("\t\n");
            }
        }//Fin ecriture

}
