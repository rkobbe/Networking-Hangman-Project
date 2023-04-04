//server_talk.c
//Reilly Kobbe Mike Blanchard & Ashlyn Hanks
//CSC-3460
//December 16th 
//This file allows the client to send messages to server, 
//also contains the hangman check for the letter  
//to compile: 
//gcc -c -g server.c
//gcc -c -g server_talk.c 
//gcc -o S server.o sevrer_talk.c
//./S <ip>  <port>
#include"common.h"
#include"server.h"


void str_cli(int sockfd,int cinfd, int coutfd)
{
   char sendline[MAXLINE];
   char recvline[MAXLINE];
   char user[MAXLINE];
   char username[MAXLINE];
   bool done = false;

//   int maxfdp1;
//   int stdineof;
//   int n;
//   fd_set rset;

   char guessWords[][50] = {
   "pink",
   "red",
   "green",
   "blue",
   "purple"
   };

   int numCorrect = 0; 
   int oldCorrect = 0;
   int numLives = 20;
 
   srand(time(NULL));
   int randomIndex = rand()%5;

   int lengthOfWord = strlen(guessWords[randomIndex]);
   int quit = 0;
   int loopIndex = 0; 
   char letterEntered; 

   int letterGuessed[8] = {0,0,0,0,0,0,0,0};

   int maxfdp1;
   int stdineof;
   int n;
   fd_set rset;

   stdineof = 0;
   FD_ZERO(&rset);
   printf("Child ready - sockfd:%d  cinfd:%d coutfd:%d\n",sockfd, cinfd, coutfd);

   sendline[0]='\0'; 
   for(;;) {
      if(stdineof == 0)
         FD_SET(cinfd, &rset);
      FD_SET(sockfd, &rset);
      maxfdp1 = ((cinfd>sockfd)?cinfd:sockfd) + 1;

      select(maxfdp1, &rset, NULL, NULL, NULL);
      sendline[n]='\0';
  
      //Creating the Word and Sending it back to the client
      while(numCorrect < lengthOfWord) {
         printf("The Word: ");
         sendline[n]='\0';
         strcpy(sendline,guessWords[randomIndex]);
         write(sockfd,sendline,lengthOfWord);
         bzero(sendline,sizeof(sendline));
         for (loopIndex = 0; loopIndex < lengthOfWord; loopIndex ++){
              printf("-");
         }
         printf("\n");
         printf("Number Correct So Far: %d \n", numCorrect);
         
         if((n=read(sockfd,recvline,MAXLINE))==0){
            if(stdineof == 1)
               return;
            else{
               printf("Str_cli: server terminated prematurley\n");
               exit(1);    
            }
         }
         recvline[n-1]='\0';
         write(coutfd,recvline,n);
         printf("Letter Guessed: %s\n",recvline);

         if(strncmp(recvline, "quit",4)==4){
            quit = 1;
            exit(1);
         }

         //Setting Letter Enteted to the value of recvline
         //then clearing recvline 
         letterEntered = recvline[0];
         bzero(recvline,sizeof(recvline));

         oldCorrect = numCorrect; 

         for(loopIndex = 0; loopIndex < lengthOfWord; loopIndex++){
            if(letterEntered == guessWords[randomIndex][loopIndex]){
               letterGuessed[loopIndex]=1;
               numCorrect++;
            }
         } 
         
         //Checking if the guess was correct and telling the client
         if(oldCorrect == numCorrect){
            printf("the guess is wrong\n");
               sendline[n] = '\0';
               numLives--;
               strcpy(sendline,"w");
               printf("Guess is wrong sendline printed %s\n",sendline);
               if(numLives == 0){
                  printf("you lost\n");
                  exit(1);
               }           
               write(sockfd, sendline, n);
               printf(".....message %s to client: fd-%d\n",sendline,sockfd);
               bzero(sendline,sizeof(sendline));
        } else {
           sendline[n] = '\0';
           strcpy(sendline,"c");
           write(sockfd,sendline,n);
           printf("...message %s to client: fd-%d\n",sendline,sockfd);
           bzero(sendline,sizeof(sendline));
         }
         if(quit == 1){
            printf("The client wants to quit");
            exit(1);
         } else if (numLives == 0){
            strcpy(sendline,"l");
            exit(1);
         }else if(numCorrect== lengthOfWord)  {
            printf("you win\n");
            exit(1);
      }
      if(strcmp(sendline,"exit")==0)
         return;
      }
   }
}

void str_echo(int sockfd)
{
   ssize_t n;
   char buff[MAXLINE];

   bool done = false;


   while(!done) {
      while(!done && (n = read(sockfd, buff, MAXLINE) > 0)) {
          printf("Server read: %s size:%ld\n",buff,strlen(buff));
          if(strcmp(buff,"exit") == 0)
             done = true;
          else
             write(sockfd,buff,strlen(buff)+1);
      }
      if(n<0 && errno != EINTR) {
         printf("str_echo: read error\n");
         done = true;
      }
   }
}

void sig_child(int signo)
{
   pid_t pid;
   int stat;

   while((pid = waitpid(-1, &stat, WNOHANG))>0)
      printf("Child %d terminated\n",pid);
}
