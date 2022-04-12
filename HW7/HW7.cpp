/*
* Chaos Camp: Ray-Tracing Course
* HOMEWORK: LECTURE 7, PROBLEMS 1-5
* Author: Asen Markov
* Date: 12/04/2022
*
* Description:
* We extend the Ray-Tracer architecture to include a Scene object (hold all scene data) and a Renderer object
* (do all the rendering). The Scene consists of three main components - Settings, Camera, Objects (mesh list for now).
* We create separate classes for each. Furthermore, we implement the functionality for loading a scene from a custom
* JSON scene file via the rapidjson library.
* 
* Problems 1 - 5: Render a scene from a file.
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
#include "Renderer.h"

using namespace ChaosCampAM;

int main() {
  Renderer renderer;

  //Problem 1
  //Scene scene1("input/scene0.crtscene");
  //renderer.render(scene1, "output/scene0.ppm");

  //Problem 2
  //Scene scene2("input/scene1.crtscene");
  //renderer.render(scene2, "output/scene1.ppm");

  //Problem 3
  //Scene scene3("input/scene2.crtscene");
  //renderer.render(scene3, "output/scene2.ppm");

  //Problem 4
  //Scene scene4("input/scene3.crtscene");
  //renderer.render(scene4, "output/scene3.ppm");

  //Problem 5
  Scene scene5("input/scene4.crtscene");
  renderer.render(scene5, "output/scene4.ppm");

  return 0;
}
