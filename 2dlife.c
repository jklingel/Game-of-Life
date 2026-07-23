// 2D Life (The Game of Life) for ANSI Terminals
// Idea by John Conway, 1978
// Coding by Jan Klingel, 07/2026

// See https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life

#include <stdio.h>
#include <stdlib.h> // rand(), srand()
#include <time.h>   // time()
#include <unistd.h> // sleep()

#define MAXX 120 // Maximum x value on screen
#define MAXY 36 // Maximum y value on screen
#define LOOP 50 // Number of iterations
#define ALIVE '*'
#define DEAD ' '
#define cursorOff printf("%c[?25l", 27)
#define cursorOn printf("%c[?25h", 27)
#define clearScr printf("%c[2J", 27)
#define soundBell putchar('\a')
#define setColor putchar(27); printf("[1;32;40m") // green on black

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

int main(void) {

  // Byte array to hold a value for each cell on the screen
  char screenX[MAXX][MAXY]; // Current generation
  char screenY[MAXX][MAXY]; // Next generation

  unsigned int x, y; // Coordinates on the screen

  clearScr;
  cursorOff;
  setColor;

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
    printf("\nNo alive start cells!\n");
    cursorOn;
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

    if(alive == 0) {
      printf("\nNo more alive cells!\n");
      cursorOn;
      return(0);
    }
  } while(n<LOOP);
  printf("\n");
  cursorOn;
  return(0);
}
