#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum { NUM = 173 };

typedef struct {
    long beg, end;
} Range;

static int cmp_range(const void *lhs_, const void *rhs_)
{
    const Range *lhs = lhs_;
    const Range *rhs = rhs_;
    if (lhs->beg < rhs->beg) {
        return -1;
    }
    if (lhs->beg > rhs->beg) {
        return 1;
    }
    if (lhs->end < rhs->end) {
        return -1;
    }
    if (lhs->end > rhs->end) {
        return 1;
    }
    return 0;
}

int main(void)
{
    Range range[NUM];
    long num = 0;

    FILE *file = fopen("input/05.txt", "r");
    assert(file);

    char line[128];
    while (strlen(fgets(line, sizeof(line), file)) > 1) {
        sscanf(line, "%ld-%ld", &range[num].beg, &range[num].end);
        num += 1;
    }

    long count1 = 0;
    while (fgets(line, sizeof(line), file)) {
        long ingredient = strtol(line, 0, 10);
        for (long i = 0; i < num; i++) {
            if (range[i].beg <= ingredient && ingredient <= range[i].end) {
                count1 += 1;
                break;
            }
        }
    }
    fclose(file);
    printf("%ld\n", count1);

    qsort(range, num, sizeof(*range), cmp_range);
    long count2 = 0;
    Range curr = range[0];
    for (long i = 1; i < num; i++) {
        Range next = range[i];
        if (next.beg <= curr.end || next.beg == curr.end + 1) {
            if (next.end > curr.end) {
                curr.end = next.end;
            }
        }
        else {
            count2 += curr.end - curr.beg + 1;
            curr = next;
        }
    }
    count2 += curr.end - curr.beg + 1;
    printf("%ld\n", count2);
}
