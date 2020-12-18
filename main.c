#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <sys/sem.h>
#include <pthread.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/ipc.h>
#include "rand.h"
#include "voiture.h"
#include "cirquit.h"
#define MAXFORK 20


int sem_init(sem_t *sem, int pshared, unsigned int value);
int sem_destroy(sem_t *sem);
int sem_wait(sem_t *sem);
int sem_post(sem_t *sem);



int main(int argc, char**argv)
{
    maVoiture voitureEnMem[20]; //tableau de voitures


    /* initialisation de la mémoire partagée */


    struct shmid_ds shmbuffer;

    /* Alloue le segment de mémoire partagée. */
    int segment_id = shmget (IPC_PRIVATE, sizeof(maVoiture)*20, IPC_CREAT | 0666);

    /* Attache le segment de mémoire partagée. */

    maVoiture *shared_memory  = (maVoiture *) shmat(segment_id, NULL, 0);
    printf ("memoire partagee attachee a l'adresse %p\n", shared_memory );

    /* Écrit une chaîne dans le segment de mémoire partagée. */
    sprintf (shared_memory , "Hello, world.");
//    memcpy(voitureEnMem, shared_memory , sizeof(maVoiture)*20);

    /* affiche la chaine en mémoire partagée. */
    printf ("%s\n", shared_memory);


    //========================================

   // pthread_t threads[MAXFORK];
    //int sem_init(&semaphore, PTHREAD_PROCESS_SHARED, 1);









    //========================================
    int tableauResult[20][6] ;// creation du tableau global
    //srand(time(NULL)^ getpid());//ATTENTION PAS TOUCHER --> la graine du générateur aléatoire doit être en dehors de la fct printRandom()

    int tableau[20] = {44, 77, 16, 5, 33, 23, 55, 4, 3, 31, 10, 26, 11, 18, 7, 99, 20, 8, 63, 6};
    /*création d'un tableau de structure pour les 20 voitures
    Si vous avez une meilleure idée n'hésitez pas à changer*/


    int k = 0;
    for(k=0; k<20; k++)
    {
        maVoiture tableau[k];       //Chaque élément du tableau devient une structure

    }
    //rouler();
    sleep(1);
    lancerCourse(20,45.0,voitureEnMem);
    sleep(2);





    return 0;
}



