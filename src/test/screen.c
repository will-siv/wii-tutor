/* screen.c */

#include <curses.h>
#include <stdlib.h>
#include <unistd.h>

#define ROWS 24
#define COLS 84

int main() {

    WINDOW *main, *left, *center, *right;

    initscr();
    noecho();

    main = newwin(ROWS, COLS, 0,0);
    left = subwin(main, ROWS / 3, COLS / 3 - 1, (ROWS * 2)/3 - 1, 1);
    center = subwin(main, ROWS / 3, COLS / 3, (ROWS * 2)/3 - 1, (COLS * 1) / 3);
    right = subwin(main, ROWS / 3, COLS / 3 - 1, (ROWS * 2)/3 - 1, (COLS * 2) / 3);
    do {
        box(left, 0,0);
        box(center, 0,0);
        box(right, 0,0);
        box(main, 0,0);
        wrefresh(main);
    } while (1);
    endwin();

    exit(EXIT_SUCCESS);

}
