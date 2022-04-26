#include"Renderer.h"
#include"Scene.h"
#include"ColorRGB.h"
#include"Ray.h"
#include"Mesh.h"
#include"Math/MathUtil.h"
#include"Constants.h"
#include<assert.h>
#include<algorithm>

#include<iostream>
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
      Vector3 triNormal;
      float dist = findIntersection(ray, meshes, intersection,triNormal);

      if (dist > -EPSILON) {
        //intersection occured - colour pixel based on index
        ColorRGB triColor = shade(intersection, triNormal,scene);
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
  Vector3& intersection, Vector3& triNormal) {
  //Closest intersection among all meshes
  float closestDist = FLT_MAX;
  Vector3 closestIntersect;

  //Loop through all meshes and find closest intersection
  Vector3 intersect;
  Vector3 normal;
  for (const Mesh& mesh : meshes) {
    float dist = mesh.intersect(ray, intersect, normal);
    if (dist > -EPSILON && dist < closestDist) {
      closestDist = dist;
      closestIntersect = intersect;
    }
  }
 
  if (closestDist < FLT_MAX) {
    //intersection occured
    intersection = closestIntersect;
    triNormal = normal;
    return closestDist;
  } else {
    //no intersection
    return -1.0f;
  }
}

ChaosCampAM::ColorRGB ChaosCampAM::Renderer::shade(const Vector3& point, const Vector3& triNormal, const Scene& scene) {
  Vector3 finalColor;
  for (PointLight pointLight : scene.getPointLights()) {
    //Direction from point to light
    Vector3 lightDir = pointLight.pos - point;
    //Sphere radius
    float rad = lightDir.getLen();
    //Normalise light direction
    lightDir = lightDir * (1 / rad);

    //Lambertian shading
    float cos = std::max(0.0f, lightDir.dot(triNormal));
    float sphereArea = 4 * PI * rad * rad;
    Ray shadowRay(point + triNormal * SHADOW_BIAS, lightDir);

    Vector3 intersection;
    Vector3 normal;
    float dist = findIntersection(shadowRay, scene.getMeshes(), intersection, normal);
    if (abs(dist + 1.0f) < EPSILON) {
      //no intersection, i.e dist = -1
      float r = (pointLight.intensity * ALBEDO.x*cos) / (sphereArea);
      float g = (pointLight.intensity * ALBEDO.y*cos) / (sphereArea );
      float b = (pointLight.intensity * ALBEDO.z*cos) / (sphereArea );

      finalColor = finalColor + Vector3(r, g, b);
    }
  }
  return ColorRGB(finalColor);
}
