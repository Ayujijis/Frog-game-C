#include<ncurses.h>
#include<stdlib.h>
#include <unistd.h>


#define CENTERX (COLS/2)
#define CENTERY (LINES/2)
#define NUMROWS (LINES/2)
#define NUMCOLS (COLS/2)
#define LATX    (2)
#define FPS     (16670000)
#define BOARDY  (30)
#define BOARDX  (10)
//ids: frog =1, car=2, etc.
//first y then x 


struct Frog{
    int pos_x;
    int pos_y;
    char symbol;
};

void renderer (){
usleep(17);
refresh();

}


void input (int frog[3]){

char ch;

while ((ch=getchar())!='3'){

    switch (ch)
    {
    case 'w':
    mvdelch(frog[0],frog[1]);
    frog[0]-=1;
    move (frog[0], frog[1]);
    printw("M");
        break;

    case 'a':
    mvdelch(frog[0],frog[1]);
    frog[1]-=LATX;
    move (frog[0], frog[1]);
    printw("M");
        break;

    case 's':
    mvdelch(frog[0],frog[1]);
    frog[0]+=1;
    move (frog[0], frog[1]);
    printw("M");
        break;

    case 'd':
    mvdelch(frog[0],frog[1]);
    frog[1]+=LATX;
    move (frog[0], frog[1]);
    printw("M");
        break;
    
    default:
        break;
            }
renderer();
}
}


int main (){

struct Frog frog;
frog.pos_x=25;
int frog[3]={CENTERY, CENTERX, 'M'};
int car[4];


initscr();
refresh();
noecho();
curs_set(0);
cbreak();
timeout(0);

char board [BOARDY][BOARDX];

    for (int i=0; i<BOARDY; i++){

        for (int j=0; j<BOARDX; j++)
            {

                board[i][j]= '@';

            }

    }


int r= CENTERY-(NUMROWS/2);
int c= CENTERX-(NUMCOLS/2);    

move(LINES/2,0);

 for (int i = 0; i < BOARDX; i++)
 {
    mvaddstr(r+i,c,board[i]);
 }
 



refresh();

frogMovement(frog);
move (LINES-1,0 );
addstr("msg");
getch();
refresh();
endwin();

    return 0;
}