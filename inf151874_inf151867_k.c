#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "inf151874_inf151867_Message.h"

void menu() {
    printf("------------------------------\nMENU\n1. Wyloguj sie\n2. Zalogowani uzytkownicy\n3. Lista grup\n4. Uzytkownicy w grupie\n5. Zapisz sie do grupy\n6. Wypisz sie z grupy\n7. Wyslij wiadomosc do grupy\n8. Wyslij wiadomosc do uzytkownika\n9. Odbierz wiadomosci\n10. Zablokuj uzytkownika\n");
}

int main() {
    struct Message message;
    int id = msgget(12345, 0600 | IPC_CREAT);
    // LOGOWANIE
    char username[10]; //zakladamy maksymalnie 10-znakowe loginy
    int pid = getpid();
    char pid_string[10];
    sprintf(pid_string, "%d", pid);
    message.mtype = 1;
    strcpy(message.mtext, pid_string);
    msgsnd(id, &message, strlen(message.mtext)+1, 0);
    msgrcv(id, &message, 1024, getpid(), 0);
    printf("%s\n",message.mtext);
    scanf("%s", username);
    message.mtype = 1;
    strcpy(message.mtext, username);
    msgsnd(id, &message, strlen(message.mtext)+1, 0);
    msgrcv(id, &message, 1024, getpid(), 0);
    if (strcmp(message.mtext,"\nBledny login lub uzytkownik juz jest zalogowany!\n") == 0) {
        printf("%s\n",message.mtext);
        return 0;
    }
    printf("%s jako: %s\n", message.mtext, username);
    // MENU
    char x[10];
    int option;
    while (1) {
        menu();
        scanf("%d", &option);
        switch (option) {
            case 1:
                message.mtype = 1;
                strcpy(message.mtext, "logout");
                msgsnd(id, &message, strlen(message.mtext)+1, 0);
                message.mtype = 1;
                strcpy(message.mtext, pid_string);
                msgsnd(id, &message, strlen(message.mtext)+1, 0);
                msgrcv(id, &message, 1024, getpid(), 0);
                printf("%s\n",message.mtext);
                return 0;
            case 2:
                message.mtype = 1;
                strcpy(message.mtext,"active users");
                msgsnd(id, &message, strlen(message.mtext)+1, 0);
                message.mtype = 1;
                strcpy(message.mtext, pid_string);
                msgsnd(id, &message, strlen(message.mtext)+1, 0);
                msgrcv(id, &message, 1024, getpid(), 0);
                printf("%s\n",message.mtext);
                break;
            case 3:
                message.mtype = 1;
                strcpy(message.mtext,"groups");
                msgsnd(id, &message, strlen(message.mtext)+1, 0);
                message.mtype = 1;
                strcpy(message.mtext, pid_string);
                msgsnd(id, &message, strlen(message.mtext)+1, 0);
                msgrcv(id, &message, 1024, getpid(), 0);
                printf("%s\n",message.mtext);
                break;
            case 4:
                printf("\nWybor: ");
                scanf("%s",x);
                
                if((strcmp(x,"grupa1")==0)||(strcmp(x,"grupa2")==0)||(strcmp(x,"grupa3")==0)||(strcmp(x,"grupa4")==0)||(strcmp(x,"grupa5")==0))
                {
                    message.mtype = 1;
                    strcpy(message.mtext,"users in group");
                    msgsnd(id, &message, strlen(message.mtext)+1, 0);
                    message.mtype = 1;
                    strcpy(message.mtext, pid_string);
                    msgsnd(id, &message, strlen(message.mtext)+1, 0);
                    strcpy(message.mtext,x);
                    message.mtype = 1;
                    msgsnd(id, &message, strlen(message.mtext)+1, 0);
                    
                    msgrcv(id, &message, 1024, getpid(), 0);
                    printf("%s\n",message.mtext);
                }
                else printf("ERROR. Podaj poprawna nazwe grupy!");
                break;
            case 5:                
                printf("\nWybor: ");
                scanf("%s",x);
                
                if((strcmp(x,"grupa1")==0)||(strcmp(x,"grupa2")==0)||(strcmp(x,"grupa3")==0)||(strcmp(x,"grupa4")==0)||(strcmp(x,"grupa5")==0))
                {
                    message.mtype = 1;
                    strcpy(message.mtext,"add to groups");
                    msgsnd(id, &message, strlen(message.mtext)+1, 0);
                    message.mtype = 1;
                    strcpy(message.mtext, pid_string);
                    msgsnd(id, &message, strlen(message.mtext)+1, 0);
                    strcpy(message.mtext,x);
                    message.mtype = 1;
                    msgsnd(id, &message, strlen(message.mtext)+1, 0);
                    
                    msgrcv(id, &message, 1024, getpid(), 0);
                    printf("%s\n",message.mtext);
                }
                else printf("ERROR. Podaj poprawna nazwe grupy!\n");
                break;
            case 6:
                printf("\nWybor: ");
                scanf("%s",x);
                
                if((strcmp(x,"grupa1")==0)||(strcmp(x,"grupa2")==0)||(strcmp(x,"grupa3")==0)||(strcmp(x,"grupa4")==0)||(strcmp(x,"grupa5")==0))
                {
                    message.mtype = 1;
                    strcpy(message.mtext,"out of groups");
                    msgsnd(id, &message, strlen(message.mtext)+1, 0);
                    message.mtype = 1;
                    strcpy(message.mtext, pid_string);
                    msgsnd(id, &message, strlen(message.mtext)+1, 0);
                    strcpy(message.mtext,x);
                    message.mtype = 1;
                    msgsnd(id, &message, strlen(message.mtext)+1, 0);
                    
                    msgrcv(id, &message, 1024, getpid(), 0);
                    printf("%s\n",message.mtext);
                }
                else printf("ERROR. Podaj poprawna nazwe grupy!\n");
                break;
            case 7:
                message.mtype = 1;
                char group[1];
                strcpy(message.mtext,"send message to group");
                msgsnd(id, &message, strlen(message.mtext)+1, 0);
                message.mtype = 1;
                strcpy(message.mtext, pid_string);
                msgsnd(id, &message, strlen(message.mtext)+1, 0);
                printf("Podaj nazwe grupy, ktorej chcesz wyslac wiadomosc: ");
                scanf("%s", group);
                message.mtype = 1;
                strcpy(message.mtext, group);
                msgsnd(id, &message, strlen(message.mtext)+1, 0);
                char text[1024];
                printf("Napisz wiadomosc: ");
                scanf(" %[^\n]s", text);
                message.mtype = 1;
                strcpy(message.mtext, text);
                msgsnd(id, &message, strlen(message.mtext)+1, 0);
                msgrcv(id, &message, 1024, getpid(), 0);
                printf("%s\n",message.mtext);
                break;
            case 8:
                message.mtype = 1;
                char user[10];
                strcpy(message.mtext,"send message to user");
                msgsnd(id, &message, strlen(message.mtext)+1, 0);
                message.mtype = 1;
                strcpy(message.mtext, pid_string);
                msgsnd(id, &message, strlen(message.mtext)+1, 0);
                printf("Podaj login uzytkownika, ktoremu chcesz wyslac wiadomosc: ");
                scanf("%s", user);
                message.mtype = 1;
                strcpy(message.mtext, user);
                msgsnd(id, &message, strlen(message.mtext)+1, 0);
                msgrcv(id, &message, 1024, getpid(), 0);
                if (strcmp(message.mtext, "blocked") == 0) {
                    printf("\nZostales zablokowany i nie mozesz wysylac wiadomosci\n");
                    break;
                }
                else {
                if (strcmp(message.mtext, "error") == 0) {
                    printf("\nPodany uzytkownik nie jest obecnie aktywny\n");
                    break;
                }
                else {
                    char text[1024];
                    printf("Napisz wiadomosc: ");
                    scanf(" %[^\n]s", text);
                    message.mtype = 1;
                    strcpy(message.mtext, text);
                    msgsnd(id, &message, strlen(message.mtext)+1, 0);
                    msgrcv(id, &message, 1024, getpid(), 0);
                    printf("%s\n",message.mtext);
                }
                }
                break;
            case 9:
                printf("------------------------------\nOTRZYMANE WIADOMOSCI:\n");
                int counter = 1;
                while (msgrcv(id, &message, 1024, getpid(), IPC_NOWAIT) != -1) {
                    printf("%d. %s", counter, message.mtext);
                    printf("\n");
                    counter += 1;
                }
                break;
            case 10:
                message.mtype = 1;
                char userToBlock[10];
                strcpy(message.mtext,"block user");
                msgsnd(id, &message, strlen(message.mtext)+1, 0);
                message.mtype = 1;
                strcpy(message.mtext, pid_string);
                msgsnd(id, &message, strlen(message.mtext)+1, 0);
                printf("Podaj login uzytkownika, ktorego chcesz zablokowac: ");
                scanf("%s", userToBlock);
                message.mtype = 1;
                strcpy(message.mtext, userToBlock);
                msgsnd(id, &message, strlen(message.mtext)+1, 0);
                msgrcv(id, &message, 1024, getpid(), 0);
                printf("%s\n",message.mtext);
                break;
        }
    }
}
