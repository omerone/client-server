//omer maoz 206947186 and dor ben shitrit 208631887
#include <stdio.h>
#include <string.h>
#include <unistd.h>
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

void handler(int signal);

int main(int argc, char* argv[]) {

    signal(1, handler);

    while (1){
        pause();
    }
}


void handler(int signal){
    char num1[50], num2[50], clientPid[50];
    char opertor[1];
    int result;
    int toServerFd;
    toServerFd = open("toServer.txt", O_RDONLY);
    read(toServerFd, &clientPid, sizeof(clientPid));
    read(toServerFd, &num1, sizeof(num1));
    read(toServerFd, &opertor, sizeof(opertor));
    read(toServerFd, &num2, sizeof(num2));
    close(toServerFd);

    int child_pid = fork();
    if (child_pid == 0){
        remove("./toServer.txt");
        switch (atoi(opertor))
        {
            case 1:
                result = atoi(num1) + atoi(num2);
                break;
            case 2:
                result = atoi(num1) - atoi(num2);
                break;
            case 3:
                result = atoi(num1) * atoi(num2);
                break;
            case 4:
                if(num2 != 0)
                    result = atoi(num1) / atoi(num2);
                break;
        }

        char buffer[256];
        char errorBuffer[256];
        int toClientFd;

        if(result == NULL){
            strcpy(errorBuffer, "cant divide by 0");
            strcpy(buffer , "toClient");
            strcat(buffer, clientPid);
            strcat(buffer, ".txt");
            toClientFd = open(buffer, O_CREAT | O_RDWR , 0777);
            if(toClientFd<0){
                printf("ERROR_FROM_EX2\n");
                exit(-1);
            }
            write(toClientFd, &errorBuffer, sizeof(errorBuffer));
            close(toClientFd);
            kill(clientPid , 2);
        }
        else {
            strcpy(buffer , "toClient");
            strcat(buffer, clientPid);
            strcat(buffer, ".txt");
            toClientFd = open(buffer, O_CREAT | O_RDWR , 0777);
            if(toClientFd<0){
                printf("ERROR_FROM_EX2\n");
                exit(-1);
            }
            write(toClientFd, &result, sizeof(result));
            close(toClientFd);
            kill(clientPid , 2);
        }
    }

}