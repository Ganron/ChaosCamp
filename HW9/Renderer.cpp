#include"Renderer.h"
#include"Scene.h"
#include"ColorRGB.h"
#include"Ray.h"
#include"Mesh.h"
#include"Material.h"
#include"Camera.h"
#include"Math/MathUtil.h"
#include"Math/Vector3.h"
#include"Constants.h"
#include"Triangle.h"
#include<assert.h>
#include<algorithm>

#include<iostream>
void ChaosCampAM::Renderer::render(const Scene& scene, const std::string& filename, const ShadingMode& shadingMode) {
  //Initial getters
  const Settings& settings = scene.getSettings();
  int imageWidth = settings.getWidth();
  int imageHeight = settings.getHeight();
  float aspectRatio = settings.getAspectRatio();
  const Camera& cam = scene.getCamera();

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

      //Calculate pixel color by the method of ray-tracing
      ColorRGB pixelColor = ColorRGB(rayTrace(ray, 0, shadingMode, scene));
      
      ppmFileStream << (int)pixelColor.r << " " << (int)pixelColor.g << " " << (int)pixelColor.b << "\t";
    }
    ppmFileStream << "\n";
  }

  ppmFileStream.close();
}

ChaosCampAM::Vector3 ChaosCampAM::Renderer::rayTrace(const Ray& ray, int depth, ShadingMode shadingMode, const Scene& scene) {
  //initial definitions
  Vector3 pixelColor = scene.getSettings().getBgColor();//default colour is background colour
  if (depth == MAX_TRACING_DEPTH) return pixelColor; //max depth reached - stop tracing
  const std::vector<Mesh>& meshes = scene.getMeshes();

  //intersect
  InfoIntersect intersectInfo;
  int meshIndex = 0;
  int triIndex = 0;
  float dist = findIntersection(ray, meshes, intersectInfo, meshIndex, triIndex);

  if (intersectInfo.hasIntersection) {
    //intersection occured - colour pixel based on shading mode
    if (shadingMode == ShadingMode::Light) {
      
      //extract material properties
      assert(scene.getMaterials().size() > 0);
      const Material& mat = scene.getMaterials()[meshes[meshIndex].getMatIndex()];
      Vector3 albedo = mat.albedo;
      
      Vector3 normal = mat.smoothShading ?
        extractHitNormal(meshes, intersectInfo, meshIndex, triIndex) : //if smooth shading is enabled, take hit normal
        intersectInfo.triNormal; //else take triangle normal
      
      //in case of diffuse material, do lambertian shading
      if (mat.type == MaterialType::Diffuse) { 
        pixelColor = shadeLambertian(intersectInfo.intersectionPoint, normal, albedo, scene);
      }
      // in case of reflective material, trace a reflected ray
      else if (mat.type == MaterialType::Reflective) {
        Ray reflectedRay = computeReflectedRay(ray.getDirection(), intersectInfo.intersectionPoint, normal);
        pixelColor = rayTrace(reflectedRay, ++depth, shadingMode, scene).compMult(albedo);
      }
    }
    else if (shadingMode == ShadingMode::Barycentric) {
      pixelColor = shadeBarycentric(intersectInfo.coords);
    }
  }
  return pixelColor;
}

float ChaosCampAM::Renderer::findIntersection(const Ray& ray, const std::vector<Mesh>& meshes,
  InfoIntersect& intersectInfo, int& meshIndex, int& triIndex) {
  //Closest intersection among all meshes
  float closestDist = FLT_MAX;
  InfoIntersect closestIntersect;
  intersectInfo.hasIntersection = false;

  //Loop through all meshes and find closest intersection
  
  int index = 0;
  int triIndexCurrent = 0;
  for (const Mesh& mesh : meshes) {
    float dist = mesh.intersect(ray, closestIntersect, triIndexCurrent);
    if (closestIntersect.hasIntersection && dist < closestDist) {
      closestDist = dist;
      intersectInfo = closestIntersect;
      meshIndex = index;
      triIndex = triIndexCurrent;
    }
    index++;
  }
 
  //If no intersection occurred, return negative distance
  return intersectInfo.hasIntersection ? closestDist : -1.0f;
}

ChaosCampAM::Vector3 ChaosCampAM::Renderer::extractHitNormal(const std::vector<Mesh>& meshes, const InfoIntersect& intersectInfo, 
  int meshIndex, int triIndex) {

  //extract the triangles and vertex normals of the intersected mesh
  const std::vector<TriProxy>& triangles = meshes[meshIndex].getTriangles();
  const std::vector<Vector3>& vertexNormals = meshes[meshIndex].getVertNormals();

  //find the vertex normals of the intersected triangle within the mesh
  Vector3 normal0 = vertexNormals[triangles[triIndex].v0];
  Vector3 normal1 = vertexNormals[triangles[triIndex].v1];
  Vector3 normal2 = vertexNormals[triangles[triIndex].v2];

  //calculate hit normal as a weighted sum of the three vertex normals
  return intersectInfo.coords[0] * normal0 + intersectInfo.coords[1] * normal1 + intersectInfo.coords[2] * normal2;
}

ChaosCampAM::Vector3 ChaosCampAM::Renderer::shadeLambertian(const Vector3& point, const Vector3& normal, 
  const Vector3& albedo, const Scene& scene) {
  Vector3 finalColor;
  for (PointLight pointLight : scene.getPointLights()) {
    //Direction from point to light
    Vector3 lightDir = pointLight.pos - point;
    //Sphere radius
    float rad = lightDir.getLen();
    //Normalise light direction
    lightDir = lightDir * (1 / rad);

    //Lambertian shading
    float cos = std::max(0.0f, lightDir.dot(normal));
    float sphereArea = 4 * PI * rad * rad;
    Ray shadowRay(point + normal * SHADOW_BIAS, lightDir);

    InfoIntersect intersectInfo;
    int meshIndexDummyVar = 0;
    int triIndexDummyVar = 0;
    float dist = findIntersection(shadowRay, scene.getMeshes(), intersectInfo, meshIndexDummyVar, triIndexDummyVar);
    if (!intersectInfo.hasIntersection) {
      //no intersection, i.e. no shadow
      float r = (pointLight.intensity * albedo.x*cos) / (sphereArea);
      float g = (pointLight.intensity * albedo.y*cos) / (sphereArea );
      float b = (pointLight.intensity * albedo.z*cos) / (sphereArea );

      finalColor = finalColor + Vector3(r, g, b);
    }
  }
  return finalColor;
}

ChaosCampAM::Vector3 ChaosCampAM::Renderer::shadeBarycentric(float coords[3]){
  assert(abs(coords[0] + coords[1] + coords[2] - 1.0f) < EPSILON_RELAXED);
  return Vector3(coords[0], coords[1], coords[2]);
}
