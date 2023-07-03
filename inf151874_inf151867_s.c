#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "inf151874_inf151867_Message.h"

char users[15][10]; // MAKSYMALNIE: 15 userow
int userInGroups[5][15]; // w jakiej grupie sa uzytkownicy
int activeUsers[15] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int blockedUsers[15] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
char groups[5][10]; //zakladamy, ze jest 5 grup o max 10 znakowych nazwach

void getGroupsFromFile(int fd) {
    char c;
    int i = 0;
    int j = 0;
    while (read(fd,&c,1) > 0) {
        if (c == '\n') {
            i = 0;
            j++;
        }
        else if (c == '0') {
            userInGroups[j][i] = 0;
            i++;
        }
        else if (c == '1') {
            userInGroups[j][i] = 1;
            i++;
        }
        if (c == '#')
            return;
    }
}
void getNameGroupsFromFile(int fd)
{
    char c;
    int i = 0;
    int j = 0;
    while (read(fd,&c,1) > 0) {
        if(c==';')
        {
            i++;
            j=0;
        }
        if((((int)c >='a')&&((int)c <= 'z'))||(((int)c>='0')&&((int)c<='9')))
        {
            groups[i][j]=c;
            j++;
        }
        if(c=='\n') break;
    }
}

void getUsersFromFile() {
    int fd = open("inf151874_inf151867_users.txt",O_RDONLY);
    char c;
    int i = 0;
    int j = 0;
    while (read(fd,&c,1) > 0) {
        if (c == '#') {
            read(fd,&c,1);
            getGroupsFromFile(fd);
            return;
        }
        if (c == '*')
        {
            read(fd,&c,1);
            getNameGroupsFromFile(fd);            
        }
        if (c == ';') {
            i = 0;
            j++;
        }
        else if (c != '\n') {
            users[j][i] = c;
            i++;
        }
    }
}

int loginCheck(char username[10], int pid) {
    for (int i=0;i<15;i++) {
        if (strcmp(users[i],username) == 0 && activeUsers[i] == 0) {
            activeUsers[i] = pid;
            return 1;
        }
    }
    return 0;
}

int main() {
    int id = msgget(12345, 0600 | IPC_CREAT);
    struct Message message;
    getUsersFromFile();
    char username[10]; //zakladamy maksymalnie 10-znakowe loginy
    int badLoginCounter = 0;
    while (msgrcv(id, &message, 1024, 1, 0) > 0) { //serwer zawsze dostaje message.mtype = 1
     if (badLoginCounter < 3) {
        if (message.mtext[0] >= 48 && message.mtext[0] <= 57) { //logowanie
            int clientid = atoi(message.mtext);
            strcpy(message.mtext,"Witaj!\nPodaj login:");
            message.mtype = clientid;
            msgsnd(id, &message, strlen(message.mtext)+1, 0);
            msgrcv(id, &message, 1024, 1, 0);
            strcpy(username, message.mtext);

            if (loginCheck(username, clientid) == 0) {
                message.mtype = clientid;
                strcpy(message.mtext,"\nBledny login lub uzytkownik juz jest zalogowany!\n");
                msgsnd(id, &message, strlen(message.mtext)+1, 0);
                badLoginCounter++;
            }
            else {
                message.mtype = clientid;
                strcpy(message.mtext,"Udalo sie zalogowac");
                msgsnd(id, &message, strlen(message.mtext)+1, 0);
            }
        }
        if (strcmp(message.mtext,"logout") == 0) {  //wylogowanie
            msgrcv(id, &message, 1024, 1, 0);
            int clientid = atoi(message.mtext);
            message.mtype = clientid;
            strcpy(message.mtext,"Wylogowano\n");
            msgsnd(id, &message, strlen(message.mtext)+1, 0);
            for (int i=0;i<15;i++) {
                if(activeUsers[i] == clientid) {
                    activeUsers[i] = 0;
                }
            }
        }
        if (strcmp(message.mtext,"active users") == 0) { //lista zalogowanych
            msgrcv(id, &message, 1024, 1, 0);
            int clientid = atoi(message.mtext);
            message.mtype = clientid;
            strcpy(message.mtext,"------------------------------\nLISTA ZALOGOWANYCH UZYTKOWNIKOW:\n");
            for (int i=0;i<15;i++) {
                if(activeUsers[i] > 0) {
                    strcat(message.mtext,users[i]);
                    strcat(message.mtext,"\n");
                }
            }
            msgsnd(id, &message, strlen(message.mtext)+1, 0);
        }
        if (strcmp(message.mtext,"groups") == 0) { //lista grup
            msgrcv(id, &message, 1024, 1, 0);
            int clientid = atoi(message.mtext);
            message.mtype = clientid;
            strcpy(message.mtext,"------------------------------\nLISTA GRUP:\n");
            for(int i=0;i<5;i++)
            {
                strcat(message.mtext, groups[i]);
                strcat(message.mtext, "\n");
            }
            msgsnd(id, &message, strlen(message.mtext)+1, 0);
        }
        if (strcmp(message.mtext,"users in group") == 0) { //lista danej grupy
            msgrcv(id, &message, 1024, 1, 0);
            int clientid = atoi(message.mtext);
            message.mtype = clientid;
            msgrcv(id, &message, 1024, 1, 0);
            int x= 0;
            for(int i=0;i<5;i++)
            {
                if(strcmp(message.mtext,groups[i])==0)
                x=i;
            }
            message.mtype = clientid;
            strcpy(message.mtext,"------------------------------\nLISTA GRUPY:\n");
            strcat(message.mtext,groups[x]);
            strcat(message.mtext,": ");
                for(int j=0;j<15;j++)
                {
                    if(userInGroups[x][j] == 1)
                    {
                        strcat(message.mtext, users[j]);
                        strcat(message.mtext, " ");
                    }
                }
                msgsnd(id, &message, strlen(message.mtext)+1, 0);
            }
        if (strcmp(message.mtext,"add to groups") == 0) { //dodanie do grupy
            msgrcv(id, &message, 1024, 1, 0);
            int clientid = atoi(message.mtext);
            message.mtype = clientid;
            msgrcv(id, &message, 1024, 1, 0);
            int x= 0;
            for(int i=0;i<5;i++)
            {
                if(strcmp(message.mtext,groups[i])==0)
                x=i;
            }
            for(int i=0;i<15;i++)
                if(activeUsers[i]==clientid)
                    userInGroups[x][i]=1;
            message.mtype = clientid;
            strcpy(message.mtext,"DODANO CIE DO GRUPY\n");
            msgsnd(id, &message, strlen(message.mtext)+1, 0);
        }
        if (strcmp(message.mtext,"out of groups") == 0) { //wypisanie z grupy
            msgrcv(id, &message, 1024, 1, 0);
            int clientid = atoi(message.mtext);
            msgrcv(id, &message, 1024, 1, 0);
            message.mtype = clientid;
            int x= 0;
            for(int i=0;i<5;i++)
            {
                if(strcmp(message.mtext,groups[i])==0)
                x=i;
            }
            for(int i=0;i<15;i++)
                if(activeUsers[i]==clientid)
                    userInGroups[x][i]=0;
            message.mtype = clientid;
            strcpy(message.mtext,"USUNIETO CIE Z GRUPY\n");
            msgsnd(id, &message, strlen(message.mtext)+1, 0);
        }
        if (strcmp(message.mtext,"send message to group") == 0) { //wiadomosc do grupy
            msgrcv(id, &message, 1024, 1, 0);
            int clientid = atoi(message.mtext);
            msgrcv(id, &message, 1024, 1, 0);
            int group;
            group = atoi(message.mtext);
            msgrcv(id, &message, 1024, 1, 0);
            for(int j=0;j<15;j++) {
                if(userInGroups[group][j] == 1 && activeUsers[j] != 0) {
                    int receiver = activeUsers[j];
                    message.mtype = receiver;
                    msgsnd(id, &message, strlen(message.mtext)+1, 0);
                }
            }
            message.mtype = clientid;
            strcpy(message.mtext,"Wiadomosc wyslana do wszystkich aktywnych uzytkownikow z wybranej grupy!\n");
            msgsnd(id, &message, strlen(message.mtext)+1, 0);
        }
        if (strcmp(message.mtext,"send message to user") == 0) { //wiadomosc do uzytkownika
            msgrcv(id, &message, 1024, 1, 0);
            int clientid = atoi(message.mtext);
            int receiver = 0;
            char user[10];
            msgrcv(id, &message, 1024, 1, 0);
            strcpy(user, message.mtext);
            strcpy(message.mtext,"error");
            for (int i=0; i<15; i++) {
                if (strcmp(users[i],user) == 0 && activeUsers[i] != 0) {
                    receiver = activeUsers[i];
                    strcpy(message.mtext,"ok");
                }
            }
            for (int i=0; i<15; i++) {
                if (blockedUsers[i] == 1 && activeUsers[i] == clientid) {
                    strcpy(message.mtext,"blocked");
                    message.mtype = clientid;
                }
            }
            if (strcmp(message.mtext, "error") == 0) {
                message.mtype = clientid;
                msgsnd(id, &message, strlen(message.mtext)+1, 0);
            }
            else {
                if (strcmp(message.mtext, "blocked") == 0) {
                    message.mtype = clientid;
                    msgsnd(id, &message, strlen(message.mtext)+1, 0);
                }
                else {
                    message.mtype = clientid;
                    msgsnd(id, &message, strlen(message.mtext)+1, 0);
                    msgrcv(id, &message, 1024, 1, 0);
                    message.mtype = receiver;
                    msgsnd(id, &message, strlen(message.mtext)+1, 0);
                    message.mtype = clientid;
                    strcpy(message.mtext,"Wiadomosc wyslana!\n");
                    msgsnd(id, &message, strlen(message.mtext)+1, 0);
                }
            }
        }
        if (strcmp(message.mtext,"block user") == 0) { //zablokuj uzytkownika
            msgrcv(id, &message, 1024, 1, 0);
            int clientid = atoi(message.mtext);
            char user[10];
            msgrcv(id, &message, 1024, 1, 0);
            strcpy(user, message.mtext);
            message.mtype = clientid;
            for (int i=0; i<15; i++) {
                if (strcmp(users[i],user) == 0) {
                    blockedUsers[i] = 1;
                }
            }
            strcpy(message.mtext,"Zablokowano wybranego uzytkownika!\n");
            msgsnd(id, &message, strlen(message.mtext)+1, 0);
    }}
    }
}
