/*
* Chaos Camp: Ray-Tracing Course
* HOMEWORK: LECTURE 6, PROBLEMS 1-5
* Author: Asen Markov
* Date: 05/04/2022
*
* Description:
* We build a camera class some camera animation functionality. Problems 1 & 2 use a scene of a single triangle.
* Problems 3-5 use a scene where the camera is surrounded by a circle of evenly spaced triangles.
*  - Problem 1: Implement camera pan of the forward vector.
*  - Problem 2: Generate a picture of the given triangle with arbitrary camera position and orientation.
*  - Problem 3: Implement some more diverse camera movement (dolly, truck, pedestal, etc.)
*  - Problem 4: Generate a picture where the camera has gone through a composite transformation.
*  - Problem 5: Generate a short animation (succession of frames).
*/

#include <fstream>
#include <iostream>
#include <assert.h>
#include <vector>
#include <utility>
#include <string>

#include "Math/Vector3.h"
#include "Math/Matrix3x3.h"
#include "Math/MathUtil.h"
#include "Triangle.h"
#include "Ray.h"
#include "Camera.h"

using namespace ChaosCampAM;

//Output image resolution
static const int imageWidth = 1920;
static const int imageHeight = 1080;

static const int maxColorComponent = 255;

// A struct to represent an RGB color value. Every channel is an unsigned integer in the interval [0;255].
struct ColorRGB {
  unsigned char r;
  unsigned char g;
  unsigned char b;
  ColorRGB() : r(0), g(0), b(0) {}
  ColorRGB(unsigned char red, unsigned char green, unsigned char blue) : r(red), g(green), b(blue) {}
};

void setUpScene(std::vector<std::pair<Triangle, ColorRGB>>& triangleList);

int main() {

  //Set up Scene - consists of (triangle,colour) pairs
  std::vector<std::pair<Triangle, ColorRGB>> triangles;
  triangles.reserve(36);
  setUpScene(triangles);

  Camera cam;
  //Problems 1 & 2 - move camera away fromt he origin
  //cam.truck(2.0f);
  //cam.pan(degToRad(30.0f));

  //Problem 3 - demonstrate another transformation
  //cam.roll(degToRad(30.0f));

  //Problem 4 - composite transformation; demonstrate all remaining transforms
  //cam.dolly(-5.0f);
  //cam.pedestal(7.0f);
  //cam.tilt(-45.0f);
  //cam.dolly(-1.0f);

  float aspectRatio = (float)imageWidth / imageHeight;

  //Problem 5 - Produce 72 frames
  for (int i = 0; i < 72; i++) {
    //Set up output stream. Ensure file naming will produce proper file ordering.
    std::string zeroFill = "0";
    if (i > 9) zeroFill = "";
    std::ofstream ppmFileStream("video/am_hw6_e_after_" + zeroFill + std::to_string(i) + ".ppm",
      std::ios::out | std::ios::binary);
    ppmFileStream << "P3\n";
    ppmFileStream << imageWidth << " " << imageHeight << "\n";
    ppmFileStream << maxColorComponent << "\n";

    //Problem 5 - Pan camera by 5 deg every frame
    cam.pan(degToRad(i * 5.0f));

    //Draw
    for (int rowIdx = 0; rowIdx < imageHeight; ++rowIdx) {
      for (int colIdx = 0; colIdx < imageWidth; ++colIdx) {
        Ray ray = computeCameraRay(colIdx, rowIdx, imageHeight, aspectRatio, cam);

        float closestDist = FLT_MAX;
        ColorRGB finalColor(51, 51, 0); //background colour

        for (const auto& tri : triangles) {
          Vector3 intersection;
          float dist = tri.first.intersect(ray, intersection);
          if (dist > 0.0 && dist < closestDist) {
            //This is now the closest triangle
            closestDist = dist;
            finalColor = tri.second;
          }
        }

        ppmFileStream << (int)finalColor.r << " " << (int)finalColor.g << " " << (int)finalColor.b << "\t";
      }
      ppmFileStream << "\n";
    }

    ppmFileStream.close();

    if (i % 10 == 0) {
      //Re-orthogonalize camera orientation matrix to ensure that it stays orthogonal -
      // used to combat accumulated numeric error.
      //Repeat once every 10 frames.
      cam.orthogonalize();
    }
  }

  return 0;
}

void setUpScene(std::vector<std::pair<Triangle, ColorRGB>>& triangleList) {
  //Problems 1 & 2
  //triangleList.push_back(std::make_pair(
  //    Triangle(
  //        Vector3(-1.75, -1.75, -3),
  //        Vector3(1.75, -1.75, -3),
  //        Vector3(0, 1.75, -3)),
  //    ColorRGB(255, 255, 100)));

  //Problems 3 - 5
  float sqrt3 = sqrtf(3);
  Vector3 a = Vector3(-sqrt3, -1.0f, -10.0f);
  Vector3 b = Vector3(sqrt3, -1.0f, -10.0f);
  Vector3 c = Vector3(0.0f, 2.0f, -10.0f);
  Vector3 m = Vector3(0.0f, 0.0f, -10.0f);

  Vector3 aFlip = Vector3(-sqrt3, 2.0f, -10.0f);
  Vector3 bFlip = Vector3(sqrt3, 2.0f, -10.0f);
  Vector3 cFlip = Vector3(0.0f, -1.0f, -10.0f);
  Vector3 mFlip = Vector3(0.0f, 1.0f, -10.0f);

  int numSections = 12;
  float angle = 360.0f / numSections;
  for (int i = 0; i < numSections; i++) {
    Matrix3x3 rot = createRotationY(degToRad(angle * i));

    int coeff = (abs(i - numSections / 2) - numSections / 2) * 20;

    if (i % 2 == 0) {
      Triangle tri1(rot * a, rot * m, rot * c);
      Triangle tri2(rot * a, rot * b, rot * m);
      Triangle tri3(rot * b, rot * c, rot * m);
      triangleList.push_back(std::make_pair(tri1, ColorRGB(255, 255, 100)));
      triangleList.push_back(std::make_pair(tri2, ColorRGB(165 + coeff, 165 + coeff, 130 + coeff)));
      triangleList.push_back(std::make_pair(tri3, ColorRGB(255, 200, 90)));
    }
    else {
      Triangle tri1(rot * aFlip, rot * cFlip, rot * mFlip);
      Triangle tri2(rot * aFlip, rot * mFlip, rot * bFlip);
      Triangle tri3(rot * bFlip, rot * mFlip, rot * cFlip);
      triangleList.push_back(std::make_pair(tri1, ColorRGB(255, 255, 100)));
      triangleList.push_back(std::make_pair(tri2, ColorRGB(165 + coeff, 165 + coeff, 130 + coeff)));
      triangleList.push_back(std::make_pair(tri3, ColorRGB(255, 200, 90)));
    }
  }
}