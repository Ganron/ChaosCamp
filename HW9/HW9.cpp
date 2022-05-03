/*
* Chaos Camp: Ray-Tracing Course
* HOMEWORK: LECTURE 9, PROBLEMS 1-6
* Author: Asen Markov
* Date: 03/05/2022
*
* Description:
* We introduce materials to our ray-tracer. We finally reach recursive ray tracing.
* 
* Problems 1 - 2: Render a scene where pixels are shaded based on barycentric coordinates.
* Problems 3 - 4: Render a scene containing diffuse materials.
* Problems 5 - 6: Render a scene containing reflective materials.
* 
*/

#include <fstream>
#include <iostream>
#include <assert.h>
#include <vector>
#include <utility>
#include <string>

#include "ColorRGB.h"
#include "Math/Vector3.h"
#include "Math/Matrix3x3.h"
#include "Math/MathUtil.h"
#include "Triangle.h"
#include "Ray.h"
#include "Camera.h"
#include "Scene.h"
#include "SceneParser.h"
#include "Renderer.h"

using namespace ChaosCampAM;

int main() {
  Renderer renderer;
  SceneParser parser;

  //Problem 1
  //Scene scene1;
  //parser.parse("input/scene0.crtscene", scene1);
  //renderer.render(scene1, "output/scene0.ppm",ShadingMode::Barycentric);

  //Problem 2
  //Scene scene2;
  //parser.parse("input/scene1.crtscene", scene2);
  //renderer.render(scene2, "output/scene1.ppm",ShadingMode::Barycentric);

  //Problem 3
  //Scene scene3;
  //parser.parse("input/scene2.crtscene", scene3);
  //renderer.render(scene3, "output/scene2.ppm",ShadingMode::Light);

  //Problem 4
  //Scene scene4;
  //parser.parse("input/scene3.crtscene", scene4);
  //renderer.render(scene4, "output/scene3.ppm",ShadingMode::Light);

  //Problem 5
  //Scene scene5;
  //parser.parse("input/scene4.crtscene", scene5);
  //renderer.render(scene5, "output/scene4.ppm", ShadingMode::Light);

  //Problem 6
  Scene scene6;
  parser.parse("input/scene5.crtscene", scene6);
  renderer.render(scene6, "output/scene5.ppm", ShadingMode::Light);



  return 0;
}
