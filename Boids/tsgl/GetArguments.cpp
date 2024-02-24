/*
    Ethan Scheelk
*/
#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include "GetArguments.hpp"

enum argType : int8_t 
{
    // int args
    width,      // Width of plot in pixels
    height,     // Height of plot in pixels
    num,        // Number of boids.
    threads,    // Number of threads
    steps,      // Number of simulated steps
    seed,       // Random seed for initial state

    // float args
    angle,      // Number of viewing degrees
    vangle,     // Visual avoidance angle
    rcopy,      // Radius for copy vector
    rcent,      // Radius for centroid vector
    rvoid,      // Radius for avoidance vector
    rviso,      // Radius for visual avoidance vector
    wcopy,      // Weight for copy vector
    wcent,      // Weight for centroid vector
    wvoid,      // Weight for avoidance vector
    wviso,      // Weight for visual avoidance vector
    dt,         // Time-step increment
    ddt,        // Momentum factor (0 < ddt < 1)
    minv,       // Minimum velocity

    no_draw,    // whether to draw on canvas or simulate for speed test
    help
};

/**
 * @brief Get the arguments from the user's input and modify their parameters.
 * 
 * No checking on bounds is done.
 *
 * @param argc
 * @param argv
 * @param p a reference to the param struct for modification
 * @param noDraw a reference to a boolean on whether the screen will be drawn
 */
void get_arguments(int argc, char *argv[], boids::Params &p, bool& noDraw)
{
    option longopts[] = {
        {"width", required_argument, nullptr, argType::width},
        {"height", required_argument, nullptr, argType::height},
        {"num", required_argument, nullptr, argType::num},
        {"threads", required_argument, nullptr, argType::threads},
        {"steps", required_argument, nullptr, argType::steps},
        {"seed", required_argument, nullptr, argType::seed},
        {"angle", required_argument, nullptr, argType::angle},
        {"vangle", required_argument, nullptr, argType::vangle},
        {"rcopy", required_argument, nullptr, argType::rcopy},
        {"rcent", required_argument, nullptr, argType::rcent},
        {"rvoid", required_argument, nullptr, argType::rvoid},
        {"rviso", required_argument, nullptr, argType::rviso},
        {"wcopy", required_argument, nullptr, argType::wcopy},
        {"wcent", required_argument, nullptr, argType::wcent},
        {"wvoid", required_argument, nullptr, argType::wvoid},
        {"wviso", required_argument, nullptr, argType::wviso},
        {"dt", required_argument, nullptr, argType::dt},
        {"ddt", required_argument, nullptr, argType::ddt},
        {"minv", required_argument, nullptr, argType::minv},
        {"noDraw", no_argument, nullptr, argType::no_draw},
        {"help", no_argument, nullptr, argType::help},
        {0}};


    argType c;
    while (true)
    {
        int option_index = 0;

        c = (argType) getopt_long_only(argc, argv, "", longopts, &option_index);

        if (c == -1)
        {
            break;
        }

        // Highly repetitive but easy to understand.
        switch (c)
        {
        case argType::width:
            p.width = atoi(optarg);
            break;
        case argType::height:
            p.height = atoi(optarg);
            break;
        case argType::num:
            p.num = atoi(optarg);
            break;
        case argType::threads:
            p.threads = atoi(optarg);
            break;
        case argType::steps:
            p.steps = atoi(optarg);
            break;
        case argType::angle:
            p.angle = atof(optarg);
            break;
        case argType::vangle:
            p.vangle = atof(optarg);
            break;
        case argType::rcopy:
            p.rcopy = atof(optarg);
            break;
        case argType::rcent:
            p.rcent = atof(optarg);
            break;
        case argType::rvoid:
            p.rvoid = atof(optarg);
            break;
        case argType::rviso:
            p.rviso = atof(optarg);
            break;
        case argType::wcopy:
            p.wcopy = atof(optarg);
            break;
        case argType::wcent:
            p.wcent = atof(optarg);
            break;
        case argType::wvoid:
            p.wvoid = atof(optarg);
            break;
        case argType::wviso:
            p.wviso = atof(optarg);
            break;
        case argType::dt:
            p.dt = atof(optarg);
            break;
        case argType::ddt:
            p.ddt = atof(optarg);
            break;
        case argType::minv:
            p.minv = atof(optarg);
            break;
        case argType::seed:
            p.seed = atof(optarg);
            break;
        case argType::no_draw:
            noDraw = true;
            break;
        case argType::help:
            // Long print explaining 
            print_help();
            exit(0);
            break;
        default:
            // getopt has an automatic "argument wrong" command
            print_help();
            exit(0);
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
