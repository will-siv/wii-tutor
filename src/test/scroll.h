#ifndef SCROLL_H
#define SCROLL_H

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

#endif /* SCROLL_H */
