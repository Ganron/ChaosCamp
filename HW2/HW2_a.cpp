/*
* Chaos Camp: Ray-Tracing Course
* HOMEWORK: LECTURE 2, PROBLEM 1
* Author: Asen Markov
* Date: 07/03/2022
* 
* Description:
* We render a square grid where all squares have the same hue (bluish) but different brightness.
* Over that background, we render the Lemniscate of Bernoulli (curve giving the infinity symbol), going for a 
* "pixelated" look. Some aliasing may occur.
*/

#include <fstream>
#include <cmath>
#include <cstdlib>

/// Output image resolution
static const int imageWidth = 1920;
static const int imageHeight = 1080;

static const int maxColorComponent = 255;

// Number of pixels giving the side of a grid square
static const int squareSize = 40;

// A struct to represent an RGB color value. Every channel is an unsigned integer in the interval [0;255].
struct ColorRGB {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    ColorRGB() : r(0), g(0), b(0) {}
    ColorRGB(unsigned char red, unsigned char green, unsigned char blue) : r(red), g(green), b(blue) {}
};

// A struct to represent a Hue-Saturation-Value (HSV) color.
// For the color to be valid, the following restrictions hold:
//   *  0 < h < 360;
//   *  0 < s < 1.0;
//   *  0 < v < 1.0;
struct ColorHSV {
    float h;
    float s;
    float v;
    ColorHSV() : h(0.0), s(0.0), v(0.0) {}
    ColorHSV(float hue, float saturation, float value) : h(hue), s(saturation), v(value) {}

    // Converts the color to RGB format. Valid color is assumed.
    // Conversion table taken from https://www.rapidtables.com/convert/color/hsv-to-rgb.html
    ColorRGB toRGB() {
        if (!isValid()) return ColorRGB();

        float chroma = v * s;
        float x = chroma * (1 - abs((float)fmod(h / 60, 2) - 1));
        float m = v - chroma;
        
        float r = 0.0;
        float g = 0.0;
        float b = 0.0;

        if (h < 60.0) {
            r = chroma;
            g = x;
        } else if (h < 120.0) {
            r = x;
            g = chroma;
        } else if (h < 180.0) {
            g = chroma;
            b = x;
        } else if (h < 240.0) {
            g = x;
            b = chroma;
        } else if (h < 300.0) {
            r = x;
            b = chroma;
        }
        else if (h < 360.0) {
            r = chroma;
            b = x;
        }

        return ColorRGB(
            (unsigned char)((r + m) * 255),
            (unsigned char)((g + m) * 255),
            (unsigned char)((b + m) * 255));
    }


    bool isValid() {
        if (h < 0.0 || h>360.0) return false;
        if (s < 0.0 || s>1.0) return false;
        if (v < 0.0 || v>1.0) return false;
        return true;
    }
};

//Rational lemniscate parametrisation: https://en.wikipedia.org/wiki/Lemniscate_of_Bernoulli
//Function takes parameter t between -1 and 1 and sets the x and y values to the corresponding coordinates.
//Returns false on invalid parameter value.
bool lemniscate(float t, float& x, float& y) {
    if (t < -1.0 || t>1.0) return false;
    x = (t + t * t * t) / (1 + t * t * t * t);
    y = (t - t * t * t) / (1 + t * t * t * t);
    return true;
}

//Function takes the "framebuffer" (array storing a colour for each of the squares in the grid) which
//currently has background colours only (assumed!) and draws the lemniscate curve on top.
void drawLemniscate(ColorRGB * const colorLookup, int numSquaresX, int numSquaresY) {
    //Take steps along the curve - step size configurable
    float t = -1.0;
    float dt = 0.01;
    while (t < 1.0) {
        float x = 0.0;
        float y = 0.0;
        if (lemniscate(t, x, y)) {
            //Transform curve from local coordinates to pixel coordinates.
            x *= (imageWidth - 1) / 2.0;
            x += imageWidth / 2;

            //Account for both "upper" and "lower" part of the lemniscate.
            y *= -(imageWidth - 1) / 2.0;
            int yMirrored = -y;

            y += imageHeight / 2;
            yMirrored += imageHeight / 2;

            if ((int)x >= 0 && (int)x < imageWidth && (int)y >= 0 && (int)y < imageHeight) {
                //Set pixels intersecting with the curve to have a red value (fixed hue) with arbitrary brightness
                //ranging between 80% and 95%.
                int index = ((int)y / squareSize) * numSquaresX + (int)x / squareSize;
                int indexMirrored = ((int)yMirrored / squareSize) * numSquaresX + (int)x / squareSize;

                colorLookup[index] = ColorHSV(0, 0.8, 0.01 * (rand() % 16 + 80)).toRGB();
                colorLookup[indexMirrored] = ColorHSV(0, 0.8, 0.01 * (rand() % 16 + 80)).toRGB();
            }
        }

        t += dt;
    }
}

int main() {
    //Set up output stream
    std::ofstream ppmFileStream("am_hw2_a_edited.ppm", std::ios::out | std::ios::binary);
    ppmFileStream << "P3\n";
    ppmFileStream << imageWidth << " " << imageHeight << "\n";
    ppmFileStream << maxColorComponent << "\n";

    //Set up grid
    int numSquaresX = imageWidth / squareSize+1;
    int numSquaresY = imageHeight / squareSize+1;

    // All squares have the same hue but slightly different "brightness".
    // Here we store the colors for each of the squares.
    ColorRGB* colorLookup = new ColorRGB[numSquaresX * numSquaresY];
    for (int i = 0; i < numSquaresX; i++) {
        for (int j = 0; j < numSquaresY; j++) {
            int index = j * numSquaresX + i;

            //A randomised blue color, where the "Value" component ranges between 75% and 90%
            colorLookup[index] = ColorHSV(240, 0.5 , 0.01 * (rand() % 16 + 75)).toRGB();
        }
    }

    //Draw the Lemniscate curve with "reddish" tones on top of the blue background.
    drawLemniscate(colorLookup, numSquaresX, numSquaresY);

    //Draw
    for (int rowIdx = 0; rowIdx < imageHeight; ++rowIdx) {
        for (int colIdx = 0; colIdx < imageWidth; ++colIdx) {
            // We get the index of the square from the grid containing the current pixel
            int index = (rowIdx / squareSize) * numSquaresX + (colIdx/squareSize);
          
            ppmFileStream << (int)(colorLookup[index].r) << " ";
            ppmFileStream << (int)(colorLookup[index].g) << " ";
            ppmFileStream << (int)(colorLookup[index].b) << "\t";
        }
        ppmFileStream << "\n";
    }

    ppmFileStream.close();

    delete[] colorLookup;

    return 0;
}
