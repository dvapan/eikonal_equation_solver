#include "eikonal_equation.h"
#include "ndvector.h"
#include <math.h>
#include <stdlib.h>

struct eikonal_data* eikonal_data_alloc(size_t dims, size_t* sizes, double step)
{
        struct eikonal_data *ed = malloc(sizeof(struct eikonal_data));
        ed->dims = dims;
        ed->curr_solve = ndvector_alloc(dims, sizes);
        ed->velocites = ndvector_alloc(dims, sizes);
        ed->step = step;
        return ed;
}

int eikonal_data_dims(struct eikonal_data *ed)
{
        return ed->curr_solve->dimensions;
}

double solve_ndims(struct eikonal_data *ed, struct node_data *nd, size_t dims, double *T_vals)
{
        double coeff = ed->step/ndvector_get(ed->velocites,nd->pos);
        if (dims == 1)
                return T_vals[0] + coeff;
        double sumT = 0;
        double sqr_sumT = 0;
        for (int i = 0; i < nd->dims; i++)
                if (isfinite(T_vals[i])){
                        sumT += T_vals[i];
                        sqr_sumT += T_vals[i]*T_vals[i];
                }
        double a = dims;
        double b = -2*sumT;
        double c = sqr_sumT - pow(coeff,2);
        double q = b*b - 4*a*c;
        if (q<0){
                return INFINITY;
        }
        else
                return (-b + sqrt(q))/(2*a);
}

void sort(double* a, size_t dim)
{
        for (int i = 0; i < dim; i++){
                int min = i;
                for(int j = i+1; j < dim; j++)
                        if (isfinite(a[j]) && a[j]<a[min])
                                min = j;
                double tmp = a[i];
                a[i] = a[min];
                a[min] = tmp;

        }
}

double solve_eikonal(struct eikonal_data *ed, struct node_data *nd)
{
        size_t cnt_finite_T_vals = ed->dims;
        double T_vals[ed->dims];
        for (int dim = 0; dim < ed->dims; dim++) {
                nd->pos[dim] += 1;
                double l_val = ndvector_get(ed->curr_solve,nd->pos);
                nd->pos[dim] -= 2;
                double g_val = ndvector_get(ed->curr_solve,nd->pos);
                nd->pos[dim] += 1;
                double min_val = l_val < g_val ? l_val : g_val;
                T_vals[dim] = min_val;
                if (!isfinite(min_val))
                        cnt_finite_T_vals--;
        }

        if (cnt_finite_T_vals == 0) return INFINITY;
        size_t counter = 0;
        sort(T_vals,nd->dims);

        double res = INFINITY;
        for (int dim = 1; dim <= cnt_finite_T_vals; dim++) {
                res = solve_ndims(ed, nd, dim, T_vals);
                if (dim == cnt_finite_T_vals ||
                    res < ndvector_get(ed->curr_solve, nd->pos))
                        break;
        }
        return res;
}

struct node_data *node_data_alloc(size_t dims)
{
        struct node_data *nd = malloc(sizeof(struct node_data));
        nd->pos = malloc(sizeof(size_t)*dims);
        nd->dims = dims;
}

