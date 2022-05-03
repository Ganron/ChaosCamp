#include "Ray.h"
#include "Camera.h"
#include "Constants.h"
#include<assert.h>

namespace ChaosCampAM {

  Vector3 Ray::getPointOnRay(float t) const {
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

  Ray computeReflectedRay(const Vector3& incomingRay, const Vector3& intersectionPoint, const Vector3& normal) {
    Vector3 reflectedDir = incomingRay - 2 * incomingRay.dot(normal) * normal;
    return Ray(intersectionPoint + normal * REFLECTION_RAY_BIAS, reflectedDir);
  }
}