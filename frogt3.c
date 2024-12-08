#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>


//80x24 screen size
#define BOARDX  (60)
#define BOARDY  (20)
#define LATX    (1)
#define NCARS   (7)
#define TOPOFBOARD (1)
#define GAMESPEED     (100)
#define NROCKS  (4)
#define ROCKPERROW (16)
#define ROCKROWS (BOARDY/2)

typedef struct car {
    int alive;
    int x;
    int y;
    int type;
    int collision;

} car;

typedef struct rock
{
    int rocky;
    int nrocks;
    
} rock;

typedef struct Symbols
{
    char frog;
    char rock;
    char stork;
    char car;
    char log;
}symbols;



int summer(int numbersummer[3]){

    int sum=0;

    if(numbersummer[2]==1){

        sum=numbersummer[0];
    }
    else{
    for (int i = 0; i < 2; i++)
    {
        if (i==1)
        {
            sum+=numbersummer[i];
        }
        if (i==0)
        {
            if(numbersummer[i]==0){
                sum+=10;
            }
            else
            sum+=(numbersummer[i]*10);
        }

        
    }
    }
    return sum;

}

void startboard(char board[BOARDY][BOARDX] ){


//initialize board
for (int i = 0; i < BOARDY; i++)
{
    for (int j = 0; j < BOARDX; j++)
    {
         board[i][j]=' ';
    }
    
}    
//Superior Border
for (int i=0; i<BOARDX; i++)
{
    board[0][i]='O';
}
//Inferior Border
for (int i=0; i<BOARDX; i++)
{
    board[BOARDY-1][i]='O';
}

//Left border
for (int i=0; i<BOARDY; i++)
{
    board[i][0]='O';
}
//Right border
for (int i=0; i<BOARDY; i++)
{
    board[i][BOARDX-1]='O';
}

board[BOARDY-2][BOARDX/2]='M';

}

void render(char board[BOARDY][BOARDX]){
//Función de imprimir el tablero
clear();
int boardstartx=(LINES/2)-(BOARDY/2);
int boardstarty=(COLS/2)-(BOARDX/2);
move(boardstartx,boardstarty);

for (int i = 0; i < BOARDY; i++)
{
    for (int j = 0; j < BOARDX; j++)
    {
        
            mvaddch(boardstartx+i, boardstarty+j, board[i][j]);

            
            
           
    }

    
}
mvaddstr(LINES-1,0,"    MGDC 201319");

refresh();

}




void rockcheck (int frog[2], char board[BOARDY][BOARDX], int old_x, int old_y){

if (board[frog[1]][frog[0]] == 'O' ) {
    frog[0] = old_x;  
    frog[1] = old_y;
}

}

void input (int frog[2], char board[BOARDY][BOARDX]){

int old_x = frog[0], old_y = frog[1];

char ch=getch();
    switch (ch) {
        case 'w': frog[1]--; break;
        case 'a': frog[0] -= LATX; break;
        case 's': frog[1]++; break;
        case 'd': frog[0] += LATX; break;
        case 'e':  exit(0);
    }
rockcheck(frog, board, old_x, old_y);

}

void frogmove (int frog[2], char board[BOARDY][BOARDX]){

board[frog[1]][frog[0]]=' ';
input(frog, board);
board[frog[1]][frog[0]]='M';


}

int wincheck (int frog[2]){

if (frog[1]==(TOPOFBOARD)){

return 1;
}

else return 0;
}

void cargenerate(car cars[NCARS], rock rocks[NROCKS]) {
    for (int i = 0; i < NCARS; i++) {
        if (((rand() % 18) == 1) && (cars[i].alive == 0)) {
            int random = (rand() % (BOARDY - 4)) + 2;  // Generate random row
            int valid_row = 1;  // Assume the row is valid initially

            // Check against all rocks
            for (int j = 0; j < NROCKS; j++) {
                if (random == rocks[j].rocky) {
                    valid_row = 0;  // Mark row as invalid if it matches a rock
                    break;          // No need to check further
                }
            }

            // Place car if the row is valid
            if (valid_row) {
                cars[i].y = random;
                cars[i].alive = 1;
                cars[i].type = 1;
                cars[i].x = 2;
            }
        }
    }
}


void cardelete (car cars[NCARS]){

for(int i=0; i< NCARS; i++){
if (cars[i].x==0|| cars[i].x==(BOARDX-1))
{
    cars[i].alive=0;
    cars[i].x=0;
    cars[i].y=0;
}
}
}

void carmove(car cars[NCARS], char board[BOARDY][BOARDX]){

for (int i = 0; i < NCARS; i++)
{
    if (cars[i].alive == 1) {
    board[cars[i].y][cars[i].x] = ' ';  // Clear old car's position
    cars[i].x++;
    if (cars[i].x < BOARDX - 1) {
        board[cars[i].y][cars[i].x] = 'X';  
    } else {
        cars[i].alive = 0;  
    }
}
}
}


void carzero(car cars[NCARS]){

for (int i = 0; i < NCARS; i++)
{
        cars[i].alive=0;
        cars[i].type=0;
        cars[i].x=0;
        cars[i].y=0;
}


}

void collisioncheck(int frog[2], car cars[NCARS]){

     for(int i=0; i<NCARS; i++){

        if(cars[i].x==frog[0]&&cars[i].y==frog[1]){
            exit(0);
        }
    }

}


void statecheck (int gamestate){

    switch (gamestate)
    {
   
    case 1:
    
        break;
    
    case 2:
    
        break;
    
    case 3:
    
        break;
    case 4:
        
        break;
    
    default:
        break;
    }

}


void rockgenerator (rock rocks[NROCKS], char board[BOARDY][BOARDX]){

    int rockcounter[ROCKPERROW];
    for (int i=0; i<ROCKPERROW;i++){
    rockcounter[i]=0;
    }


    for (int i = 0; i < NROCKS; i++)
    {
        
        rocks[i].rocky=rand()%ROCKROWS+4;
        rocks[i].nrocks=0;
        for (int j = 0; j < ROCKPERROW; j++)
        {

            board[rocks[i].rocky][rand()%(BOARDX-1)+1]='O';
        }
        
    }

    

        
    }
 



int main () {

//screen setup
initscr();
noecho();
curs_set(0);
srand(time(NULL));
cbreak();
timeout(0);

//variables principales

int frog[2]; //x,y
frog[0]=(BOARDX/2);
frog[1]=(BOARDY-2);

char board[BOARDY][BOARDX];
int windcondition= 0;
car cars[NCARS];
carzero(cars);
rock rocks[NROCKS];
int gamestate=0; //0 playing, 1 level one, 2 level two, 3 level three, 4 win screen, 5 lose screen 

//iniciar el tablero
startboard(board);
rockgenerator(rocks, board);
//bucle principal
while (windcondition==0)
{
    
    collisioncheck(frog, cars);
    frogmove(frog, board);
    windcondition=wincheck(frog);
    cargenerate(cars, rocks);
    carmove(cars, board);
    render(board);
    
   usleep(22000);
}

refresh();




    return 0;
}



