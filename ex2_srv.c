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

void handler();
void readRow(char* buff, int fd);

int main(int argc, char* argv[]) {

    signal(SIGUSR1, handler);

    while (1){
        pause();
    }
}

void handler(){
    char num1[50];
    char num2[50];
    char clientPid[50];
    char operator[1];
    int result=0;
    int err =0;
    int toServerFd;

    if((toServerFd = open("toServer.txt", O_RDONLY))<0){
        printf("ERROR_FROM_EX2\n");
        exit(-1);
    }
    readRow(clientPid, toServerFd);
    readRow(num1, toServerFd);
    readRow(operator, toServerFd);
    readRow(num2, toServerFd);
    close(toServerFd);


    int child_pid = fork();
    if (child_pid == 0) {
        char pat[50];
        strcpy(pat, "rm ");
        strcat(pat, "toServer.txt");
        strcat(pat, NULL);
        execvp("./toServer.txt" , pat);
    }
    else{
        wait(NULL);
    }

    switch (atoi(operator)) {
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
            if(atoi(num2) !=0)
                result = atoi(num1) / atoi(num2);
            else err = 1;
            break;
    }

    char temp[30];
    strcat(temp ,  clientPid);
    char buffer[50];
    strcpy(buffer , "toClient");
    strcat(buffer, temp);
    strcat(buffer, ".txt");
    int toClientFd;

    char resultString[50];
    sprintf(resultString, "%d", result);


    toClientFd = open(buffer, O_CREAT | O_RDWR , 0777);
    if(toClientFd<0){
        printf("ERROR_FROM_EX2\n");
        exit(-1);
    }

    //error calculate - divide by zero
    if(err == 1){
        char errorBuffer[50];
        strcpy(errorBuffer, "can't divide by 0 !");
        write(toClientFd, errorBuffer, strlen(errorBuffer));
    }
    //good calculate
    else {
        write(toClientFd, resultString, strlen(resultString));
    }
    close(toClientFd);
    kill(atoi(clientPid) , SIGUSR2);
    signal(SIGUSR1, handler);
}

//reading from a file row by row
void readRow(char* buff, int fd){
    int i = 0;
    int readBytes;
    char tav;
    readBytes = read(fd, &tav, 1);
    while (tav != '\n' && readBytes != 0)
    {
        buff[i] = tav;
        readBytes = read(fd, &tav, 1);
        i++;
    }
    buff[i] = '\0';
}
