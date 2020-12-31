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



double printRandoms(){  //nombre aléatoire qui définit le temps d'un secteur

    double random_value = 0;
    //for (int i=0;i<20;i++){
    random_value = (double)rand()/(double)RAND_MAX*(20.00)+25.00;//float in range 25 to 45
    //printf ( "%f\n", random_value);
    return random_value;
}


int myRandom(int nbreMin, int nbreMax){
    int randomNumber = rand() % (nbreMax-nbreMin + 1) +nbreMin;
    //printf("%d\n", randomNumber);
    return randomNumber;
}

int out(tauxDeCrash){  //permet de dire si se crash ou non
    int crash = myRandom(1,100);
    //printf("%d\n", crash);
    if (crash <= tauxDeCrash){
        return 1; //se crash
    }
    else {
        return 0;
    }
}

int stand(tauxDeStand){  //permet de dire si va au stand ou pas

    int crash = myRandom(1,100);
    //printf("%d\n", crash);
    if (crash <= tauxDeStand){
        return 1;  //va au stand
    }
    else {
        return 0;
    }
}

double tempsStand(){ //temps d'arrêt au stand (entre 25 et 45 s)

    double temps =0;
    temps = (double)rand()/(double)RAND_MAX*(20.00)+25.00;//float in range 25 to 45
    //printf("%f\n", temps);
    return temps ;
}
