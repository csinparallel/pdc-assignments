#include <getopt.h>


// #include "boids.hpp"
#include "GetArguments.hpp"
#include <stdlib.h>
#include <stdio.h>


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


void get_arguments(int argc, char* argv[], boids::Params& p) 
{
    option longopts[] = {
        {"num", required_argument, nullptr, 0},
        {"width", required_argument, nullptr, 1},
        {"height", required_argument, nullptr, 2},
        {"angle", required_argument, nullptr, 3},
        {0}
    };

    int c;
    int digit_optind = 0;

    while (true) 
    {
        int this_option_optind = optind ? optind : 1;
        int option_index = 0;

        c = getopt_long_only(argc, argv, "", longopts, &option_index);

        if (c == -1) break;

        printf("%s\t%s\n", longopts[option_index].name, optarg);

        switch (c) 
        {
            case 0:
                p.num = atoi(optarg);
                break;
            case 1:
                p.width = atoi(optarg);
                break;
            case 2: 
                p.height = atoi(optarg);
                break;
            case 3:
                p.angle = atof(optarg);
                break;
            default:
                fprintf(stderr, "Argument not found\n");
                break;
        }
    }
}
