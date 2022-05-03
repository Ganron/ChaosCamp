#pragma once
#include<string>
#include<fstream>
#include<vector>

namespace ChaosCampAM {

  //Forward declarations
  class Scene;
  class Ray;
  class Mesh;
  class Material;
  class Camera;
  class Vector3;
  class ColorRGB;
  class InfoIntersect;

  //Shading mode
  enum class ShadingMode {Light, Barycentric};

  /*
  * A Ray-Tracing Renderer. Takes a scene description and renders an image file.
  * De-coupled from any scene data - a universal renderer that can be applied to many different scenes.
  */
  class Renderer {
  public:
    Renderer() {} //empty constructor

    //Render a scene to a .ppm file with the given filename (newly created).
    void render(const Scene& scene, const std::string& filename, const ShadingMode& shadingMode);

  private:
    
    //Trace the given ray into the scene and determine colour at intersection point (if any). In case of no intersection, returns
    //the background colour.
    //Recursively traces new rays into the scene uppon hitting a reflective material.
    //
    //Note: colour is returned as a vector (colour values between 0.0 and 1.0)
    Vector3 rayTrace(const Ray& ray, int depth, ShadingMode shadingMode, const Scene& scene);

    //Find the closest intersection point (if any) of a ray with a collection of meshes.
    // - Returns distance to closest intersection. Intersection point stored in 'intersection'.
    // - If no intersection found, returns -1.0.
    float findIntersection(const Ray& ray, const std::vector<Mesh>& meshes, InfoIntersect& intersectInfo, 
      int& meshIndex, int& triIndex);

    //Given the available intersection information (intersection point, index of intersected mesh, index of intersected triangle),
    //compute the hit normal (interpolated from the three vertex normals at the vertices of the triangle).
    Vector3 extractHitNormal(const std::vector<Mesh>& meshes, const InfoIntersect& intersectInfo, int meshIndex, int triIndex);

    //Perform Lambertian shading on a given point. 
    //Diffuse lighting for now.
    Vector3 shadeLambertian(const Vector3& point, const Vector3& normal, const Vector3& albedo, const Scene& scene);
    
    //Color point based on its barycentric coordinates. No lights required.
    Vector3 shadeBarycentric(float coords[3]);
    
  };
}