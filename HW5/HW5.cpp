/*
* Chaos Camp: Ray-Tracing Course
* HOMEWORK: LECTURE 5, PROBLEM 1
* Author: Asen Markov
* Date: 28/03/2022
*
* Description:
* We introduce several new classes to implement ray-triangle intersection. All 4 homework problems are solved
* in this single file. Each homework problem corresponds to a different scene configuration.
*  - Problem 1: Render the given triangle.
*  - Problem 2: Render some other arbitrary triangle.
*  - Problem 3: Render two triangles.
*  - Problem 4: Create and render a more complex scene composed of triangles. We create a "beehive" net of triangles
* with appropriate colouring to simulate a composition of 3D cubes.
*/

#include <fstream>
#include <iostream>
#include <assert.h>
#include <vector>
#include <utility>

namespace ChaosCampAM {
    //Used when doing floating-point comparisons
    static const int EPSILON = 1e-7f;

    //Output image resolution
    static const int imageWidth = 1920;
    static const int imageHeight = 1080;

    static const int maxColorComponent = 255;

    //Class to represent a vector in 3-dimensional space
    class Vector3 {
    public:
        float x;
        float y;
        float z;

        //Zero vector is the default
        Vector3() : x(0.0), y(0.0), z(0.0) {}
        Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
        Vector3(const Vector3& v) : x(v.x), y(v.y), z(v.z) {}

        //Arithmetics
        Vector3 operator+(const Vector3& v) const { return Vector3(x + v.x, y + v.y, z + v.z); }
        Vector3 operator-(const Vector3& v) const { return Vector3(x - v.x, y - v.y, z - v.z); }
        Vector3 operator*(float s) const { return Vector3(x * s, y * s, z * s); }
        Vector3 operator-() const { return Vector3(-x, -y, -z); }

        //Dot product
        float dot(const Vector3& v) const { return x * v.x + y * v.y + z * v.z; }

        //Cross product
        Vector3 cross(const Vector3& v) const {
            return Vector3(
                y * v.z - z * v.y,
                z * v.x - x * v.z,
                x * v.y - y * v.x
            );
        }

        //Get the vector magnitude (euclidian norm)
        float getLen() const { return sqrt(x * x + y * y + z * z); }

        //Get the square of the vector magnitude to avoid sqrt() operation.
        float getLenSquared() const { return x * x + y * y + z * z; }

        //Get a unit vector pointing in the same direction as the current vector
        void normalize() {
            float len = this->getLen();

            //handle division by zero
            assert(abs(len) > 0.000001f);

            float coeff = 1 / len;
            x = x * coeff;
            y = y * coeff;
            z = z * coeff;
        }
    };

    //Non-member functin to handle (scalar)*(vector3) operation. Encapsulated in the ChaosCampAM namespace.
    Vector3 operator*(float s, const Vector3& v) { return v * s; }

    // Triangle in 3D space. Vectices are ordered in counter-clockwise manner.
    class Triangle {
    public:
        Triangle(const Vector3& v0, const Vector3& v1, const Vector3& v2) : vert{ v0,v1,v2 }, n((v1 - v0).cross(v2 - v0)) {
            n.normalize();
        }

        //Get a vertex of the triangle (v0/v1/v2). Indices outside the [0;2] range are forbidden.
        Vector3 v(int index) const {
            assert(index >= 0 && index < 3);
            return vert[index];
        }

        //Get an edge of the triangle. Index encoding:
        // * e0 = v1 - v0 (edge from v0 to v1)
        // * e1 = v2 - v1 (edge from v1 to v2)
        // * e2 = v0 - v2 (edge from v2 to v0)
        //Indices outside the [0;2] range are forbidden.
        Vector3 e(int index) const {
            assert(index >= 0 && index < 3);
            if (index == 0) {
                return vert[1] - vert[0];
            }
            else if (index == 1) {
                return vert[2] - vert[1];
            }
            else if (index == 2) {
                return vert[0] - vert[2];
            }
        }

        //Get the normal of the triangle. It is assumed that the vertices are ordered in counter-clockwise order.
        Vector3 normal() const {
            return this->n;
        }

        //Set a vertex of the triangle (v0/v1/v2 = newV) and update normal. Indices outside the [0;2] range are forbidden.
        void setVertex(int index, const Vector3& newV) {
            assert(index >= 0 && index < 3);
            vert[index] = newV;
            n = (vert[1] - vert[0]).cross(vert[2] - vert[0]);
            n.normalize();
        }
    private:
        //Vertices
        Vector3 vert[3];

        //Normal vector
        Vector3 n;
    };

    class Ray {
    public:
        //Default: ray in the -z direction starting from the origin
        Ray() : origin(), dir(0.0, 0.0, -1.0) {}
        Ray(const Vector3& origin, const Vector3& direction) :origin(origin), dir(direction) { dir.normalize(); };

        //Getters
        Vector3 getOrigin() const { return origin; }
        Vector3 getDirection() const { return dir; }

        //Get a point lying on the ray, based on parameter t in the range [0;+inf). Negative values forbidden!
        Vector3 getPointOnRay(int t) const {
            assert(t >= 0);
            //We assume direction is normalised
            return origin + t * dir;
        }

        //Ray-Triangle intersection implemented after T.Moller and B.Trumbore, '97: 
        // - https://cadxfem.org/inf/Fast%20MinimumStorage%20RayTriangle%20Intersection.pdf
        // NOTE: This algorithm does NOT require the normal vector to be explicitly stored in memory.
        // 
        //The 'intersection' parameter is an OUT parameter that is set to the intersection point if such exists.
        //Return value:
        // * distance from ray origin to the intersection point if such exists (i.e. the value of the t parameter for the ray)
        // * -1.0 if no intersection (negative value)
        float intersect(const Triangle& tri, Vector3& intersection) {
            //E0 = edge from vertex 0 to vertex 1
            Vector3 e0 = tri.e(0);
            //E1 = edge from vertex 0 to vertex 2
            Vector3 e1 = -tri.e(2);

            //p - coefficient used when calculating the determinant, but also used later!
            Vector3 p = dir.cross(e1);
            float det = e0.dot(p);

            //If det is 0, then ray direction is parallel to triangle plane
            //If det is negative, then triangle is back-facing
            if (det < EPSILON) {
                return -1.0;
            }

            //t - distance from vertex 0 to ray origin
            Vector3 t = origin - tri.v(0);

            //The 'U' barycentric coordinate - the one associated with v1 (yet unnormalised) 
            float uCoord = t.dot(p);
            if (uCoord<0.0 || uCoord > det) {
                return -1.0;
            }

            //The 'V' barycentric coordinate - the one associated with v2 (yet unnormalised)
            Vector3 q = t.cross(e0);
            float vCoord = dir.dot(q);
            if (vCoord<0.0 || uCoord + vCoord > det) {
                return -1.0;
            }

            //Calculate distance (along the ray) to the intersection point and calculate intersection itself.
            float dist = e1.dot(q);
            float invDet = 1 / det;
            dist *= invDet;
            uCoord *= invDet;
            vCoord *= invDet;

            intersection = getPointOnRay(dist);
            return dist;
        }

        //Compute the camera ray through the centre of pixel (x,y).
        //Pixel (0,0) assumed in top left corner.
        static Ray computeCameraRay(int xIndex, int yIndex, int imageHeight, float aspectRatio) {
            //Calculate normalised coordinates of pixel centre
            float x = (float)xIndex + 0.5;
            float y = (float)yIndex + 0.5;
            x /= (float)imageHeight*aspectRatio;
            y /= imageHeight;

            //Calculate screen-space coordinates
            x = (2.0 * x) - 1.0;
            y = 1.0 - (2.0 * y);
            x *= aspectRatio;

            //Extract normalised ray through pixel. Normalisation happens in Ray constructor.
            return Ray(Vector3(),Vector3(x, y, -1.0));
        }

    private:
        Vector3 origin;
        Vector3 dir;
    };

    // A struct to represent an RGB color value. Every channel is an unsigned integer in the interval [0;255].
    struct ColorRGB {
        unsigned char r;
        unsigned char g;
        unsigned char b;
        ColorRGB() : r(0), g(0), b(0) {}
        ColorRGB(unsigned char red, unsigned char green, unsigned char blue) : r(red), g(green), b(blue) {}
    };
}

using namespace ChaosCampAM;

void setUpScene(std::vector<std::pair<Triangle, ColorRGB>>& triangleList);

int main() {
    //Set up output stream
    std::ofstream ppmFileStream("am_hw5_d.ppm", std::ios::out | std::ios::binary);
    ppmFileStream << "P3\n";
    ppmFileStream << imageWidth << " " << imageHeight << "\n";
    ppmFileStream << maxColorComponent << "\n";

    //Set up Scene - consists of (triangle,colour> pairs
    std::vector<std::pair<Triangle,ColorRGB>> triangles;
    setUpScene(triangles);

    float aspectRatio = (float)imageWidth / imageHeight;

    //Draw
    for (int rowIdx = 0; rowIdx < imageHeight; ++rowIdx) {
        for (int colIdx = 0; colIdx < imageWidth; ++colIdx) {
            Ray ray = Ray::computeCameraRay(colIdx, rowIdx, imageHeight, aspectRatio);

            float closestDist = FLT_MAX;
            ColorRGB finalColor(51,51,0); //background colour

            for (const auto& tri : triangles) {
                Vector3 intersection;
                float dist = ray.intersect(tri.first, intersection);
                if (dist > 0.0 && dist < closestDist) {
                    //This is now the closest triangle
                    closestDist = dist;
                    finalColor = tri.second;
                }
            }

            ppmFileStream << (int)finalColor.r << " " << (int)finalColor.g << " " << (int)finalColor.b << "\t";
        }
        ppmFileStream << "\n";
    }

    ppmFileStream.close();

    return 0;
}

void setUpScene(std::vector<std::pair<Triangle, ColorRGB>>& triangleList) {
    //Problem 1
    //triangleList.push_back(std::make_pair(
    //    Triangle(
    //        Vector3(-1.75, -1.75, -3),
    //        Vector3(1.75, -1.75, -3),
    //        Vector3(0, 1.75, -3)),
    //    ColorRGB(255, 255, 100)));


    //Problem 2
    //triangleList.push_back(std::make_pair(
    //    Triangle(
    //        Vector3(-5, -2.75, -3),
    //        Vector3(3, -1.75, -5),
    //        Vector3(0, 3, -3)),
    //    ColorRGB(255, 255, 100)));

    //Problem 3
    //triangleList.push_back(std::make_pair(
    //    Triangle(
    //        Vector3(-1.75, 0, -3),
    //        Vector3(0, -1.75, -3),
    //        Vector3(0, 1.75, -3)),
    //    ColorRGB(255, 255, 100)));
    //triangleList.push_back(std::make_pair(
    //    Triangle(
    //        Vector3(0, -1.75, -3),
    //        Vector3(1.75, 0, -3),
    //        Vector3(0, 1.75, -3)),
    //    ColorRGB(255, 100, 0)));

    //Problem 4
    //Create a "beehive" triangular net to simulate a composition of 3D cubes.
    //Idea taken from the following image:
    //https://cdn3.vectorstock.com/i/1000x1000/19/07/stylish-background-with-small-triangular-shapes-vector-23251907.jpg

    int numTriSidesOnRow = 15;
    float triSide = 2.0 / numTriSidesOnRow;
    float triSideHalf = triSide / 2;
    float aspectRatio = (float)imageWidth / imageHeight;
    float triHeight = triSide * aspectRatio * sqrt(3) / 2;
    int numTriOnCol = ceilf(2 / triHeight);

    //Template triangles (top left) - will be copied over (translated) to generate the whole net.
    Triangle tri1(
        Vector3(aspectRatio * (- 1.0 - triSideHalf), 1.0 - triHeight, -1.0),
        Vector3(aspectRatio * (- 1 + triSideHalf), 1.0 - triHeight, -1.0),
        Vector3(-aspectRatio, 1.0, -1.0)
    );

    Triangle tri2(
        Vector3(-aspectRatio, 1.0, -1.0),
        Vector3(aspectRatio *(- 1.0 + triSideHalf), 1.0 - triHeight, -1),
        Vector3(aspectRatio *(- 1 + triSide), 1.0, -1.0)
    );
    
    //3-channel image (composed of three colours only)
    ColorRGB colors[3] = {
        ColorRGB(248,147,31), //yellow
        ColorRGB(198,156,108), //grey
        ColorRGB(118,76,36) //brown
    };

    //Replicate the template triangles to fill the whole image plane
    for (int i = 0; i < numTriOnCol; i++) {
        for (int j = 0; j < numTriSidesOnRow+1; j++) {
            Vector3 adjustment;
            int colorIdx1 = 0;
            int colorIdx2 = 0;
            if (i % 2 == 0) { //even rows
                adjustment = Vector3(aspectRatio*(j * triSide), -i * triHeight, 0.0);
                if (j % 3 == 0) {
                    colorIdx1 = 0;
                    colorIdx2 = 1;
                }
                else if (j % 3 == 1) {
                    colorIdx1 = 2;
                    colorIdx2 = 2;
                }
                else {
                    colorIdx1 = 1;
                    colorIdx2 = 0;
                }
            }
            else { //odd rows
                adjustment = Vector3(aspectRatio*(j * triSide-triSideHalf), -i * triHeight, 0.0);
                if (j % 3 == 0) {
                    colorIdx1 = 2;
                    colorIdx2 = 2;
                }
                else if (j % 3 == 1) {
                    colorIdx1 = 1;
                    colorIdx2 = 0;
                }
                else {
                    colorIdx1 = 0;
                    colorIdx2 = 1;
                }
            }

            Triangle first(tri1.v(0) + adjustment, tri1.v(1) + adjustment, tri1.v(2) + adjustment);
            Triangle second(tri2.v(0) + adjustment, tri2.v(1) + adjustment, tri2.v(2) + adjustment);

            triangleList.push_back(std::make_pair(first, colors[colorIdx1]));
            triangleList.push_back(std::make_pair(second, colors[colorIdx2]));
        }
    }
}