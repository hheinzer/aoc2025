#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

enum { BEG = 50 };
enum { MOD = 100 };

int main(void)
{
    long dial = BEG;
    long count1 = 0;
    long count2 = 0;

    FILE *file = fopen("input/01.txt", "r");
    assert(file);

    char line[128];
    while (fgets(line, sizeof(line), file)) {
        long distance = strtol(&line[1], 0, 10);
        if (distance >= MOD) {
            count2 += distance / MOD;
            distance %= MOD;
        }
        if (line[0] == 'L') {
            distance = -distance;
        }
        if (dial != 0 && (dial + distance < 0 || MOD < dial + distance)) {
            count2 += 1;
        }
        dial = (dial + distance + MOD) % MOD;
        if (dial == 0) {
            count1 += 1;
            count2 += 1;
        }
    }
    fclose(file);

    printf("%ld\n", count1);
    printf("%ld\n", count2);
}
