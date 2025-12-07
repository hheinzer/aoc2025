#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum { ROWS = 5 };
enum { COLS = 3800 };

int main(void)
{
    char table[ROWS][COLS] = {0};
    long rows = 0;
    long cols = 0;

    FILE *file = fopen("input/06.txt", "r");
    assert(file);

    char line[4096];
    while (fgets(line, sizeof(line), file)) {
        long len = strlen(line) - 1;
        memcpy(table[rows++], line, len);
        if (len > cols) {
            cols = len;
        }
    }
    fclose(file);

    long total1 = 0;
    for (long j = 0; j < cols; j++) {
        char operation = table[rows - 1][j];
        if (operation != '+' && operation != '*') {
            continue;
        }
        long result = (operation == '+') ? 0 : 1;
        for (long i = 0; i < rows - 1; i++) {
            long num = strtol(&table[i][j], 0, 10);
            if (operation == '+') {
                result += num;
            }
            else {
                result *= num;
            }
        }
        total1 += result;
    }
    printf("%ld\n", total1);

    long total2 = 0;
    for (long j = 0; j < cols; j++) {
        char operation = table[rows - 1][j];
        if (operation != '+' && operation != '*') {
            continue;
        }
        long result = (operation == '+') ? 0 : 1;
        long col = j;
        while (true) {
            char str[ROWS] = {0};
            for (long i = 0; i < rows - 1; i++) {
                str[i] = isdigit(table[i][col]) ? table[i][col] : ' ';
            }
            char *end;
            long num = strtol(str, &end, 10);
            if (end == str) {
                break;
            }
            if (operation == '+') {
                result += num;
            }
            else {
                result *= num;
            }
            col += 1;
        }
        total2 += result;
    }
    printf("%ld\n", total2);
}
