#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

enum { ROWS = 150 };
enum { COLS = 150 };

typedef struct {
    long row, col;
} Vector;

static Vector find_start(const char *map, long rows, long cols)
{
    for (long i = 0; i < rows; i++) {
        for (long j = 0; j < cols; j++) {
            if (map[(i * cols) + j] == 'S') {
                return (Vector){i, j};
            }
        }
    }
    return (Vector){-1, -1};
}

static long bfs(Vector start, const char *map, long rows, long cols)
{
    long count = 0;
    bool seen[ROWS][COLS] = {0};
    Vector stack[ROWS * COLS];
    long top = 0;
    stack[top++] = start;
    while (top > 0) {
        Vector cur = stack[--top];
        while (cur.row < rows && map[(cur.row * cols) + cur.col] != '^') {
            cur.row += 1;
        }
        if (cur.row < rows && !seen[cur.row][cur.col]) {
            count += seen[cur.row][cur.col] = true;
            if (cur.col - 1 >= 0) {
                stack[top++] = (Vector){cur.row, cur.col - 1};
            }
            if (cur.col + 1 < cols) {
                stack[top++] = (Vector){cur.row, cur.col + 1};
            }
        }
    }
    return count;
}

static long dfs(Vector cur, const char *map, long rows, long cols)
{
    static long cache[ROWS][COLS] = {0};
    if (cur.row >= rows || cur.col < 0 || cols <= cur.col) {
        return 1;
    }
    long *value = &cache[cur.row][cur.col];
    if (*value != 0) {
        return *value;
    }
    while (cur.row < rows && map[(cur.row * cols) + cur.col] != '^') {
        cur.row += 1;
    }
    if (cur.row >= rows) {
        return *value = 1;
    }
    long left = dfs((Vector){cur.row, cur.col - 1}, map, rows, cols);
    long right = dfs((Vector){cur.row, cur.col + 1}, map, rows, cols);
    return *value = left + right;
}

int main(void)
{
    char map[ROWS * COLS];
    long rows = 0;
    long cols = 0;

    FILE *file = fopen("input/07.txt", "r");
    assert(file);

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        cols = strlen(line) - 1;
        memcpy(&map[rows * cols], line, cols);
        rows += 1;
    }
    fclose(file);

    Vector start = find_start(map, rows, cols);
    assert(start.row != -1 && start.col != -1);

    printf("%ld\n", bfs(start, map, rows, cols));
    printf("%ld\n", dfs(start, map, rows, cols));
}
