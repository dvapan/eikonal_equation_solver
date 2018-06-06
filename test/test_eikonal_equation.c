#include <assert.h>
#include <math.h>

#include "eikonal_equation.h"
#include "ndvector.h"
#include "graph.h"


static void test_solve_ndims_one_dimension()
{

        int cnt_vertices = 3;
        int dims = 1;

        double *coords[cnt_vertices];
        double  values[cnt_vertices];
        double  init_u[cnt_vertices];

        for(int i = 0; i < cnt_vertices; i++)
        {
                coords[i] = malloc(sizeof(double)* 2);
                values[i] = 1;
                init_u[i] = INFINITY;
        }
        coords[0][0] = 0;
        coords[1][0] = 0.1;
        coords[2][0] = 0.3;

        struct graph* mesh = make_graph(cnt_vertices,dims, coords, values, init_u);
        graph_connect(mesh, 0,1);
        graph_connect(mesh, 1,2);

        mesh->vertices[0].ux = 1;

        double res = solve_ndims(mesh,1);
        assert(round(res*10)/10 == 1.1);
        printf("DONE\n");
}


static void test_solve_ndims_two_dimensions()
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
                init_u[i] = INFINITY;
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
        
        struct graph* mesh = make_graph(cnt_vertices,dims, coords, values, init_u);
        graph_connect(mesh, 0,1);
        graph_connect(mesh, 0,2);
        graph_connect(mesh, 0,3);
        graph_connect(mesh, 1,3);
        graph_connect(mesh, 1,4);
        graph_connect(mesh, 2,3);
        graph_connect(mesh, 2,5);
        graph_connect(mesh, 3,5);
        graph_connect(mesh, 3,6);
        graph_connect(mesh, 3,4);

        mesh->vertices[0].ux = 0;
        mesh->vertices[1].ux = 1;
        mesh->vertices[2].ux = 1;
        mesh->vertices[3].ux = 1;

        double res = solve_ndims(mesh,5);
        printf("%lf %lf\n",res, 1+sqrt(3.0)/2.0);
        assert(abs(res - (1+sqrt(3.0)/2.0)) < 0.01);
        res = solve_ndims(mesh,4);
        assert(abs(res - (1+sqrt(3.0)/2.0)) < 0.01);
        printf("DONE\n");
        mesh->vertices[4].ux = (1+sqrt(3.0)/2.0);
        mesh->vertices[6].ux = (1+sqrt(3.0)/2.0);
        res = solve_ndims(mesh,6);
        assert(abs(res - (2.00) < 0.01));

}


int main(int argc, char *argv[])
{


        test_solve_ndims_one_dimension();
        test_solve_ndims_two_dimensions();

        return EXIT_SUCCESS;
}

