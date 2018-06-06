#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "graph.h"

void fill_vertex(struct vertex* v, int cnt_dims, double* coords, double fx, double ux)
{
        v->cnt_dims = cnt_dims;
        v->coords = coords;
        v->fx = fx;
        v->ux = ux;
}

struct link *new_link(struct vertex* vertex, struct link* next)
{
        struct link* x = malloc(sizeof(struct link));
        x->vertex = vertex;
        x->next = next;
        return x;

}

struct graph* make_graph(int cnt_vertices, int cnt_dims ,
                         double* coords[],double* values,
                         double* init_u)
{
        struct graph* g = malloc(sizeof (struct graph));
        g->cnt_vertices = cnt_vertices;
        g->cnt_edges = 0;
        g->vertices = malloc(sizeof(struct vertex) * cnt_vertices);
        for (int v = 0; v < cnt_vertices; v++) {
                fill_vertex(&g->vertices[v],cnt_dims,coords[v],values[v],init_u[v]);
                g->vertices[v].id = v;
        }
        g->adj = malloc(sizeof(struct link) * cnt_vertices);
        for (int v = 0; v < cnt_vertices; v++) {
                g->adj[v] = NULL;
        }
        return g;
}

void graph_connect(struct graph* g, int id1, int id2)
{
        struct vertex* v1 = &g->vertices[id1];
        struct vertex* v2 = &g->vertices[id2];
        g->adj[id1] = new_link(v2, g->adj[id1]);
        g->adj[id2] = new_link(v1, g->adj[id2]);
        g->cnt_edges++;
}

bool graph_is_connected(struct graph* g, int id1, int id2)
{
        for(struct link* t =g->adj[id1]; t != NULL; t = t->next)
                if (t->vertex->id == id2) return true;
        return false;
}

struct link* graph_get_neighbours(struct graph *g, int id)
{
    return g->adj[id];
}
