#pragma once
#include"Math/Vector3.h"

namespace ChaosCampAM {

  struct PointLight {
    Vector3 pos;
    float intensity;//intensity coefficient, no specific unit of measurement
    PointLight(const Vector3& lightPos, float lightIntensity) :
      pos(lightPos), intensity(lightIntensity) {}
  };

}