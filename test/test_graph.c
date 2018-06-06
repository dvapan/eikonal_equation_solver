#include <stdlib.h>
#include <assert.h>

#include "graph.h"


void test_graph()
{
    int cnt_nodes = 5;
    int cnt_dims  = 2;
    double *coords[cnt_nodes];
    for(int i = 0; i < cnt_nodes; i++)
    {
        coords[i] = malloc(sizeof(double)* 2);
        coords[i][0] = 0;
        coords[i][1] = 0;

    }
    coords[4][0] = 4;
    coords[4][1] = 3;
    double values[cnt_nodes];
    double init_u[cnt_nodes];

    struct graph* g = make_graph(cnt_nodes,cnt_dims,
                                 coords,values, init_u);

    graph_connect(g, 0, 1);
    graph_connect(g, 0, 2);
    graph_connect(g, 0, 3);
    graph_connect(g, 3, 4);
    graph_connect(g, 1, 4);
    graph_connect(g, 2, 3);

    /* check out connections */
    assert( graph_is_connected(g, 0, 1));
    assert( graph_is_connected(g, 0, 2));
    assert( graph_is_connected(g, 0, 3));
    assert( graph_is_connected(g, 3, 4));
    assert( graph_is_connected(g, 1, 4));
    assert( graph_is_connected(g, 2, 3));
    assert(!graph_is_connected(g, 1, 2));
    assert(!graph_is_connected(g, 0, 4));

    /* check out neighbours of node */
    struct link* ngbrs = graph_get_neighbours(g,0);
    int cnt_nbrs = 4;
    int check_nbrs[cnt_nbrs];
    for(int i = 0; i < cnt_nbrs; i++)
        check_nbrs[i] = 0;
    for(struct link* t = ngbrs; t != NULL; t = t->next) {
        check_nbrs[t->vertex->id]++;
    }
    for(int i = 1; i < cnt_nbrs; i++){
        assert(check_nbrs[i] == 1);
    }
}


int main(int argc, char *argv[])
{
    test_graph();

    return EXIT_SUCCESS;
}
