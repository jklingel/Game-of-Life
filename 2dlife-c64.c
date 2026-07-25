/* 2D Life (The Game of Life) for ANSI Terminals */
/* Idea by John Conway, 1978                     */
/* Coding by Jan Klingel, 07/2026                */
/* Version for Commodore 64 & 128 in POWER C     */

#include <stdio.h>
#include <poke.h>   /* poke(), peek()            */

#define MIN 0xa1 /* Jiffy minutes */
#define SEC 0xa2 /* Jiffy seconds */
#define SCRMEM 1024 /* Start of screen memory    */
#define MAXX 40 /* Maximum x value on screen     */
#define MAXY 24 /* Maximum y value on screen     */
#define SIZEH 10 /* Depth of population history  */
#define ALIVE 42 /* Asterix                      */
#define DEAD 32  /* Space                        */
#define SMALL 14 /* upper-/lowercase             */
#define clearScr() putchar(147)

/* Define registers for gotoxy()      */
#define SCREEN 0xffed
#define PLOT   0xfff0

/* testArray() - Test if all elements */
/* in array are the same              */
int testArray(array, size)
int *array;
int size; 
{
  unsigned int n;
  for(n=1;n<size;n++) {
    if(array[n] != array[0])
      return(1);
  }
  return(0);
}

/* gotoxy() - Place cursor at screen  */
/* position x,y. The first position   */
/* is 0,0!                            */
int gotoxy(x,y)
char x,y;
{
  char a, xMax, yMax;
  sys (SCREEN, &a, &xMax, &yMax);
  if (x <= xMax && y <= yMax) 
    sys (PLOT, &a, &y, &x);
  else
    return(1);
  return(0);
}

int main(void) {

  /* Byte array to hold a value for each cell on the screen */
  char screenX[MAXX][MAXY]; /* Current generation */
  char screenY[MAXX][MAXY]; /* Next generation */

  unsigned int x, y; /* Coordinates on the screen */
  poke (char,53280,0); /* Black background */
  poke (char,53281,0); /* Black border     */
  poke (char,646,5); /* Green text         */
  
  unsigned int seed;
  seed = peek(char,MIN) * peek(char,SEC);
  /* Seed the random number generator */
  srandom(seed);

  clearScr();
  putchar(SMALL);

  /* Set the array to all cells dead */
  for(y=0;y<=MAXX;y++) {
     for(x=0;x<=MAXY;x++) {
       screenX[x][y] = DEAD;
       screenY[x][y] = DEAD;
     }
  }

  unsigned int n = 1; /* Generation */
  unsigned int alive = 0; /* Number of alive cells */
  unsigned int aliveS = 0; /* Number of cells to start with */
  unsigned int aliveN = 0; /* Alive neighbors */
  unsigned int aliveH[SIZEH+1];

  /* Storage for population history */
  aliveH[SIZEH+1] = EOF;
  unsigned int aliveP = 0;

  /* Seed initial group of cells (25) with random values */
  for(y=MAXY/2-2;y<=MAXY/2+2;y++) {
    for(x=MAXX/2-2;x<=MAXX/2+2;x++) {
      screenX[x][y] = random() % 2; /* Generate random 0s and 1s */
      if(screenX[x][y] == 0) {
        screenX[x][y] = DEAD;
        poke(char, SCRMEM+x-1+(MAXX*(y-1)), DEAD);  
      }
      else {
        screenX[x][y] = ALIVE;
        poke(char, SCRMEM+x-1+(MAXX*(y-1)), ALIVE);
        aliveS++;
      }
    }
  }

  if(aliveS == 0) {
    printf("\nNo alive start cells!\n");
    return(0);
  }
  else {
    alive = aliveS;
    gotoxy(0,MAXY);
    printf("Gen %d Population %d Start %d   ", n, alive, aliveS);
  }

  do {
    for(y=1;y<MAXY-1;y++) {
      for(x=1;x<MAXX-1;x++) {

        /* Count neighbors */
        aliveN = 0;
        if(screenX[x][y-1] == ALIVE) /* North */
          aliveN++;
        if(screenX[x+1][y-1] == ALIVE) /* Northeast */
          aliveN++;
        if(screenX[x+1][y] == ALIVE) /* East */
          aliveN++;
        if(screenX[x+1][y+1] == ALIVE) /*Southeast */
          aliveN++;
        if(screenX[x][y+1] == ALIVE) /* South */
          aliveN++;
        if(screenX[x-1][y+1] == ALIVE) /* Southwest */
          aliveN++;
        if(screenX[x-1][y] == ALIVE) /* West */
          aliveN++;
        if(screenX[x-1][y-1] == ALIVE) /* Northwest */
          aliveN++;

        if(screenX[x][y] == DEAD && aliveN == 0)
          ;
        else { 
          /* Apply rule book */
          if(screenX[x][y] == ALIVE && aliveN < 2)
            screenY[x][y] = DEAD;
          else if(screenX[x][y] == ALIVE && (aliveN == 2  aliveN == 3))
            screenY[x][y] = ALIVE;
          else if(screenX[x][y] == ALIVE && aliveN > 3)
            screenY[x][y] = DEAD;
          else if(screenX[x][y] == DEAD && aliveN == 3)
            screenY[x][y] = ALIVE;
          else
            screenY[x][y] = DEAD;
        }
      }
    }

    /* Draw new screen with next generation */
    alive = 0;
    for(y=1;y<MAXY-1;y++) {
      for(x=1;x<MAXX-1;x++) {
        if(screenY[x][y] == ALIVE)
          alive++;
        if(screenX[x][y] != screenY[x][y]) {
          poke(char, SCRMEM+x-1+(MAXX*(y-1)), screenY[x][y]);
          screenX[x][y] = screenY[x][y];
        }
      }
    }
    n++;
    gotoxy(0,MAXY);
    printf("Gen %d Population %d Start %d   ", n, alive, aliveS);


    /* Track history of the last SIZEH   */
    /* population. If there is no change,*/
    /* we assume the cells are dead.     */
    if(aliveP == SIZEH)
      aliveP = 0;
    aliveH[aliveP] = alive;
    aliveP++;
    if(!testArray(aliveH, SIZEH))
      alive = 0;

    if(alive == 0) {
      printf("\nNo more alive cells!\n");
      return(0);
    }
  } while(1);
  return(0);
}
