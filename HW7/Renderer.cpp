#include"Renderer.h"
#include"Scene.h"
#include"ColorRGB.h"
#include"Ray.h"
#include"Mesh.h"
#include<assert.h>

//Used for const shading
ColorRGB colors[6] = {
  {251, 255, 224},
  {213, 230, 85},
  {18, 45, 48},
  {39, 219, 135},
  {204, 234, 237},
  {84, 91, 92}
};

void ChaosCampAM::Renderer::render(const Scene& scene, const std::string& filename) {
  //Initial getters
  const Settings& settings = scene.getSettings();
  int imageWidth = settings.getWidth();
  int imageHeight = settings.getHeight();
  float aspectRatio = settings.getAspectRatio();
  const Camera& cam = scene.getCamera();
  const std::vector<Mesh>& meshes = scene.getMeshes();

  //Set up output file stream
  std::ofstream ppmFileStream(filename, std::ios::out | std::ios::binary);
  assert(ppmFileStream.is_open());
  ppmFileStream << "P3\n";
  ppmFileStream << imageWidth << " " << imageHeight << "\n";
  ppmFileStream << ColorRGB::maxColorComponents << "\n";

  //Loop through pixels and shoot camera rays
  for (int rowIdx = 0; rowIdx < imageHeight; ++rowIdx) {
    for (int colIdx = 0; colIdx < imageWidth; ++colIdx) {
      Ray ray = computeCameraRay(colIdx, rowIdx, imageHeight, aspectRatio, cam);

      //intersect
      Vector3 intersection;
      int triIndex=0;
      float dist = findIntersection(ray, meshes, intersection,triIndex);

      if (dist > 0.0f) {
        //intersection occured - colour pixel based on index
        ColorRGB triColor = colors[triIndex % 6];
        ppmFileStream << (int)triColor.r << " " << (int)triColor.g << " " << (int)triColor.b << "\t";
      }
      else {
        //no intersection - use background colour instead
        ColorRGB bgColor = settings.getBgColor();
        ppmFileStream << (int)bgColor.r << " " << (int)bgColor.g << " " << (int)bgColor.b << "\t";
      }
      
    }
    ppmFileStream << "\n";
  }

  ppmFileStream.close();
}

float ChaosCampAM::Renderer::findIntersection(const Ray& ray, const std::vector<Mesh>& meshes, 
  Vector3& intersection, int& triIndex) {
  //Closest intersection among all meshes
  float closestDist = FLT_MAX;
  Vector3 closestIntersect;

  //Loop through all meshes and find closest intersection
  Vector3 intersect;
  for (const Mesh& mesh : meshes) {
    float dist = mesh.intersect(ray, intersect,triIndex);
    if (dist > 0.0f && dist < closestDist) {
      closestDist = dist;
      closestIntersect = intersect;
    }
  }

  if (closestDist < FLT_MAX) {
    //intersection occured
    intersection = closestIntersect;
    return closestDist;
  } else {
    //no intersection
    return -1.0f;
  }
}
