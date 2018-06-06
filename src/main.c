#include <stdio.h>
#include <unistd.h>
#include "fsm.h"
#include "graph.h"
#include <math.h>


int main(int argc, char *argv[])
{
        FILE *ifp, *ofp;
        char output_filename[] = "out.dat";
        char *input_filename;
        int dims = 2;
        input_filename = argv[1];
        ifp = fopen(input_filename, "r");

        if (ifp == NULL) {
                fprintf(stderr, "Can't open input file in.list!\n");
                exit(1);
        }

        /* Input file format:
           #(vertex count) #(reference points) *
           x[0]..x[n] fx ux } #(vertex count) times
           x[0]..x[n] } #(reference points)
        */

        int cnt_vertices, cnt_pt_ref;
        fscanf(ifp,"%d%d",&cnt_vertices, &cnt_pt_ref);

        double *coords[cnt_vertices];
        double  values[cnt_vertices];
        double  init_u[cnt_vertices];

        for(int i = 0; i < cnt_vertices; i++)
        {
                double x1, x2, fx, ux;
                coords[i] = malloc(sizeof(double)* 2);
                int cnt = fscanf(ifp,"%lf%lf%lf%lf", &x1, &x2,&fx,&ux);
                coords[i][0] = x1;
                coords[i][1] = x2;
                values[i] = fx;
                if(cnt >= 4)
                        init_u[i] = ux;
                else
                        init_u[i] = INFINITY;
        }
        fclose(ifp);

        struct graph* mesh = make_graph(cnt_vertices,dims, coords, values, init_u);

        triag_mesh(mesh);
        fsm(mesh);
        ofp = fopen(output_filename, "w");

        if (ofp == NULL) {
                fprintf(stderr, "Can't open output file %s!\n",
                        output_filename);
                exit(EXIT_FAILURE);
        }

        for(int i = 0; i< cnt_vertices;i++){
                struct vertex* v = &mesh->vertices[i];
                if(i>0 && v->coords[1] == 0) fprintf(ofp,"\n");
                fprintf(ofp,"% lf % lf % lf\n", v->coords[0],v->coords[1],v->ux);
        }
        fclose(ofp);
        return EXIT_SUCCESS;
}
