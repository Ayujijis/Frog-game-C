#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>


//80x24 screen size

#define TOPOFBOARD (1)
#define NOFCONFG (7)
#define MAXLINE  (20)

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


typedef struct Config {
    int BOARDX;
    int BOARDY;
    int LATX;
    int NCARS;
    int NROCKS;
    int ROCKPERROW;
    int ROCKROWS;
} Config;

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

void readconfig (Config *config){

    FILE *file;

  
    file = fopen("config.txt", "r");
    
  if (file == NULL) {
        printf("Unable to open the file.\n");
       
        
    }
int numbersummer[3]={0,0,0};
char linebuffer[MAXLINE];

for(int i=0; i<NOFCONFG;i++){
    int counter=0;
  
    fgets(linebuffer, MAXLINE, file);

        for (int j = 0; j <2; j++)
        {
            
            if(linebuffer[j+1]==' '&&counter==0){

                numbersummer[j]= (int)linebuffer[j]-48;
                numbersummer[2]=1;//third value is a flag 1 for single digit, 0 for two digits
                break;       
            }

            else
                numbersummer[j]= (int)linebuffer[j]-48;
                counter++;
                
        
        }

    switch (i)
    {
    case 0: config->BOARDX=summer(numbersummer); break;
    case 1: config->BOARDY=summer(numbersummer);break;
    case 2: config->LATX=summer(numbersummer);break;
    case 3: config->NCARS=summer(numbersummer);break;
    case 4: config->NROCKS=summer(numbersummer);break;
    case 5: config->ROCKPERROW=summer(numbersummer);break;
    case 6: config->ROCKROWS=summer(numbersummer);break;
    default:
        break;
    }
    numbersummer[0]=0;
    numbersummer[1]=0;
    numbersummer[2]=0;

}
fclose(file);
}



void startboard(Config config, char **board) {
    for (int i = 0; i < config.BOARDY; i++) {
        for (int j = 0; j < config.BOARDX; j++) {
            board[i][j] = ' ';
        }
    }
    // Set borders and initial frog position
    for (int i = 0; i < config.BOARDX; i++) {
        board[0][i] = 'O';
        board[config.BOARDY - 1][i] = 'O';
    }
    for (int i = 0; i < config.BOARDY; i++) {
        board[i][0] = 'O';
        board[i][config.BOARDX - 1] = 'O';
    }
    board[config.BOARDY - 2][config.BOARDX / 2] = 'M';
}

void render(Config config, char **board){
//Función de imprimir el tablero
clear();
int boardstartx=(LINES/2)-(config.BOARDY/2);
int boardstarty=(COLS/2)-(config.BOARDX/2);
move(boardstartx,boardstarty);

for (int i = 0; i < config.BOARDY; i++)
{
    for (int j = 0; j < config.BOARDX; j++)
    {
        
            mvaddch(boardstartx+i, boardstarty+j, board[i][j]);

            
            
           
    }

    
}
mvaddstr(LINES-1,0,"    MGDC 201319");

refresh();

}




void rockcheck (int frog[2], char **board, int old_x, int old_y){

if (board[frog[1]][frog[0]] == 'O' ) {
    frog[0] = old_x;  
    frog[1] = old_y;
}

}

void input (Config config, int frog[2], char **board){

int old_x = frog[0], old_y = frog[1];

char ch=getch();
    switch (ch) {
        case 'w': frog[1]--; break;
        case 'a': frog[0] -= config.LATX; break;
        case 's': frog[1]++; break;
        case 'd': frog[0] += config.LATX; break;
        case 'e':  exit(0);
    }
rockcheck(frog, board, old_x, old_y);

}

void frogmove (Config config, int frog[2], char **board){

board[frog[1]][frog[0]]=' ';
input(config,frog, board);
board[frog[1]][frog[0]]='M';


}

int wincheck (Config config, int frog[2]){

if (frog[1]==(TOPOFBOARD)){

return 1;
}

else return 0;
}

void cargenerate(Config config, car *cars, rock *rocks) {
    for (int i = 0; i < config.NCARS; i++) {
        if (((rand() % 18) == 1) && (cars[i].alive == 0)) {
            int random = (rand() % (config.BOARDY - 4)) + 2;  // Generate random row
            int valid_row = 1;  // Assume the row is valid initially

            // Check against all rocks
            for (int j = 0; j < config.NROCKS; j++) {
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


void cardelete (Config config, car *cars){

for(int i=0; i< config.NCARS; i++){
if (cars[i].x==0|| cars[i].x==(config.BOARDX-1))
{
    cars[i].alive=0;
    cars[i].x=0;
    cars[i].y=0;
}
}
}

void carmove(Config config, car *cars, char **board){

for (int i = 0; i < config.NCARS; i++)
{
    if (cars[i].alive == 1) {
    board[cars[i].y][cars[i].x] = ' ';  // Clear old car's position
    cars[i].x++;
    if (cars[i].x < config.BOARDX - 1) {
        board[cars[i].y][cars[i].x] = 'X';  
    } else {
        cars[i].alive = 0;  
    }
}
}
}


void carzero(Config config, car *cars){

for (int i = 0; i < config.NCARS; i++)
{
        cars[i].alive=0;
        cars[i].type=0;
        cars[i].x=0;
        cars[i].y=0;
}


}

void collisioncheck(Config config,int frog[2], car *cars){

     for(int i=0; i<config.NCARS; i++){

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


void rockgenerator (Config config, rock *rocks, char **board){

    int rockcounter[config.NROCKS];
    for (int i=0; i<config.ROCKPERROW;i++){
    rockcounter[i]=0;
    }


    for (int i = 0; i < config.NROCKS; i++)
    {
        
        rocks[i].rocky=rand()%config.ROCKPERROW+4;
        rocks[i].nrocks=0;
        for (int j = 0; j < config.ROCKPERROW; j++)
        {

            board[rocks[i].rocky][rand()%(config.BOARDX-1)+1]='O';
        }
        
    }

    

        
    }
 



int main () {

 Config config;
  
    // Read configurations from the file
    readconfig(&config);

    // Allocate memory for the board
    char **board = malloc(config.BOARDY * sizeof(char *));
    for (int i = 0; i < config.BOARDY; i++) {
        board[i] = malloc(config.BOARDX * sizeof(char));
    }
//screen setup
initscr();
noecho();
curs_set(0);
srand(time(NULL));
cbreak();
timeout(0);

//variables principales

 int frog[2];
    frog[0] = config.BOARDX / 2;
    frog[1] = config.BOARDY - 2;

    car cars[config.NCARS];
    rock rocks[config.NROCKS];
    carzero(config,cars);
int windcondition= 0;

int gamestate=0; //0 playing, 1 level one, 2 level two, 3 level three, 4 win screen, 5 lose screen 

//iniciar el tablero
startboard(config,board);
rockgenerator(config,rocks, board);
//bucle principal
while (windcondition==0)
{
    
     collisioncheck(config, frog, cars);
        frogmove(config, frog, board);
        windcondition = wincheck(config, frog);
        cargenerate(config, cars, rocks);
        carmove(config,cars, board);
        render(config,board);
        usleep(20000);
}

refresh();




    return 0;
}



