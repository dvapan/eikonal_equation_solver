#include "fsm.h"



void set_sweep_dirs(int *sweep_dirs, size_t dims)
{
        for(int i = 0; i < dims; i++) {
                sweep_dirs[i] += 2;
                if (sweep_dirs[i] <= 1) break;
                else                    sweep_dirs[i] = -1;
        }
}

bool sweep(struct eikonal_data *ed, size_t* pos, int *sweep_dirs, int n)
{
        bool stop = true;
        int len = gsl_vector_get(ed->curr_solve->sizes, n);
        if (n > 0)
                for (int i = 0; i < len; i++){
                        int _i = 0;
                        if (sweep_dirs[n] == 1) _i = i;
                        else                    _i = len - i - 1;
                        pos[n] = _i;
                        stop = sweep(ed, pos, sweep_dirs, n - 1);
                }
        else
                for (int i = 0; i < len; i++) {
                        int _i = 0;
                        if (sweep_dirs[n] == 1) _i = i;
                        else                    _i = len - i - 1;
                        pos[n] = _i;
                        double new_T = solve_eikonal(ed,pos);
                        if (new_T < ndvector_get(ed->curr_solve, pos)){
                                ndvector_set(ed->curr_solve, pos, new_T);
                                stop = false;
                        }
                }
        return stop;

}
