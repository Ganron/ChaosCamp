#include"Mesh.h"
#include"Triangle.h"
#include<assert.h>

namespace ChaosCampAM {

  Mesh::Mesh(int vertexHint, int triangleHint) {
    vertexList.reserve(vertexHint);
    triIndexList.reserve(triangleHint);
  }

  Mesh::Mesh(const std::vector<Vector3>& vertices, const std::vector<TriProxy>& triangles) :
    vertexList(vertices), triIndexList(triangles) {}

  void Mesh::pushVertex(const Vector3& vert) {
    vertexList.push_back(vert);
  }

  void Mesh::pushTriangle(const TriProxy& tri) {
    triIndexList.push_back(tri);
  }

  float Mesh::intersect(const Ray& ray, Vector3& intersection) const {
    //For each triangle index tuple, construct an actual triangle object and test for intersection
    float closestDist = FLT_MAX;
    Vector3 closestIntersect;

    for (TriProxy tri : triIndexList) {
      //ensure valid indices
      int vertexCount = vertexList.size();
      assert(tri.v0 < vertexCount && tri.v1 < vertexCount && tri.v2 < vertexCount);

      //construct actual triangle object and intersect
      Triangle realTri(vertexList.at(tri.v0), vertexList.at(tri.v1), vertexList.at(tri.v2));
      float dist = realTri.intersect(ray, closestIntersect);

      //keep only closest intersection
      if (dist > 0.0 && dist < closestDist) {
        closestDist = dist;
        intersection = closestIntersect;
      }
    }

    if (closestDist < FLT_MAX) {
      //intersection occured
      return closestDist;
    } else {
      //no intersect
      return -1.0f;
    }

    return 0.0f;
  }
}