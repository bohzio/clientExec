#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <errExit.h>

struct message {
    long mtype;
    char mtext[100]; /* array of chars as message body */
};


/**
 * Only for testing purpose
 * This function create a queue with key
 * @param key
 */
void createQueue(int key) {
    int msqid = msgget(key, IPC_CREAT | S_IRUSR | S_IWUSR);
    if (msqid == -1)
        errExit("msgget failed");
}


void readQueue(int msqid) {
    struct message message;
    // read a message from the message queue
    size_t mSize = sizeof(struct message) - sizeof(long);
    if (msgrcv(msqid, &message, mSize, 0, 0) == -1)
        errExit("msgget failed");

    // print the order on standard output
    printf("%s\n", message.mtext);
}


int main(int argc, char *argv[]) {
    printf("Hi, I'm Invia program!\n");
    fflush(stdout);

    key_t key = atoi(argv[3]);

    if (key <= 0) {
        errExit("key must be greater of 0\n");
    }


    createQueue(key);

    int msqid = msgget(key, S_IRUSR | S_IWUSR);
    if (msqid == -1)
        errExit("queue doesn't exitst");


    struct message m;
    // message has type 1
    m.mtype = 1;
    // message contains the following string


    char buf[100];

    for (int i = 4; i < argc; i++) {
        strcat(buf, argv[i]);
        if (argc - i != 1)
            strcat(buf, " ");
    }

    memcpy(m.mtext, buf, strlen(buf) + 1);
    // size of m is only the size of its mtext attribute!
    size_t mSize = sizeof(struct message) - sizeof(long);
    // sending the message in the queue
    if (msgsnd(msqid, &m, mSize, 0) == -1)
        errExit("msgsnd failed");


    readQueue(msqid);


    return 0;
}
