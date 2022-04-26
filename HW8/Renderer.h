#pragma once
#include<string>
#include<fstream>
#include<vector>

namespace ChaosCampAM {

  //Forward declarations
  class Scene;
  class Ray;
  class Mesh;
  class Vector3;
  class ColorRGB;

  /*
  * A Ray-Tracing Renderer. Takes a scene description and renders an image file.
  * De-coupled from any scene data - a universal renderer that can be applied to many different scenes.
  */
  class Renderer {
  public:
    Renderer() {} //empty constructor

    //Render a scene to a .ppm file with the given filename (newly created).
    void render(const Scene& scene, const std::string& filename);

  private:
    //Find the closest intersection point (if any) of a ray with a collection of meshes.
    // - Returns distance to closest intersection. Intersection point stored in 'intersection'.
    // - If no intersection found, returns -1.0.
    float findIntersection(const Ray& ray, const std::vector<Mesh>& meshes, Vector3& intersection, Vector3& triNormal);

    //Perform shading on a given point. 
    //Diffuse lighting for now.
    ColorRGB shade(const Vector3& point, const Vector3& triNormal, const Scene& scene);
  };
}