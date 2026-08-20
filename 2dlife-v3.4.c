// 2D Life (The Game of Life) for ANSI Terminals
// Idea by John Conway, 1978
// Coding by Jan Klingel, 08/2026
// Version 3.4 (as of 8/20/2026)
// New: Added Windows compatible code
// New: Fixed a bug in writing to aliveH
// New: Fixed the mix-up with rows and columns

// See https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life

#include <stdio.h>
#include <stdlib.h> // rand(), srand(), abs()
#include <string.h> // strcmp()
#include <time.h>   // time()

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h> // Sleep() via synchapi.h
#elif defined(__linux__)
#include <unistd.h> // sleep()
#endif

#define VERSION "3.4"
#define MAXC 120 // Maximum x value on screen
#define MAXR 36 // Maximum y value on screen
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
    if (x > 0 && y > 0)
        printf("%c[%d;%df", 27, y, x);
    return(0);
}

// testArray() - Test if all elements in array are the same
int testArray(int* array, int size) {
    for (int n = 1;n < size - 1;n++) {
        if (array[n] != array[0])
            return(1);
    }
    return(0);
}

// help() - Print the usage and a help text in Posix format
int help(void) {
    printf("Usage: 2dlife [--help] [--version]\n\n");
    printf("Optional arguments:\n");
    printf("  --help\t\tdispay this help text and exit\n");
    printf("  --version\t\toutput version information and exit\n\n");
    printf("2dlife is a version of Conway’s Game of Life, a cellular \n");
    printf("automaton devised by the British mathematician John Horton \n");
    printf("Conway in 1970. 2dlife was written for ANSI terminals by \n");
    printf("Jan Klingel (info @ janklingel.de) in August of 2026.\n\n");
    printf("The simulation shows an initial cell colony in a 5x5 matrix, \n");
    printf("with a random population between 1 and 25 cells. Based on \n");
    printf("Conway's rule book, the cells survive, divide, and die, forming \n");
    printf("intricate patterns, some chaotic, some structured, some even \n");
    printf("symmetrical.\n\n");
    printf("The game runs until there are no more alive cells. If the \n");
    printf("population does not change for 10 generations, it is assumed \n");
    printf("that the cells are dead or in a deadlock.\n");
    return(0);
}

// version() - Print the version and license information
int version(void) {
    printf("2dlife %s\n", VERSION);
    printf("License GPLv3+: GNU GPL version 3 or later ");
    printf("<https://gnu.org/licenses/gpl.html>.\n");
    printf("This is free software: you are free to change and ");
    printf("redistribute it.\n");
    printf("There is NO WARRANTY, to the extent permitted by law.\n\n");
    printf("Written by Jan Klingel <info @ janklingel.de>\n");
    return(0);
}

// checkOS() - Check the running operating system
int checkOS(void) {
#if defined(_WIN32) || defined(_WIN64)
    return(1); // Windows
#elif defined(__linux__)
    return(2); // Linux
#else
    return(3); // Unknown OS
#endif
}

int main(argc, argv)
int argc;
char* argv[];
{

#if defined(_WIN32) || defined(_WIN64)
    // The following block is necessary to tell Windows CMD to accept ANSI ESC sequences 
    // Copied from YangXiaoPo-MSFT @ StackOverflow

    // Set output mode to handle virtual terminal sequences
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) {
        return GetLastError();
    }
    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) {
        return GetLastError();
    }
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if (!SetConsoleMode(hOut, dwMode)) {
        return GetLastError();
    }
#endif

    // Check OS
    if (checkOS() != (1 || 2)) {
        printf("This software is written for Windows and Linux only!\n");
        return(1);
    }

    // Check for arguments
    if (argc != 1) {
        if (strcmp(argv[1], "--help") == 0) {
            help();
            return(0);
        }
        else if (strcmp(argv[1], "--version") == 0) {
            version();
            return(0);
        }
        else {
            printf("Usage: 2dlife [--help] [--version]\n");
            return(0);
        }
    }

    // Byte array to hold a value for each cell on the screen
    char screenX[MAXR][MAXC] = { 0 }; // Current generation
    char screenY[MAXR][MAXC]; // Next generation

    unsigned int c, r; // Coordinates on the screen

    // Try to set the width of the screen
    printf("%c[=%dh", 27, MAXC);
    clearScr();
    cursorOff();
    setColor();

    // Seed the random number generator with current time
    srand((unsigned int)time(NULL));

    unsigned int n = 1; // Generation. Counter for LOOP
    unsigned int alive = 0; // Number of alive cells
    unsigned int aliveS = 0; // Number of cells to start with
    unsigned int aliveN = 0; // Alive neighbors

    // Storage for population history and its pointer
    unsigned int aliveH[SIZEH];
    aliveH[SIZEH - 1] = EOF; // End of array sign
    unsigned int* aliveP = aliveH; // Pointer to storage

    // Seed initial group of cells (25) with random values
    for (c = MAXC / 2 - 2;c <= MAXC / 2 + 2;c++) {
        for (r = MAXR / 2 - 2;r <= MAXR / 2 + 2;r++) {
            screenX[r][c] = abs(rand() % 2); // Generate random 0s and 1s
            gotoxy(c, r);
            if (screenX[r][c] == 0) {
                screenX[r][c] = DEAD;
                putchar(DEAD);
            }
            else {
                screenX[r][c] = ALIVE;
                putchar(ALIVE);
                aliveS++;
            }
        }
    }

    if (aliveS == 0) {
        soundBell();
        printf("\nNo alive start cells!\n");
        restoreScreen();
        cursorOn();
        return(0);
    }
    else {
        alive = aliveS;
        gotoxy(1, MAXR);
        printf("Generation %d Population %d Start cells %d   ", n, alive, aliveS);
    }

    // Loop
    do {
#if defined(_WIN32) || defined(_WIN64)
        Sleep(1000);
#elif defined(__linux__)
        sleep(1);
#endif

        for (c = 1;c < MAXC - 1;c++) {
            for (r = 1;r < MAXR - 1;r++) {

                // Count neighbors of current cell X(x,y)
                aliveN = 0;
                if (screenX[r][c - 1] == ALIVE) // North
                    aliveN++;
                if (screenX[r + 1][c - 1] == ALIVE) // Northeast
                    aliveN++;
                if (screenX[r + 1][c] == ALIVE) // East
                    aliveN++;
                if (screenX[r + 1][c + 1] == ALIVE) //Southeast
                    aliveN++;
                if (screenX[r][c + 1] == ALIVE) // South
                    aliveN++;
                if (screenX[r - 1][c + 1] == ALIVE) // Southwest
                    aliveN++;
                if (screenX[r - 1][c] == ALIVE) // West
                    aliveN++;
                if (screenX[r - 1][c - 1] == ALIVE) // Northwest
                    aliveN++;

                // Apply rule book to Y(x,y)
                if (screenX[r][c] == ALIVE && aliveN < 2)
                    screenY[r][c] = DEAD;
                else if (screenX[r][c] == ALIVE && (aliveN == 2 || aliveN == 3))
                    screenY[r][c] = ALIVE;
                else if (screenX[r][c] == ALIVE && aliveN > 3)
                    screenY[r][c] = DEAD;
                else if (screenX[r][c] == DEAD && aliveN == 3)
                    screenY[r][c] = ALIVE;
                else
                    screenY[r][c] = DEAD;
            }
        }

        // Draw new screen with next generation
        n++;
        alive = 0;
        for (c = 1;c < MAXC - 1;c++) {
            for (r = 1;r < MAXR - 1;r++) {
                if (screenY[r][c] == ALIVE)
                    alive++;
                gotoxy(c, r);
                putchar(screenY[r][c]);
                // Copy the new generation over to the old generation
                screenX[r][c] = screenY[r][c];
            }
        }
        gotoxy(1, MAXR);
        printf("Generation %d Population %d Start cells %d   ", n, alive, aliveS);

        // Track the history of the last SIZEH-1 populations. If there is no
        // change, the cells are dead
        if (*aliveP == EOF)
            aliveP = &aliveH[0];
        *aliveP = alive;
        aliveP++;
        if (!testArray(aliveH, SIZEH))
            alive = 0;

        if (alive == 0) {
            soundBell();
            printf("\nNo more alive cells!\n");
            restoreScreen();
            cursorOn();
            return(0);
        }
    } while (1);
    printf("\n");
    cursorOn();
    return(0);
}
