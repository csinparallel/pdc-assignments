/**
 * ImageConverter.cpp declares a class for
 *  various image conversion operations.
 *
 *  Author:  Joel Adams, CS112 @ Calvin College.
 *  Created: March 2015
 *  Revised: November 2016
 *  Revised for TSGL-2: November 2020
 *
 * Usage: ./main
 */

#include "ImageConverter.h"
#include <unistd.h> // sleep()

//-----------------  Method definitions ------------------------------

/** explicit constructor
 *
 * @param imageFileName, a string.
 * @param width, an int.
 * @param height, an int.
 *
 * @precondition: imageFileName contains the name of a valid image file
 *               && width > 0 && height > 0
 * @postcondition: myImageFileName == imageFileName
 *                && myWidth == width && myHeight = height
 *                && a width x height Canvas containing the image
 *                    from imageFileName has been displayed on screen
 *                && a blank width x height Canvas as also been displayed.
 */
ImageConverter::ImageConverter(const std::string &imageFileName,
                               int width, int height)
    : myCanvas0(0, 0, width, height, imageFileName),
      myWidth(width),
      myHeight(height),
      myImageFileName(imageFileName)
{
    myCanvas0.start();
    myCanvas0.getBackground()->drawImage(0, 0, 0, imageFileName,
                                         width, height, 0, 0, 0);
}

/** destructor
 *
 * @precondition: this ImageConverter object is going out of scope.
 * @postcondition: a "normal termnation" message has been displayed.
 */
ImageConverter::~ImageConverter()
{
    cout << "\nImageConverter terminated normally." << endl;
}

/** method to run image-conversion methods
 * @postcondition: the converted version of the image from myCanvas0
 *                 has been displayed in a new Canvas
 *               && myCanvas0 has been closed.
 */
void ImageConverter::run()
{
    invertImage();
    myCanvas0.wait();
}

/** invertImage() inverts the image from myImageFileName.
 *
 * @precondition: myCanvas0 contains a TSGL Image that is to be inverted
 *            && myWidth == the number of columns in which to display the image
 *            && myHeight == the number of rows in which to display the image.
 *
 * @postcondition: a myWidth x myHeight Canvas has been displayed
 *                 containing the inverse of the image from myCanvas0
 *                 (which contains the image from myImageFileName).
 */

void ImageConverter::invertImage()
{
    // record starting time
    double startTime = omp_get_wtime();

    Canvas canvas1(myWidth + 50, 0, myWidth, myHeight,
                   myImageFileName + "-inverted");
    canvas1.start();

    Background *background0 = myCanvas0.getBackground();
    Background *background1 = canvas1.getBackground();

    const int WIN_WIDTH = myCanvas0.getWindowWidth(),
              WIN_HEIGHT = myCanvas0.getWindowHeight(),
              START_X = -WIN_WIDTH / 2, // left column
        START_Y = -WIN_HEIGHT / 2,      // bottom row
        STOP_X = +WIN_WIDTH / 2,        // right column + 1
        STOP_Y = +WIN_HEIGHT / 2;       // top row + 1

    // loop through the image rows
    for (int y = START_Y; y < STOP_Y; ++y)
    { // bottom to top
        // loop through the image columns
        for (int x = START_X; x < STOP_X; ++x)
        { // left to right
            // read the pixel at canvas0[row][col]
            ColorInt pixelColor = background0->getPixel(x, y);
            // compute its inverse
            int invertedR = 255 - pixelColor.R;
            int invertedG = 255 - pixelColor.G;
            int invertedB = 255 - pixelColor.B;
            ColorInt invertedColor(invertedR, invertedG, invertedB);
            // draw the inverse at the same spot in canvas1
            background1->drawPixel(x, y, invertedColor);
        } // inner for
        // slow the processing to simulate a very large image
        canvas1.sleep();
    } // outer for

    // compute and display the time difference
    double totalTime = omp_get_wtime() - startTime;
    cout << "\n\nImage inversion took "
         << totalTime << " seconds.\n"
         << endl;

    canvas1.wait();
} // invertImage
