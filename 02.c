#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

bool is_invalid(const char *str, long num, long len)
{
    for (long i = num; i < len; i += num) {
        if (memcmp(str, &str[i], num) != 0) {
            return false;
        }
    }
    return true;
}

int main(void)
{
    long count1 = 0;
    long count2 = 0;

    FILE *file = fopen("input/02.txt", "r");
    assert(file);

    long beg;
    long end;
    char str[128];
    while (fscanf(file, "%ld-%ld%*c", &beg, &end) == 2) {
        for (long id = beg; id <= end; id++) {
            long len = sprintf(str, "%ld", id);
            if (len % 2 == 0 && memcmp(str, &str[len / 2], len / 2) == 0) {
                count1 += id;
            }
            for (long num = 1; num <= len / 2; num++) {
                if (is_invalid(str, num, len)) {
                    count2 += id;
                    break;
                }
            }
        }
    }
    fclose(file);

    printf("%ld\n", count1);
    printf("%ld\n", count2);
}
