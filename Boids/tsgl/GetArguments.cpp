/*
    Ethan Scheelk
*/
#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include "GetArguments.hpp"

// int args
#define WIDTH 0   // Width of plot in pixels
#define HEIGHT 1  // Height of plot in pixels
#define NUM 2     // Number of boids.
#define THREADS 3 // Number of threads
#define STEPS 4   // Number of simulated steps
#define SEED 5    // Random seed for initial state

// float args
#define ANGLE 6  // Number of viewing degrees
#define VANGLE 7 // Visual avoidance angle
#define RCOPY 8  // Radius for copy vector
#define RCENT 9  // Radius for centroid vector
#define RVOID 10 // Radius for avoidance vector
#define RVISO 11 // Radius for visual avoidance vector
#define WCOPY 12 // Weight for copy vector
#define WCENT 13 // Weight for centroid vector
#define WVOID 14 // Weight for avoidance vector
#define WVISO 15 // Weight for visual avoidance vector
#define DT 16    // Time-step increment
#define DDT 17   // Momentum factor (0 < ddt < 1)
#define MINV 18  // Minimum velocity

#define NO_DRAW 19 // whether to draw on canvas or simulate for speed test

#define HELP 20

/**
 * @brief Get the arguments from the user's input and modify their parameters
 *
 * @param argc
 * @param argv
 * @param p
 */
void get_arguments(int argc, char *argv[], boids::Params &p, bool& noDraw)
{
    option longopts[] = {
        {"width", required_argument, nullptr, WIDTH},
        {"height", required_argument, nullptr, HEIGHT},
        {"num", required_argument, nullptr, NUM},
        {"threads", required_argument, nullptr, THREADS},
        {"steps", required_argument, nullptr, STEPS},
        {"seed", required_argument, nullptr, SEED},
        {"angle", required_argument, nullptr, ANGLE},
        {"vangle", required_argument, nullptr, VANGLE},
        {"rcopy", required_argument, nullptr, RCOPY},
        {"rcent", required_argument, nullptr, RCENT},
        {"rvoid", required_argument, nullptr, RVOID},
        {"rviso", required_argument, nullptr, RVISO},
        {"wcopy", required_argument, nullptr, WCOPY},
        {"wcent", required_argument, nullptr, WCENT},
        {"wvoid", required_argument, nullptr, WVOID},
        {"wviso", required_argument, nullptr, WVISO},
        {"dt", required_argument, nullptr, DT},
        {"ddt", required_argument, nullptr, DDT},
        {"minv", required_argument, nullptr, MINV},
        {"noDraw", no_argument, nullptr, NO_DRAW},
        {"help", no_argument, nullptr, HELP},
        {0}};


    int c;
    while (true)
    {
        int option_index = 0;

        c = getopt_long_only(argc, argv, "", longopts, &option_index);

        if (c == -1)
        {
            break;
        }

        // Highly repetitive but easy to understand.
        switch (c)
        {
        case WIDTH:
            p.width = atoi(optarg);
            break;
        case HEIGHT:
            p.height = atoi(optarg);
            break;
        case NUM:
            p.num = atoi(optarg);
            break;
        case THREADS:
            p.threads = atoi(optarg);
            break;
        case STEPS:
            p.steps = atoi(optarg);
            break;
        case ANGLE:
            p.angle = atof(optarg);
            break;
        case VANGLE:
            p.vangle = atof(optarg);
            break;
        case RCOPY:
            p.rcopy = atof(optarg);
            break;
        case RCENT:
            p.rcent = atof(optarg);
            break;
        case RVOID:
            p.rvoid = atof(optarg);
            break;
        case RVISO:
            p.rviso = atof(optarg);
            break;
        case WCOPY:
            p.wcopy = atof(optarg);
            break;
        case WCENT:
            p.wcent = atof(optarg);
            break;
        case WVOID:
            p.wvoid = atof(optarg);
            break;
        case WVISO:
            p.wviso = atof(optarg);
            break;
        case DT:
            p.dt = atof(optarg);
            break;
        case DDT:
            p.ddt = atof(optarg);
            break;
        case MINV:
            p.minv = atof(optarg);
            break;
        case SEED:
            p.seed = atof(optarg);
            break;
        case NO_DRAW:
            noDraw = true;
            break;
        case HELP:
            // Long print explaining 
            print_help();
            break;
        default:
            // getopt has an automatic "argument wrong" command
            print_help();
            break;
        }
    }
}

void print_help()
{
    // Prevents from printing several times
    static bool printed = false;
    if (printed) return;

    boids::Params p = boids::getDefaultParams();

    char s[] = "\
\nSimulate a flock of boids according to rules that determine their \
individual behaviors as well as the ``physics'' of their universe. \
A boid greedily attempts to apply four rules with respect to its \
neighbors: it wants to fly in the same direction, be in the center \
of the local cluster of boids, avoid collisions with boids too close, \
and maintain a clear view ahead by skirting around others that block \
its view.  Changing these rules can make the boids behave like birds, \
gnats, bees, fish, or magnetic particles.  See the RULES section of \
the manual pages for more details.\
    \n\nOptions with defaults in parentheses are:\n\n";


    fprintf(stderr, "%s", s);
    fprintf(stderr, "-help\t\t\tDisplay this text\n");
    fprintf(stderr, "\n-noDraw\t\t\tDo not draw canvas (false)\n\n");
    fprintf(stderr, "-width\t\t[int]\tWidth of plot in pixels (%d)\n", p.width);
    fprintf(stderr, "-height\t\t[int]\tHeight of plot in pixels (%d)\n", p.height);
    fprintf(stderr, "-num\t\t[int]\tNumber of boids (%d)\n", p.num);
    fprintf(stderr, "-threads\t[int]\tNumber of threads (%d)\n", p.threads);
    fprintf(stderr, "-steps\t\t[int]\tNumber of simulated steps (%d)\n", p.steps);
    fprintf(stderr, "-seed\t\t[int]\tRandom seed for initial state (%d)\n\n", p.seed);


    fprintf(stderr, "-angle\t\t[float]\tNumber of viewing degrees (%.2lf)\n", p.angle);
    fprintf(stderr, "-vangle\t\t[float]\tVisual avoidance angle (%.2lf)\n", p.vangle);
    fprintf(stderr, "-rcopy\t\t[float]\tRadius for copy vector (%.2lf)\n", p.rcopy);
    fprintf(stderr, "-rcent\t\t[float]\tRadius for centroid vector (%.2lf)\n", p.rcent);
    fprintf(stderr, "-rvoid\t\t[float]\tRadius for avoidance vector (%.2lf)\n", p.rvoid);
    fprintf(stderr, "-rviso\t\t[float]\tRadius for visual avoidance vector (%.2lf)\n", p.rviso);
    fprintf(stderr, "-wcopy\t\t[float]\tWeight for copy vector (%.2lf)\n", p.wcopy);
    fprintf(stderr, "-wcent\t\t[float]\tWeight for centroid vector (%.2lf)\n", p.wcent);
    fprintf(stderr, "-wvoid\t\t[float]\tWeight for avoidance vector (%.2lf)\n", p.wvoid);
    fprintf(stderr, "-wviso\t\t[float]\tWeight for visual avoidance vector (%.2lf)\n", p.wviso);
    fprintf(stderr, "-dt\t\t[float]\tTime-step increment (%.2lf)\n", p.dt);
    fprintf(stderr, "-ddt\t\t[float]\tMomentum factor (0 < ddt < 1) (%.2lf)\n", p.ddt);
    fprintf(stderr, "-minv\t\t[float]\tMinimum velocity (%.2lf)\n", p.minv);

    printed = true;
}
