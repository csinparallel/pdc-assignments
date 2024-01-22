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
        {0}};

    // int* intArgs[] = {&p.width, &p.height, &p.num, &p.threads, &p.steps, &p.seed};
    // double* floatArgs[] = {&p.angle, &p.vangle, &p.rcopy, &p.rcent, &p.rvoid, &p.rviso, &p.wcopy, &p.wvoid, &p.wviso, &p.dt, &p.ddt, &p.minv};

    int c;
    int digit_optind = 0;

    while (true)
    {
        int this_option_optind = optind ? optind : 1;
        int option_index = 0;

        c = getopt_long_only(argc, argv, "", longopts, &option_index);

        if (c == -1)
        {
            break;
        }

        // printf("%s\t%s\n", longopts[option_index].name, optarg);

        // if (c <= 5)
        // {
        //     *intArgs[c] = atoi(optarg);
        // }

        // else
        // {
        //     *floatArgs[c-6] = atof(optarg);
        // }

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
        default:
            // fprintf(stderr, "Argument [%s] with value [%s] not identifed\n", argv[optind - 1], optarg);

            // fprintf(stderr, "Argument not found\n");
            break;
        }
    }
}
