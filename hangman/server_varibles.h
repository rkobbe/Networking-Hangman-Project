//Server_Variables.h 
//Reilly Kobbe Mike Blanchard & Ashlyn Hanks
//CSC-3460
//December 16th 
//This file contains all of server.c varible declarations 
#include"common.h"
   
struct hostent  *gethostbyname();
char serverHostName[HOSTNAMELEN];
struct hostent *host;

int i, j, maxi, maxfd, listenfd, connfd, sockfd;
int nready;
struct client_type client[FD_SETSIZE];
int client_sockID;
int client_readfd;
size_t n;
fd_set rset, allset;
socklen_t cli_len;
struct sockaddr_in cli_addr;
struct sockaddr_in serv_addr;
char buff[MAXLINE];
int childpid;
int port;
