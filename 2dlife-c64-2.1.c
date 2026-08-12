/* 2D Life (The Game of Life)
   Idea by John Conway, 1978
   Coding by Jan Klingel, 07/2026
   Version for Commodore 64 & 128 in
   POWER C
   Version 2.1
*/

#include <stdio.h>
#include <poke.h>

#define MAXC 40
#define MAXR 24 
#define SIZEH 10
#define SCRMEM 1023
#define SCREEN 0xffed
#define PLOT   0xfff0
#define MIN 0xa1 /* Jiffy minutes */
#define SEC 0xa2 /* Jiffy seconds */
#define ALIVE 42
#define DEAD 32
#define SMALL 14 /* upper-/lowercase */
#define clearScr() putchar(147)

/* For randomSID() */
#define VOICE3L 0xd40e
#define VOICE3H 0xd40f
#define VOICE3C 0xd412
#define RAND 0xd41b

/* testArray() - Test if all elements
   in array are the same
*/
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

/* gotoxy() - Place cursor at screen
   position x,y. The first position
   is 0,0!
*/
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

/* randomSID() - generate random number
  between 0 and 255 using the SID
  sound generator 
*/
int randomSID(void) {
  poke(char, VOICE3L, 0xff);
  poke(char, VOICE3H, 0xff);
  poke(char, VOICE3C, 0x80);
  return(peek(char, RAND));
}

int main(void) {

  /* Arrays for current and new gen */
  unsigned int sX[MAXR][MAXC];
  unsigned int sY[MAXR][MAXC];

  unsigned int n = 1; /* Generation */
  unsigned int row, col;

  /* Stored history of aliveP */
  unsigned int aliveH[SIZEH+1];
  aliveH[SIZEH+1] = EOF;
  unsigned int aliveP = 0;

  /* Set the array to all cells dead */
  for(row=0;row<=MAXR;row++) {
     for(col=0;col<=MAXC;col++) {
       sX[row][col] = 0;
     }
  }

  /* Set up screen */
  poke (char,53280,0); /* Black background */
  poke (char,53281,0); /* Black border     */
  poke (char,646,5); /* Green text         */
  clearScr();
  putchar(SMALL);

  unsigned int alive, aliveS, aliveN;
  /* Seed initial group of cells (25) 
     with random values 
  */
  aliveS = 0;
  for(row=MAXR/2-2;row<=MAXR/2+2;row++) {
    for(col=MAXC/2-2;col<=MAXC/2+2;col++) {
      
      if(randomSID() < 127)
        sX[row][col] = 0;
      else { 
        sX[row][col] = 1; 
        poke(char, SCRMEM+col+(MAXC*(row-1)), ALIVE);
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
    gotoxy(0,MAXR);
    printf("Gen %u Population %u Start %u   ", n, alive, aliveS);
  }

  do {
    for(row=1;row<MAXR;row++) {
      for(col=1;col<MAXC;col++) {

        /* Count neighbors */
        aliveN = sX[row][col-1] 
        + sX[row+1][col-1]
        + sX[row+1][col]
        + sX[row+1][col+1]
        + sX[row][col+1]
        + sX[row-1][col+1]
        + sX[row-1][col]
        + sX[row-1][col-1];

        /* Set new generation */
        if(sX[row][col] == 0 && aliveN == 0)
          ;
        else {
        /* Apply rule book */
        if(sX[row][col] == 1) {
          switch(aliveN) {
          case 0:
            sY[row][col] = 0;
            break;
          case 1:
            sY[row][col] = 0;
            break;
          case 2:
            sY[row][col] = 1;
            break;
          case 3:
            sY[row][col] = 1;
            break;
          default:
            sY[row][col] = 0;
            break;
          }
        }
        else {
          switch(aliveN) {
          case 3:
            sY[row][col] = 1;
            break;
          default:
            sY[row][col] = 0;
            break;
          }             
        }
        }
      }
    }

    /* Draw new screen with next generation */
    alive = 0;
    for(row=1;row<MAXR;row++) {
      for(col=1;col<MAXC;col++) {
        if(sY[row][col] == 1)
          alive++;
        if(sX[row][col] != sY[row][col]) {
          switch(sY[row][col]) {
            case 1:
              poke(char, SCRMEM+col+(MAXC*(row-1)), ALIVE);
              break;
            default:
              poke(char, SCRMEM+col+(MAXC*(row-1)), DEAD);
              break;
          }
          sX[row][col] = sY[row][col];
        }
      }
    }

    n++;
    gotoxy(0,MAXR);
    printf("Gen %u Population %u Start %u   ", n, alive, aliveS);

    /* Track history of the last SIZEH
       population. If there is no 
       change, we assume the cells are 
       dead.
    */
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
}
