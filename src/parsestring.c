#include "csvparse.h"
#include "parsestring.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>

char *tutor_class_to_string(char (*classes)[CLASS_STR_LENGTH], size_t class_cnt)
{
    int i;
    /* +2 isn't needed even though there's the comma because
     * CLASS_STR_LENGTH is supposed to include NULLs we don't
     * care about */
    const size_t equation = class_cnt * (CLASS_STR_LENGTH + 1) * sizeof(char);
    char *ret = malloc(equation);
    memset(ret, 0, equation);
    char *ptr = ret;
    for (i = 0; i < class_cnt; ++i) {
        ptr = stpncpy(ptr, classes[i], CLASS_STR_LENGTH);
        if (i < class_cnt - 1) {
            *ptr++ = ',';
            *ptr++ = ' ';
        }

    }
    return ret;
}

char *tutor_times_to_string(struct time_interval *tms, size_t time_cnt)
{
    int i;
    char *ptr;
    /* 5 chars per time, 1 dash, 2 space, 3 weekday abbrev, 1 comma */
    char *ret = malloc(17 * time_cnt);
    memset(ret, 0, 17 * time_cnt);
    ptr = ret;
    for (i = 0; i < time_cnt; ++i) {
        short t1 = tms[i].time_1;
        short t2 = tms[i].time_2;
        int wkdy = tms[i].weekday;
        ptr = ptr + snprintf(ptr, 19, "%02d:00%2s-%02d:00%2s ", t1 < 13 ? t1 : t1-12, t1 < 13 ? "AM" : "PM", t2 < 13 ? t2 : t2-12, t2 < 13 ? "AM" : "PM");
        switch (wkdy) {
        case 0:
            ptr = stpncpy(ptr, "Sun", 4);
            break;
        case 1:
            ptr = stpncpy(ptr, "Mon", 4);
            break;
        case 2:
            ptr = stpncpy(ptr, "Tue", 4);
            break;
        case 3:
            ptr = stpncpy(ptr, "Wed", 4);
            break;
        case 4:
            ptr = stpncpy(ptr, "Thu", 4);
            break;
        case 5:
            ptr = stpncpy(ptr, "Fri", 4);
            break;
        case 6:
            ptr = stpncpy(ptr, "Sat", 4);
            break;
        default:
            fprintf(stderr, "Bad weekday number?\n");
            return NULL;
        }
        if (i < time_cnt-1)
            ptr = stpncpy(ptr, ", ", 3);
    }
    return ret;
}
