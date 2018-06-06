#include "fsm.h"
#include "triangle.h"
#include <math.h>



double distance(double x11, double x12, double x21, double x22)
{
        /* printf("[%lf %lf] - [%lf %lf] = sqrt(%lf) = %lf\n",x11,x12,x21,x22, */
               /* pow(x21-x11,2)+ pow(x22-x12,2),sqrt(pow(x21-x11,2)+ pow(x22-x12,2))); */
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
                                /* printf("LOL\n"); */
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

/* bool sweep(struct eikonal_data *ed, size_t* pos, int *sweep_dirs, int n) */
/* { */
/*         bool stop = true; */
/*         int len = gsl_vector_get(ed->curr_solve->sizes, n); */
/*         if (n > 0) */
/*                 for (int i = 0; i < len; i++){ */
/*                         int _i = 0; */
/*                         if (sweep_dirs[n] == 1) _i = i; */
/*                         else                    _i = len - i - 1; */
/*                         pos[n] = _i; */
/*                         stop = sweep(ed, pos, sweep_dirs, n - 1); */
/*                 } */
/*         else */
/*                 for (int i = 0; i < len; i++) { */
/*                         int _i = 0; */
/*                         if (sweep_dirs[n] == 1) _i = i; */
/*                         else                    _i = len - i - 1; */
/*                         pos[n] = _i; */
/*                         double new_T = solve_eikonal(ed,pos); */
/*                         if (new_T < ndvector_get(ed->curr_solve, pos)){ */
/*                                 ndvector_set(ed->curr_solve, pos, new_T); */
/*                                 stop = false; */
/*                         } */
/*                 } */
/*         return stop; */

/* } */

/* void fsm(struct eikonal_data *ed) */
/* { */
/*         int sweep_dirs[ed->dims]; */
/*         for (int i = 0; i < ed->dims; i++) */
/*                 sweep_dirs[i] = 1; */
/*         size_t pos[ed->dims]; */
/*         bool stop = false; */
/*         while (!stop) { */
/*                 set_sweep_dirs(sweep_dirs, ed->dims); */
/*                 stop = sweep(ed,pos,sweep_dirs, ed->dims - 1); */
/*         } */
/* } */

