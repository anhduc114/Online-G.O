#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <conio.h>

#define Boardsize 5
char board[Boardsize][Boardsize];
int i=0;
int j=0;
int o_points=0;
int x_points=0;
int turn =1;
int gameover;

//identify current player
int whichPlayer(){
    if( ( turn % 2 ) == 0)    return 2;
    else return 1;

}

//check if the move is valid
int checkValid(int a){
if( a <= Boardsize - 1) return 1;
else return 0;
}

//set up a board to draw onscreen
void setBoard(){
for(i = 0; i< Boardsize; i++){
    for(j = 0; j< Boardsize; j++){
        board[i][j] = '-';
    }
}

}

//kiem tra xem quan co da bi vay hay k
/*
void floodfill(int turn, int col, int row){
    if(turn == 1){
         if(col< 0 || row< 0) return;
         if(board[col][row] == '-' ) return;
         if(board[col][row] == 'o' ){
            board[col][row] = '-';
            floodfill(1, col-1, row);
            floodfill(1, col+1, row);
            floodfill(1, col, row-1);
            floodfill(1, col, row+1);
        }
    }
   else if(turn == 2){
        if(board[col][row] == 'x'){
            board[col][row] = '-';
            floodfill(2, col-1, row);
            floodfill(2, col+1, row);
            floodfill(2, col, row-1);
            floodfill(2, col, row+1);
        }
    }
}



void chessHit(int turn){
    for(i = 0; i< Boardsize; i++){
        for(j = 0; j< Boardsize; j++){
            floodfill(turn, i, j);
        }
        }
}
*/

int checkleft(int m, int n)
{
    int i;
    i = n;
    if (board[m][i-1] == board[m][n])
    {
        i--;
        checkleft(m, i);
    }
    else if ((board[m][i-1] != board[m][n]) && (board[m][i-1] != '-' ))
    {
        return 1;
    }
    else if (board[m][i-1] == '-')
    {
        return 0;
    }
}

int checkright(int m, int n)
{
    int i;
    i = n;
    if (board[m][i+1] == board[m][n])
    {
        i++;
        checkleft(m, i);
    }
    else if ((board[m][i+1] != board[m][n]) && (board[m][i+1] != '-' ))
    {
        return 1;
    }
    else if (board[m][i+1] == '-')
    {
        return 0;
    }
}

int checkup(int m, int n)
{
    int i;
    i = m;
    if (board[i-1][n] == board[m][n])
    {
        i--;
        checkleft(m, i);
    }
    else if ((board[i-1][n] != board[m][n]) && (board[i-1][n] != '-' ))
    {
        return 1;
    }
    else if (board[i-1][n] == '-')
    {
        return 0;
    }
}

int checkdown(int m, int n)
{
    int i;
    i = m;
    if (board[i+1][n] == board[m][n])
    {
        i++;
        checkleft(m, i);
    }
    else if ((board[i+1][n] != board[m][n]) && (board[i+1][n] != '-' ))
    {
        return 1;
    }
    else if (board[i+1][n] == '-')
    {
        return 0;
    }
}

int checkki(int m, int n)
{
    checkup( m, n);
    checkdown( m, n);
    checkleft( m, n);
    checkright( m, n);
    if(checkup( m, n) + checkdown(m,  n) + checkleft( m,  n) + checkright( m,  n) == 4) return 1;
        else return 0;

}


int checkNeighborKi(int m, int n)
{
    //check neighbor up
    if (board[m-1][n] ==  board[m][n]) checkki(m-1,n);
    else if (board[m+1][n] ==  board[m][n]) checkki(m+1,n);
    else if (board[m][n-1] ==  board[m][n]) checkki(m,n-1);
    else if (board[m-1][n] ==  board[m][n]) checkki(m,n+1);




}

//tinh diem cuoi game
void score(){
    int score1 = 0;
    int score2 = 0;
for(i = 0; i< Boardsize; i++){
    for(j = 0; j< Boardsize; j++){
            if (board[i][j] == 'x') score1++;
            if (board[i][j] == 'o') score2++;
    }
}
printf("Player 1 score: %d\n",score1);
printf("Player 2 score: %d\n",score2);
if(score1 > score2) printf("player 1 won");
if(score1 < score2) printf("player 2 won");
if(score1 == score2) printf("game is draw");
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

void play()
{

    int x;
    int y;
    int placeorpass;
    int hold = 1;
    printf("player %d's turn. Place (press 1) or pass (press 2) or quit game(press3)? \n", whichPlayer());
    scanf("%d",&placeorpass);
        if(placeorpass == 2) {whichPlayer() == 2; return;}
        if(placeorpass == 3) {gameover = 1; return;}
    do{

        if(placeorpass == 1) break;
        if(placeorpass != 1 && placeorpass != 2 && placeorpass!= 3) printf("Pls input 1 or 2 or 3 \n"); scanf("%d",&placeorpass);
    } while(hold == 1);

    printf("place your move:\n");

    do {
            printf("x:");
            scanf("%d",&x);
            if(checkValid(x) == 0){printf("nhap sai, moi nhap lai:\n"); } 	  } while(checkValid(x) == 0);

    do {
            printf("y:");
            scanf("%d",&y);
            if(checkValid(y) == 0){printf("nhap sai, moi nhap lai:\n"); } 	  } while(checkValid(y) == 0);

    if(board[x][y] == '-' && whichPlayer()== 1  )
    {
        printf("player %d done\n",whichPlayer());
        board[x][y] = 'x';
    }
    else if(board[x][y] == '-' && whichPlayer()== 2  )
    {
        printf("player %d done\n",whichPlayer());
        board[x][y] = 'o';

    }
    //xu ly bat quan cac thu o day
    //printf("%d \n",checkleft(x, y)+checkright(x, y)+checkup(x, y)+checkdown(x, y));

    //score();


}

int main() {
    setBoard();
    for(;;){

            drawBoard();
            play();
            turn ++;
            if (gameover == 1)  {score(); break;}

    }

return 0;
}
