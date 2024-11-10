#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include "csvparse.h"

static inline void free_tutor_name(char *n);
static inline void free_tutor_classes(char **c);
static inline void free_tutor_times(struct time_interval *t, size_t cnt);
static int parse_class_list(char *classes, struct person *p);
static int parse_times(char *times, struct person *p);
static struct person *parse_tutor(char *line);

struct person **parse_tutors(FILE *csv, size_t *tutor_count)
{
    char *buf;
    size_t *len;
    ssize_t read;
    int i;
    int lines = count_lines(csv);
    if (lines != 0)
        return NULL;
    rewind(csv);
    struct person **tutors = calloc(lines, sizeof(struct person *));
    if (!tutors) {
        return NULL;
    }
    *count = 0;
    while ((read = getline(&buf, &len, csv)) > 0) {
        tutor_count++;
        tutors[cnt-1] = parse_tutor(buf);
        if (!tutors[tutor_count-1]) {
            free_tutors(tutors, tutor_count);
            return NULL;
        }
    }
    if (ferror(csv)) {
        fprintf(stderr, "Error detected reading the CSV");
        free_tutors(tutors, cnt);
        return NULL;
    }
    return tutors;
}

static void free_tutors(struct person **t, int cnt)
{
    int i;
    for (i = 0; i < cnt; ++i)
        if (t[i]) {
            free_person(t[i]);
            t[i] = NULL;
        }
    free(t);
}

static int count_lines(FILE *f)
{
    int n, i;
    int lns;
    char buf[BUFSIZ];
    while (!ferror(f) && !feof(f)) {
        n = fread(buf, 1, BUFSIZ, f);
        for (i = 0; i < n; ++i)
            if (buf[i] == '\n')
                ++lns;
    }
    if (ferror(f)) {
        fprintf(stderr, "File had error on read\n");
        return -1;
    }

}
/* Reads a singular tutor expecting the format:
 * Name\tClass List(comma separated)\tTime List(comma separated) */
static struct person *parse_tutor(char *line)
{
    int i, ret;
    _Bool err = 0;
    char *name, *classes, *times;
    char *tok;
    struct person *cur = malloc(sizeof(struct person));
    if (!cur)
        return NULL;
    for (i = 0; i < 3; ++i) {
        tok = strtok(i == 0 ? buf : NULL, "\t");
        if (!tok) {
            fprintf(stderr, "Line %d: Incorrect amount of fields in CSV, skipping...\n", linenum);
            err = 1;
            break;
        }
        if (i == 0)
            name = tok;
else if (i == 1)
            classes = tok;
        else
            times = tok;
    }
    if (err) {
        return NULL;
    }
    cur->name_len = strlen(name);
    cur->name = strdup(buf);
    if (!cur->name)
        return NULL;
    ret = parse_class_list(classes, &cur);
    if (ret != 0)
        return NULL;
    ret = cur->times = parse_times(times, &cur);
    if (ret != 0)
        return NULL;
}

static int parse_class_list(char *classes, struct person *p)
{
    char *tok;
    size_t nbytes;
    for (tok = strtok(classes, ","); tok; tok = strtok(NULL, ",")) {
        ++p->class_cnt;
        tok = strtok(NULL, ",");
    }
    nbytes = sizeof(*p->classes) * p->class_cnt;
    p->classes = malloc(nbytes);
    if (!p->classes)
        return -1;
    memcpy(p->classes, classes, nbytes);
    return 0;
}

static int parse_times(char *times, struct person *p)
{
    char *tok;
    char *ptr = times;
    int i;
    _Bool in_str = 0;
    for (tok = strtok(times, ","); tok; tok = strtok(NULL, ",")) {
        ++p->time_cnt;
        tok = strtok(NULL, ",");
    }
    p->times = malloc(sizeof(struct time_interval) * p->time_cnt);
    if (!p->times)
        return -1;
    for (i = 0; i < p->time_cnt; ++i) {
        int wkday;
        sscanf(ptr, "%d:00-%d:00 %c", &p->times[i].time_1, &p->times[i].time_2, &wkday);
        switch (wkday) {
        case 'U':
        case 'u':
            wkday = 0;
            break;
        case 'M':
        case 'm':
            wkday = 1;
            break;
        case 'T':
        case 't':
            wkday = 2;
            break;
        case 'W':
        case 'w':
            wkday = 3;
            break;
        case 'R':
        case 'r':
            wkday = 4;
            break;
        case 'F':
        case 'f':
            wkday = 5;
            break;
        case 'S':
        case 's':
            wkday = 6;
            break;
        default:
            fprintf(stderr, "Incorrect weekday character for %s, something may break...\n", p->name);
            wkday = 7;
            break;
        }
        p->times[i].weekday = wkday;
        while (*ptr)
            ++ptr;
        ++ptr;
    }
    return 0;
}

void free_person(struct person *p)
{
    int i;
    if (p->name) {
        free_tutor_name(p->name);
        p->name = NULL;
        p->name_len = 0;
    }
    if (p->classes) {
        free_tutor_classes(p->classes);
        p->classes = NULL;
        p->class_cnt = 0;
    }
    if (p->times) {
free_tutor_times(p->times, p->time_cnt);
        p->times = NULL;
        p->time_cnt = 0;
    }
    free(p);
}

static inline void free_tutor_name(char *n)
{
    free(n);
}

static inline void free_tutor_classes(char **c)
{
    free(c);
}

static inline void free_tutor_times(struct time_interval *t, size_t cnt)
{
    int i;
    for (i = 0; i < cnt; ++i)
        free(t[i]);
}
