#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define sq(val) ((val) * (val))

enum { NUM = 1000 };

typedef struct {
    long x, y, z;
} Vector;

static long dist2(Vector lhs, Vector rhs)
{
    return sq(lhs.x - rhs.x) + sq(lhs.y - rhs.y) + sq(lhs.z - rhs.z);
}

typedef struct {
    long dist2, lhs, rhs;
} Edge;

static int cmp_edge(const void *lhs_, const void *rhs_)
{
    const Edge *lhs = lhs_;
    const Edge *rhs = rhs_;
    return (lhs->dist2 > rhs->dist2) - (lhs->dist2 < rhs->dist2);
}

static Edge *create_edges(const Vector *box, long num, long num_edges)
{
    Edge *edge = malloc(num_edges * sizeof(*edge));
    long idx = 0;
    for (long i = 0; i < num; i++) {
        for (long j = i + 1; j < num; j++) {
            edge[idx].dist2 = dist2(box[i], box[j]);
            edge[idx].lhs = i;
            edge[idx].rhs = j;
            idx += 1;
        }
    }
    qsort(edge, num_edges, sizeof(*edge), cmp_edge);
    return edge;
}

typedef struct {
    long *off, *idx;
} Graph;

static Graph create_graph(const Edge *edge, long num, long num_pairs)
{
    long *deg = calloc(num, sizeof(*deg));
    for (long i = 0; i < num_pairs; i++) {
        deg[edge[i].lhs] += 1;
        deg[edge[i].rhs] += 1;
    }

    long *off = malloc((num + 1) * sizeof(*off));
    off[0] = 0;
    for (long i = 0; i < num; i++) {
        off[i + 1] = off[i] + deg[i];
    }

    long *idx = malloc(2 * num_pairs * sizeof(*idx));
    for (long i = 0; i < num; i++) {
        off[i + 1] -= deg[i];
    }
    for (long i = 0; i < num_pairs; i++) {
        idx[off[edge[i].lhs + 1]++] = edge[i].rhs;
        idx[off[edge[i].rhs + 1]++] = edge[i].lhs;
    }

    free(deg);
    return (Graph){off, idx};
}

static long component_count(long start, Graph graph, long num)
{
    long count = 0;
    bool *seen = calloc(num, sizeof(*seen));
    long *stack = malloc(num * sizeof(*stack));
    long top = 0;
    seen[start] = true;
    stack[top++] = start;
    while (top > 0) {
        count += 1;
        long cur = stack[--top];
        for (long i = graph.off[cur]; i < graph.off[cur + 1]; i++) {
            long next = graph.idx[i];
            if (!seen[next]) {
                seen[next] = true;
                stack[top++] = next;
            }
        }
    }
    free(stack);
    free(seen);
    return count;
}

typedef struct {
    long *parent, *size, components;
} DisjointSetUnion;

static DisjointSetUnion dsu_create(long num)
{
    DisjointSetUnion dsu;
    dsu.parent = malloc(num * sizeof(*dsu.parent));
    dsu.size = malloc(num * sizeof(*dsu.size));
    dsu.components = num;
    for (long i = 0; i < num; i++) {
        dsu.parent[i] = i;
        dsu.size[i] = 1;
    }
    return dsu;
}

static long dsu_find(DisjointSetUnion *dsu, long node)
{
    while (dsu->parent[node] != node) {
        dsu->parent[node] = dsu->parent[dsu->parent[node]];
        node = dsu->parent[node];
    }
    return node;
}

static bool dsu_union(DisjointSetUnion *dsu, long lhs, long rhs)
{
    lhs = dsu_find(dsu, lhs);
    rhs = dsu_find(dsu, rhs);
    if (lhs == rhs) {
        return false;
    }
    if (dsu->size[lhs] < dsu->size[rhs]) {
        long swap = lhs;
        lhs = rhs;
        rhs = swap;
    }
    dsu->parent[rhs] = lhs;
    dsu->size[lhs] += dsu->size[rhs];
    dsu->components -= 1;
    return true;
}

static long last_connection_product(const Vector *box, const Edge *edge, long num, long num_edges)
{
    DisjointSetUnion dsu = dsu_create(num);
    long lhs = -1;
    long rhs = -1;
    for (long i = 0; i < num_edges; i++) {
        if (dsu_union(&dsu, edge[i].lhs, edge[i].rhs)) {
            lhs = edge[i].lhs;
            rhs = edge[i].rhs;
            if (dsu.components == 1) {
                break;
            }
        }
    }
    free(dsu.parent);
    free(dsu.size);
    return box[lhs].x * box[rhs].x;
}

int main(void)
{
    Vector *box = malloc(NUM * sizeof(*box));
    long num = 0;

    FILE *file = fopen("input/08.txt", "r");
    assert(file);

    char line[128];
    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%ld,%ld,%ld", &box[num].x, &box[num].y, &box[num].z);
        num += 1;
    }
    fclose(file);

    const long num_pairs = (num == 20) ? 10 : 1000;
    long num_edges = num * (num - 1) / 2;
    Edge *edge = create_edges(box, num, num_edges);
    Graph graph = create_graph(edge, num, num_pairs);

    long max_count[3] = {0};
    for (long i = 0; i < num; i++) {
        long count = component_count(i, graph, num);
        for (long j = 0; j < 3; j++) {
            if (count == max_count[j]) {
                break;
            }
            if (count > max_count[j]) {
                max_count[j] = count;
                break;
            }
        }
    }
    printf("%ld\n", max_count[0] * max_count[1] * max_count[2]);
    free(graph.off);
    free(graph.idx);

    printf("%ld\n", last_connection_product(box, edge, num, num_edges));
    free(edge);
    free(box);
}
