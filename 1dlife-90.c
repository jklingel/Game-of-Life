// 1dlife - 1D Life (Game of Life)
// A Cellular Automaton for ANSI Terminals
// Idea by John Conway, 1978
// Coding by Jan Klingel, 07/2026

// "A new Kind of Science" by Stephen Wolfram, chapter 2, section 1, page 25
// This celluar automaton #90 produces triangles from a single starting
// point.
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
    x[i] = DEAD;
  }
  x[MAXX/2] = ALIVE;

  for(int i=2;i<MAXX-2;i++)
    putchar(x[i]);

  unsigned int n = 0;
  while(n < LOOPS) {
    for(int i=2;i<MAXX-2;i++) {
      aliveN = 0;
      if(x[i-1] == ALIVE)
        aliveN++;
      if(x[i+1] == ALIVE)
        aliveN++;

      // Rule 1 - Cell reborn if exactly 1 live neighbor cell
      if(x[i] == DEAD && aliveN == 1)
        y[i] = ALIVE;
      // Rule 2 - Otherwise cell dies
      else
        y[i] = DEAD;
    }

    printf("\n");
    for(int i=2;i<MAXX-2;i++) {
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
