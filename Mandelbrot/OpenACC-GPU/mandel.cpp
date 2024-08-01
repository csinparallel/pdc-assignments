/** Compute/draw the Mandelbrot set using TSGL and OpenMP
 *
 * Joel Adams, for CS 374, Fall 2023, Calvin University.
 *
 * NOTE: Drawing on the canvas using the TSGL library must be done on the host.
 * The library does not support using OpenACC to draw on the displayed canvas from the device.
 */

#include <cstdio>  // C-style I/O
#include <complex> // complex<T>
#include <cstdlib> // abs()
#include <tsgl.h>  // CartesianCanvas, etc.
#include <omp.h>   // OpenMP
using namespace tsgl;

const int THRESHOLD = 500; // our Mandelbrot 'escape' threshold

int doMandelbrotCalc(long double x, long double y, int MAX_REPS = THRESHOLD);
void drawMandelbrotPoint(CartesianCanvas &canvas,
                         long double x, long double y,
                         int reps, int MAX_REPS = THRESHOLD);

int main(int argc, char *argv[])
{
    const int WINDOW_HEIGHT = 800;
    const int WINDOW_WIDTH = 1200;
    double startTime, totalTime;

    printf("\nComputing and drawing the Mandelbrot Set...\n");

    // Start timing
    startTime = omp_get_wtime();

    // set up canvas
    CartesianCanvas canvas(-1, -1, WINDOW_WIDTH, WINDOW_HEIGHT,
                           -2, -1.125, 1, 1.125,
                           "Mandelbrot Set (Calvin U)", GRAY);
    canvas.start();

    // retrieve data to be used in mandelbrot calculations
    unsigned winHeight = canvas.getWindowHeight() - 1, // TSGL bug ?
        winWidth = canvas.getWindowWidth();
    long double startX = canvas.getMinX(),
                deltaX = canvas.getPixelWidth(),
                startY = canvas.getMinY(),
                deltaY = canvas.getPixelHeight();

    // begin mandelbrot calculations/drawing
    for (unsigned row = 0; row < winHeight; ++row)
    {
        long double y = startY + row * deltaY;
        for (unsigned col = 0; col < winWidth; ++col)
        {
            long double x = startX + col * deltaX;
            int iterations = doMandelbrotCalc(x, y);
            drawMandelbrotPoint(canvas, x, y, iterations);
        }
    }

    // End timing
    totalTime = omp_get_wtime() - startTime;

    // pause so the program doesn't terminate
    printf("\nMandelbrot Set completed in %f seconds.\n"
           "\nPress ESC or click the window's close-box to quit...\n\n",
           totalTime);
    canvas.wait();
}

/** perform the Mandelbrot calculation for a given x,y point
 * @param: x, a long double
 * @param: y, a long double
 * @param: MAX_REPS, an int
 * @precondition: MAX_REPS is a value, such that we assume
 *                calculations that iterate more than
 *                that many times never converge.
 * @postcondition: count == MAX_REPS ||
 *                count == the number of Mandelbrot iterations
 *                          required for (x,y) to converge.
 * @return: count
 */
int doMandelbrotCalc(long double x, long double y, int MAX_REPS)
{
    std::complex<long double> originalComplex(x, y);
    std::complex<long double> comp(x, y);
    int count = 0;
    while (std::abs(comp) < 2.0 && count < MAX_REPS)
    {
        comp = comp * comp + originalComplex;
        ++count;
    }
    return count;
}

/** draw a given (x,y) point, color based on whether or not
 *   (x,y) is within or outside the Mandelbrot set
 * @param: canvas, a TSGL CartesianCanvase
 * @param: x, a long double
 * @param: y, a long double
 * @param: reps, an int
 * @param: MAX_REPS, an int
 * @precondition: reps == doMandelbrotCalcFor(x,y)
 *            && canvas models a slice through the complex plane.
 * @postcondition: the (x,y) pixel on canvas has been shaded [using reps]
 *                 for inclusion/exclusion in the Mandelbrot set
 *                 depending on how reps relates to MAX_REPS.
 */
void drawMandelbrotPoint(CartesianCanvas &canvas,
                         long double x, long double y,
                         int reps, int MAX_REPS)
{
    CartesianBackground *bg = canvas.getBackground();

    ColorInt figureColor(238, 204, 10);  // Gold
    ColorInt surroundColor(137, 27, 47); // Maroon

    // ColorInt figureColor(0, 0, 0);       // Alternative
    // ColorInt surroundColor(255, 255, 0); //  colors

    if (reps >= MAX_REPS)
    {
        bg->drawPixel(x, y, figureColor);
    }
    else
    {
        bg->drawPixel(x, y, surroundColor);
    }
}
