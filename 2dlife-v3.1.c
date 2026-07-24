// 2D Life (The Game of Life) for ANSI Terminals
// Idea by John Conway, 1978
// Coding by Jan Klingel, 07/2026
// Version 3.1 (as of 7/24/2026)
// New: Using a pointer aliveP instead of a counter
// New: Added help function

// See https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life

#include <stdio.h>
#include <stdlib.h> // rand(), srand()
#include <string.h> // strcmp()
#include <time.h>   // time()
#include <unistd.h> // sleep()

#define MAXX 120 // Maximum x value on screen
#define MAXY 36 // Maximum y value on screen
#define LOOP 50 // Number of iterations
#define SIZEH 10 // Depth of population history
#define ALIVE '*'
#define DEAD ' '
#define clearScr() printf("%c[2J", 27)
#define cursorOff() printf("%c[?25l", 27)
#define cursorOn() printf("%c[?25h", 27)
#define soundBell() putchar('\a')
#define setColor() putchar(27); printf("[1;32;40m") // green on black
#define restoreScreen() printf("%c[?47l", 27)

// Rule 1 - Underpopulation: Live cell with less than 2 neighbors dies
// Rule 2 - Survival: Live cell with 2 or 3 neighbors lives
// Rule 3 - Overpopulation: Live cell with more than 3 neighbors dies
// Rule 4 - Reproduction: Dead cell with exactly 3 neighbors becomes alive

// gotoxy() - Position the cursor at x,y
int gotoxy(unsigned int x, unsigned int y) {
  if(x > 0 && y > 0)
    printf("%c[%d;%df", 27, y, x);
  return(0);
}

// testArray() - Test if all elements in array are the same
int testArray(int *array, int size) {
  for(int n=1;n<size;n++) {
    if(array[n] != array[0])
      return(1);
  }
  return(0);
}

int help(void) {
  printf("Usage: 2dlife [--help]\n\n");
  printf("2dlife is a version of Conway’s Game of Life, a cellular \n");
  printf("automaton devised by the British mathematician John Horton \n");
  printf("Conway in 1970. 2dlife was written for ANSI terminals by \n");
  printf("Jan Klingel (info @ janklingel.de) in July of 2026.\n\n");
  printf("The simulation shows an initial cell colony in a 5x5 matrix, \n");
  printf("with a random population between 1 and 25 cells. Based on \n");
  printf("Conway's rule book, the cells survive, divide, and die, forming \n");
  printf("intricate patterns, some chaotic, some structured, some even \n");
  printf("symmetrical.\n\n");
  printf("The game runs until there are no more alive cells. If the \n");
  printf("population does not change for 10 generations, it is assumed \n");
  printf("that the cells are dead.\n");
  return(0);
}

int main(argc,argv)
int argc;
char *argv[];
{

  // Check for arguments
  if(argc != 1) {
    if(strcmp(argv[1],"--help") == 0) {
      help();
      return(0);
    }
    else {
      printf("Usage: 2dlife [--help]\n");
      return(0);
    }
  }

  // Byte array to hold a value for each cell on the screen
  char screenX[MAXX][MAXY]; // Current generation
  char screenY[MAXX][MAXY]; // Next generation

  unsigned int x, y; // Coordinates on the screen

  // Try to set the width of the screen
  printf("%c[=%dh", 27, MAXX);
  clearScr();
  cursorOff();
  setColor();

  // Seed the random number generator with current time
  srand((unsigned int) time(NULL));

  // Set the arrays to all cells dead
  for(y=0;y<=MAXX;y++) {
     for(x=0;x<=MAXY;x++) {
       screenX[x][y] = DEAD;
       screenY[x][y] = DEAD;
     }
  }

  unsigned int n = 1; // Generation. Counter for LOOP
  unsigned int alive = 0; // Number of alive cells
  unsigned int aliveS = 0; // Number of cells to start with
  unsigned int aliveN = 0; // Alive neighbors
  // Storage for population history and its pointer
  unsigned int aliveH[SIZEH+1] = {1,0}; // Dummy values
  aliveH[SIZEH+1] = EOF; // End of array sign
  unsigned int *aliveP = aliveH; // Pointer to storage

  // Seed initial group of cells (25) with random values
  for(y=MAXY/2-2;y<=MAXY/2+2;y++) {
    for(x=MAXX/2-2;x<=MAXX/2+2;x++) {
      screenX[x][y] = rand() % 2; // Generate random 0s and 1s
      gotoxy(x,y);
      if(screenX[x][y] == 0) {
        screenX[x][y] = DEAD;
        putchar(DEAD);
      }
      else {
        screenX[x][y] = ALIVE;
        putchar(ALIVE);
        aliveS++;
      }
    }
  }

  if(aliveS == 0) {
    soundBell();
    printf("\nNo alive start cells!\n");
    restoreScreen();
    cursorOn();
    return(0);
  }
  else {
    alive = aliveS;
    gotoxy(1,MAXY);
    printf("Generation %d Population %d Start cells %d   ", n, alive, aliveS);
  }

  // Loop
  do {
    sleep(1);
    for(y=1;y<MAXY-1;y++) {
      for(x=1;x<MAXX-1;x++) {

        // Count neighbors of current cell X(x,y)
        aliveN = 0;
        if(screenX[x][y-1] == ALIVE) // North
          aliveN++;
        if(screenX[x+1][y-1] == ALIVE) // Northeast
          aliveN++;
        if(screenX[x+1][y] == ALIVE) // East
          aliveN++;
        if(screenX[x+1][y+1] == ALIVE) //Southeast
          aliveN++;
        if(screenX[x][y+1] == ALIVE) // South
          aliveN++;
        if(screenX[x-1][y+1] == ALIVE) // Southwest
          aliveN++;
        if(screenX[x-1][y] == ALIVE) // West
          aliveN++;
        if(screenX[x-1][y-1] == ALIVE) // Northwest
          aliveN++;

        // Apply rule book to Y(x,y)
        if(screenX[x][y] == ALIVE && aliveN < 2)
          screenY[x][y] = DEAD;
        else if(screenX[x][y] == ALIVE && (aliveN == 2 || aliveN == 3))
          screenY[x][y] = ALIVE;
        else if(screenX[x][y] == ALIVE && aliveN > 3)
          screenY[x][y] = DEAD;
        else if(screenX[x][y] == DEAD && aliveN == 3)
          screenY[x][y] = ALIVE;
        else
          screenY[x][y] = DEAD;
      }
    }

    // Draw new screen with next generation
    n++;
    alive = 0;
    for(y=1;y<MAXY-1;y++) {
      for(x=1;x<MAXX-1;x++) {
        if(screenY[x][y] == ALIVE)
          alive++;
        gotoxy(x,y);
        putchar(screenY[x][y]);
        // Copy the new generation over to the old generation
        screenX[x][y] = screenY[x][y];
      }
    }
    gotoxy(1,MAXY);
    printf("Generation %d Population %d Start cells %d   ", n, alive, aliveS);

    // Track the history of the last SIZEH populations. If there is no
    // change, the cells are dead
    if(*aliveP == EOF)
      aliveP = &aliveH[0];
    *aliveP = alive;
    // gotoxy(60,MAXY);
    // for(int m=0;m<SIZEH;m++) {
    //   printf("%d ", aliveH[m]);
    // }
    aliveP++;
    if(!testArray(aliveH, SIZEH))
      alive = 0;

    if(alive == 0) {
      soundBell();
      printf("\nNo more alive cells!\n");
      restoreScreen();
      cursorOn();
      return(0);
    }
  } while(1);
  printf("\n");
  cursorOn();
  return(0);
}
