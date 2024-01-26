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
 * 
 * 	Modified by Libby Shoop, Ethan Scheelk
 * 	for Peachy Parallel Assignments EduPar2024
 * 	2024-01-25
 */


#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "misc.h"
#include "boids.hpp"


/**
 * @brief Destructively normalize a vector.
 * 
 * @param x 
 * @param y 
 */
void boids::norm(float* x, float* y)
{
	double len;

	len = LEN(*x, *y);
	if (len != 0.0)
	{
		*x /= len;
		*y /= len;
	}
}

/**
 * @brief Default parameters for the simulation
 * 
 * @return boids::Params 
 */
boids::Params boids::getDefaultParams()
{
	struct boids::Params defaultParams = 
    {
		.width = 1024, .height = 1024, 
		.num = 512, .len = 20, 
		.mag = 1, .seed = 0, 
		.invert = 0, .steps = 1000, 
		.psdump = 0, .angle = 270.0, 
		.vangle = 90, .minv = 0.5, 
		.ddt = 0.95, .dt = 3.0, 
		.rcopy = 80, .rcent = 30, 
		.rviso = 40, .rvoid = 15, 
		.wcopy = 0.2, .wcent = 0.4, 
		.wviso = 0.8, .wvoid = 1.0, .
		threads = 1, .term = NULL
    };

	return defaultParams;
}

/**
 * @brief Computes the hehadings for all boids.
 * 
 * LS note: the following function will work on all of the boids.
 * This is needed so that the outer loop over all boids can
 * be parallelized for all compilers, especially openacc.
 * 
 * @param p 
 * @param xp 
 * @param yp 
 * @param xv 
 * @param yv 
 * @param xnv 
 * @param ynv 
 */
void boids::compute_new_headings(
	struct boids::Params p, float *xp, float *yp,
	float *xv, float *yv,
	float *xnv, float *ynv)
{

	// for each boid, we will examine every other boid
	for (int which = 0; which < p.num; which++)
	{
		// int i, j, k,
		int numcent = 0;
		float xa, ya, xb, yb, xc, yc, xd, yd, xt, yt;
		float mindist, mx = 0, my = 0, d;
		float cosangle, cosvangle, costemp;
		float xtemp, ytemp, maxr, u, v;

		/* This is the maximum distance in which any rule is activated. */
		maxr = MAX(p.rviso, MAX(p.rcopy, MAX(p.rcent, p.rvoid)));

		/* These two values are used to see if a boid can "see" another
		 * boid in various ways.
		 */
		cosangle = cos(p.angle / 2);
		cosvangle = cos(p.vangle / 2);

		/* These are the accumulated change vectors for the four rules. */
		xa = ya = xb = yb = xc = yc = xd = yd = 0;

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