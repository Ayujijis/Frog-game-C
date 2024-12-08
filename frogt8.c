#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>


//80x24 screen size

#define TOPOFBOARD (1)
#define GAMESPEED  (100)
#define NOFCONFG (7)
#define MAXLINE  (20)
#define CARTILES (5)


typedef struct car {
    int alive;
    int x;
    int y;
    int type;
    int collision;
    int speedmodifier;
    int speedcheck;
    int timesmoved;

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

void startboard(Config config,char **board ){


//initialize board
for (int i = 0; i < config.BOARDY; i++)
{
    for (int j = 0; j < config.BOARDX; j++)
    {
         board[i][j]=' ';
    }
    
}    
//Superior Border
for (int i=0; i<config.BOARDX; i++)
{
    board[0][i]='O';
}
//Inferior Border
for (int i=0; i<config.BOARDX; i++)
{
    board[config.BOARDY-1][i]='O';
}

//Left border
for (int i=0; i<config.BOARDY; i++)
{
    board[i][0]='O';
}
//Right border
for (int i=0; i<config.BOARDY; i++)
{
    board[i][config.BOARDX-1]='O';
}

board[config.BOARDY-2][config.BOARDX/2]='M';

}



void printstatus(int score){
 
    move(LINES - 1, 0);
    clrtoeol();
    mvprintw(LINES - 1, 0, "MGDC 201319 | Your score is: %d", score);



}

void render(Config config, char **board, int score){
//Función de imprimir el tablero
clear();
int boardstartx=(LINES/2)-(config.BOARDY/2);
int boardstarty=(COLS/2)-(config.BOARDX/2);
move(boardstartx,boardstarty);

for (int i = 1; i < config.BOARDX-1; i++)
{
    attron(COLOR_PAIR(4));
    mvaddch(boardstartx+1,boardstarty+i,' ');
    attroff(COLOR_PAIR(4));
}
for (int i = 0; i < config.BOARDY; i++)
{
    for (int j = 0; j < config.BOARDX; j++)
    {
        
        switch (board[i][j])
        {
        case 'O':
            attron(COLOR_PAIR(1));
            mvaddch(boardstartx+i, boardstarty+j, board[i][j]);
            attroff(COLOR_PAIR(1));
            break;
            case 'M':
            attron(COLOR_PAIR(3));
            mvaddch(boardstartx+i, boardstarty+j, board[i][j]);
            attroff(COLOR_PAIR(3));
            break;
            case 'X':
            attron(COLOR_PAIR(2));
            mvaddch(boardstartx+i, boardstarty+j, board[i][j]);
            attroff(COLOR_PAIR(2));
            break;
            case 'J':
            attron(COLOR_PAIR(5));
            mvaddch(boardstartx+i, boardstarty+j, board[i][j]);
            attroff(COLOR_PAIR(5));
            break;
            case 'K':
            attron(COLOR_PAIR(6));
            mvaddch(boardstartx+i, boardstarty+j, board[i][j]);
            attroff(COLOR_PAIR(6));
            break;
        
        default:
            break;
        }
       
            
           
    }

}

printstatus(score);


refresh();

}




void rockcheck (Config config, int frog[2], char **board, int oldx, int oldy){

if (board[frog[1]][frog[0]] == 'O' ) {
    frog[0] = oldx;  
    frog[1] = oldy;
}

}


//if there is a frindly car(cyan), if you press x you'll be taken to the top (only if it is safe)
void carhop(int frog[2], char **board){

    if(board[frog[1]][frog[0]-1]=='K'&&board[TOPOFBOARD+1][frog[0]]==' '){
        board[TOPOFBOARD+1][frog[0]-1]='K';
        
        frog[1]=TOPOFBOARD+1;
    }

}

void input (Config config, int frog[2], char **board){

int oldx = frog[0], oldy = frog[1];

char ch=getch();
    switch (ch) {
        case 'w': frog[1]--; break;
        case 'a': frog[0] -= config.LATX; break;
        case 's': frog[1]++; break;
        case 'd': frog[0] += config.LATX; break;
        case 'e':  exit(0);
        case 'x': carhop(frog, board); break;
    }
rockcheck(config, frog, board, oldx, oldy);

}

void frogmove (Config config, int frog[2], char **board){

board[frog[1]][frog[0]]=' ';
input(config,frog, board);
board[frog[1]][frog[0]]='M';


}



void cargenerate(Config config, car *cars, rock *rocks) {
    for (int i = 0; i < config.NCARS; i++) {
        if (((rand() % 18) == 1) && (cars[i].alive == 0)) {
            int random = (rand() % (config.BOARDY - 4)) + 2;  // Generate random row
            int valid_row = 1;  // The row is valid

            // Check against all rocks
            for (int j = 0; j < config.NROCKS; j++) {
                if (random == rocks[j].rocky) {
                    valid_row = 0;  // Mark row as invalid if there is a rock
                    break;          
                }
            }

            // Place car if the row is valid
            if (valid_row) {
                cars[i].y = random;
                cars[i].alive = 1;
                cars[i].type = rand()%3+1;
                cars[i].x = 2;
                cars[i].speedmodifier=rand()%6+5;
            }
        }
    }
}


void cardelete (Config config, car *cars){

for(int i=0; i< config.NCARS; i++){
if (cars[i].x==0|| cars[i].x==(config.BOARDX-1))
{
    cars[i].type=0;
    cars[i].alive=0;
    cars[i].x=0;
    cars[i].y=0;
    cars[i].speedmodifier=0;
    cars[i].speedcheck=0;
    cars[i].timesmoved=0;
    }
}
}

void carmove(Config config, car *cars, char **board){

for (int i = 0; i < config.NCARS; i++)
{

    if (cars[i].alive == 1) {
        cars[i].speedcheck+=cars[i].speedmodifier;
        

    }
    if (cars[i].type==3 && board[cars[i].y][cars[i].x+1]=='M'){
        cars[i].speedcheck=0;
            }



    if (cars[i].alive == 1&& cars[i].speedcheck>15) {
    board[cars[i].y][cars[i].x] = ' ';  // Clear old car's position
    cars[i].x++;
    cars[i].speedcheck=0;
     if (cars[i].type==2&& cars[i].timesmoved==CARTILES){

        cars[i].speedmodifier=rand()%12+2;  
        cars[i].timesmoved=0;

    }
    if (cars[i].type==2){

        cars[i].timesmoved+=1;

    }

    if (cars[i].x < config.BOARDX - 1) {
        switch (cars[i].type)
        {
        case 1:
            board[cars[i].y][cars[i].x] = 'X'; 
            break;
        case 2:
            board[cars[i].y][cars[i].x] = 'J'; 
            break;
        case 3:
            board[cars[i].y][cars[i].x] = 'K'; 
            break;
        
        default:
            break;
        }
         
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

void collisioncheck(Config config, int frog[2], car *cars){

     for(int i=0; i<config.NCARS; i++){

        if(cars[i].x==frog[0]&&cars[i].y==frog[1]){
            exit(0);
        }
    }

}




void rockgenerator (Config config, rock *rocks, char **board){

    


    for (int i = 0; i < config.NROCKS; i++)
    {
        
        rocks[i].rocky=rand()%config.ROCKROWS+4;
        rocks[i].nrocks=0;
        for (int j = 0; j < config.ROCKPERROW; j++)
        {

            board[rocks[i].rocky][rand()%(config.BOARDX-1)+1]='O';
        }
        
    }

    

        
    }
 


void screensetup(){


initscr();
noecho();
curs_set(0);
srand(time(NULL));
cbreak();
timeout(30);
start_color();  
init_pair(1, COLOR_WHITE, COLOR_WHITE);  // Text then background
init_pair(2, COLOR_RED, COLOR_RED); 
init_pair(3, COLOR_BLACK, COLOR_GREEN);
init_pair(4, COLOR_YELLOW, COLOR_YELLOW);
init_pair(5, COLOR_MAGENTA, COLOR_MAGENTA);
init_pair(6, COLOR_CYAN, COLOR_CYAN);
}


void levelstart(Config config, char **board, rock *rocks, int *frog, car *cars){

startboard(config, board);
rockgenerator(config, rocks, board);
frog[0]=(config.BOARDX/2);
frog[1]=(config.BOARDY-2);
carzero(config, cars);



}

int wincheck (Config config, char **board, rock *rocks, int *frog, time_t start_time, car *cars){

if (frog[1]==(TOPOFBOARD)){
levelstart(config, board, rocks, frog, cars);
 time_t current_time = time(NULL);
 int elapsed_time = 60-((int)(current_time - start_time));
return elapsed_time;
}

else return 0;
}

//current count for main 685 characters
int main () {


 Config config;
  
    
readconfig(&config);

//screen setup
screensetup();
//variables principales

int frog[2]; //x,y

//board start
char **board = malloc(config.BOARDY * sizeof(char *));
for (int i = 0; i < config.BOARDY; i++) {
    board[i] = malloc(config.BOARDX * sizeof(char));
}
int exit=0;



car cars[config.NCARS];
rock rocks[config.NROCKS];
carzero(config, cars);
int windcondition= 0;
int gamestate=0; //0 playing, 1 level one, 2 level two, 3 level three, 4 win screen, 5 lose screen 
int score=0; //counts 90s - time elapsed so, the less time elapse the bigger the score


levelstart(config, board, rocks, frog, cars);
time_t start_time = time(NULL);
//bucle principal
while (exit==0)
{
    
    collisioncheck(config, frog, cars);
    frogmove(config, frog, board);
    score+=wincheck(config, board, rocks, frog, start_time, cars);
    cargenerate(config, cars, rocks);
    carmove(config, cars, board);
    render(config, board,score);
    
   usleep(5000);
}

refresh();

for (int i = 0; i < config.BOARDY; i++) {
    free(board[i]);
}
free(board);


    return 0;
}



