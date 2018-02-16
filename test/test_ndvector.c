#include <assert.h>
#include <glib.h>
#include <glib/gi18n.h>

#include "ndvector.h"

/* static void test_solving_in_node() */
/* { */
/*         struct eikonal_data data */
/*         int dim_1 = 5; dim_2 = 5; */

/*         data.dimensions = 2; */
/*         data.velocities = gsl_matrix_alloc(dim_1, dim_2); */
/*         data.curr_solve = gsl_matrix_alloc(dim_1, dim_2); */

/*         for(int i = 0; i < dim_1; i++) */
/*                 for(int j = 0; j < dim_2; j++) { */
/*                         gsl_matrix_set(F,i,j,1); */
/*                         gsl_matrix_set(T,i,j,INFINITY); */
/*                 } */
/*         gsl_matrix_set(T,2,2,0); */
/*         int node[2]; */
/*         node[0] = 1; */
/*         node[1] = 2; */
/*         double time = solve_in_node(data,node); */
/*         g_assert_cmpfloat(abs(time - 1.0), < ,0.0001); */

/* } */

static void test_nvector_alloc() {
        int dimensions = 2;
        size_t sizes[dimensions];
        sizes[0] = 10;
        sizes[1] = 15;
        struct ees_ndvector* nv = ees_ndvector_alloc(dimensions, sizes);
        g_assert_cmpint(ees_ndvector_dimensions(nv), == ,2);
}

static void test_nvector_get_set() {
        int dimensions = 2;
        size_t sizes[dimensions];
        sizes[0] = 3;
        sizes[1] = 4;
        struct ees_ndvector* nv = ees_ndvector_alloc(dimensions, sizes);
        for (int i = 0; i < sizes[0]; i++)
                for(int j = 0; j < sizes[1]; j++){
                        size_t vec[dimensions];
                        vec[0] = i; vec[1] = j;
                        ees_ndvector_set(nv,vec,i*j);
                }
        for (int i = 0; i < sizes[0]; i++)
                for(int j = 0; j < sizes[1]; j++){
                        size_t vec[dimensions];
                        vec[0] = i; vec[1] = j;
                        g_assert_cmpint(ees_ndvector_get(nv,vec), ==, i*j);
                }
}

static void test_nvector_get_set_3d() {
        int dimensions = 3;
        size_t sizes[dimensions];
        sizes[0] = 3;
        sizes[1] = 4;
        sizes[2] = 5;
        struct ees_ndvector* nv = ees_ndvector_alloc(dimensions, sizes);
        for (int i = 0; i < sizes[0]; i++)
                for(int j = 0; j < sizes[1]; j++)
                        for(int k = 0; k < sizes[2]; k++){
                                size_t vec[dimensions];
                                vec[0] = i; vec[1] = j; vec[2] = k;
                                ees_ndvector_set(nv,vec,i*j*k);

                }
        for (int i = 0; i < sizes[0]; i++)
                for(int j = 0; j < sizes[1]; j++)
                        for(int k = 0; k < sizes[2]; k++){
                                size_t vec[dimensions];
                                vec[0] = i; vec[1] = j; vec[2] = k;
                                g_assert_cmpint(ees_ndvector_get(nv,vec), ==, i*j*k);
                }
}


static void test_nvector_calloc() {
        int dimensions = 3;
        size_t sizes[dimensions];
        sizes[0] = 3;
        sizes[1] = 4;
        sizes[2] = 5;
        struct ees_ndvector* nv = ees_ndvector_calloc(dimensions, sizes);
        for(int i = 0; i < sizes[0]; i++ )
                for (int j = 0; j < sizes[1]; j++)
                        for (int k = 0; k < sizes[2]; k++){
                                size_t vec[dimensions];
                                vec[0] = i; vec[1] = j; vec[2] = k;
                                g_assert_cmpint(ees_ndvector_get(nv, vec), == ,0);
                        }

}

int main(int argc, char *argv[])
{
        g_test_init(&argc, &argv, NULL);
        g_test_add_func("/ndvector/alloc", test_nvector_alloc);
        g_test_add_func("/ndvector/calloc", test_nvector_calloc);
        g_test_add_func("/ndvector/get_set/2d", test_nvector_get_set);
        g_test_add_func("/ndvector/get_set/3d", test_nvector_get_set);
        return g_test_run();
}

