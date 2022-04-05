#pragma once
#include "Math.h"
#include <assert.h>

namespace ChaosCampAM {
    //Forward declaration
    class Camera;

    class Ray {
    public:
        //Default: ray in the -z direction starting from the origin
        Ray() : origin(), dir(0.0, 0.0, -1.0) {}
        Ray(const Vector3& origin, const Vector3& direction) :origin(origin), dir(direction) { dir.normalize(); };

        //Getters
        Vector3 getOrigin() const { return origin; }
        Vector3 getDirection() const { return dir; }

        //Get a point lying on the ray, based on parameter t in the range [0;+inf). Negative values forbidden!
        Vector3 getPointOnRay(float t) const;

    private:
        Vector3 origin;
        Vector3 dir;
    };

    //Non-member to compute the camera ray through the centre of pixel (x,y).
    //Pixel (0,0) assumed in top left corner.
    Ray computeCameraRay(int xIndex, int yIndex, int imageHeight, float aspectRatio, const Camera& cam);
}
