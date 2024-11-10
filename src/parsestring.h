#ifndef PARSESTRING_H
#define PARSESTRING_H

#include <stddef.h>
#include "csvparse.h"

char *tutor_class_to_string(char (*classes)[CLASS_STR_LENGTH], size_t class_cnt);

char *tutor_times_to_string(struct time_interval *times, size_t time_cnt);

#endif /* PARSESTRING_H */
