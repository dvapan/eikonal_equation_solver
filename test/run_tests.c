#include <assert.h>
#include <glib.h>
#include <glib/gi18n.h>
#include <math.h>

#include "eikonal_equation.h"
#include "ndvector.h"

#include <assert.h>
#include <math.h>

#include "eikonal_equation.h"
#include "ndvector.h"
#include "fsm.h"


static void test_nvector_alloc() {
        int dimensions = 2;
        size_t sizes[dimensions];
        sizes[0] = 10;
        sizes[1] = 15;
        struct ndvector* nv = ndvector_alloc(dimensions, sizes);
        assert (ndvector_dimensions(nv) == 2 && "Compare dimensions");
}

static void test_nvector_get_set() {
        int dimensions = 2;
        size_t sizes[dimensions];
        sizes[0] = 3;
        sizes[1] = 4;
        struct ndvector* nv = ndvector_alloc(dimensions, sizes);
        for (int i = 0; i < sizes[0]; i++)
                for(int j = 0; j < sizes[1]; j++){
                        size_t vec[dimensions];
                        vec[0] = i; vec[1] = j;
                        ndvector_set(nv,vec,i*j);
                }
        for (int i = 0; i < sizes[0]; i++)
                for(int j = 0; j < sizes[1]; j++){
                        size_t vec[dimensions];
                        vec[0] = i; vec[1] = j;
                        assert(ndvector_get(nv,vec) == i*j);
                }
}

static void test_nvector_get_set_3d() {
        int dimensions = 3;
        size_t sizes[dimensions];
        sizes[0] = 3;
        sizes[1] = 4;
        sizes[2] = 5;
        struct ndvector* nv = ndvector_alloc(dimensions, sizes);
        for (int i = 0; i < sizes[0]; i++)
                for(int j = 0; j < sizes[1]; j++)
                        for(int k = 0; k < sizes[2]; k++){
                                size_t vec[dimensions];
                                vec[0] = i; vec[1] = j; vec[2] = k;
                                ndvector_set(nv,vec,i*j*k);

                }
        for (int i = 0; i < sizes[0]; i++)
                for(int j = 0; j < sizes[1]; j++)
                        for(int k = 0; k < sizes[2]; k++){
                                size_t vec[dimensions];
                                vec[0] = i; vec[1] = j; vec[2] = k;
                                assert(ndvector_get(nv,vec) == i*j*k);
                }
}


static void test_nvector_calloc() {
        int dimensions = 3;
        size_t sizes[dimensions];
        sizes[0] = 3;
        sizes[1] = 4;
        sizes[2] = 5;
        struct ndvector* nv = ndvector_calloc(dimensions, sizes);
        for(int i = 0; i < sizes[0]; i++ )
                for (int j = 0; j < sizes[1]; j++)
                        for (int k = 0; k < sizes[2]; k++){
                                size_t vec[dimensions];
                                vec[0] = i; vec[1] = j; vec[2] = k;
                                assert(ndvector_get(nv, vec) == 0);
                        }

}

void test_ndvector_get_size()
{
        int dimensions = 2;
        size_t sizes[dimensions];
        sizes[0] = 3;
        sizes[1] = 4;
        struct ndvector* nv = ndvector_alloc(dimensions, sizes);
        assert(ndvector_get_size(nv, 0) == 3);
        assert(ndvector_get_size(nv, 1) == 4);
}


static void test_eikonal_equation_data()
{
        int dims = 2;
        size_t sizes[dims];
        sizes[0] = 10;
        sizes[1] = 15;
        struct eikonal_data *ed = eikonal_data_alloc(dims, sizes, 0.1);
        assert(eikonal_data_dims(ed) == 2);
        struct ndvector *vec = ed->velocites;
        assert(ndvector_dimensions(vec) == 2);
        vec = ed->curr_solve;
        assert(ndvector_dimensions(vec) == 2);

}

static void test_solve_ndims_one_dimension()
{
        size_t dims = 1;
        double T_vals[dims];
        T_vals[0] = 1.0;
        size_t pos[dims];
        pos[0] = 1;
        size_t size[1];
        size[0] = 3;
        struct ndvector *F = ndvector_alloc(1, size);
        size_t vec[1];
        vec[0] = 0;
        ndvector_set(F, vec, 1.0);
        vec[0] = 1;
        ndvector_set(F, vec, 1.0);
        vec[0] = 2;
        ndvector_set(F, vec, 1.0);
        struct eikonal_data* ed = eikonal_data_alloc(dims, size, 0.1);
        double res = solve_ndims(ed,pos,1, T_vals);
        assert(abs(res - 1.1) < 0.01);
}

void test_T_vals_2d(struct eikonal_data* ed,
                 double t1, double t2, double asrtres)
{
        size_t pos[ed->dims];
        pos[0] = 2;
        pos[1] = 2;
        double T_vals[ed->dims];
        T_vals[0] = t1;
        T_vals[1] = t2;
        double res = solve_ndims(ed,pos,2, T_vals);
//        printf ("\n%lf %lf",res,asrtres);
        assert(isfinite(res)||(isinf(asrtres)&&isinf(res)));
        assert(abs(res - asrtres) < 0.001);
}

static void test_solve_ndims_two_dimensions()
{

        size_t dims = 2;
        size_t pos[dims];
        pos[0] = 2;
        pos[1] = 2;
        size_t size[dims];
        size[0] = 5;
        size[1] = 5;
        struct eikonal_data* ed = eikonal_data_alloc(dims, size, 1.0);
        size_t vec[dims];
        for (int i = 0; i < size[0]; i++)
            for (int j = 0; j < size[1]; j++){
                vec[0] = i;
                vec[1] = j;
                ndvector_set(ed->velocites, vec, 1.0);
                ndvector_set(ed->curr_solve, vec, INFINITY);
            }        

        test_T_vals_2d(ed,1.0,1.0,1.707);

//        test_T_vals_2d(ed,nd,INFINITY,1.0,1.00);

        test_T_vals_2d(ed,2.0,1.707,2.545329);

        test_T_vals_2d(ed,3.0,1.0,INFINITY);

}

void test_solve_eikonal()
{
        size_t dims = 2;
        size_t pos[dims];
        pos[0] = 2;
        pos[1] = 2;
        size_t size[dims];
        size[0] = 5;
        size[1] = 5;
        struct eikonal_data* ed = eikonal_data_alloc(dims, size, 1.0);
        size_t vec[dims];
        for (int i = 0; i < size[0]; i++)
            for (int j = 0; j < size[1]; j++){
                vec[0] = i;
                vec[1] = j;
                ndvector_set(ed->velocites, vec, 1.0);
                ndvector_set(ed->curr_solve, vec, INFINITY);
            }
        vec[0] = 2;
        vec[1] = 2;
        ndvector_set(ed->curr_solve, vec, 0);
        vec[0] = 1;
        vec[1] = 2;
        ndvector_set(ed->curr_solve, vec, 1);
        vec[0] = 2;
        vec[1] = 1;
        ndvector_set(ed->curr_solve, vec, 1);


        pos[0] = 1;
        pos[1] = 1;

        double new_T = solve_eikonal(ed, pos);

        assert(isfinite(new_T));
        assert(abs(new_T - 1.707) < 0.001);

        pos[0] = 2;
        pos[1] = 3;

        new_T = solve_eikonal(ed, pos);

        assert(isfinite(new_T));
        assert(abs(new_T - 1.0) < 0.001);

        pos[0] = 1;
        pos[1] = 3;

        new_T = solve_eikonal(ed, pos);

        assert(isfinite(new_T));
        assert(abs(new_T - 2.0) < 0.001);

}

static void test_set_sweep_dirs()
{
        size_t dims = 3;
        int sweep_dirs[dims];
        for (int i = 0; i < dims; i++)
                sweep_dirs[i] = 1;
        set_sweep_dirs(sweep_dirs, dims);
        for (int i = 0; i < dims; i++)
                assert(sweep_dirs[i] == -1);
        set_sweep_dirs(sweep_dirs, dims);
        assert(sweep_dirs[0] == 1);
        assert(sweep_dirs[1] == -1);
        assert(sweep_dirs[2] == -1);
        set_sweep_dirs(sweep_dirs, dims);
        assert(sweep_dirs[0] == -1);
        assert(sweep_dirs[1] == 1);
        assert(sweep_dirs[2] == -1);

}

static void test_sweep()
{
        size_t dims = 2;
        size_t size[dims];
        size[0] = 5;
        size[1] = 5;
        struct eikonal_data* ed = eikonal_data_alloc(dims, size, 1.0);
        size_t vec[dims];
        for (int i = 0; i < size[0]; i++)
            for (int j = 0; j < size[1]; j++){
                vec[0] = i;
                vec[1] = j;
                ndvector_set(ed->velocites, vec, 1.0);
                ndvector_set(ed->curr_solve, vec, INFINITY);
            }
        vec[0] = 2;
        vec[1] = 2;
        ndvector_set(ed->curr_solve, vec, 0);
        int sweep_dirs[dims];
        for (int i = 0; i < dims; i++)
                sweep_dirs[i] = 1;
        set_sweep_dirs(sweep_dirs, dims);
        size_t pos[dims];
        bool rs =sweep(ed, pos, sweep_dirs, dims - 1);

        vec[0] = 2;
        vec[1] = 2;
        assert(ndvector_get(ed->curr_solve,vec) == 0);
        vec[0] = 1;
        vec[1] = 2;
        assert(ndvector_get(ed->curr_solve,vec) == 1.0);
        vec[0] = 0;
        vec[1] = 2;
        assert(ndvector_get(ed->curr_solve,vec) == 2.0);
        vec[0] = 2;
        vec[1] = 1;
        assert(ndvector_get(ed->curr_solve,vec) == 1.0);
        vec[0] = 2;
        vec[1] = 0;
        assert(ndvector_get(ed->curr_solve,vec) == 2.0);

        set_sweep_dirs(sweep_dirs, dims);
        rs =sweep(ed, pos, sweep_dirs, dims - 1);

        vec[0] = 2;
        vec[1] = 2;
        assert(ndvector_get(ed->curr_solve,vec) == 0);
        vec[0] = 1;
        vec[1] = 1;
        assert(abs(ndvector_get(ed->curr_solve,vec) - 1.707) < 0.001);
        vec[0] = 3;
        vec[1] = 3;
        assert(abs(ndvector_get(ed->curr_solve,vec) - 2.0) < 0.001);
        vec[0] = 3;
        vec[1] = 1;
        assert(abs(ndvector_get(ed->curr_solve,vec) - 1.707) < 0.001);
        vec[0] = 1;
        vec[1] = 3;
        assert(abs(ndvector_get(ed->curr_solve,vec) - 1.707) < 0.001);

}

int main(int argc, char *argv[])
{
        test_nvector_alloc();
        test_nvector_calloc();
        test_nvector_get_set();
        test_nvector_get_set_3d();
        test_ndvector_get_size();

        test_eikonal_equation_data();

        test_solve_ndims_one_dimension();
        test_solve_ndims_two_dimensions();
        test_solve_eikonal();

        test_set_sweep_dirs();
        test_sweep();
        return EXIT_SUCCESS;
}

