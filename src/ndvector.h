/**
   \todo: add commentary
 */

#ifndef NDVECTOR
#define NDVECTOR

#include <stddef.h>
#include <gsl/gsl_vector.h>

struct ndvector
{
        int dimensions;
        gsl_vector* sizes;
        gsl_vector* data;
};

struct ndvector* ndvector_alloc(size_t dimensions,
                                        size_t* sizes);
struct ndvector* ndvector_calloc(size_t dimensions,
                                        size_t* sizes);

void ndvector_free(struct ndvector* nvec);
size_t ndvector_dimensions(struct ndvector* nvec);
double ndvector_get(struct ndvector* nvec,
                        size_t* vec);
void ndvector_set(struct ndvector* nvec,
                      size_t* vec,
                      double val);
int ndvector_get_size(struct ndvector* nv, int dim);

#endif
