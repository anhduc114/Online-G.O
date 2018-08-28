#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>
 
 
pthread_t thread;
int gamestart =0; 
char accepted[256];
char mes[256];
char checkmes[256];
char buffer2[256];
char to2[100],from2[100];
int p,q;
char checkmes2[256];
//xu li ve game
#define Boardsize 9
char board[Boardsize][Boardsize];
int i=0;
int j=0;
int o_points=0;
int x_points=0;
int turn =1;
int gameover;
int colx,rowy;
int move;
char col[256],row[256];
//set up a board to draw onscreen
void setBoard(){
for(i = 0; i< Boardsize; i++){
    for(j = 0; j< Boardsize; j++){
        board[i][j] = '-';
    }
}

}


//draw the go board on screen
void drawBoard(){
int count = 0;
for(i = 0; i< Boardsize; i++){
    for(j = 0; j< Boardsize; j++){
        count++;
        if( ( count % Boardsize ) == 0)
        {
            printf("%c",board[i][j]);
            printf("\n");
        }
        else printf("%c",board[i][j]);
    }
}
}





//xu li ve server 
void error (char *msg){
  perror(msg);
  exit(0);
}
 
 
void* receive(void* d){
  int sockfd=*((int *)d);
  int a;
  char buf[256];
 
while(1){

  bzero(buf,256);
  a=recv(sockfd,buf,255,0);
  if(a<=0) error("Error reading from socket");
  else
  printf("\n %s\n ",buf);
}
close(sockfd);
pthread_exit(NULL);
}
 
   
int main (int argc, char *argv[]) {
  int sockfd, portno, n,m,newsockfd,userno;
  char user[100],pass[30];
  struct sockaddr_in addr_in, serv_addr;
  struct hostent *server;
  char buffer[256];
  FILE *file;
   
  if (argc<3) {
  fprintf (stderr,"Usage %s Host Name Port\n", argv[0]);
  exit(0);
   }
   portno=atoi(argv[2]);
   sockfd=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
 
 
  if (sockfd<0)
   error("ERROR Opening Socket");
    
   server=gethostbyname(argv[1]);
    
   if (server==NULL) {
    fprintf(stderr, "ERROR, No Such Host\n");
    exit(0);
    }
     
    bzero((char*)&serv_addr, sizeof(serv_addr));
     
    serv_addr.sin_family=AF_INET;
    bcopy((char*)server->h_addr, (char*)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port=htons(portno);
    newsockfd = connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr));
    if (newsockfd<0)
    error("ERROR Connecting");
     
 
else
{
reg:
bzero(user,100);
username:
printf("\nENTER THE USERNAME  : ");
fgets(user,100,stdin);
if(strncmp(user,"user-",5)!=0)
{
printf("USERNAME FORMAT NOT CORRECT  (FORMAT - user-<no>)");
goto username;
}
n=send(sockfd, user, strlen(user), 0);
printf("\nENTER THE PASSWORD  : ");
fgets(pass,30,stdin);
n=send(sockfd, pass, strlen(pass), 0);
bzero(buffer,256);
n=recv(sockfd,buffer,255,0);
if(n>0)
printf("%s\n",buffer);
if(strcmp(buffer,"USERNAME EXISTS")==0)
goto reg;
}
file=fopen("user.txt","a+");
char str[100];
bzero(str,100);
printf("\nLOGGED IN USERS\n");
while(fscanf(file,"%s",str)!=EOF)
{
if(str[0]=='u')
printf("\n%s\n",str);
fscanf(file,"%s",str);
}
if(strcmp(buffer,"USER REGISTERED")==0)
{
fprintf(file,"%s",user);
fprintf(file,"\n%s\n",pass);}
fclose(file);
 
//invite: 
pthread_create(&thread,NULL,receive,(void*)&sockfd);
invite: 

while(gamestart==0)
{
      
enter:
//      n=recv(sockfd,buffer,255,0);
// printf("tu server %s\n",buffer );

     //printf("%s\n",user );
    

    printf("\nPlease Enter The Message: ");
     
    bzero(buffer, 256);
    fgets(buffer, 255, stdin);
    
if((strncmp(buffer,"list",4)==0))
{
  file=fopen("user.txt","a+");
char str[100];
bzero(str,100);
printf("\nLOGGED IN USERS\n");
  while(fscanf(file,"%s",str)!=EOF)
{
if(str[0]=='u')
printf("\n%s\n",str);
fscanf(file,"%s",str);
}
fclose(file);

}
else if((strncmp(buffer,"To user-",8)==0)||(strncmp(buffer,"exit",4)==0)||(strncmp(buffer,"newgame",7)==0))
{
sprintf(buffer,"%s|%s",buffer,user);
n=send(sockfd, buffer, strlen(buffer), 0);
bzero(accepted,256);
// n=recv(sockfd,accepted,255,0);
// if(strstr(accepted,"accepted") != NULL)
// {
//   gamestart=1;
//   break;
// }      
    if(n<0)
    error("ERROR Writing To Socket");
   if(strncmp(buffer,"exit",4)==0)
  break;   
     
     if(strncmp(buffer,"newgame",7)==0)
  {
  gamestart=1;    
  break;  
  }

}
else if((strncmp(buffer,"To user-",8)==0)&&(strstr(buffer,"accept") != NULL))
{
    //printf("nhan duoc chua\n");
    gamestart = 1;
      break;

    sprintf(buffer,"%s|%s",buffer,user);
    n=send(sockfd, buffer, strlen(buffer), 0);
  
  //bat dau game
  //printf("%s",user);

}
else
{
printf("\nMESSAGE FORMAT NOT CORRECT\n");
goto enter;}
 
bzero(accepted,256);

     }


//pthread_create(&thread,NULL,receive,(void*)&sockfd);
while(gamestart==1)
{
      enter2:
       //printf("\nGame Started\n");
       //setBoard();
       //drawBoard();
       //printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
       //printf("\nPlease enter your move: ");

       bzero(buffer, 256);
       fgets(buffer, 255, stdin);
      

      p=3;
bzero(buffer2,256);
sprintf(buffer2,"%s|%s",buffer,user);

strcpy(checkmes,buffer2);
while(checkmes[p]!=':')
{
to2[p-3]=checkmes[p];
p++;
}
to2[p-3]='\0';
q=0;
bzero(buffer2,256);
while(checkmes[p]!='|')
{
buffer2[q]=checkmes[p];
p++;
q++;
}
buffer2[q]='\0';
q=0;
for(p+=1;checkmes[p]!='\0';p++)
{
from2[q]=checkmes[p];
q++;
}
from2[q-1]='\0';
strcpy(mes,buffer2);
strncpy(col, mes+1, 1);
 strncpy(row, mes+2, 1);
 strncpy(checkmes2,mes+1,4);
 move = atoi(checkmes2);
 colx = atoi(col);
 rowy = atoi(row);
 if ((strncmp(buffer,"To user-",8)==0)&&((colx>=Boardsize)||(rowy>=Boardsize)||(move > 99)))
 {
  printf("Invalid move.Please input another:\n");
  goto enter2;
 }

  //printf("%d\n%d\n%d",colx,rowy,move);
  //printf("%s\n",mes );

 else if((strncmp(buffer,"To user-",8)==0)||(strncmp(buffer,"quitgame",8)==0))
{
      sprintf(buffer,"%s|%s",buffer,user);
      n=send(sockfd, buffer, strlen(buffer), 0);

      if(strncmp(buffer,"quitgame",8)==0)
      {
        gamestart = 0;
        n=send(sockfd, buffer, strlen(buffer), 0);

        break;
      }
}
 else if((strncmp(buffer,"To user-",8)==0)&&(strstr(buffer,"surrender")!=NULL))
{
      //sprintf(buffer,"%s|%s",buffer,user);
      n=send(sockfd, buffer, strlen(buffer), 0);

        gamestart =0;
        break;      
}


      else
{
      printf("\nMESSAGE FORMAT NOT CORRECT2\n");
      goto enter2;
}

      if(n<0)
      error("ERROR Writing To Socket");
}  
    if(gamestart == 0) goto invite;  
    close(sockfd); 
    
   return 0;
    
  }
   
