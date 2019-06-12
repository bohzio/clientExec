#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <errExit.h>

struct mymsg {
    long mtype;
    char mtext[100]; /* array of chars as message body */
};


int main(int argc, char *argv[]) {
    printf("Hi, I'm Invia program!\n");

    key_t key = atoi(argv[3]);

    int msqid = msgget(key, S_IRUSR | S_IWUSR);


    struct mymsg m;
    // message has type 1
    m.mtype = 1;
    // message contains the following string


    char buf[256];

    for(int i=4;i<argc;i++){
        strcat(buf, argv[i]);
        if(argc - i != 1)
            strcat(buf, " ");
    }

    memcpy(m.mtext, buf, strlen(buf) + 1);
    // size of m is only the size of its mtext attribute!
    size_t mSize = sizeof(struct mymsg) - sizeof(long);
    // sending the message in the queue
    if (msgsnd(msqid, &m, mSize, 0) == -1)
        errExit("msgsnd failed");


    return 0;
}
