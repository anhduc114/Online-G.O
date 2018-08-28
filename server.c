#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
 
FILE *fp;
char sender[256];
int gamestart=0;
char move[1000];
char accepted[256];
int count = 0;
char result[256];
int result1,result2;
char failmove[256];
 
//xu li ve game
#define Boardsize 9
char board[Boardsize][Boardsize];
int firstturn=1;
int x=0;
int y=0;
char col[2], row[2];
int colx,rowy;
int o_points=0;
int x_points=0;
int turn =1;
int gameover;
//set up a board to draw onscreen
void setBoard(){
for(x = 0; x< Boardsize; x++){
    for(y = 0; y< Boardsize; y++){
        board[x][y] = '-';
    }
}

}

void checkKi(){
  for(x = 0; x< Boardsize; x++){
    for(y = 0; y< Boardsize; y++){
        if((board[x-1][y]!=board[x][y])&&(board[x-1][y]!='-')&&
          (board[x][y-1]!=board[x][y])&&(board[x-1][y]!='-')&&
          (board[x+1][y]!=board[x][y])&&(board[x+1][y]!='-')&&
          (board[x+1][y]!=board[x][y])&&(board[x+1][y]!='-')&&
          ((x-1>=0)&&(y-1>=0)&&(x+1<Boardsize)&&(y+1<Boardsize))
          )
        board[x][y] = '-';
    }
}
}

//draw the go board on screen
void drawBoard(){
int count = 0;
for(x = 0; x< Boardsize; x++){
    for(y = 0; y< Boardsize; y++){
        count++;
        if( ( count % Boardsize ) == 0)
        {
            printf("-");
            printf("\n");
        }
        else printf("-");
}
}
}
//identify current player
int whichPlayer(){
    if( ( turn % 2 ) == 0)    return 2;
    else return 1;

}

//tinh diem cuoi game
void score(){
    int score1 = 0;
    int score2 = 0;
for(x = 0; x< Boardsize; x++){
    for(y = 0; y< Boardsize; y++){
            if (board[x][y] == 'x') score1++;
            if (board[x][y] == 'o') score2++;
    }
}
printf("Player 1 score: %d\n",score1);
printf("Player 2 score: %d\n",score2);
if(score1 > score2) strcat(result,"player 1 won");
if(score1 < score2) strcat(result,"player 2 won");
if(score1 == score2) strcat(result,"game is draw");

}
// void sendBoard(){
// int count = 0;
// for(x = 0; x< Boardsize; x++){
//     for(y = 0; y< Boardsize; y++){
//         count++;
//         if( ( count % Boardsize ) == 0)
//         {
//             send(newsockfd,buffer,sizeof buffer,0);
//             send(newsockfd,buffer,sizeof buffer,0);
//         }
//         else send(newsockfd,buffer,sizeof buffer,0);
//     }
// }
// }




struct userinfo
{
char username[100];
int socket;
}u[1000];
 
pthread_t thread;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
 
char buffer[256],user[100],str[100],pass[30];
char mes[256];
 struct sockaddr_in serv_addr, cli_addr;
 int n,z,nu=1,i;
 
void error(char *msg) 
 {
 perror(msg); 
 exit(0);
 }
 
 
 
 
void* server(void*);
 
 
int main (int argc, char *argv[]) 
 {
 
fp=fopen("user.txt","w");
fprintf(fp,"server started\n");
fclose(fp);
 
  int i,sockfd, newsockfd[1000], portno, clilen,no=0,n;
  
 
   
 if (argc<2) 
  {
  fprintf (stderr,"ERROR! Provide A Port!\n");
  exit(1);
  }
   
 sockfd = socket(AF_INET, SOCK_STREAM, 0);
   
 if (sockfd<0) 
  error("ERROR! Cannnot Open Socket");
   
 bzero((char *) &serv_addr, sizeof(serv_addr));
   
 portno = atoi(argv[1]);
 serv_addr.sin_family = AF_INET;
 serv_addr.sin_addr.s_addr = INADDR_ANY;
 serv_addr.sin_port = htons(portno);
   
 if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))<0)
  error("ERROR! On Binding");
   
 listen(sockfd, 5);
 
  clilen = sizeof(cli_addr);
 
 
 
while(1)
{
 newsockfd[no] = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
  
 if (newsockfd<0)
  error("ERROR! On Accepting");
 
pthread_mutex_lock(&mutex);
pthread_create(&thread,NULL,server,(void*)&newsockfd[no]);
no+=1;
 
}
   
  
 close(newsockfd[no]); 
  
 close(sockfd);
 return 0;
   
 }
 
void* server(void* sock)
{
int newsockfd=*((int*)sock),j=0,m;
 
 
char to[100],from[100],name[100];
fp=fopen("user.txt","r+");
 
checking:
m=1;
bzero(user,100);
bzero(to,100);
bzero(from,100);
bzero(str,100);
recv(newsockfd, user,100,0);
recv(newsockfd, pass,30,0);
while(fscanf(fp,"%s",str)!=EOF)
{
 
n=strncmp(user,str,strlen(str));
if(n==0)
{
fscanf(fp,"%s",str);
n=strncmp(pass,str,strlen(str));
if(n==0)
{
m=2;
break;
}
else
{
send(newsockfd,"USERNAME EXISTS",15,0);
m=0;
break;}
fscanf(fp,"%s",str);
}
 
}
if(m==0)
goto checking;
if(m==1)
{
fclose(fp);
send(newsockfd,"USER REGISTERED",15,0);
bzero(u[nu].username,100);
u[nu].socket=newsockfd;
strncpy(u[nu].username,user,strlen(user));
nu++;
}
if(m==2)
{
fclose(fp);
send(newsockfd,"USER LOGGED IN",14,0);
for(i=1;i<nu;i++)
if(strcmp(user,u[i].username)==0)
break;
u[i].socket=newsockfd;
}


pthread_mutex_unlock(&mutex);
bzero(buffer, 256);
int newsockfd1;

invite:

//tien trinh xu ly chat cac user
while(gamestart==0)
{ 
 n = recv(newsockfd, buffer, 255, 0);
if(n<0)
  error("ERROR! Reading From Socket");
   
  if(strncmp(buffer,"exit",3)==0)
  {
  close(newsockfd);
  pthread_exit(NULL);
  } 
   
  if(strncmp(buffer,"newgame",7)==0)
  {
    gamestart=1;
  break;
  } 

//xu ly mes, tu buffer lay ra nguoi gui, nguoi nhan, noi dung mes 
i=3;
strcpy(name,buffer);
while(name[i]!=':')
{
to[i-3]=name[i];
i++;
}
to[i-3]='\0';
j=0;
bzero(buffer,256);
while(name[i]!='|')
{
buffer[j]=name[i];
i++;
j++;
}
buffer[j]='\0';
j=0;
for(i+=1;name[i]!='\0';i++)
{
from[j]=name[i];
j++;
}
from[j-1]='\0';
 
printf("To %s  From %s Message %s",to,from,buffer);
strcpy(mes,buffer);
  
for(j=1;j<nu;j++)
{
if((strncmp(u[j].username,to,strlen(to)))==0)
{
newsockfd1=u[j].socket;
break;
}
}

if(strncmp(mes,":invite",7)==0)
{
  //strcat(from,mes);
  bzero(buffer,256);
  strcat(from," invited you. Accept or deny? ");
  strcpy(buffer,from);  
  n=send(newsockfd1,buffer,sizeof buffer,0);

}
if(strncmp(mes,":accept",7)==0)
{
  gamestart = 1;
  strcat(from,mes);
  bzero(buffer,256);
  strcpy(buffer,from);
  bzero(accepted,256);
  //strcat(buffer," accepted.Type 'newgame' to enter as player 1 ");  
  strcat(accepted," Invitation accepted.Type 'newgame' to enter as player 2 (as 'o').Please wait for player 1 to go first ");
  //z=send(newsockfd,accepted,sizeof accepted,0);
  n=send(newsockfd1,accepted,sizeof accepted,0);
  break;
}

strcat(from,buffer);
bzero(buffer,256);
strcpy(buffer,"From ");
strcat(buffer,from); 
  
 n=send(newsockfd1,buffer,sizeof buffer,0);
if(n<0)
{
send(newsockfd, "SENDING FAILED : USER LOGGED OUT",32,0); 
}
else
{
n = send(newsockfd, "Message sent", 18, 0);
   
 if (n<0)
  error("ERROR! Writing To Socket");}
}

//pthread_mutex_unlock(&mutex);
bzero(buffer, 256);




//Tien trinh xu ly game
while(gamestart==1)
{
  if(firstturn ==1)
  {setBoard();
   for(x = 0; x< Boardsize; x++){
    for(y = 0; y< Boardsize; y++){
        count++;
        if( ( count % Boardsize ) == 0)
        {
            strcat(move,"-\n ");
          }
        else 
        {
           strcat(move,"-");
            //send(newsockfd1,"-",1,0);
        }
    }
}
strcat(move,"\n\nYou are player 1(as x).You go first");
firstturn++;
  n=send(newsockfd,move,sizeof move,0);
 bzero(move,sizeof move);
   }
  //printf("%c\n",board[1][1]);
  //nhan nuoc di cua cac player o day
  n = recv(newsockfd, buffer, 255, 0);

if(strncmp(buffer,"quitgame",8)==0)
{

  //setBoard();
  gamestart=0;
  printf("a player has quit the game\n");
  break;
}

  i=3;
strcpy(name,buffer);
while(name[i]!=':')
{
to[i-3]=name[i];
i++;
}
to[i-3]='\0';
j=0;
bzero(buffer,256);
while(name[i]!='|')
{
buffer[j]=name[i];
i++;
j++;
}
buffer[j]='\0';
j=0;
for(i+=1;name[i]!='\0';i++)
{
from[j]=name[i];
j++;
}
from[j-1]='\0';
strcpy(mes,buffer);
strncpy(col, mes+1, 1);
 strncpy(row, mes+2, 1);
 colx = atoi(col);
 rowy = atoi(row);
//printf("day la hangx coty%d\n%d",colx,rowy);


      // if ((board[colx][rowy] != '-')||(colx>= Boardsize)||(rowy>=Boardsize))
      // {
      //   strcat(failmove,"Not valid move!");
      //   n=send(newsockfd,failmove,sizeof buffer,0);

      // } 
   



if(strncmp(mes,":surrender",10)==0)
{
  score();
    strcat(result," \nGamefinish. Type 'quitgame' to quit the game ");
  gamestart==0;

    result1=send(newsockfd,result,sizeof buffer,0);
    result2=send(newsockfd1,result,sizeof buffer,0);

  //strcat(from,mes);
  //bzero(buffer,256);
  //strcpy(buffer,from);  
  //n=send(newsockfd1,buffer,sizeof buffer,0);
  break;
}
  
for(j=1;j<nu;j++)
{
if((strncmp(u[j].username,to,strlen(to)))==0)
{
newsockfd1=u[j].socket;
break;
}
}
strcat(from,buffer);
bzero(buffer,256);
strcpy(buffer,"From ");
strcat(buffer,from); 
  
// n=send(newsockfd1,buffer,sizeof buffer,0);
if(n<0)
{
send(newsockfd, "SENDING FAILED : USER LOGGED OUT",32,0); 
}


//  xu ly nuoc di, ve lai ban co, gui ban co di
if(board[colx][rowy] == '-' && whichPlayer()== 1  )
   {
      //printf("player %d done\n",whichPlayer());
       board[colx][rowy] = 'x';
  }
   else if(board[colx][rowy] == '-' && whichPlayer()== 2  )
   {
        //printf("player %d done\n",whichPlayer());
       board[colx][rowy] = 'o';

   }
  //printf("day la %c",board[5][5]);
checkKi();
for(x = 0; x< Boardsize; x++){
    for(y = 0; y< Boardsize; y++){
        count++;
        if( ( count % Boardsize ) == 0)
        {
             if (board[x][y]=='-')       strcat(move,"-\n ");
             else if (board[x][y]=='o')  strcat(move,"o\n ");
             else if (board[x][y]=='x')  strcat(move,"x\n ");
            //strcat(move,"-\n ");
          }
        else 
        {
            
             if (board[x][y]=='-')       strcat(move,"-");
             else if (board[x][y]=='o')  strcat(move,"o");
             else if (board[x][y]=='x')  strcat(move,"x");
            //strcat(move,"-");
            //send(newsockfd1,"-",1,0);
        }
    }
}
//for(x = 0; x< 5; x++)
//  {strcat(move,"-");}
printf("%d",turn);
z=send(newsockfd,move,sizeof move,0);
strcat(move,"\n\nYour turn. Pls enter your move");
n=send(newsockfd1,move,sizeof move,0);
//sleep(2);

 memset(move, '\0', sizeof move);

turn++  ;
firstturn = 0;


  printf("game start here\n");
if(n<0)
  error("ERROR! Reading From Socket");




  printf("%s\n",buffer);
if(n<0)
  error("ERROR! Reading From Socket");
//else printf("received\n");
}

if(gamestart == 0) goto invite;
close(newsockfd);
pthread_exit(NULL);
}
