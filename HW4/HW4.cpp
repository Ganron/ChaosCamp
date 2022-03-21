/*
* Chaos Camp: Ray-Tracing Course
* HOMEWORK: LECTURE 4, PROBLEM 1
* Author: Asen Markov
* Date: 21/03/2022
*
* Description:
* Include triangle representation in a previous homework project.
*/

#include <fstream>
#include <cmath>
#include <cstdlib>

/// Output image resolution
static const int imageWidth = 1920;
static const int imageHeight = 1080;

static const int maxColorComponent = 255;

// Vector in 3D space
class Vector3 {
public:
    float x;
    float y;
    float z;

    Vector3() :x(0.0), y(0.0), z(0.0) {}
    Vector3(float x, float y, float z) :x(x), y(y), z(z) {}

    Vector3 operator+(const Vector3& v) { return Vector3(x + v.x, y + v.y, z + v.z); }
    Vector3 operator-(const Vector3& v) { return Vector3(x - v.x, y - v.y, z - v.z); }
};

// Triangle in 3D space. Vectices are ordered in counter-clockwise manner.
class Triangle {
public:
    Vector3 a;
    Vector3 b;
    Vector3 c;
};

int main() {
    //Set up output stream
    std::ofstream ppmFileStream("am_hw2_b.ppm", std::ios::out | std::ios::binary);
    ppmFileStream << "P3\n";
    ppmFileStream << imageWidth << " " << imageHeight << "\n";
    ppmFileStream << maxColorComponent << "\n";

    //Draw
    for (int rowIdx = 0; rowIdx < imageHeight; ++rowIdx) {
        for (int colIdx = 0; colIdx < imageWidth; ++colIdx) {
            // Set coordinate system such that the origin is at the centre of the screen and y-axis points upwards.
            float xNew = colIdx - imageWidth / 2;
            float yNew = imageHeight/2 - 1 - rowIdx;

            //Convert to polar coordinates
            float r = sqrt(xNew * xNew + yNew * yNew);
            float theta = atan2f(yNew, xNew);

            //Polar equation of the Quadrifolium: https://en.wikipedia.org/wiki/Quadrifolium
            if(r<abs(500*cos(2*theta))) ppmFileStream << "255 249 146\t";
            else ppmFileStream << "103 138 82\t";
        }
        ppmFileStream << "\n";
    }

    ppmFileStream.close();

    return 0;
}
