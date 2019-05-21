#include <stdlib.h>
#include <stdio.h>
#include <sys/shm.h>
#include "errExit.h"
#include <sys/stat.h>
#include "struct_definitions.h"
#include "shared_memory.h"


struct SharedItem *entries;


int main (int argc, char *argv[]) {
    printf("Hi, I'm ClientExec program!\n");

    int shmid =  alloc_shared_memory(0);

    entries = (struct SharedItem *) get_shared_memory(shmid,0);

    printf("%s",entries[0].idUser);


    return 0;
}
