#ifndef EIKONAL_EQUATION
#define EIKONAL_EQUATION

#include <math.h>
#include "ndvector.h"
#include "graph.h"

/* struct eikonal_data */
/* { */
/*         size_t dims; */
/*         struct graph* mesh */
/* }; */


/* struct eikonal_data *make_eikonal_data(size_t dims, size_t* sizes, double step); */

/* int eikonal_data_dims(struct eikonal_data *ed); */
double solve_ndims(struct graph *mesh, int id);
double solve_anis(struct graph* mesh, int id);

/* double solve_eikonal(struct eikonal_data *ed,size_t *pos); */


#endif
