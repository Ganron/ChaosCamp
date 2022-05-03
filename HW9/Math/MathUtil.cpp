#include "MathUtil.h"

namespace ChaosCampAM {

  float degToRad(float angle) {
    return angle * TO_RAD_CONST;
  }

  float radToDeg(float angle) {
    return angle * TO_DEG_CONST;
  }
}