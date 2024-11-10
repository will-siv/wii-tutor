#include <stdio.h>
#include "../csvparse.h"

int main(int argc, char **argv)
{
    FILE *fil;
    struct person **tutors;
    size_t tutor_list_len = 0;
    int i;
    if (argc != 2) {
        printf("Incorrect arg count, 1 arg expected\n");
        return 1;
    }
    fil = fopen(argv[1], "r");
    if (!fil) {
        return 1;
    }
    tutors = parse_tutors(fil, &tutor_list_len);
    if (!tutors) {
        printf("Tutor allocation failed??\n");
        return 1;
    }
    for (i = 0; i < tutor_list_len; ++i) {
        int j;
        printf("Tutor #%d:\nName: %s\nCourse List: ", i, tutors[i]->name);
        for (j = 0; j < tutors[i]->class_cnt; ++j) {
            printf("%s", tutors[i]->classes[j]);
            if (j != tutors[i]->class_cnt-1)
                printf(", ");
        }
        printf("\nTimes Available: ");
        for (j = 0; j < tutors[i]->time_cnt; ++j) {
            struct time_interval *t = &tutors[i]->times[j];
            printf("%hd-%hd on day #%d", t->time_1, t->time_2, t->weekday);
            if (j != tutors[i]->time_cnt-1)
                printf(", ");
        }
        printf("\n\n");
    }
    free_tutors(tutors, tutor_list_len);
    fclose(fil);
    return 0;
}