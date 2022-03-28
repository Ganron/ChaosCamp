/*
* Chaos Camp: Ray-Tracing Course
* HOMEWORK: LECTURE 2, PROBLEM 2
* Author: Asen Markov
* Date: 07/03/2022
*
* Description:
* Draw a single-color backgroud and a solid shape on top of it, where the outline of the shape is given by
* the Quadrifolium curve. Some aliasing may occur.
*/

#include <fstream>
#include <cmath>
#include <cstdlib>

/// Output image resolution
static const int imageWidth = 1920;
static const int imageHeight = 1080;

static const int maxColorComponent = 255;

int main() {
    //Set up output stream
    std::ofstream ppmFileStream("am_hw2_b_edited.ppm", std::ios::out | std::ios::binary);
    ppmFileStream << "P3\n";
    ppmFileStream << imageWidth << " " << imageHeight << "\n";
    ppmFileStream << maxColorComponent << "\n";

    //Draw
    for (int rowIdx = 0; rowIdx < imageHeight; ++rowIdx) {
        for (int colIdx = 0; colIdx < imageWidth; ++colIdx) {
            // Set coordinate system such that the origin is at the centre of the screen and y-axis points upwards.
            float xNew = colIdx - imageWidth / 2;
            float yNew = imageHeight / 2 - 1 - rowIdx;

            //Convert to polar coordinates
            float rSquared = xNew * xNew + yNew * yNew;
            float theta = atan2f(yNew, xNew);

            //Polar equation of the Quadrifolium: https://en.wikipedia.org/wiki/Quadrifolium
            float RHS = 500 * cos(2 * theta);
            if (rSquared < RHS*RHS) ppmFileStream << "255 249 146\t";
            else ppmFileStream << "103 138 82\t";
        }
        ppmFileStream << "\n";
    }

    ppmFileStream.close();

    return 0;
}

