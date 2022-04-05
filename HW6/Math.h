#pragma once
#include<utility>

namespace ChaosCampAM {
    /*** CONSTANTS ***/

    //Used when doing floating-point comparisons
    static const float EPSILON = 1e-7f;

    static const float TO_RAD_CONST = 0.01745329251f; //pi/180
    static const float TO_DEG_CONST = 57.2957795131f; //180/pi

    /*** UTILITY FUNCTIONS ***/

    float degToRad(float angle);

    float radToDeg(float angle);

    /*** CLASSES ***/

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


    /* 
    * Class to represent a 3x3 matrix.
    */
    class Matrix3 {
    public:
        /* CONSTRUCTORS */

        //Default constructor - zero matrix
        Matrix3() :Matrix3(0, 0, 0, 0, 0, 0, 0, 0, 0) {}

        //The matrix entries are initialized to a set of values.
        //NOTE: Parameters are ordered in row-major fashion for convenience
        // I.e.
        //   entries are encoded as 'a[row][col]' - row0: <a00, a01, a02>, row1: <a10,a11,a12>, row2: <a20,a21,a22>
        Matrix3(float a00, float a01, float a02, float a10, float a11, float a12, float a20, float a21, float a22);

        //The matrix entries are initialized to an array.
        //NOTE: It is assumed that the array contains the 9 matrix entries correctly ordered in column-major fashion
        // (the way they are stored in memory).
        Matrix3(float* entries);

        //The matrix is initialized to another matrix. (Copy constructor)
        Matrix3(const Matrix3& m);


        /* ARITHMETIC OPERATORS */

        Matrix3 operator+(const Matrix3& m) const;
        Matrix3 operator-(const Matrix3& m) const;
        Matrix3 operator*(float s) const;
        //NOTE: Multiplication proceeds right-to-left, i.e. [Result] = [Matrix] * [Vector]
        //Convention was adopted in accordance with the A*x=b equation in linear algebra.
        Vector3 operator*(const Vector3& v) const;
        //NOTE: Multiplication proceeds right-to-left, i.e. [Result] = [Matrix 2] * [Matrix 1]
        Matrix3 operator*(const Matrix3& m) const;
        Matrix3& operator=(const Matrix3& m);


        /* GETTERS */
        
        //Get the a[i][j] entry in the matrix. Note that indices must be in [0,2] range.
        float a(int row, int col) const;

        //Get the i-th row, where i is in the [0,2] range.
        Vector3 row(int row) const;

        //Get the j-th col, where j is in the [0,2] range.
        Vector3 col(int col) const;

        //Get transposed matrix
        Matrix3 getTranspose() const;


        /* OTHER */
        
        //Sets a column of the matrix. Col index must be in [0,2] range.
        void setCol(int col, Vector3& v);

        //Perform Gram-Schmidt Orthogonalization to make the columns of the matrix orthogonal vectors.
        //Then normalize the columns to ensure an orthogonal matrix (columns are orthonormal).
        void orthogonalize();

    private:
        //Entries will be encoded in col-major fashion.
        float entry[9];
    };

    //Create a rotation matrix about the X-axis. Angle must be given in radians.
    Matrix3 createRotationX(float angle);

    //Create a rotation matrix about the Y-axis. Angle must be given in radians.
    Matrix3 createRotationY(float angle);

    //Create a rotation matrix about the Z-axis. Angle must be given in radians.
    Matrix3 createRotationZ(float angle);
}
