#ifndef BOIDS_HPP
#define BOIDS_HPP

#include "misc.h"

namespace boids {
    #define LEN(x, y) sqrt(SQR(x) + SQR(y))
    #define DIST(x1, y1, x2, y2) LEN(((x1) - (x2)), ((y1) - (y2)))
    #define DOT(x1, y1, x2, y2) ((x1) * (x2) + (y1) * (y2))

    struct Params
    {
        int width;
        int height;
        int num;
        int len;
        int mag;
        int seed;
        int invert;
        int steps;
        int psdump;

        double angle;
        double vangle;
        double minv;
        double ddt;
        double dt;
        double rcopy;
        double rcent;
        double rviso;
        double rvoid;
        double wcopy;
        double wcent;
        double wviso;
        double wvoid;
        // double  wrand = 0.0;   // eliminate for simplicity

        int threads; // will ignore for openACC version; used for multicore

        char *term;
    };

    void norm(float* x, float* y);

    void compute_new_headings(struct Params p, float* xp, float* yp, float* xv, float* yv, float* xnv, float* ynv);

    // Remnant from psplot version
    // void draw_boid(struct Params p, int which, int color, float *xp, float *yp, float *xv, float *yv);

    OPTION* setOptions(struct Params& params);

}
#endif