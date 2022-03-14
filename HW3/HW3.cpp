/*
* Chaos Camp: Ray-Tracing Course
* HOMEWORK: LECTURE 3, PROBLEM 1
* Author: Asen Markov
* Date: 14/03/2022
*
* Description:
* Trace a ray through each pixel and save the resulting coordinates
* for each ray (normalized) as color components in an image map.
*/

#include <fstream>
#include <iostream>

/// Output image resolution
static const int imageWidth = 1920;
static const int imageHeight = 1080;

static const int maxColorComponent = 255;

//Class to represent a vector in 3-dimensional space
class Vector3D {
public:
    float x;
    float y;
    float z;

    //Zero vector is the default
    Vector3D() : x(0.0), y(0.0), z(0.0) {}
    Vector3D(float x, float y, float z) : x(x), y(y), z(z) {}

    //Get the vector magnitude (euclidian norm)
    float getLength() const { return sqrt(x * x + y * y + z * z); }

    //Get a unit vector pointing in the same direction as the current vector
    Vector3D getNormalized() const {
        float length = this->getLength();
        return Vector3D(x / length, y / length, z / length);
    }
};

int main() {
    //Set up output stream
    std::ofstream ppmFileStream("am_hw3.ppm", std::ios::out | std::ios::binary);
    ppmFileStream << "P3\n";
    ppmFileStream << imageWidth << " " << imageHeight << "\n";
    ppmFileStream << maxColorComponent << "\n";

    //Draw
    for (int rowIdx = 0; rowIdx < imageHeight; ++rowIdx) {
        for (int colIdx = 0; colIdx < imageWidth; ++colIdx) {
            //Calculate pixel centre normalized coordinates
            float x = (float)colIdx + 0.5;
            float y = (float)rowIdx + 0.5;
            x /= imageWidth;
            y /= imageHeight;

            //Calculate screen-space coordinates
            x = (2.0 * x) - 1.0;
            y = 1.0 - (2.0 * y);
            x *= (float)imageWidth / imageHeight;

            //Extract normalized ray through pixel
            Vector3D ray = Vector3D(x, y, -1.0).getNormalized();

            //Convert to RGB, treating each coordinate as a color coefficient and taking the abs() when negative
            //The absolute value will make the image look symmetric.
            ppmFileStream << (int)(abs(ray.x) * 255) << " ";
            ppmFileStream << (int)(abs(ray.y) * 255) << " ";
            ppmFileStream << (int)(abs(ray.z) * 255) << "\t";
        }
        ppmFileStream << "\n";
    }

    ppmFileStream.close();

    return 0;
}