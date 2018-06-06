#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "triangle.h"



int main(int argc, char *argv[])
{
        struct triangulateio in, mid, out, vorout;

        /* Define input points. */

        in.numberofpoints = 7;
        in.numberofpointattributes = 1;
        in.pointlist = (REAL *) malloc(in.numberofpoints * 2 * sizeof(REAL));
        in.pointlist[0] = 0;
        in.pointlist[1] = 0;
        in.pointlist[2] = 1;
        in.pointlist[3] = 0;
        in.pointlist[4] = -sin(M_PI/6);
        in.pointlist[5] = -cos(M_PI/6);
        in.pointlist[6] =  sin(M_PI/6);
        in.pointlist[7] = -cos(M_PI/6);
        in.pointlist[8] = 1+sin(M_PI/6);
        in.pointlist[9] = -cos(M_PI/6);
        in.pointlist[10] = 0;
        in.pointlist[11] = -2*cos(M_PI/6);
        in.pointlist[12] = 1;
        in.pointlist[13] = -2*cos(M_PI/6);
     

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
        /* in.regionlist[0] = 0; */
        /* in.regionlist[1] = 0; */
        /* in.regionlist[2] = 0;            /\* Regional attribute (for whole mesh). *\/ */
        /* in.regionlist[3] = 0;          /\* Area constraint that will not be used. *\/ */

        /* Make necessary initializations so that Triangle can return a */
        /*   triangulation in `mid' and a voronoi diagram in `vorout'.  */

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
                printf("Edge %4d points:", i);
                for (int j = 0; j < 2; j++) {
                        printf("  %4d", mid.edgelist[i * 2 + j]);
                }
                printf("\n");
        }

        
        return 0;
}

