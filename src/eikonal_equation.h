#ifndef EIKONAL_EQUATION
#define EIKONAL_EQUATION

#include <math.h>
#include "ndvector.h"

//TODO: Необходимо из node_data вытащить T_vals, поскольку - это внутренняя штука для solve_ndims

struct eikonal_data
{
        int dims;
        double step;
        struct ndvector* velocites;
        struct ndvector* curr_solve;
};

struct node_data
{
        int dims;
        size_t* pos;
};

struct eikonal_data *eikonal_data_alloc(size_t dims, size_t* sizes, double step);
struct node_data *node_data_alloc(size_t dims);

void node_data_set(struct node_data *nd, double* pos, double* T_vals);
int eikonal_data_dims(struct eikonal_data *ed);
double solve_ndims(struct eikonal_data *ed, struct node_data *nd, size_t dim, double* T_vals);
double solve_eikonal(struct eikonal_data *ed, struct node_data *nd);


#endif
