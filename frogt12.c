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
#define SPEED1   (20)
#define USLEEP      (5000)


typedef struct car {
    int alive;
    int x;
    int y;
    int type;
    int speedmodifier;
    int speedcheck;
    int timesmoved;
    
} car;

typedef struct rock
{
    int rocky;
    int nrocks;
    
} rock;


typedef struct Config {
    int BOARDX;
    int BOARDY;
    int LATX;
    int NCARS;
    int NROCKS;
    int ROCKPERROW;
    int ROCKROWS;
} Config;
typedef struct stork{

    int x;
    int y;
    int movecheck;
    char previouschar;
    int safeguard; //prevents replication from save
     
}stork;



void storkzero(stork *thestork, Config config){

    thestork->movecheck=0;
    thestork->previouschar=' ';
    thestork->x = config.BOARDX - 2;  
    thestork->y = 2;     
    thestork->safeguard=0;
}

void savegame(const char *filename, int frog[2], stork *thestork, rock *rocks, int num_rocks, char **board, Config config, car *cars, int num_cars, int score, int level) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        mvprintw(LINES - 1, 0, "Error: Could not save the game!");
        
        refresh();
        sleep(1);
        return;
    }

    // Save level and score
    fprintf(file, "LEVEL %d\n", level);
    fprintf(file, "SCORE %d\n", score);

    // Save frog position
    fprintf(file, "FROG %d %d\n", frog[0], frog[1]);

   
   
    // Save cars
    fprintf(file, "CARS %d\n", num_cars);
    for (int i = 0; i < num_cars; i++) {
        fprintf(file, "CAR %d  %d %d %d %d %d %d\n",
                cars[i].alive, cars[i].x, cars[i].y, cars[i].type,
                 cars[i].speedmodifier, cars[i].speedcheck, cars[i].timesmoved);
    }

    // Save board
    fprintf(file, "BOARD\n");
    for (int i = 0; i < config.BOARDY; i++) {
        for (int j = 0; j < config.BOARDX; j++) {
            fprintf(file, "%c", board[i][j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
    
    refresh();
}


int loadgame(const char *filename, int *level, int frog[2], stork *thestork, rock *rocks, int num_rocks, char **board, Config config, car *cars, int num_cars, int *score) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        mvprintw(LINES - 1, 0, "Error: Could not load the game!");
        
        refresh();
        sleep(1);
        return 0; // Load failed
    }

    // Load level and score
    fscanf(file, "LEVEL %d\n", level);
    fscanf(file, "SCORE %d\n", score);

    int levellocal=*level;

    // Load frog position
    fscanf(file, "FROG %d %d\n", &frog[0], &frog[1]);

   
    

    // Load cars
    int loaded_cars;
    fscanf(file, "CARS %d\n", &loaded_cars);
    for (int i = 0; i < loaded_cars; i++) {
        fscanf(file, "CAR %d %d %d  %d %d %d %d\n",
               &cars[i].alive, &cars[i].x, &cars[i].y, &cars[i].type,
                &cars[i].speedmodifier, &cars[i].speedcheck, &cars[i].timesmoved);
    }

    // Load board
    char line[config.BOARDX + 2]; // Allow for newline character
    fscanf(file, "BOARD\n");
    for (int i = 0; i < config.BOARDY; i++) {
        fgets(line, sizeof(line), file);
        for (int j = 0; j < config.BOARDX; j++) {
            board[i][j] = line[j];
        }
    }
    
    
    storkzero(thestork, config);
    sleep(1);
    thestork->safeguard=1;
    


    fclose(file);
    for (int  j = 0; j < config.BOARDY; j++)
{
    for (int k = 0; k < config.BOARDX; k++)
    {
        if(board[j][k]=='S'){
            board[j][k]=' ';
        }
    }
    
}
    refresh();
    return 1; // Load succeeded
}







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



}



void printstatus(int level, time_t start_time, int total_score) {
    move(LINES - 1, 0);
    clrtoeol();

    time_t current_time = time(NULL);
    int elapsed_time = (int)(current_time - start_time);
    int level_score = 90 - elapsed_time;

    if (level_score < 0) {
        level_score = 0;  // Prevent negative scores
    }

    mvprintw(LINES - 1, 0, "%d | Time: %d | Score: %d | Total Score: %d    Matias G. Dávila Chio 201319", 
             level + 1, 90 - elapsed_time, level_score, total_score + level_score);
}

void colorchooser(char **board, int i, int j, int boardstartx, int boardstarty){

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
            case 'S':
            attron(COLOR_PAIR(7));
            mvaddch(boardstartx+i, boardstarty+j, board[i][j]);
            attroff(COLOR_PAIR(7));
            break;
        
        default:
            break;
        }

}

void render(Config config, char **board, int level, time_t start_time, int total_score) {
    // Clear and render the game board
    clear();
    int boardstartx = (LINES / 2) - (config.BOARDY / 2);
    int boardstarty = (COLS / 2) - (config.BOARDX / 2);
    move(boardstartx, boardstarty);
    for (int i = 1; i < config.BOARDX-1; i++)
{
    attron(COLOR_PAIR(4));
    mvaddch(boardstartx+1,boardstarty+i,' ');
    attroff(COLOR_PAIR(4));
}

    for (int i = 0; i < config.BOARDY; i++) {
        for (int j = 0; j < config.BOARDX; j++) {
            colorchooser(board, i, j, boardstartx, boardstarty);
        }
    }

    // Print the current level and score status
    printstatus(level, start_time, total_score);

    refresh();
}






void rockcheck (Config config, int frog[2], char **board, int oldx, int oldy){

if (board[frog[1]][frog[0]] == 'O' ) {
    frog[0] = oldx;  
    frog[1] = oldy;
}

}


//if there is a friendly car(cyan), if you press x you'll be taken to the top (only if it is safe)
void carhop(int frog[2], char **board){

    if(board[frog[1]][frog[0]-1]=='K'&&board[TOPOFBOARD+1][frog[0]]==' '){
        board[TOPOFBOARD+1][frog[0]-1]='K';
        
        frog[1]=TOPOFBOARD+1;
    }

}

void input(Config config, int frog[2], stork *thestork, rock *rocks, int num_rocks, char **board, car *cars, int num_cars, int *score, int *level) {
    int oldx = frog[0], oldy = frog[1];
    char ch = getch();

    switch (ch) {
        case 'w': frog[1]--; break;
        case 'a': frog[0] -= config.LATX; break;
        case 's': frog[1]++; break;
        case 'd': frog[0] += config.LATX; break;
        case 'e': exit(0); break;
        case 'x': carhop(frog, board); break;
        case 'r': // Save game
            savegame("savefile.txt", frog, thestork, rocks, num_rocks, board, config, cars, num_cars, *score, *level);
            break;
        case 'l': // Load game
            if (loadgame("savefile.txt", level, frog, thestork, rocks, num_rocks, board, config, cars, num_cars, score)) {
                
            }
            break;
    }

    rockcheck(config, frog, board, oldx, oldy);
}



void frogmove(Config config, int frog[2], stork *thestork, rock *rocks, int num_rocks, char **board, car *cars, int num_cars, int *score, int *level) {
    int oldx = frog[0], oldy = frog[1]; // Save old position
    board[frog[1]][frog[0]] = ' ';     
    input(config, frog, thestork, rocks, num_rocks, board, cars, num_cars, score, level);

    // Restore the old position if the frog lands on a rock
    if (board[frog[1]][frog[0]] == 'O') {
        frog[0] = oldx;
        frog[1] = oldy;
    }

    board[frog[1]][frog[0]] = 'M'; 
}



void storkmove(Config config, char **board, int frog[2], stork *thestork) {
    // Check if the stork is ready to move
    if (thestork->movecheck < 20) {
        thestork->movecheck += 2;
        return;
    }

    // Restore the previous character where the stork was
    board[thestork->y][thestork->x] = thestork->previouschar;

    // Move the stork towards the frog
    if (thestork->x < frog[0]) {
        thestork->x++;  // Move right
    } else if (thestork->x > frog[0]) {
        thestork->x--;  // Move left
    }

    if (thestork->y < frog[1]) {
        thestork->y++;  // Move down
    } else if (thestork->y > frog[1]) {
        thestork->y--;  // Move up
    }

    // Check for collision with the frog
    if (thestork->x == frog[0] && thestork->y == frog[1]) {
        endwin();  // Exit ncurses mode
        printf("Game Over! The stork caught the frog!\n");
        exit(0);  // End the game
    }

    // Update the stork's previous character
    thestork->previouschar = board[thestork->y][thestork->x];

    // Place the stork at the new position
    board[thestork->y][thestork->x] = 'S';

    // Reset move delay
    thestork->movecheck = 0;
}





void cargenerate(Config config, car *cars, char **board) {
    for (int i = 0; i < config.NCARS; i++) {
        if (((rand() % 18) == 1) && (cars[i].alive == 0)) {
            int random_row = (rand() % (config.BOARDY - 4)) + 2;  // Generate a random row within the valid range
            int valid_row = 1;  // Assume the row is valid initially

            // Validate the row: Check if there are any rocks or other obstacles
            for (int col = 1; col < config.BOARDX - 1; col++) {
                if (board[random_row][col] == 'O') {  // If there's a rock in the row
                    valid_row = 0;
                    break;
                }
            }

            // Place the car if the row is valid
            if (valid_row) {
                cars[i].y = random_row;
                cars[i].alive = 1;
                cars[i].type = rand() % 3 + 1; // Randomize the car type (1, 2, or 3)
                cars[i].x = 2; // Start at the left side
                cars[i].speedmodifier = rand() % 6 + 5; // Assign a random speed modifier
                // Update the board to reflect the car
                switch (cars[i].type) {
                    case 1: board[random_row][cars[i].x] = 'X'; break;//Enemy
                    case 2: board[random_row][cars[i].x] = 'J'; break;//Fast
                    case 3: board[random_row][cars[i].x] = 'K'; break;//Frienly
                }
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
        cars[i].speedmodifier=0;
        cars[i].timesmoved=0;
        cars[i].speedcheck=0;
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
timeout(SPEED1);
start_color();  
init_pair(1, COLOR_WHITE, COLOR_WHITE);  // Text then background
init_pair(2, COLOR_RED, COLOR_RED); 
init_pair(3, COLOR_BLACK, COLOR_GREEN);
init_pair(4, COLOR_YELLOW, COLOR_YELLOW);
init_pair(5, COLOR_MAGENTA, COLOR_MAGENTA);
init_pair(6, COLOR_CYAN, COLOR_CYAN);
init_pair(7,COLOR_WHITE, COLOR_RED);
}




int wincheck(Config config, char **board, int *frog) {
    if (frog[1] == TOPOFBOARD) {
        return 1;  // Win
    }
    return 0;  // Continue playing
}




void levelone(Config config, char **board, rock *rocks, int *frog, time_t start_time, car *cars, stork *thestork, int *total_score, int *level) {

    if(thestork->safeguard==0){
    startboard(config, board);
    frog[0] = config.BOARDX / 2;
    frog[1] = config.BOARDY - 2;
    carzero(config, cars);
    }
    
    int exit = 0;

    while (!exit) {

        if(*level!=0){
            if (*level==2)
            {
                *level=1;
                break;
            }
        
            *level=0;
            break;
        }
        collisioncheck(config, frog, cars);
        frogmove(config, frog, thestork, rocks, config.NROCKS, board, cars, config.NCARS, total_score, level);
        
        exit = wincheck(config, board, frog);
        cargenerate(config, cars, board);
        carmove(config, cars, board);
        render(config, board, *level, start_time, *total_score);  // Pass total score
        usleep(USLEEP);
    }

    // Update total score after the level
    time_t current_time = time(NULL);
    int elapsed_time = (int)(current_time - start_time);
    int level_score = 90 - elapsed_time;

    if (level_score < 0) {
        level_score = 0; // Prevent negative scores
    }

    *total_score += level_score;
}

void leveltwo(Config config, char **board, rock *rocks, int *frog, time_t start_time, car *cars, stork *thestork, int *total_score, int *level) {
    if(thestork->safeguard==0){
    startboard(config, board);
    rockgenerator(config, rocks, board);
    frog[0] = config.BOARDX / 2;
    frog[1] = config.BOARDY - 2;
    
    carzero(config, cars);
    }
    int exit = 0;

    while (!exit) {

        if(*level!=1){
            if (*level==2)
            {
                *level=1;
                break;
            }
            
            *level=0;
            break;
        }
        collisioncheck(config, frog, cars);
        frogmove(config, frog, thestork, rocks, config.NROCKS, board, cars, config.NCARS, total_score, level);
        exit = wincheck(config, board, frog);
        cargenerate(config, cars, board);
        carmove(config, cars, board);
        render(config, board, *level, start_time, *total_score);
        usleep(USLEEP);
    }

    // Update total score after the level
    time_t current_time = time(NULL);
    int elapsed_time = (int)(current_time - start_time);
    int level_score = 90 - elapsed_time;

    if (level_score < 0) {
        level_score = 0; // Prevent negative scores
    }

    *total_score += level_score;
}

void levelthree(Config config, char **board, rock *rocks, int *frog, time_t start_time, car *cars, stork *thestork, int *total_score, int *level) {
    
    if(thestork->safeguard==0){
    startboard(config, board);
    rockgenerator(config, rocks, board);
    frog[0] = config.BOARDX / 2;
    frog[1] = config.BOARDY - 2;
    carzero(config, cars);
    storkzero(thestork, config);
    }
    int exit = 0;
    

     


    while (!exit) {

        
            if(*level!=2){
                if (*level==1)
            {
                *level=0;
                break;
            }
                *level-=2;
            break;
        }
        
   
        storkmove(config, board, frog, thestork);
        
        

        collisioncheck(config, frog, cars);
        frogmove(config, frog, thestork, rocks, config.NROCKS, board, cars, config.NCARS, total_score, level);
        exit = wincheck(config, board, frog);
        cargenerate(config, cars, board);
        carmove(config, cars, board);
        
        render(config, board, *level, start_time, *total_score);
        usleep(USLEEP);
    }

    // Update total score after the level
    time_t current_time = time(NULL);
    int elapsed_time = (int)(current_time - start_time);
    int level_score = 90 - elapsed_time;

    if (level_score < 0) {
        level_score = 0; // Prevent negative scores
    }

    *total_score += level_score;
}




//current count for main 685 characters
int main() {
    Config config;
    readconfig(&config);

    // Screen setup
    screensetup();

    // Main variables
    int frog[2];  // x, y
    char **board = malloc(config.BOARDY * sizeof(char *));
    for (int i = 0; i < config.BOARDY; i++) {
        board[i] = malloc(config.BOARDX * sizeof(char));
    }

    car cars[config.NCARS];
    rock rocks[config.NROCKS];
    stork thestork[1];
    carzero(config, cars);
    storkzero(thestork, config);
    int total_score = 0;
    time_t start_time = time(NULL);

    int level = 0;  
    

    while (level < 3) {  // Loop through levels or resume from save
        
           
           
        

        switch (level) {
            case 0:
                levelone(config, board, rocks, frog, start_time, cars, thestork, &total_score, &level);
                break;
            case 1:
                leveltwo(config, board, rocks, frog, start_time, cars, thestork, &total_score, &level);
                break;
            case 2:
                levelthree(config, board, rocks, frog, start_time, cars, thestork, &total_score, &level);
                break;
            default:
                break;
        }

       
        level++;
        start_time = time(NULL);  // Reset timer for the next level
    }

    
    clear();
    mvprintw(LINES / 2, (COLS / 2) - 10, "Game Complete!");
    mvprintw((LINES / 2) + 1, (COLS / 2) - 10, "Final Score: %d", total_score);
    refresh();
    sleep(2);

    // Cleanup
    for (int i = 0; i < config.BOARDY; i++) {
        free(board[i]);
    }
    free(board);

    endwin();
    return 0;
}