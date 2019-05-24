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
int totalEntries = 10;


key_t getKey(){
    open(fileFtokPath,O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);

    return ftok(fileFtokPath,projid);
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

    semOp(semid, 0 , -1);

    for(int i=0; i<totalEntries;i++){
        if(keyUser == entries[i].key && strcmp(idUser,entries[i].idUser) == 0){

            int idService = entries[i].key - entries[i].timestamp;

            entries[i].idUser[0] = 0;
            entries[i].key = 0;
            entries[i].timestamp = 0;

            semOp(semid, 0 ,1);

            if(idService == 3){
                printf("stampa\n");
                execv("stampa",argv);
            }

            else if(idService == 7){
                printf("invia\n");
                execv("invia",argv);
            }

            else if(idService == 11){
                printf("salva\n");
                execv("salva",argv);
            }



            printf("chiave e utente trovati");
        }
    }

    semOp(semid, 0 , 1);





    return 0;
}
