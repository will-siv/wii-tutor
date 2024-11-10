#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include "csvparse.h"

static inline void free_tutor_name(char *n);
static inline void free_tutor_classes(char (*c)[]);
static inline void free_tutor_times(struct time_interval *t);
static int parse_class_list(char *classes, struct person *p);
static int parse_times(char *times, struct person *p);
static struct person *parse_tutor(char *line);
static int count_lines(FILE *f);

struct person **parse_tutors(FILE *csv, size_t *tutor_count)
{
    char *buf = NULL;
    size_t len = 0;
    ssize_t read;
    struct person **tutors;
    int lines = count_lines(csv);
    if (lines <= 0)
        return NULL;
    rewind(csv);
    tutors = calloc(lines, sizeof(struct person *));
    if (!tutors) {
        return NULL;
    }
    *tutor_count = 0;
        printf("%d\n", *tutor_count);
    while ((read = getline(&buf, &len, csv)) > 0) {
        ++*tutor_count;
        printf("%d\n", *tutor_count);
        tutors[*tutor_count-1] = parse_tutor(buf);
        printf("%d\n", *tutor_count);
        if (!tutors[*tutor_count-1]) {
            free_tutors(tutors, *tutor_count);
            free(buf);
            return NULL;
        }
    }
    if (ferror(csv)) {
        fprintf(stderr, "Error detected reading the CSV");
        free_tutors(tutors, *tutor_count);
        free(buf);
        return NULL;
    }
    free(buf);
    return tutors;
}

void free_tutors(struct person **t, int cnt)
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
    size_t i, n;
    int lns;
    char buf[BUFSIZ];
    while (1) {
        if (ferror(f)) {
            fprintf(stderr, "File had error on read\n");
            return -1;
        }
        n = fread(buf, 1, BUFSIZ, f);
        for (i = 0; i < n; ++i) {
            if (buf[i] == '\n') {
                ++lns;
            }
        }
        if (feof(f))
            break;
    }
    return lns;
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
        tok = strtok(i == 0 ? line : NULL, "\t");
        if (!tok) {
            fprintf(stderr, "Incorrect amount of fields in CSV, skipping a line...\n");
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
    cur->name = strdup(line);
    if (!cur->name)
        return NULL;
    ret = parse_class_list(classes, cur);
    if (ret != 0)
        return NULL;
    ret = parse_times(times, cur);
    if (ret != 0)
        return NULL;
    return cur;
}

static int parse_class_list(char *classes, struct person *p)
{
    char *tok;
    size_t nbytes;
    size_t i, clsnum;
    char *ptr;
    for (tok = strtok(classes, ","); tok; tok = strtok(NULL, ",")) {
        ++p->class_cnt;
        tok = strtok(NULL, ",");
    }
    nbytes = sizeof(*p->classes) * p->class_cnt;
    p->classes = malloc(nbytes);
    if (!p->classes)
        return -1;
    /*memcpy(p->classes, classes, nbytes);*/
    clsnum = p->class_cnt;
    ptr = classes;
    for (i = 0; i < clsnum; ++i) {
        strncpy(*(p->classes+i), ptr, 8);
        printf("%s\n", ptr);
        ptr = strchr(ptr, '\0');
        ++ptr;
    }
    return 0;
}

static int parse_times(char *times, struct person *p)
{
    char *tok;
    char *ptr = times;
    size_t i;
    printf("%s", ptr);
    for (tok = strtok(times, ","); tok; tok = strtok(NULL, ",")) {
        ++p->time_cnt;
        tok = strtok(NULL, ",");
    }
    p->times = malloc(sizeof(struct time_interval) * p->time_cnt);
    if (!p->times)
        return -1;
    for (i = 0; i < p->time_cnt; ++i) {
        int wkday;
        sscanf(ptr, "%2hd-%2hd%1s", &p->times[i].time_1, &p->times[i].time_2, &wkday);
        printf("%d\n", wkday);
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
free_tutor_times(p->times);
        p->times = NULL;
        p->time_cnt = 0;
    }
    free(p);
}

static inline void free_tutor_name(char *n)
{
    free(n);
}

static inline void free_tutor_classes(char (*c)[])
{
    free(c);
}

static inline void free_tutor_times(struct time_interval *t)
{
    free(t);
}
