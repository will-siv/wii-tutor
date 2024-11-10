/* screen.c
 * but no longer testing it
 */

#include <curses.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#include "scroll.h"
#include "csvparse.h"
#include "parsestring.h"

#define ROWS 28
#define COLS 84

static char *title = "YWCC Tutoring: Powered by ACM";

void print_center(WINDOW *win, struct scroll_text e) {
    int center = e.width / 2;
    int half_length = e.n / 2;
    int adj_col = center - half_length;
    mvwprintw(win, e.y, adj_col, e.message);
}

int main() {

    WINDOW *main, *left, *center, *right;
    WINDOW *tutorbox;
    WINDOW *tutor_column[4];

    FILE *fp;
    struct scroll_text temp;
    time_t current_time;
    struct tm *time_info;
    char timeString[8], **tutor_times;
    struct person **tutors;
    size_t tutor_list_len = 0;
    int i;

    fp = fopen("sample.csv", "r");
    if (!fp) {
        return 1;
    }
    tutors = parse_tutors(fp, &tutor_list_len);
    if (!tutors) {
        printf("Tutor allocation failed??\n");
        return 1;
    }
    fclose(fp);
    char **tutor_class_strs = malloc(tutor_list_len * sizeof(char *));
    tutor_times = malloc(tutor_list_len * sizeof(char *));

    for (i = 0; i < tutor_list_len; ++i) {
        int j;
        char *classes = tutor_class_to_string(tutors[i]->classes, tutors[i]->class_cnt);
        /*for (j = 0; j < tutors[i]->class_cnt; ++j) {*/
            /*printf("%s", tutors[i]->classes[j]);*/
            /*if (j != tutors[i]->class_cnt-1)*/
                /*printf(", ");*/
        /*}*/
        char *times = tutor_times_to_string(tutors[i]->times, tutors[i]->time_cnt);
        /*for (j = 0; j < tutors[i]->time_cnt; ++j) {*/
            /*struct time_interval *t = &tutors[i]->times[j];*/
            /*printf("%hd-%hd on day #%d", t->time_1, t->time_2, t->weekday);*/
            /*if (j != tutors[i]->time_cnt-1)*/
                /*printf(", ");*/
        /*}*/
        tutor_class_strs[i] = classes;
        tutor_times[i] = times;
    }
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
        box(tutorbox, 0,0);
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
        temp = create_text(main, title, 1);
        print_center(main, temp);

        {
            struct scroll_text S_tutor_names[8];
            struct scroll_text S_tutor_times[8];
            struct scroll_text S_tutor_classes[8];
    do {
        int ii, tutor_index=0;
        time(&current_time);
        time_info = localtime(&current_time);
        strftime(timeString, 8, "%H:%M:%S", time_info);

        for (ii = 0; ii < 2; ii++) {
            WINDOW *col1, *col2;
            col1 = tutor_column[ii*2];
            col2 = tutor_column[ii*2+1];
            for (int i = 0; i < 4; i++) {
                S_tutor_names[tutor_index] = create_text(col1, tutors[tutor_index]->name, 3*i+2);
                S_tutor_times[tutor_index] = create_text(col2, tutor_times[tutor_index], 3*i+2);
                S_tutor_classes[tutor_index] = create_text(col2, tutor_class_strs[tutor_index], 3*i+3);
                tutor_index++;
            }
            for (int i = ii*4; i < ii*4 + 4; i++) {
                scroll_update(col1, &S_tutor_names[i]);
                scroll_update(col2, &S_tutor_times[i]);
                scroll_update(col2, &S_tutor_classes[i]);
            }
        }

        /* TODO: "semaphores", page flipping
         * sub boxes on their own schedule?
         */

        // left sub box: Current tutors
        print_center(left, create_text(left, "Current Tutors:", 2));
        // middle sub box: Current classes
        print_center(center, create_text(center, "Classes Offered Today", 2));
        // right sub box: motd
        print_center(right, create_text(right, "MOTD", 2));

        // time: bottom
        print_center(main, create_text(main, timeString, ROWS-1));
        for (int i = 0; i < 8; i++) {
            touchwin(main);
            wrefresh(tutorbox);
            touchwin(tutorbox);
            wrefresh(tutor_column[i]);
        }
        wrefresh(main);
        delay_output(1000);
    } while (1);
        }
    endwin();

    exit(EXIT_SUCCESS);

}
