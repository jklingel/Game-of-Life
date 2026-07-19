// 1dlife - 1D Life (Game of Life)
// A Cellular Automaton for ANSI Terminals
// Idea by John Conway, 1978
// Coding by Jan Klingel, 07/2026

// "A new Kind of Science" by Stephen Wolfram, chapter 2, section 1, page 24
// This celluar automaton #254 produces a simple triangle from a single
// starting point.
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

  clearScr;
  setColor;

  // Seed the random number generator with current time
  srand((unsigned int) time(NULL));

  // Seed the current generation
  for(int i=0;i<MAXX;i++) {
    x[i] = DEAD;
  }
  x[MAXX/2] = ALIVE;

  for(int i=1;i<MAXX-1;i++)
    putchar(x[i]);

  unsigned int n = 0;
  while(n < LOOPS) {
    for(int i=1;i<MAXX-1;i++) {

      // Rule 1 - Cell stays alive if it or one of its direct neighbors
      // was alive
      if(x[i] == ALIVE || x[i+1] == ALIVE || x[i-1] == ALIVE)
        y[i] = ALIVE;
      // Rule 2 - Otherwise cell is dead
      else
        y[i] = DEAD;
    }

    printf("\n");
    for(int i=1;i<MAXX-1;i++) {
      putchar(y[i]);
      x[i] = y[i];
    }
    n++;
    sleep(0); // Set to 1 if pattern should slowly appear
  }
  printf("\n");
  soundBell; // Program is done

  return(0);
}
