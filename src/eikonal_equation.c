#include "eikonal_equation.h"
#include "ndvector.h"
#include <math.h>
#include <stdlib.h>

void set_triangle(struct vertex* v, struct vertex* v1, struct vertex* v2,
                  double* a, double* b, double* c, double *al, double* bt)
{
        double b2c = 0;
        double a2c = 0;
        *a=*b=*c=0;
        for (int i = 0; i < v->cnt_dims; i++){
                *a += (v->coords[i] - v2->coords[i])*(v->coords[i] - v2->coords[i]);
                *b += (v->coords[i] - v1->coords[i])*(v->coords[i] - v1->coords[i]);
                *c += (v1->coords[i]- v2->coords[i])*(v1->coords[i]- v2->coords[i]);
                b2c+= (v->coords[i] - v2->coords[i])*(v1->coords[i]- v2->coords[i]);
                a2c+= (v->coords[i] - v1->coords[i])*(v2->coords[i]- v1->coords[i]);
        }
        *a = sqrt(*a);
        *b = sqrt(*b);
        *c = sqrt(*c);
        *al = acos(b2c/(*a*(*c)));
        *bt = acos(a2c/(*b*(*c)));
}

double solve_ndims(struct graph* mesh, int id)
{
        double min_ux = INFINITY;
        struct vertex* v = &mesh->vertices[id];
        struct link* ngbrs = graph_get_neighbours(mesh,id);
        int cnt_known = 0; int id_known = -1;
        for(struct link* t = ngbrs; t != NULL; t = t->next){
                for (struct link* r = t; r != NULL; r = r->next){
                        struct vertex* v1 = t->vertex;
                        struct vertex* v2 = r->vertex;

                        if (isfinite(t->vertex->ux) && isfinite(r->vertex->ux)){
                                if (t->vertex->ux < r->vertex->ux) {
                                        v1 = t->vertex;
                                        v2 = r->vertex;
                                } else {
                                        v2 = t->vertex;
                                        v1 = r->vertex;
                                }
                        } else if (isfinite(t->vertex->ux))
                        {
                                v1 = t->vertex;
                                v2 = r->vertex;

                        } else if (isfinite(r->vertex->ux))
                        {
                                v2 = t->vertex;
                                v1 = r->vertex;

                        }

                        double a,b,c,al,bt;
                        set_triangle(v,v1,v2,&a,&b,&c,&al,&bt);
                        /* printf("%lf %lf %lf | %lf %lf\n", a,b,c,al,bt); */
                        if (isfinite(v2->ux)&& isfinite(v1->ux) && r != t && abs(v2->ux - v1->ux) <= c/v->fx){
                                double th = asin(abs(v1->ux - v2->ux)/(c/v->fx));
                                double lcorn = (al - M_PI/2 > 0) ? al - M_PI/2 : 0;
                                double rcorn = (M_PI/2 - bt < 0) ? M_PI/2 - bt : 0;
                                if (lcorn <= th && th <= M_PI/2 - bt || al - M_PI/2 <= th <= rcorn){
                                        double h = a*sin(al - th);
                                        double H = b*sin(bt + th);
                                        double new_ux = 0.5*(h/v->fx + v2->ux) +
                                                0.5*(H/v->fx + v1->ux);
                                        if (new_ux < min_ux)
                                                min_ux = new_ux;
                                }
                                else if (isfinite(v1->ux) && v1->ux + b/v->fx < min_ux)
                                        min_ux = v1->ux + b/v->fx;
                                else if (isfinite(v2->ux) && v2->ux + a/v->fx < min_ux)
                                        min_ux = v2->ux + a/v->fx;
                        }
                        else if (isfinite(v1->ux) && v1->ux + b/v->fx < min_ux)
                                min_ux = v1->ux + b/v->fx;
                        else if (isfinite(v2->ux) && v2->ux + a/v->fx < min_ux)
                                min_ux = v2->ux + a/v->fx;

                }
        }
        return min_ux;

}



double vg(struct vertex* C, struct vertex* A, struct vertex* B, double s, double* value)
{
        double Ax = A->coords[0], Ay = A->coords[1];
        double Bx = B->coords[0], By = B->coords[1];
        double Cx = C->coords[0], Cy = C->coords[1];
        double F_s_x= Ax*s + Bx*(1-s);
        double F_s_y= Ay*s + By*(1-s);
        double e_x = Cx  - F_s_x, e_y = Cy - F_s_y;
        double len = sqrt(e_x*e_x+e_y*e_y);
        e_x/=len;e_y/=len;
        return sqrt(pow(e_x*(value[0]+value[2]),2) +pow(e_y*(value[1]+value[2]),2));
}

                    
double solve_anis(struct graph* mesh, int id)
{
        double value[4] = {1,0,0,2};
        double min_ux = INFINITY;
        struct vertex* v = &mesh->vertices[id];
        struct link* ngbrs = graph_get_neighbours(mesh,id);
        int cnt_known = 0; int id_known = -1;
        for(struct link* t = ngbrs; t != NULL; t = t->next){
                for (struct link* r = t; r != NULL; r = r->next){
                        struct vertex* v1 = t->vertex;
                        struct vertex* v2 = r->vertex;
                        double new_ux;
                        double a,b,c,al,bt;
                        set_triangle(v,v1,v2,&a,&b,&c,&al,&bt);

                        if (isfinite(t->vertex->ux) && isfinite(r->vertex->ux)){
                                for (double s = 0; s<=1;s+=0.1){
                                        double ds = sqrt(b*b+c*c*s*s-2*b*c*s*cos(al));
                                        new_ux = s*v2->ux + (1-s)*v1->ux + ds/vg(v, v1,v2,s,value);
                                        /* printf("%lf,%lf :: %lf, %lf: %lf\n",ux ); */
                                        if (new_ux< min_ux)
                                                min_ux = new_ux;
                                }
                        } else if (isfinite(t->vertex->ux))
                        {
                                double ds = sqrt(b*b+c*c*-2*b*c*cos(al));
                                new_ux = v1->ux + ds/vg(v, v1,v2,0,value);


                        } else if (isfinite(r->vertex->ux))
                        {
                                double ds = sqrt(b*b);
                                new_ux = v2->ux + ds/vg(v, v1,v2,1,value);
                        }
                        else
                                continue;
                        if (new_ux < min_ux)
                                min_ux = new_ux;


                }
        }
        return min_ux;

}


