/* screen.c
 * but just testing it
 */

#include <curses.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "scroll.h"

#define ROWS 28
#define COLS 84

static char *title = "YWCC Tutoring: Powered by ACM";
char *big = "REALLY REALLY LONG STRING SO LONG YOU SHIT YOUR PANTR";

void print_center(WINDOW *win, int start_row, int cols, char *str, int n) {
    int center = cols / 2;
    int half_length = n / 2;
    int adj_col = center - half_length;
    mvwprintw(win, start_row, adj_col, str);
}

int main() {

    WINDOW *main, *left, *center, *right;
    WINDOW *tutorbox;
    WINDOW *tutor_column[4];

    initscr();
    noecho();

    /* main, boxes at bottom */
    main = newwin(ROWS, COLS, 0,0);
    left = subwin(main, ROWS / 3, COLS / 3 - 1, (ROWS * 2)/3, 1);
    center = subwin(main, ROWS / 3, COLS / 3, (ROWS * 2)/3, (COLS * 1) / 3);
    right = subwin(main, ROWS / 3, COLS / 3 - 1, (ROWS * 2)/3, (COLS * 2) / 3);
    
    tutorbox = subwin(main, 2 * ROWS / 3 - 2, COLS - 2, 2,1);
        box(left, 0,0);
        box(center, 0,0);
        box(right, 0,0);
        box(main, 0,0);
        int t_height, top, bottom;
        int t_cols = 4;
        wgetscrreg(tutorbox, &top, &bottom);
        for (int i = 0; i < t_cols; i++) {
            int col = i * COLS / t_cols - i;
            mvwvline(tutorbox, 1, col, ACS_VLINE, 99);
            mvwaddch(tutorbox, 0, col, ACS_TTEE);
            mvwaddch(tutorbox, bottom, col, ACS_BTEE);
            tutor_column[i] = subwin(tutorbox, 0, COLS/t_cols, 2,col+1);
            if (i == t_cols)
                tutor_column[i] = subwin(tutorbox, 0,  COLS/t_cols, 2, col+1);
        }
        box(tutorbox, 0,0);
        print_center(main, 1, COLS, title, strlen(title));

        struct scroll_text new = create_text(tutor_column[0], big, 3);
    do {
        scroll_update(tutor_column[0], &new);
        touchwin(main);
        wrefresh(tutor_column[0]);
        wrefresh(main);
        delay_output(1000);
    } while (1);
    endwin();

    exit(EXIT_SUCCESS);

}
