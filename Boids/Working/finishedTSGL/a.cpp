#include <getopt.h>


int main(int argc, char const *argv[])
{
    /* code */
    option longopts[] = {
        {"num", optional_argument, nullptr, ' '},
        {0}
    };
    return 0;
}
