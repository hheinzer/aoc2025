#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

long joltage(const char *line, long num)
{
    long tot = strlen(line) - 1;
    long drop = tot - num;

    long buf[num];
    long len = 0;
    for (long i = 0; i < tot; i++) {
        long digit = line[i] - '0';
        while (len > 0 && buf[len - 1] < digit && drop > 0) {
            len -= 1;
            drop -= 1;
        }
        if (len < num) {
            buf[len++] = digit;
        }
        else {
            drop -= 1;
        }
    }

    long result = 0;
    for (long i = 0; i < num; i++) {
        result = (10 * result) + buf[i];
    }
    return result;
}

int main(void)
{
    long total1 = 0;
    long total2 = 0;

    FILE *file = fopen("input/03.txt", "r");
    assert(file);

    char line[128];
    while (fgets(line, sizeof(line), file)) {
        total1 += joltage(line, 2);
        total2 += joltage(line, 12);
    }
    fclose(file);

    printf("%ld\n", total1);
    printf("%ld\n", total2);
}
