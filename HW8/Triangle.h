#pragma once
#include "Math/Vector3.h"

namespace ChaosCampAM {
  //Forward declaration
  class Ray;

  // Triangle in 3D space. Vectices are ordered in counter-clockwise manner.
  class Triangle {
  public:
    Triangle(const Vector3& v0, const Vector3& v1, const Vector3& v2);

    //Get a vertex of the triangle (v0/v1/v2). Indices outside the [0;2] range are forbidden.
    Vector3 v(int index) const;

    //Get an edge of the triangle. Index encoding:
    // * e0 = v1 - v0 (edge from v0 to v1)
    // * e1 = v2 - v1 (edge from v1 to v2)
    // * e2 = v0 - v2 (edge from v2 to v0)
    //Indices outside the [0;2] range are forbidden.
    Vector3 e(int index) const;

    //Get the normal of the triangle. It is assumed that the vertices are ordered in counter-clockwise order.
    //Returned normal has unit length.
    Vector3 normal() const;

    //The 'intersection' parameter is an OUT parameter that is set to the intersection point if such exists.
    //Return value:
    // * distance from ray origin to the intersection point if such exists (i.e. the value of the t parameter for the ray)
    // * -1.0 if no intersection (negative value)
    float intersect(const Ray& ray, Vector3& intersection) const;

    //Set a vertex of the triangle (v0/v1/v2 = newV) and update normal. Indices outside the [0;2] range are forbidden.
    void setVertex(int index, const Vector3& newV);

  private:
    //Vertices
    Vector3 vert[3];

    //Normal vector
    Vector3 n;
  };

}
