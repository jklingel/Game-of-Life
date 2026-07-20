// 1dlife - 1D Life (Game of Life)
// A Cellular Automaton for ANSI Terminals
// Idea by John Conway, 1978
// Jan Klingel, 07/2026

// "A new Kind of Science" by Stephen Wolfram
// This celluar automaton #20m produces triangle patterns from a row
// of cells with random states.
// https://www.wolframscience.com/nks/p25--how-do-simple-programs-behave/

#include <stdio.h>
#include <stdlib.h> // rand(), srand()
#include <time.h>   // time()
#include <unistd.h> // sleep()

#define MAXX 120
#define LOOPS 50
#define ALIVE 'o'
#define DEAD ' '
#define clearScr putchar(27); printf("[2J")
#define soundBell putchar('\a')
#define setColor putchar(27); printf("[1;32;40m") // green on black

int main(void) {
  char x[MAXX]; // Current generation
  char y[MAXX]; // Next generation
  unsigned int aliveN; // Number of alive neighbors

  clearScr;
  setColor;

  // Seed the random number generator with current time
  srand((unsigned int) time(NULL));

  // Seed the current generation
  for(int i=2;i<MAXX-2;i++) {
    x[i] = rand() % 2; // Generate random 0s and 1s
    if(x[i] == 0)
      x[i] = DEAD; // Dead cell
    else
      x[i] = ALIVE; // Alive cell
    putchar(x[i]);
  }

  unsigned int n = 0;
  while(n < LOOPS) {
    for(int i=2;i<MAXX-2;i++) {
      aliveN = 0;
      if(x[i-2] == ALIVE)
        aliveN++;
      if(x[i-1] == ALIVE)
        aliveN++;
      if(x[i+1] == ALIVE)
        aliveN++;
      if(x[i+2] == ALIVE)
        aliveN++;

      // Rule 1 - Cell stays alive if 1 or 2 live neighbor cells
      if(x[i] == ALIVE && (aliveN == 1 || aliveN == 2))
        y[i] = ALIVE;
      // Rule 2 - Alive cell dies if 3 or 4 alive neighbor cells
      else if(x[i] == ALIVE && (aliveN == 3 || aliveN == 4))
        y[i] = DEAD;
      // Rule 3 - Cell reborn if 2 or 3 alive neighbor cells
      else if(x[i] == DEAD && (aliveN == 2 || aliveN == 3))
        y[i] = ALIVE;
      // Rule 4 - Otherwise cell dies
      else
        y[i] = DEAD;
    }

    printf("\n");
    for(int i=2;i<MAXX-2;i++) {
      putchar(y[i]);
      x[i] = y[i];
    }
    n++;
    sleep(0); // Replace with 1 is pattern should appear line by line
  }
  printf("\n");

  return(0);
}
