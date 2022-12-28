//omer maoz 206947186 and dor ben shitrit 208631887
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdbool.h>
#include <sys/random.h>
#include <math.h>


void clientHandler();

int main(int argc, char* argv[]) {

    signal(2, clientHandler);

    if(argc!=5){
        printf("input error !\n");
        exit(-1);
    }

    int toServerFd;
    int i;
    pid_t c_pid = getpid();
    char buffer[256];
    sprintf(buffer, "%d", c_pid);
    unsigned int sleep_time;

    int s_pid =     atoi(argv[1]);
    int num1 =      atoi(argv[2]);
    int operator = atoi(argv[3]);
    int num2 =      atoi(argv[4]);



    for (i = 0; i < 10; i++) {
        toServerFd = open("toServer.txt", O_CREAT | O_RDWR, 0777);
        if(toServerFd<0){
            sleep_time = rand() % 10 + 1;
            sleep(sleep_time);

        }else break;
    }

    if(i==10){
        printf("can't connect to the server.out , bye bye \n");
        exit(-1);
    }

    if(write(toServerFd, &buffer, sizeof(buffer))<0) {
        printf("ERROR_FROM_EX2\n");
        exit(-1);
    }
    if(write(toServerFd, &num1, sizeof(num1))< 0){
        printf("ERROR_FROM_EX2\n");
        exit(-1);
    }
    if(write(toServerFd, &operator, sizeof(operator))<0){
        printf("ERROR_FROM_EX2\n");
        exit(-1);
    }
    if(write(toServerFd, &num2, sizeof(num2))<0){
        printf("ERROR_FROM_EX2\n");
        exit(-1);
    }
    close(toServerFd);

    kill(s_pid, 1);
    pause();
}



void clientHandler(){
    int toClientfd;
    char clientPath[256];
    char buffer1 [256];
    pid_t c_pid = getpid();
    sprintf(buffer1, "%d", c_pid);

    strcpy(clientPath, "toClient");
    strcat(clientPath, buffer1);
    strcat(clientPath, ".txt");

    toClientfd = open(clientPath, O_RDONLY);
    if(toClientfd<0){
        printf("error with clientHandler-> open(toClient)\n");
        exit(-1);
    }

    char result[256];
    read(toClientfd, &result, sizeof(result));
    printf("result: %d" , atoi(result));
    close(toClientfd);



}