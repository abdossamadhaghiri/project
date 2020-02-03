#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include "cJSON.h"
#include "cJSON.c"
#define MAX 1023
#define PORT 8080
#define SA struct sockaddr
int client_socket, server_socket;
struct sockaddr_in servaddr, cli;
int x1, x2, x3;
char token[100] ={0};
void make_socket(){
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        printf("Socket creation failed...\n");
        exit(0);
    }
}
void connect_socket(){
    if (connect(client_socket, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        printf("Connection to the server failed...\n");
        exit(0);
    }
}
void chat()
{
    char str1[100], str2[100], account_name[100], channel_name[100];
	char buffer[MAX];
	int flag = 1;
	while (true){
        if(flag == 1){
            printf("Account menu:\n1: Register\n2: Login\n");
            scanf("%d", &x1);
            if(x1 == 1){
                printf("Enter Username : ");
                getchar();
                gets(str1);
                printf("Enter Password : ");
                gets(str2);
                sprintf(buffer, "register %s, %s\n", str1, str2);
                make_socket();
                connect_socket();
                send(client_socket, buffer, sizeof(buffer), 0);
                memset(buffer, 0, sizeof(buffer));
                recv(client_socket, buffer, sizeof(buffer), 0);
                printf("%s", buffer);
                closesocket(client_socket);
                cJSON * rec =cJSON_Parse(buffer);
                if(strcmp(cJSON_GetObjectItemCaseSensitive(rec,"type")->valuestring,"Error") != 0)
                    printf("successfully registered\n");
                else
                    printf("%s\n", cJSON_GetObjectItemCaseSensitive(rec, "content")->valuestring);
                memset(str1, 0, sizeof(str1));
                memset(str2, 0, sizeof(str2));
            }
            if(x1 == 2){
                printf("Enter Username : ");
                getchar();
                gets(str1);
                sprintf(account_name,"%s", str1);
                printf("Enter Password : ");
                gets(str2);
                sprintf(buffer, "login %s, %s\n", str1, str2);
                memset(str1, 0, sizeof(str1));
                memset(str2, 0, sizeof(str2));
                make_socket();
                connect_socket();
                send(client_socket, buffer, sizeof(buffer), 0);
                memset(buffer, 0, sizeof(buffer));
                recv(client_socket, buffer, sizeof(buffer), 0);
                closesocket(client_socket);
                cJSON * rec =cJSON_Parse(buffer);
                if(strcmp(cJSON_GetObjectItemCaseSensitive(rec,"type")->valuestring,"Error") != 0){
                    printf("User %s successfully logged in\n", account_name);
                    strcpy(token,cJSON_GetObjectItemCaseSensitive(rec,"content")->valuestring);
                    flag++;
                }
                else
                    printf("%s\n", cJSON_GetObjectItemCaseSensitive(rec, "content")->valuestring);
                }
            }
        if(flag == 2){
            printf("1 : Create Channel\n2 : Join Channel\n3 : Logout\n");
            scanf("%d", &x2);
            if(x2 == 1){
                printf("Enter Name Of Your Channel: ");
                getchar();
                gets(str1);
                sprintf(channel_name, "%s", str1);
                sprintf(buffer, "create channel %s, %s\n", str1, token );
                make_socket();
                connect_socket();
                send(client_socket, buffer, sizeof(buffer), 0);
                memset(buffer, 0, sizeof(buffer));
                recv(client_socket, buffer, sizeof(buffer), 0);
                closesocket(client_socket);
                cJSON * rec =cJSON_Parse(buffer);
                if(strcmp(cJSON_GetObjectItemCaseSensitive(rec,"type")->valuestring,"Error") != 0){
                    printf("Channel %s successfully created\n", channel_name);
                    flag++;
                }
                else
                    printf("%s\n", cJSON_GetObjectItemCaseSensitive(rec, "content")->valuestring);
                memset(str1, 0, sizeof(str1));
            }
            if(x2 == 2){
                printf("Enter Name Of Your Channel: ");
                getchar();
                gets(str1);
                sprintf(channel_name, "%s", str1);
                sprintf(buffer, "join channel %s, %s\n", str1, token );
                make_socket();
                connect_socket();
                send(client_socket, buffer, sizeof(buffer), 0);
                memset(buffer, 0, sizeof(buffer));
                recv(client_socket, buffer, sizeof(buffer), 0);
                closesocket(client_socket);
                cJSON * rec =cJSON_Parse(buffer);
                if(strcmp(cJSON_GetObjectItemCaseSensitive(rec,"type")->valuestring,"Error") != 0){
                    printf("You successfully joined to %s\n", channel_name);
                    flag++;
                }
                else
                    printf("%s\n", cJSON_GetObjectItemCaseSensitive(rec, "content")->valuestring);
                memset(str1, 0, sizeof(str1));
            }
            if(x2 == 3){
                sprintf(buffer, "logout %s\n", token);
                make_socket();
                connect_socket();
                send(client_socket, buffer, sizeof(buffer), 0);
                memset(buffer, 0, sizeof(buffer));
                recv(client_socket, buffer, sizeof(buffer), 0);
                closesocket(client_socket);
                cJSON * rec =cJSON_Parse(buffer);
                if(strcmp(cJSON_GetObjectItemCaseSensitive(rec,"type")->valuestring,"Error") != 0){
                    printf("User %s successfully logged out\n", account_name);
                    flag--;
                }
                else
                    printf("%s\n", cJSON_GetObjectItemCaseSensitive(rec, "content")->valuestring);
            }
        }
        if(flag == 3){
            printf("1 : Send Message\n2 : Refresh\n3 : Channel Members\n4 : Leave Channel\n");
            scanf("%d", &x3);
            if(x3 == 1){
                getchar();
                gets(str1);
                memset(buffer, 0, sizeof(buffer));
                sprintf(buffer, "send %s, %s\n", str1, token);
                memset(str1, 0, sizeof(str1));
                make_socket();
                connect_socket();
                send(client_socket, buffer, sizeof(buffer), 0);
                memset(buffer, 0, sizeof(buffer));
                recv(client_socket, buffer, sizeof(buffer), 0);
                closesocket(client_socket);
            }
            if(x3 == 2){
                sprintf(buffer, "refresh %s\n", token);
                make_socket();
                connect_socket();
                send(client_socket, buffer, sizeof(buffer), 0);
                memset(buffer, 0, sizeof(buffer));
                recv(client_socket, buffer, sizeof(buffer), 0);
                closesocket(client_socket);
                cJSON * rec = cJSON_Parse(buffer);
                cJSON * mainarray = cJSON_GetObjectItemCaseSensitive(rec,"content");
                for(int i = 0; i < cJSON_GetArraySize(mainarray); i++){
                    cJSON * ara = cJSON_GetArrayItem(mainarray,i);
                    printf("%s : %s\n",cJSON_GetObjectItemCaseSensitive(ara,"sender")->valuestring,cJSON_GetObjectItemCaseSensitive(ara,"content")->valuestring);
                }
            }
            if(x3 == 3){
                sprintf(buffer, "channel members %s\n", token);
                printf("Channel members : ");
                make_socket();
                connect_socket();
                send(client_socket, buffer, sizeof(buffer), 0);
                memset(buffer, 0, sizeof(buffer));
                recv(client_socket, buffer, sizeof(buffer), 0);
                closesocket(client_socket);
                cJSON * req = cJSON_Parse(buffer);
                cJSON * array = cJSON_GetObjectItemCaseSensitive(req,"content");
                for(int i = 0; i < cJSON_GetArraySize(array); i++)
                    printf("%s ", cJSON_GetArrayItem(array, i)->valuestring);
                printf("\n");
            }
            if(x3 == 4){
                sprintf(buffer, "leave %s\n", token);
                make_socket();
                connect_socket();
                send(client_socket, buffer, sizeof(buffer), 0);
                memset(buffer, 0, sizeof(buffer));
                recv(client_socket, buffer, sizeof(buffer), 0);
                closesocket(client_socket);
                printf("You left form %s\n", channel_name);
                flag--;
            }
        }
            if ((strcmp(buffer, "exit")) == 0){
                printf("Client stopping...\n");
                return;
            }
    }
}
int main()
{

	WORD wVersionRequested;
    WSADATA wsaData;
    int err;
    wVersionRequested = MAKEWORD(2, 2);
    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) {
        printf("WSAStartup failed with error: %d\n", err);
        return 1;
    }
	client_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (client_socket == -1) {
		printf("Socket creation failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully created..\n");
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(PORT);
	if (connect(client_socket, (SA*)&servaddr, sizeof(servaddr)) != 0) {
		printf("Connection to the server failed...\n");
		exit(0);
	}
	else
		printf("Successfully connected to the server..\n");
	chat();
}
