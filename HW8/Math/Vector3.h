#pragma once
#include<utility>

namespace ChaosCampAM {
 /*
 * Class to represent a vector in 3 - dimensional space
 */
  class Vector3 {
  public:
    float x;
    float y;
    float z;

    //Zero vector is the default
    Vector3() : x(0.0), y(0.0), z(0.0) {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
    Vector3(const Vector3& v) : x(v.x), y(v.y), z(v.z) {}

    //Arithmetics
    Vector3 operator+(const Vector3& v) const { return Vector3(x + v.x, y + v.y, z + v.z); }
    Vector3 operator-(const Vector3& v) const { return Vector3(x - v.x, y - v.y, z - v.z); }
    Vector3 operator*(float s) const { return Vector3(x * s, y * s, z * s); }
    Vector3 operator-() const { return Vector3(-x, -y, -z); }

    //Dot product
    float dot(const Vector3& v) const;

    //Cross product
    Vector3 cross(const Vector3& v) const;

    //Get the vector magnitude (euclidian norm)
    float getLen() const { return sqrtf(x * x + y * y + z * z); }

    //Get the square of the vector magnitude to avoid sqrt() operation.
    float getLenSquared() const { return x * x + y * y + z * z; }

    //Get a unit vector pointing in the same direction as the current vector
    void normalize();
  };

  //Non-member functin to handle (scalar)*(vector3) operation. Encapsulated in the ChaosCampAM namespace.
  Vector3 operator*(float s, const Vector3& v);
}
