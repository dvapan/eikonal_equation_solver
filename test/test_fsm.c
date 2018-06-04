#include "fsm.h"
#include <assert.h>
#include <glib.h>
#include <glib/gi18n.h>


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

static void test_fsm()
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
    fsm(ed);

    double expected_res[5][5] =
    {{3.25,2.55,2.00,2.55,3.25},
     {2.55,1.71,1.00,1.71,2.55},
     {2.00,1.00,0.00,1.00,2.00},
     {2.55,1.71,1.00,1.71,2.55},
     {3.25,2.55,2.00,2.55,3.25}};

    size_t height = gsl_vector_get (ed->curr_solve->sizes,0);
    size_t width = gsl_vector_get(ed->curr_solve->sizes,1);
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            vec[0] = i;
            vec[1] = j;
            assert(abs(expected_res[i][j] - ndvector_get(ed->curr_solve,vec)) < 0.1);
        }

    }

}

int main(int argc, char *argv[])
{
    test_set_sweep_dirs();
    test_sweep();

    test_fsm();
    return EXIT_SUCCESS;
}
