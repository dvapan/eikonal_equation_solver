#include "fsm.h"
#include "triangle.h"
#include <math.h>
#include <stdio.h>


double distance(double x11, double x12, double x21, double x22)
{
        return sqrt(pow(x21-x11,2)+ pow(x22-x12,2));
}

void sort(int cnt_vertices, double x1, double x2, struct vertex* vertices, int* sweep)
{
        for (int i = 0; i < cnt_vertices; i++){
                int min = i;
                for(int j = i+1; j < cnt_vertices; j++){
                        struct vertex* v = &vertices[sweep[j]];
                        struct vertex* v_min = &vertices[sweep[min]];
                        if (distance(x1,x2,v->coords[0],v->coords[1]) <
                            distance(x1,x2,v_min->coords[0],v_min->coords[1])){
                                min = j;
                        }
                }
                int tmp = sweep[i];
                sweep[i] = sweep[min];
                sweep[min] = tmp;

        }
}

void triag_mesh(struct graph* mesh)
{
        struct triangulateio in, mid, out, vorout;
        in.numberofpoints = mesh->cnt_vertices;
        in.numberofpointattributes = 1;
        in.pointlist = (REAL *) malloc(in.numberofpoints * 2 * sizeof(REAL));
        int id = 0;
        for(int i = 0; i < mesh->cnt_vertices; i++){
                for (int j = 0; j<2; j++)
                {
                        in.pointlist[id] = mesh->vertices[i].coords[j];
                        id++;
                }
        }
        in.pointattributelist = (REAL *) malloc(in.numberofpoints *
                                                in.numberofpointattributes *
                                                sizeof(REAL));
        in.pointattributelist[0] = 0.0;
        in.pointattributelist[1] = 0.0;
        in.pointattributelist[2] = 0.0;
        in.pointattributelist[3] = 0.0;
        in.pointmarkerlist = (int *) malloc(in.numberofpoints * sizeof(int));
        in.pointmarkerlist[0] = 0;
        in.pointmarkerlist[1] = 0;
        in.pointmarkerlist[2] = 0;
        in.pointmarkerlist[3] = 0;

        in.numberofsegments = 0;
        in.numberofholes = 0;
        in.numberofregions = 0;
        in.regionlist = (REAL *) malloc(in.numberofregions * 4 * sizeof(REAL));

        mid.pointlist = (REAL *) NULL;            /* Not needed if -N switch used. */
        /* Not needed if -N switch used or number of point attributes is zero: */
        mid.pointattributelist = (REAL *) NULL;
        mid.pointmarkerlist = (int *) NULL; /* Not needed if -N or -B switch used. */
        mid.trianglelist = (int *) NULL;          /* Not needed if -E switch used. */
        /* Not needed if -E switch used or number of triangle attributes is zero: */
        mid.triangleattributelist = (REAL *) NULL;
        mid.neighborlist = (int *) NULL;         /* Needed only if -n switch used. */
        /* Needed only if segments are output (-p or -c) and -P not used: */
        mid.segmentlist = (int *) NULL;
        /* Needed only if segments are output (-p or -c) and -P and -B not used: */
        mid.segmentmarkerlist = (int *) NULL;
        mid.edgelist = (int *) NULL;             /* Needed only if -e switch used. */
        mid.edgemarkerlist = (int *) NULL;   /* Needed if -e used and -B not used. */

        vorout.pointlist = (REAL *) NULL;        /* Needed only if -v switch used. */
        /* Needed only if -v switch used and number of attributes is not zero: */
        vorout.pointattributelist = (REAL *) NULL;
        vorout.edgelist = (int *) NULL;          /* Needed only if -v switch used. */
        vorout.normlist = (REAL *) NULL;         /* Needed only if -v switch used. */

        /* Triangulate the points.  Switches are chosen to read and write a  */
        /*   PSLG (p), preserve the convex hull (c), number everything from  */
        /*   zero (z), assign a regional attribute to each element (A), and  */
        /*   produce an edge list (e), a Voronoi diagram (v), and a triangle */
        /*   neighbor list (n).                                              */

        triangulate("ze", &in, &mid, &vorout);

        for (int i = 0; i < mid.numberofedges; i++) {
                graph_connect(mesh, mid.edgelist[i*2], mid.edgelist[i*2+1]);
        }


}


void set_sweep_dirs(int *sweep_dirs, size_t dims)
{
        for(int i = 0; i < dims; i++) {
                sweep_dirs[i] += 2;
                if (sweep_dirs[i] <= 1) break;
                else                    sweep_dirs[i] = -1;
        }
}


bool solve_on_sweep(int* sweep, struct graph* mesh)
{
        double eps = 0.01;
        bool stop = true;
        for(int i = 0; i < mesh->cnt_vertices; i++){
                /* double res = solve_ndims(mesh,sweep[i]); */
                double res = solve_anis(mesh,sweep[i]);

                if ((mesh->vertices[sweep[i]].ux-res) > eps) {
                        mesh->vertices[sweep[i]].ux = res;
                        stop = false;
                }
        }

        for(int i = mesh->cnt_vertices - 1; i >=0 ; i--){
                /* double res = solve_ndims(mesh,sweep[i]); */
                double res = solve_anis(mesh,sweep[i]);

                if ((mesh->vertices[sweep[i]].ux-res) > eps) {
                        mesh->vertices[sweep[i]].ux = res;
                        stop = false;
                }
        }
        return stop;
}

void fsm(struct graph* mesh)
{

        int cnt_pt_ref = 3;
        int dim = 2;
        int ref_points[cnt_pt_ref][dim];
        /* FIXME: ref_point loaded from file */
        ref_points[0][0] = 0;
        ref_points[0][1] = 0;
        ref_points[1][0] = 20;
        ref_points[1][1] = 1;
        ref_points[2][0] = 3;
        ref_points[2][1] = 60;

        int** sweep = malloc(sizeof(int)*cnt_pt_ref);
        for(int i = 0; i<cnt_pt_ref; i++){
                sweep[i] = malloc(sizeof(int)*mesh->cnt_vertices);
                for(int j = 0; j < mesh->cnt_vertices;j++)
                        sweep[i][j]=j;
                sort(mesh->cnt_vertices,ref_points[i][0],ref_points[i][1],mesh->vertices, sweep[i]);
        }
        bool stop =false;
        int iterator = 0;
        while(!stop){
                printf("it:%d\n", iterator++);
                for(int id_ref = 0; id_ref<cnt_pt_ref; id_ref++){
                        stop = solve_on_sweep(sweep[id_ref], mesh);
                }
        }
}

