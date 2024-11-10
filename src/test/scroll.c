/* scroll.c
 * functions to place scrolling text on screen */

#include <curses.h>
#include <string.h>

#include "scroll.h"

struct scroll_text create_text(WINDOW *win, char *message, int y) {
    struct scroll_text ret;
    int h, w;
    getmaxyx(win, h, w);

    ret.width = w - 2;
    ret.message = message;
    ret.direction = -1; /* starts at border; will flip when update is called */
    ret.i = 0;
    ret.n = strnlen(message, 128);
    ret.y = y;

    return ret;
}

int on_border(WINDOW *win, struct scroll_text e) {
    return e.i == 0 || e.i == e.n-e.width;
}

void scroll_update(WINDOW *win, struct scroll_text *e) {

    /* move text in direction if not on border */
    mvwaddnstr(win, e->y, 1, e->message + e->i, e->width);
    e->i += e->direction;
    if (on_border(win,*e))
        e->direction *= -1;
}
/* usage: in main:
 * check if text is on border beforehand
 * DO NOT CALL if so, until all necessary text is on border.
 */

void paste_scroll_text(WINDOW *win, int y, int x, char *message) {
    int direction = -1;
    int h, w, n, i;
    int width, height;

    getmaxyx(win, h, w);
    width = w - 2;
    height = h - 2;
    n = strlen(message);

    i = 0;
    while (1) {

    mvwaddnstr(win, height/2, 1, message + i, width);

    wrefresh(win);
    nodelay(win, TRUE);

    delay_output(250);

    i += direction;
    if (i == 0 || i == n-width) {
        direction *= -1;
        delay_output(2000);
    }
    }
}

