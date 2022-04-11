#include "Vector3.h"
#include "MathUtil.h"
#include <assert.h>
#include <utility>

namespace ChaosCampAM {

  float Vector3::dot(const Vector3& v) const { return x * v.x + y * v.y + z * v.z; }

  Vector3 Vector3::cross(const Vector3& v) const {
    return Vector3(
      y * v.z - z * v.y,
      z * v.x - x * v.z,
      x * v.y - y * v.x
    );
  }

  void Vector3::normalize() {
    float len = this->getLen();

    //handle division by zero
    assert(abs(len) > EPSILON);

    float coeff = 1 / len;
    x = x * coeff;
    y = y * coeff;
    z = z * coeff;
  }

  Vector3 operator*(float s, const Vector3& v) { return v * s; }
}