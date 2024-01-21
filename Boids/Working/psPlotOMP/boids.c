
/* NAME
 *   boids - simulate a flock of android birds from Brooklyn
 * NOTES
 *   (LS) This version eliminates the use of global
 *   variables.
 *
 *   The compute_new_heading() function is somewhat monolithic,
 *   but it is well documented at least.  Change it with some care.
 * RULES
 *   All of the rules have a weight option and a radius option.
 *   The radius option specifies how close a boid need to be to
 *   another in order for the rule to be acted upon.  The weight
 *   is used when combining all of the rules actions into a single
 *   new velocity vector.
 *
 *   The four rules can be simply described as follows:
 *
 *      Centering: move towards the center of any boids in my
 *                 viewing area.
 *
 *      Copying: attempt to move in the average direction of
 *               that all boids that can be seen are moving
 *               in.
 *
 *      Avoidance: ``Please don't stand so close to me.''
				  Move away from any close flyers.
 *
 *      Visual: move in such a way that the bonehead
 *              obstructing your view no longer interferes.
 *
 *   The four rules are then normalized and added together to make
 *   the next velocity vector of the boid.  All radii in the rules
 *   are in terms of pixels.
 *
 *   You may wish to try turning on and off different combinations
 *   of the rules to see how the boids' behaviors change.  For example,
 *   if you turn off the avoidance rule, increase the centering radius
 *   and weight, and increase the viewing angle to nearly 360 degrees,
 *   then the boids will behave like a pack of wolves fighting over
 *   the center.  Other changes can yield similar surprises.
 * BUGS
 *   No sanity checks are performed to make sure that any of the
 *   options make sense.
 * AUTHOR
 *   Copyright (c) 1997, Gary William Flake.
 *
 *   Permission granted for any use according to the standard GNU
 *   ``copyleft'' agreement provided that the author's comments are
 *   neither modified nor removed.  No warranty is given or implied.
 */

// Modified by Libby Shoop (LS)  for Macalester COllege COMP 445

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "misc.h"
#include "omp.h"

// LS The following variables are all global. Make a struct and use in main
// int width = 640, height = 480, num = 20, len = 20, mag = 1;
// int seed = 0, invert = 0, steps = 100000000, psdump = 0;

// double angle = 270.0, vangle = 90, minv = 0.5, ddt = 0.95, dt = 3.0;
// double rcopy = 80, rcent = 30, rviso = 40, rvoid = 15;
// double wcopy = 0.2, wcent = 0.4, wviso = 0.8, wvoid = 1.0, wrand = 0.0;

// int threads = 1;

// char *term = NULL;

// LS the struct for simulation parameters
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

/*
// char help_string[] = "\
// Simulate a flock of boids according to rules that determine their \
// individual behaviors as well as the ``physics'' of their universe. \
// A boid greedily attempts to apply four rules with respect to its \
// neighbors: it wants to fly in the same direction, be in the center \
// of the local cluster of boids, avoid collisions with boids too close, \
// and maintain a clear view ahead by skirting around others that block \
// its view.  Changing these rules can make the boids behave like birds, \
// gnats, bees, fish, or magnetic particles.  See the RULES section of \
// the manual pages for more details.\
// ";

// OPTION options[] = {
//   { "-width",  OPT_INT,     &width,  "Width of the plot in pixels." },
//   { "-height", OPT_INT,     &height, "Height of the plot in pixels." },
//   { "-num",    OPT_INT,     &num,    "Number of boids." },
//   { "-steps",  OPT_INT,     &steps,  "Number of simulated steps." },
//   { "-seed",   OPT_INT,     &seed,   "Random seed for initial state." },
//   { "-angle",  OPT_DOUBLE,  &angle,  "Number of viewing degrees." },
//   { "-vangle", OPT_DOUBLE,  &vangle, "Visual avoidance angle." },
//   { "-rcopy",  OPT_DOUBLE,  &rcopy,  "Radius for copy vector." },
//   { "-rcent",  OPT_DOUBLE,  &rcent,  "Radius for centroid vector." },
//   { "-rvoid",  OPT_DOUBLE,  &rvoid,  "Radius for avoidance vector." },
//   { "-rviso",  OPT_DOUBLE,  &rviso,  "Radius for visual avoidance vector." },
//   { "-wcopy",  OPT_DOUBLE,  &wcopy,  "Weight for copy vector." },
//   { "-wcent",  OPT_DOUBLE,  &wcent,  "Weight for centroid vector." },
//   { "-wvoid",  OPT_DOUBLE,  &wvoid,  "Weight for avoidance vector." },
//   { "-wviso",  OPT_DOUBLE,  &wviso,  "Weight for visual avoidance vector." },
//   // { "-wrand",  OPT_DOUBLE,  &wrand,  "Weight for random vector." },
//   { "-dt",     OPT_DOUBLE,  &dt,     "Time-step increment." },
//   { "-ddt",    OPT_DOUBLE,  &ddt,    "Momentum factor (0 < ddt < 1)." },
//   { "-minv",   OPT_DOUBLE,  &minv,   "Minimum velocity." },
//   { "-len",    OPT_INT,     &len,    "Tail length." },
//   { "-psdump", OPT_SWITCH,  &psdump, "Dump PS at the very end?" },
//   { "-inv",    OPT_SWITCH,  &invert, "Invert all colors?" },
//   { "-mag",    OPT_INT,     &mag,    "Magnification factor." },
//   { "-term",   OPT_STRING,  &term,   "How to plot points." },
//   { "-t",      OPT_INT,     &threads, "Number of threads." },
//   { NULL,      OPT_NULL,    NULL,    NULL }
// };
*/

// LS Note: wrand is ignored for simplicity, so we are not generating random
//          numbers during flight simulation.

/* These are global to avoid passing them around all of time.  They
   represent the boids (x, y) positions, velocity vectors, and new
   velocity vectors. */

// LS Note: Better code practice to have these arrays declared
//          in main instead of global.
// double *xp, *yp, *xv, *yv, *xnv, *ynv;

/* Some handy macros ... */

#define LEN(x, y) sqrt(SQR(x) + SQR(y))
#define DIST(x1, y1, x2, y2) LEN(((x1) - (x2)), ((y1) - (y2)))
#define DOT(x1, y1, x2, y2) ((x1) * (x2) + (y1) * (y2))

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

/* Destructively normalize a vector. */

void norm(double *x, double *y)
{
	double len;

	len = LEN(*x, *y);
	if (len != 0.0)
	{
		*x /= len;
		*y /= len;
	}
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

/* Compute the heading for a particular boid based on its current
   environment. */

// void compute_new_heading(int which)
//  LS note: the following function will work on all of the boids.
//           This is needed so that the outer loop over all boids can
//           be parallelized for all compilers, especially openacc.
void compute_new_headings(struct Params p, double *xp, double *yp,
						  double *xv, double *yv,
						  double *xnv, double *ynv)
{

	// for each boid, we will examine every other boid
	for (int which = 0; which < p.num; which++)
	{
		// variables declared in this block become private when using pragmas
		// int i, j, k,
		int numcent = 0;
		double xa, ya, xb, yb, xc, yc, xd, yd, xt, yt;
		double mindist, mx = 0, my = 0, d;
		double cosangle, cosvangle, costemp;
		double xtemp, ytemp, maxr, u, v;

		/* This is the maximum distance in which any rule is activated. */
		maxr = MAX(p.rviso, MAX(p.rcopy, MAX(p.rcent, p.rvoid)));

		/* These two values are used to see if a boid can "see" another
		 * boid in various ways.
		 */
		cosangle = cos(p.angle / 2);
		cosvangle = cos(p.vangle / 2);

		/* These are the accumulated change vectors for the four rules. */
		xa = ya = xb = yb = xc = yc = xd = yd = 0;

		///////////////////////////////////////////////////////////////////////
		// LS NOTE: this calculation is independent in each step
		// so that it can be distributed among threads, each working on a
		// subset of boids. Each boid in outer loop is examining all other boids
		// in this inner loop.
		///////////////////////////////////////////////////////////////////////
		/* For every boid... */

		for (int i = 0; i < p.num; i++)
		{

			/* Don't include self for computing new heading. */
			if (i == which)
				continue;

			/* Since we want boids to "see" each other around the borders of
			 * the screen, we need to check if a boid on the left edge is
			 * actually "close" to a boid on the right edge, etc.  We do this
			 * by searching over nine relative displacements of boid(i) and
			 * pick the one that is closest to boid(which).  These coordinates
			 * are then used for all remaining calculations.
			 */
			mindist = 10e10;

			for (int j = -p.width; j <= p.width; j += p.width)
				for (int k = -p.height; k <= p.height; k += p.height)
				{
					d = DIST(xp[i] + j, yp[i] + k, xp[which], yp[which]);
					if (d < mindist)
					{
						mindist = d;
						mx = xp[i] + j;
						my = yp[i] + k;
					}
				}

			/* If that distance is farther than any of the rule radii,
			 * then skip.
			 */
			if (mindist > maxr)
				continue;

			/* Make a vector from boid(which) to boid(i). */
			xtemp = mx - xp[which];
			ytemp = my - yp[which];

			/* Calculate the cosine of the velocity vector of boid(which)
			 * and the vector from boid(which) to boid(i).
			 */
			costemp = DOT(xv[which], yv[which], xtemp, ytemp) /
					  (LEN(xv[which], yv[which]) * LEN(xtemp, ytemp));

			/* If this cosine is less than the cosine of one half
			 * of the boid's eyesight, i.e., boid(which) cannot see
			 * boid(i), then skip.
			 */
			if (costemp < cosangle)
				continue;

			/* If the distance between the two boids is within the radius
			 * of the centering rule, but outside of the radius of the
			 * avoidance rule, then attempt to center in on boid(i).
			 */
			if (mindist <= p.rcent && mindist > p.rvoid)
			{
				xa += mx - xp[which];
				ya += my - yp[which];
				numcent++;
			}

			/* If we are close enough to copy, but far enough to avoid,
			 * then copy boid(i)'s velocity.
			 */
			if (mindist <= p.rcopy && mindist > p.rvoid)
			{
				xb += xv[i];
				yb += yv[i];
			}

			/* If we are within collision range, then try to avoid boid(i). */
			if (mindist <= p.rvoid)
			{

				/* Calculate the vector which moves boid(which) away from boid(i). */
				xtemp = xp[which] - mx;
				ytemp = yp[which] - my;

				/* Make the length of the avoidance vector inversely proportional
				 * to the distance between the two boids.
				 */
				d = 1 / LEN(xtemp, ytemp);
				xtemp *= d;
				ytemp *= d;
				xc += xtemp;
				yc += ytemp;
			}

			/* If boid(i) is within rviso distance and the angle between this boid's
			 * velocity vector and the boid(i)'s position relative to this boid is
			 * less than vangle, then try to move so that vision is restored.
			 */
			if (mindist <= p.rviso && cosvangle < costemp)
			{

				/* Calculate the vector which moves boid(which) away from boid(i). */
				xtemp = xp[which] - mx;
				ytemp = yp[which] - my;

				/* Calculate another vector that is orthogonal to the previous,
				 * But try to make it in the same general direction of boid(which)'s
				 * direction of movement.
				 */
				u = v = 0;
				if (xtemp != 0 && ytemp != 0)
				{
					u = sqrt(SQR(ytemp / xtemp) / (1 + SQR(ytemp / xtemp)));
					v = -xtemp * u / ytemp;
				}
				else if (xtemp != 0)
					u = 1;
				else if (ytemp != 0)
					v = 1;
				if ((xv[which] * u + yv[which] * v) < 0)
				{
					u = -u;
					v = -v;
				}

				/* Add the vector that moves away from boid(i). */
				u = xp[which] - mx + u;
				v = yp[which] - my + v;

				/* Make this vector's length inversely proportional to the
				 * distance between the two boids.
				 */
				d = LEN(xtemp, ytemp);
				if (d != 0)
				{
					u /= d;
					v /= d;
				}
				xd += u;
				yd += v;
			}
		} // end of loop for every boid

		/* Avoid centering on only one other boid;
		 * it makes you look aggressive!
		 */
		if (numcent < 2)
			xa = ya = 0;

		/* Normalize all big vectors. */
		if (LEN(xa, ya) > 1.0)
			norm(&xa, &ya);
		if (LEN(xb, yb) > 1.0)
			norm(&xb, &yb);
		if (LEN(xc, yc) > 1.0)
			norm(&xc, &yc);
		if (LEN(xd, yd) > 1.0)
			norm(&xd, &yd);

		/* Compute the composite trajectory based on all of the rules. */
		xt = xa * p.wcent + xb * p.wcopy + xc * p.wvoid + xd * p.wviso;
		yt = ya * p.wcent + yb * p.wcopy + yc * p.wvoid + yd * p.wviso;

		/* Optionally add some noise. */
		// LS Note: eliminate this option for simplicity
		// if(wrand > 0) {
		//   xt += random_range(-1, 1) * wrand;
		//   yt += random_range(-1, 1) * wrand;
		// }

		/* Update the velocity and renormalize if it is too small. */
		xnv[which] = xv[which] * p.ddt + xt * (1 - p.ddt);
		ynv[which] = yv[which] * p.ddt + yt * (1 - p.ddt);
		d = LEN(xnv[which], ynv[which]);
		if (d < p.minv)
		{
			xnv[which] *= p.minv / d;
			ynv[which] *= p.minv / d;
		}
	}
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

void draw_boid(struct Params p, int which, int color, double *xp, double *yp,
			   double *xv, double *yv)
{
	double x1, x2, x3, y1, y2, y3, a, t;

	/* Plot a line in the direction that it is heading. */
	x3 = xv[which];
	y3 = yv[which];
	norm(&x3, &y3);
	x1 = xp[which];
	y1 = yp[which];
	x2 = x1 - x3 * p.len;
	y2 = y1 - y3 * p.len;
	plot_line(x1, y1, x2, y2, color);

	/* Plot the head of the boid, with the angle of the arrow head
	 * indicating its viewing angle.
	 */
	t = (x1 - x2) / p.len;
	t = (t < -1) ? -1 : (t > 1) ? 1
								: t;
	a = acos(t);
	a = (y1 - y2) < 0 ? -a : a;

	/* This is for the right portion of the head. */
	x3 = x1 + cos(a + p.angle / 2) * p.len / 3.0;
	y3 = y1 + sin(a + p.angle / 2) * p.len / 3.0;
	plot_line(x1, y1, x3, y3, color);

	/* This is for the left portion of the head. */
	x3 = x1 + cos(a - p.angle / 2) * p.len / 3.0;
	y3 = y1 + sin(a - p.angle / 2) * p.len / 3.0;
	plot_line(x1, y1, x3, y3, color);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

int main(int argc, char **argv)
{
	extern int plot_mag;
	extern int plot_inverse;
	int i, j;

	// LS use struct for default parameters
	struct Params params = {
		.width = 640, .height = 480, .num = 20, .len = 20, .mag = 1, .seed = 0, .invert = 0, .steps = 100000000, .psdump = 0, .angle = 270.0, .vangle = 90, .minv = 0.5, .ddt = 0.95, .dt = 3.0, .rcopy = 80, .rcent = 30, .rviso = 40, .rvoid = 15, .wcopy = 0.2, .wcent = 0.4, .wviso = 0.8, .wvoid = 1.0, .threads = 1, .term = NULL};

	char help_string[] = "\
Simulate a flock of boids according to rules that determine their \
individual behaviors as well as the ``physics'' of their universe. \
A boid greedily attempts to apply four rules with respect to its \
neighbors: it wants to fly in the same direction, be in the center \
of the local cluster of boids, avoid collisions with boids too close, \
and maintain a clear view ahead by skirting around others that block \
its view.  Changing these rules can make the boids behave like birds, \
gnats, bees, fish, or magnetic particles.  See the RULES section of \
the manual pages for more details.\
";

	OPTION options[] = {
		{"-width", OPT_INT, &params.width, "Width of the plot in pixels."},
		{"-height", OPT_INT, &params.height, "Height of the plot in pixels."},
		{"-num", OPT_INT, &params.num, "Number of boids."},
		{"-steps", OPT_INT, &params.steps, "Number of simulated steps."},
		{"-seed", OPT_INT, &params.seed, "Random seed for initial state."},
		{"-angle", OPT_DOUBLE, &params.angle, "Number of viewing degrees."},
		{"-vangle", OPT_DOUBLE, &params.vangle, "Visual avoidance angle."},
		{"-rcopy", OPT_DOUBLE, &params.rcopy, "Radius for copy vector."},
		{"-rcent", OPT_DOUBLE, &params.rcent, "Radius for centroid vector."},
		{"-rvoid", OPT_DOUBLE, &params.rvoid, "Radius for avoidance vector."},
		{"-rviso", OPT_DOUBLE, &params.rviso, "Radius for visual avoidance vector."},
		{"-wcopy", OPT_DOUBLE, &params.wcopy, "Weight for copy vector."},
		{"-wcent", OPT_DOUBLE, &params.wcent, "Weight for centroid vector."},
		{"-wvoid", OPT_DOUBLE, &params.wvoid, "Weight for avoidance vector."},
		{"-wviso", OPT_DOUBLE, &params.wviso, "Weight for visual avoidance vector."},
		// { "-wrand",  OPT_DOUBLE,  &params.wrand,  "Weight for random vector." },
		{"-dt", OPT_DOUBLE, &params.dt, "Time-step increment."},
		{"-ddt", OPT_DOUBLE, &params.ddt, "Momentum factor (0 < ddt < 1)."},
		{"-minv", OPT_DOUBLE, &params.minv, "Minimum velocity."},
		{"-len", OPT_INT, &params.len, "Tail length."},
		{"-psdump", OPT_SWITCH, &params.psdump, "Dump PS at the very end?"},
		{"-inv", OPT_SWITCH, &params.invert, "Invert all colors?"},
		{"-mag", OPT_INT, &params.mag, "Magnification factor."},
		{"-term", OPT_STRING, &params.term, "How to plot points."},
		{"-t", OPT_INT, &params.threads, "Number of threads."},
		{NULL, OPT_NULL, NULL, NULL}};

	// LS eliminate global variables by declaring here
	double *xp, *yp, *xv, *yv, *xnv, *ynv;

	// LS debug
	fprintf(stderr, "Before options, Number of boids: %d\n", params.num);

	get_options(argc, argv, options, help_string);

	// LS debug
	fprintf(stderr, "After options, Number of boids: %d\n", params.num);

	// LS added this for debugging and Threads set
	fprintf(stderr, "Number of threads: %d\n", params.threads);
	omp_set_num_threads(params.threads);

	if (!params.psdump)
	{
		plot_mag = params.mag;
		plot_inverse = params.invert;
		plot_init(params.width, params.height, 2, params.term);
		plot_set_all(0);
	}
	srandom(params.seed);

	/* Convert angles to radians. */
	params.angle = params.angle * M_PI / 180.0;
	params.vangle = params.vangle * M_PI / 180.0;

	/* Make space for the positions, velocities, and new velocities. */
	xp = xmalloc(sizeof(double) * params.num);
	yp = xmalloc(sizeof(double) * params.num);
	xv = xmalloc(sizeof(double) * params.num);
	yv = xmalloc(sizeof(double) * params.num);
	xnv = xmalloc(sizeof(double) * params.num);
	ynv = xmalloc(sizeof(double) * params.num);

	/* Set to random initial conditions. */
	// LS note: keep sequential or change to parallel random number generation
	for (i = 0; i < params.num; i++)
	{
		xp[i] = random() % params.width;
		yp[i] = random() % params.height;
		xv[i] = random_range(-1.0, 1.0);
		yv[i] = random_range(-1.0, 1.0);
		norm(&xv[i], &yv[i]);
	}
	// LS added timing
	double start, end;
	start = omp_get_wtime();

	/* For each time step... */
	for (i = 0; i < params.steps; i++)
	{

		compute_new_headings(params, xp, yp, xv, yv, xnv, ynv);
		// /* For each boid, compute its new heading. */

		// for(j = 0; j < num; j++) {
		//   compute_new_heading(j, xp, yp, xv, yv, xnv, ynv);
		// }

		/* For each boid again... */
		for (j = 0; j < params.num; j++)
		{

			/* Undraw the boid. */
			if (!params.psdump)
				draw_boid(params, j, 0, xp, yp, xv, yv);

			/* Update the velocity and position. */
			xv[j] = xnv[j];
			yv[j] = ynv[j];
			xp[j] += xv[j] * params.dt;
			yp[j] += yv[j] * params.dt;

			/* Wrap around the screen coordinates. */
			if (xp[j] < 0)
				xp[j] += params.width;
			else if (xp[j] >= params.width)
				xp[j] -= params.width;
			if (yp[j] < 0)
				yp[j] += params.height;
			else if (yp[j] >= params.height - 1)
				yp[j] -= params.height;

			/* Redraw the boid. */
			if (!params.psdump)
				draw_boid(params, j, 1, xp, yp, xv, yv);
		}
	}
	// LS end timing before some of the plotting
	end = omp_get_wtime();
	fprintf(stderr, "Total time: %f seconds\n", end - start);

	printf("%f, %f\n", xp[0], yp[0]);

	if (!params.psdump)
		plot_finish();

	/* If we want a PS dump of the final configuration, do it. */
	if (params.psdump)
	{
		plot_inverse = 0;
		plot_init(params.width, params.height, 2, "ps");
		for (i = 0; i < params.num; i++)
		{
			draw_boid(params, i, 0, xp, yp, xv, yv);
		}
		plot_finish();
	}

	exit(0);
}
