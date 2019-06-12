#include <stdlib.h>
#include <stdio.h>
#include <sys/shm.h>
#include "errExit.h"
#include <sys/stat.h>
#include <fcntl.h>
#include "struct_definitions.h"
#include "shared_memory.h"
#include "string.h"
#include "semaphore.h"
#include <sys/sem.h>
#include <unistd.h>


struct SharedItem *entries;
char *fileFtokPath = "/tmp/fileFTOK";
int projid = 123;

#define MAXENTRIES 100
#define MUTEX 0


key_t getKey(){
    open(fileFtokPath,O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);

    return ftok(fileFtokPath,projid);
}


int getFirstDigit(int n){
    while (n >= 10)
        n /= 10;
    return n;
}



int main (int argc, char *argv[]) {
    printf("Hi, I'm ClientExec program!\n");


    if(argc <= 3){
        printf("Devi inserire qualche parametro\n");
        exit(0);
    }

    char *idUser = argv[1];
    int keyUser;
    sscanf(argv[2],"%d",&keyUser);

    key_t key = getKey();

    int semid = semget(key, 1, S_IRUSR | S_IWUSR);
    if(semid == -1)
        errExit("semget failed");


    int shmid =  alloc_shared_memory(key, 0);

    entries = (struct SharedItem *) get_shared_memory(shmid,0);

    semOp(semid, MUTEX , -1);

    for(int i=0; i < MAXENTRIES ;i++){
        if(keyUser == entries[i].key && strcmp(idUser,entries[i].idUser) == 0){

            int idService = getFirstDigit(entries[i].key);

            entries[i].idUser[0] = 0;
            entries[i].key = 0;
            entries[i].timestamp = 0;

            semOp(semid, MUTEX ,1);

            if(idService == 1){
                printf("stampa\n");
                execv("stampa",argv);
            }


            else if(idService == 2){
                printf("salva\n");
                execv("salva",argv);
            }


            else if(idService == 3){
                printf("invia\n");
                execv("invia",argv);
            }
        }
    }

    printf("chiave o utente non validi\n");
    semOp(semid, MUTEX , 1);

    return 0;
}
