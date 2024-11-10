#ifndef SCROLL_H
#define SCROLL_H

#include <curses.h>
struct scroll_text {
    char *message;
    int n;
    int i;
    int y;
    int direction;
    int width;
};

/* text is always bound to window: to create text you must also define
 * the window it is a part of
 */
struct scroll_text create_text(WINDOW *win, char *message, int y);
int on_border(WINDOW *win, struct scroll_text e);
void scroll_update(WINDOW *win, struct scroll_text *e);
void paste_scroll_text(WINDOW *win, int y, int x, char *message);

#endif /* SCROLL_H */
