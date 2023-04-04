//server.h
//Reilly Kobbe Mike Blanchard & Ashlyn Hanks
//CSC-3460
//December 16th 
//This file contains all the declarations 
//to compile: 
//gcc -c -g server.c
//gcc -c -g server_talk.c 
//gcc -o S server.o sevrer_talk.c
//./S <ip>  <port>
void str_echo(int sockfd);
void sig_child(int signc);
void str_cli(int sockfd,int cin, int cout);
void connecting(int argc, char ** argv);
void lis_otherInfor(int listenfd);
//void parent_child(int listedfd);
void num_of_clients(int i);
//void child(int listenfd,int i);
//void parent(int connfd,int i);

struct client_type {
   int cp[2];
   int pc[2];
   int id;
};
