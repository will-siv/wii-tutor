#ifndef CSVPARSE_H
#define CSVPARSE_H

#include <time.h>
#include <stdio.h>

#define CLASS_STR_LENGTH 9

struct time_interval {
    short time_1;
    short time_2;
    int weekday;
}

struct person {
    char *name;
    size_t name_len;
    char (*classes)[CLASS_STR_LENGTH];
    size_t class_cnt;
    struct time_interval *times;
    size_t time_cnt;
};

/* Reads a CSV of tutors expecting the following format:
 * Name\tClass List(comma separated)\tTime List(comma separated) */
struct person **parse_tutors(FILE *csv, size_t *tutor_count);

void free_person(struct person *p);
void free_tutors(struct person **tutors, int tutor_count);

#endif /* CSVPARSE_H */
