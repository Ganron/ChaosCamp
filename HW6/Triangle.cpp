#include "Triangle.h"
#include "Ray.h"
#include <assert.h>

namespace ChaosCampAM {
    Triangle::Triangle(const Vector3& v0, const Vector3& v1, const Vector3& v2) : vert{ v0,v1,v2 }, n((v1 - v0).cross(v2 - v0)) {
        n.normalize();
    }

    Vector3 Triangle::v(int index) const {
        assert(index >= 0 && index < 3);
        return vert[index];
    }

    Vector3 Triangle::e(int index) const {
        assert(index >= 0 && index < 3);
        if (index == 0) {
            return vert[1] - vert[0];
        }
        else if (index == 1) {
            return vert[2] - vert[1];
        }
        return vert[0] - vert[2];
    }

    void Triangle::setVertex(int index, const Vector3& newV) {
        assert(index >= 0 && index < 3);
        vert[index] = newV;
        n = (vert[1] - vert[0]).cross(vert[2] - vert[0]);
        n.normalize();
    }

    //Ray-Triangle intersection implemented after T.Moller and B.Trumbore, '97: 
    // - https://cadxfem.org/inf/Fast%20MinimumStorage%20RayTriangle%20Intersection.pdf
    // NOTE: This algorithm does NOT require the normal vector to be explicitly stored in memory. 
    float Triangle::intersect(const Ray& ray, Vector3& intersection) const {
        //E0 = edge from vertex 0 to vertex 1
        Vector3 e0 = e(0);
        //E1 = edge from vertex 0 to vertex 2
        Vector3 e1 = -e(2);

        //p - coefficient used when calculating the determinant, but also used later!
        Vector3 dir = ray.getDirection();
        Vector3 p = dir.cross(e1);
        float det = e0.dot(p);

        //If det is 0, then ray direction is parallel to triangle plane
        //If det is negative, then triangle is back-facing
        if (det < EPSILON) {
            return -1.0;
        }

        //t - distance from vertex 0 to ray origin
        Vector3 t = ray.getOrigin() - vert[0];

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

        intersection = ray.getPointOnRay(dist);
        return dist;
    }
}