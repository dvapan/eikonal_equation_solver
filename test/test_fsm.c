#include "fsm.h"
#include <assert.h>
#include <glib.h>
#include <glib/gi18n.h>
#include "triangle.h"
#include "graph.h"


static void test_fsm_rct_grid()
{
        int cnt_vertices = 25;
        int dims = 2;

        double *coords[cnt_vertices];
        double  values[cnt_vertices];
        double  init_u[cnt_vertices];

        for(int i = 0; i < cnt_vertices; i++)
        {
                coords[i] = malloc(sizeof(double)* 2);
                values[i] = 1;
                init_u[i] = INFINITY;
        }
       
        int id =0;
        int point_zero = 0;
        for(int i = -2; i<=2; i++)
                for(int j = -2; j<=2; j++){
                        coords[id][0] = i;
                        coords[id][1] = j;
                        if (i==0&&j==0)
                                point_zero = id;
                        id++;
                }

        init_u[point_zero] = 0;
        
        struct graph* mesh = make_graph(cnt_vertices,dims, coords, values, init_u);

        triag_mesh(mesh);
        fsm(mesh);

        for(int i = 0; i< cnt_vertices;i++){
                struct vertex* v = &mesh->vertices[i];
                printf("[% 4.2lf % 4.2lf] = %lf\n", v->coords[0],v->coords[1],v->ux);
        }
        
        /* double answers[cnt_vertices]= */
        /*         {} */
        
}

static void test_fsm()
{
        /* size_t dims = 2; */
        /* size_t size[dims]; */
        /* size[0] = 5; */
        /* size[1] = 5; */
        /* struct eikonal_data* ed = eikonal_data_alloc(dims, size, 1.0); */
        /* size_t vec[dims]; */
        /* for (int i = 0; i < size[0]; i++) */
        /*         for (int j = 0; j < size[1]; j++){ */
        /*                 vec[0] = i; */
        /*                 vec[1] = j; */
        /*                 ndvector_set(ed->velocites, vec, 1.0); */
        /*                 ndvector_set(ed->curr_solve, vec, INFINITY); */
        /*         } */
        /* vec[0] = 2; */
        /* vec[1] = 2; */
        /* ndvector_set(ed->curr_solve, vec, 0); */
        /* fsm(ed); */

        /* double expected_res[5][5] = */
        /*         {{3.25,2.55,2.00,2.55,3.25}, */
        /*          {2.55,1.71,1.00,1.71,2.55}, */
        /*          {2.00,1.00,0.00,1.00,2.00}, */
        /*          {2.55,1.71,1.00,1.71,2.55}, */
        /*          {3.25,2.55,2.00,2.55,3.25}}; */

        /* size_t height = gsl_vector_get (ed->curr_solve->sizes,0); */
        /* size_t width = gsl_vector_get(ed->curr_solve->sizes,1); */
        /* for(int i = 0; i < height; i++){ */
        /*         for(int j = 0; j < width; j++){ */
        /*                 vec[0] = i; */
        /*                 vec[1] = j; */
        /*                 assert(abs(expected_res[i][j] - ndvector_get(ed->curr_solve,vec)) < 0.1); */
        /*         } */

        /* } */
        int cnt_vertices = 7;
        int dims = 2;

        double *coords[cnt_vertices];
        double  values[cnt_vertices];
        double  init_u[cnt_vertices];

        for(int i = 0; i < cnt_vertices; i++)
        {
                coords[i] = malloc(sizeof(double)* 2);
                values[i] = 1;
                init_u[i] = INFINITY;
        }
        init_u[0] = 0;
        
        coords[0][0] = 0;
        coords[0][1] = 0;
        coords[1][0] = 1;
        coords[1][1] = 0;
        coords[2][0] = -sin(M_PI/6);
        coords[2][1] = -cos(M_PI/6);
        coords[3][0] =  sin(M_PI/6);
        coords[3][1] = -cos(M_PI/6);
        coords[4][0] = 1+sin(M_PI/6);
        coords[4][1] = -cos(M_PI/6);
        coords[5][0] = 0;
        coords[5][1] = -2*cos(M_PI/6);
        coords[6][0] = 1;
        coords[6][1] = -2*cos(M_PI/6);
        /* coords[7][0] = -1; */
        /* coords[7][1] = 0; */
        /* coords[8][0] = -sin(M_PI/6);; */
        /* coords[8][1] = cos(M_PI/6);; */
        /* coords[9][0] = sin(M_PI/6); */
        /* coords[9][1] = cos(M_PI/6); */
        /* coords[10][0] =  2; */
        /* coords[10][1] = 0; */

        
        struct graph* mesh = make_graph(cnt_vertices,dims, coords, values,init_u);
        mesh->vertices[0].ux = 0;
        triag_mesh(mesh);
        fsm(mesh);

        for(int i = 0; i< cnt_vertices;i++){
                struct vertex* v = &mesh->vertices[i];
                printf("[% 4.2lf % 4.2lf] = %lf\n", v->coords[0],v->coords[1],v->ux);
        }
        
        /* double answers[cnt_vertices]= */
        /*         {} */
        
}

void test_sort_for_sweep ()
{
        int cnt_vertices = 7;
        int dims = 2;

        double *coords[cnt_vertices];
        double  values[cnt_vertices];
        double  init_u[cnt_vertices];

        for(int i = 0; i < cnt_vertices; i++)
        {
                coords[i] = malloc(sizeof(double)* 2);
                values[i] = 1;
        }

        
        coords[0][0] = 0;
        coords[0][1] = 0;
        coords[1][0] = 1;
        coords[1][1] = 0;
        coords[2][0] = -sin(M_PI/6);
        coords[2][1] = -cos(M_PI/6);
        coords[3][0] =  sin(M_PI/6);
        coords[3][1] = -cos(M_PI/6);
        coords[4][0] = 1+sin(M_PI/6);
        coords[4][1] = -cos(M_PI/6);
        coords[5][0] = 0;
        coords[5][1] = -2*cos(M_PI/6);
        coords[6][0] = 1;
        coords[6][1] = -2*cos(M_PI/6);

        struct graph* mesh = make_graph(cnt_vertices,dims, coords, values,init_u);
        int* sweep = malloc(sizeof(int)*cnt_vertices);
        for(int i = 0; i < cnt_vertices;i++)
                sweep[i]=i;
        sort(cnt_vertices,0.5,0.5,mesh->vertices, sweep);
        printf("sorted ids for ref[0.5,0.5]\n");
        
        for(int i = 0; i < cnt_vertices;i++)
                printf("%d\n", sweep[i]);
        for(int i = 0; i < cnt_vertices;i++)
                sweep[i]=i;
        sort(cnt_vertices,2,0,mesh->vertices, sweep);
        printf("sorted ids for ref[2,0]\n");
        
        for(int i = 0; i < cnt_vertices;i++)
                printf("%d ", sweep[i]);
        printf("\n");
}


void test_triangulation()
{
        int cnt_vertices = 7;
        int dims = 2;

        double *coords[cnt_vertices];
        double  values[cnt_vertices];
        double  init_u[cnt_vertices];

        for(int i = 0; i < cnt_vertices; i++)
        {
                coords[i] = malloc(sizeof(double)* 2);
                values[i] = 1;
        }
       
        coords[0][0] = 0;
        coords[0][1] = 0;
        coords[1][0] = 1;
        coords[1][1] = 0;
        coords[2][0] = -sin(M_PI/6);
        coords[2][1] = -cos(M_PI/6);
        coords[3][0] =  sin(M_PI/6);
        coords[3][1] = -cos(M_PI/6);
        coords[4][0] = 1+sin(M_PI/6);
        coords[4][1] = -cos(M_PI/6);
        coords[5][0] = 0;
        coords[5][1] = -2*cos(M_PI/6);
        coords[6][0] = 1;
        coords[6][1] = -2*cos(M_PI/6);

        struct graph* mesh = make_graph(cnt_vertices,dims, coords, values,init_u);
        triag_mesh(mesh);
        
        assert(graph_is_connected(mesh, 0,1));
        assert(graph_is_connected(mesh, 0,2));
        assert(graph_is_connected(mesh, 0,3));
        assert(graph_is_connected(mesh, 1,3));
        assert(graph_is_connected(mesh, 1,4));
        assert(graph_is_connected(mesh, 2,3));
        assert(graph_is_connected(mesh, 2,5));
        assert(graph_is_connected(mesh, 3,5));
        assert(graph_is_connected(mesh, 3,6));
        assert(graph_is_connected(mesh, 3,4));

        struct link* ngbrs = graph_get_neighbours(mesh,3);
        int cnt_nbrs = 6;
        int check_nbrs[cnt_nbrs];
        for(int i = 0; i < cnt_nbrs; i++)
                check_nbrs[i] = 0;
        printf("Neighbours of vertex 3\n");
        for(struct link* t = ngbrs; t != NULL; t = t->next) {
                printf("%d\n", t->vertex->id);
                check_nbrs[t->vertex->id]++;
        }
        for(int i = 0; i < cnt_nbrs; i++){
                if (i!=3)
                        assert(check_nbrs[i] == 1);
        }

}

int main(int argc, char *argv[])
{
        /* test_set_sweep_dirs(); */
        /* test_sweep(); */

        /* test_fsm(); */
        test_triangulation();
        test_sort_for_sweep();
        test_fsm();
        test_fsm_rct_grid();

        return EXIT_SUCCESS;
}
