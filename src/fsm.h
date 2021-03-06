#ifndef FSM
#define FSM
#include <stddef.h>
#include <stdbool.h>
#include "eikonal_equation.h"

void triag_mesh(struct graph* mesh);
void sort(int cnt_vertices, double x1, double x2, struct vertex* vertices, int* sweep);

void fsm(struct graph* mesh); 

#endif
