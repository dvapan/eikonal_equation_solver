#ifndef GRAPH_H
#define GRAPH_H 1

#include <stdbool.h>

struct link
{
        struct vertex* vertex;
        struct link* next;
};

struct vertex
{
        int id;
        int cnt_dims;
        double *coords;
        double fx;
        double ux;
};

struct graph
{
        int cnt_vertices;
        int cnt_edges;
        struct vertex* vertices;
        struct link** adj;
};

struct graph* make_graph(int cnt_vertices, int cnt_dims,
                         double** coords, double* values);
void graph_connect(struct graph*, int, int);
bool graph_is_connected(struct graph*, int, int);
struct link* graph_get_neighbours(struct graph* g, int id);

#endif // GRAPH_H
