#pragma once
#include"Math/Vector3.h"

namespace ChaosCampAM {
  //Defines the different material types.
  // - Diffuse for "matte" materials that reflect light arbitrarily in all directions.
  // - Reflective for materals that reflect light in a specified direction (mirror-like)
  enum class MaterialType { Diffuse, Reflective };

  /*
  * Material used to describe the light reflection behaviour of a surface. Used for shading.
  */
  struct Material {
    MaterialType type;
    Vector3 albedo;
    bool smoothShading;
    
    Material(MaterialType type, const Vector3& albedo, bool smoothShading) :
      type(type), albedo(albedo), smoothShading(smoothShading) {}
  };
}