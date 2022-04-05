#include "Ray.h"
#include "Camera.h"

namespace ChaosCampAM {

    Vector3 Ray::getPointOnRay(float t) const {
        assert(t >= 0);
        //We assume direction is normalised
        return origin + t * dir;
    }

    Ray computeCameraRay(int xIndex, int yIndex, int imageHeight, float aspectRatio, const Camera& cam) {
        //Calculate normalised coordinates of pixel centre
        float x = (float)xIndex + 0.5f;
        float y = (float)yIndex + 0.5f;
        x /= (float)imageHeight * aspectRatio;
        y /= imageHeight;

        //Calculate screen-space coordinates
        x = (2.0f * x) - 1.0f;
        y = 1.0f - (2.0f * y);
        x *= aspectRatio;

        //Extract normalised ray through pixel. Normalisation happens in Ray constructor.
        return Ray(cam.getPosition(), cam.getOrientation() * Vector3(x, y, -1.0));
    }
}