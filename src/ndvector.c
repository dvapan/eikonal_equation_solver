#include "ndvector.h"

struct ndvector* ndvector_alloc(size_t dimensions, size_t* sizes)
{
        struct ndvector *nv = malloc(sizeof(struct ndvector));
        nv->dimensions = dimensions;
        nv->sizes = gsl_vector_alloc(dimensions);
        size_t phys_dim = 1;
        for (int i = 0; i < dimensions; i++) {
                gsl_vector_set(nv->sizes, i, sizes[i]);
                phys_dim *= sizes[i];
        }
        nv->data = gsl_vector_alloc(phys_dim);
	return nv;
}

struct ndvector* ndvector_calloc(size_t dimensions, size_t* sizes)
{
        struct ndvector *nv = malloc(sizeof(struct ndvector));
        nv->dimensions = dimensions;
        nv->sizes = gsl_vector_alloc(dimensions);
        size_t phys_dim = 1;
        for (int i = 0; i < dimensions; i++) {
                gsl_vector_set(nv->sizes, i, sizes[i]);
                phys_dim *= sizes[i];
        }
        nv->data = gsl_vector_calloc(phys_dim);
        return nv;
}

int coords_to_vec(gsl_vector* sizes, size_t dimensions, size_t* vec)
{
        int mult_coord = 1;
        int real_coord = vec[dimensions - 1];
//        printf("[%d, ", real_coord);
        for (int i = 1; i < dimensions; i++) {
                if (vec[dimensions - 1 - i] < 0 ||
                    vec[dimensions - 1 - i] >= gsl_vector_get(sizes, dimensions - 1 - i)){
                        printf("EES: %d size boundary error %d not in [0,%d]\n", i,
                               vec[dimensions - 1 - i],
                               gsl_vector_get(sizes, dimensions - 1 - i));
                        exit(EXIT_FAILURE);
                }
                mult_coord *= gsl_vector_get(sizes, dimensions - 1 - i + 1);
                real_coord += vec[dimensions - 1 - i] * mult_coord;
//                printf("%d, ",vec[dimensions - 1 - i]);
        }
//        printf("]: %d\n",real_coord);
        return real_coord;
}


double ndvector_get(struct ndvector* nvec,
                        size_t* vec)
{
//        printf("get::");
        int real_coord = coords_to_vec(nvec->sizes, nvec->dimensions, vec);
        return gsl_vector_get(nvec->data, real_coord);
}

void ndvector_set(struct ndvector* nvec,
                      size_t* vec,
                      double val)
{
//        printf("set::");
        int real_coord = coords_to_vec(nvec->sizes, nvec->dimensions, vec);
        gsl_vector_set(nvec->data, real_coord, val);
}



size_t ndvector_dimensions(struct ndvector* nvec)
{
        return nvec->dimensions;
}

int ndvector_get_size(struct ndvector* nv, int dim)
{
        if (dim < 0 || dim >= nv->dimensions){
                printf("ndvector::error: dim not in [0, %d]", nv->dimensions);
                exit(EXIT_FAILURE);
        }
        return gsl_vector_get(nv->sizes, dim);
}

void ndvector_free(struct ndvector* nvec)
{
        gsl_vector_free(nvec->sizes);
        gsl_vector_free(nvec->data);
        free(nvec);
}

