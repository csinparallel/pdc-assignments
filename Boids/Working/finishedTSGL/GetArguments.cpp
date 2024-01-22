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

#define WIDTH 0 //
#define HEIGHT 1 //
#define NUM 2 //
#define THREADS 3 //
#define STEPS 4 //
#define ANGLE 5 // 
#define VANGLE 6 // 
#define RCOPY 7 //
#define RCENT 8 //
#define RVOID 9 //
#define RVISO 10 //
#define WCOPY 11 //
#define WCENT 12 //
#define WVOID 13 //
#define WVISO 14 //
#define DT 15
#define DDT 16
#define MINV 17
#define SEED 18


void get_arguments(int argc, char* argv[], boids::Params& p) 
{
    option longopts[] = {
        {"width", required_argument, nullptr, WIDTH},
        {"height", required_argument, nullptr, HEIGHT},
        {"num", required_argument, nullptr, NUM},
        {"threads", required_argument, nullptr, THREADS},
        {"steps", required_argument, nullptr, STEPS},
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
        {"seed", required_argument, nullptr, SEED},
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
            
            default:
                // fprintf(stderr, "Argument [%s] with value [%s] not identifed\n", argv[optind - 1], optarg);
                
                // fprintf(stderr, "Argument not found\n");
                break;
        }
    }
}
