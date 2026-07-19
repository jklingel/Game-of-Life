// 1dlife - 1D Life (Game of Life)
// A Cellular Automaton for ANSI Terminals
// Idea by John Conway, 1978
// Coding by Jan Klingel, 07/2026

// "A new Kind of Science" by Stephen Wolfram, chapter 2, section 1, page 27
// This celluar automaton #30 produces a complex pattern from a single
// starting point.
// https://www.wolframscience.com/nks/p25--how-do-simple-programs-behave/

#include <stdio.h>
#include <stdlib.h> // rand(), srand()
#include <time.h>   // time()
#include <unistd.h> // sleep()

#define MAXX 120
#define LOOPS 45
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

      // Rule 1 - Cell gets state of left-hand neighbor if right-hand
      // neighbor cell dead
      if(x[i] == DEAD && x[i+1] == DEAD)
        y[i] = x[i-1];
      // Rule 2 - Otherwise cell gets the opposite state
      else if(x[i-1] == ALIVE)
        y[i] = DEAD;
      else y[i] = ALIVE;
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
