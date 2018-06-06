#ifndef FSM
#define FSM
#include <stddef.h>
#include <stdbool.h>
#include "eikonal_equation.h"

void triag_mesh(struct graph* mesh);
void sort(int cnt_vertices, double x1, double x2, struct vertex* vertices, int* sweep);

/* void set_sweep_dirs(int* sweep_dirs, size_t dims); */
/* bool sweep(struct eikonal_data *ed, size_t *pos, int* sweep_dirs, int n); */

/* void fsm(struct eikonal_data *ed); */

#endif
