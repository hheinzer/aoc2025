#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

enum { ROWS = 135 };
enum { COLS = 135 };

static long adjacent(long row, long col, const char *grid, long rows, long cols)
{
    assert(grid[(row * cols) + col] == '@');
    long num = 0;
    for (long i = -1; i <= 1; i++) {
        for (long j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) {
                continue;
            }
            if (0 <= (row + i) && (row + i) < rows) {
                if (0 <= (col + j) && (col + j) < cols) {
                    num += (grid[((row + i) * cols) + (col + j)] == '@');
                }
            }
        }
    }
    return num;
}

int main(void)
{
    char grid[ROWS * COLS];
    long rows = 0;
    long cols = 0;

    FILE *file = fopen("input/04.txt", "r");
    assert(file);

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        cols = strlen(line) - 1;
        memcpy(&grid[rows * cols], line, cols);
        rows += 1;
    }
    fclose(file);

    long count1 = 0;
    for (long i = 0; i < rows; i++) {
        for (long j = 0; j < cols; j++) {
            if (grid[(i * cols) + j] == '@' && adjacent(i, j, grid, rows, cols) < 4) {
                count1 += 1;
            }
        }
    }
    printf("%ld\n", count1);

    long count2 = 0;
    bool done = false;
    while (!done) {
        done = true;
        for (long i = 0; i < rows; i++) {
            for (long j = 0; j < cols; j++) {
                if (grid[(i * cols) + j] == '@' && adjacent(i, j, grid, rows, cols) < 4) {
                    grid[(i * cols) + j] = '.';
                    count2 += 1;
                    done = false;
                }
            }
        }
    }
    printf("%ld\n", count2);
}
