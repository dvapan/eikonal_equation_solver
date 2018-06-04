#ifndef EIKONAL_EQUATION
#define EIKONAL_EQUATION

#include <math.h>
#include "ndvector.h"

struct eikonal_data
{
        int dims;
        double step;
        struct ndvector* velocites;
        struct ndvector* curr_solve;
};


struct eikonal_data *eikonal_data_alloc(size_t dims, size_t* sizes, double step);

int eikonal_data_dims(struct eikonal_data *ed);
double solve_ndims(struct eikonal_data *ed, size_t *pos, size_t dim, double* T_vals);
double solve_eikonal(struct eikonal_data *ed,size_t *pos);


#endif
