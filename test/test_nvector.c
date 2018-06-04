#include <assert.h>

#include "ndvector.h"
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


int main(int argc, char *argv[])
{
    test_nvector_alloc();
    test_nvector_calloc();
    test_nvector_get_set();
    test_nvector_get_set_3d();
    test_ndvector_get_size();
    return EXIT_SUCCESS;
}

