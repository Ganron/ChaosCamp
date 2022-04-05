#pragma once
#include "Math.h"

namespace ChaosCampAM {

	/*
	* Class to represent camera and associated operations.
	* Adopted camera coordinate system:
	*   - Right-handed coordinate system;
	*   - X-axis point to the right of the camera;
	*   - Y-axis point up from the camera;
	*   - Z-axis points back from the camera.
	* Here the camera always looks through the -Z-axis.
	*/
	class Camera {
	public:
		//Default camera is centred at the origin and is aligned to to the world-coordinate axis
		Camera() :pos(), orientation(1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f) {}

		//It is assumed that the orientation matrix is orthogonal. NO EXPLICIT VERIFICATION!
		Camera(const Vector3& pos, const Matrix3& orientation) : pos(pos), orientation(orientation) {};


		/* MOVEMENT TRANSFORMS */

		//Move the camera forward-backward (along Z-axis).
		//Positive distance = move camera forward.
		void dolly(float dist);

		//Move the camera left-right (along X-axis)
		//Positive distance = move camera right
		void truck(float dist);

		//Move the camera up-down (along Y-axis)
		//Positive distatnce = move camera up
		void pedestal(float dist);


		/* ROTATION TRANSFORMS */

		//Rotate the camera left-right (about Y-axis), aka "yaw" angle
		//Positive angle = rotate camera left
		//Angle must be given in radians.
		void pan(float angle);

		//Rotate the camera up-down (about X axis), aka "pitch" angle
		//Positive angle = rotate camera up
		//Angle must be given in radians.
		void tilt(float angle);

		//Rotate the camera about Z-axis
		//Positive angle = "left side of camera plunges down, right side goes up"
		//Angle must be given in radians.
		void roll(float angle);


		/* GETTERS */

		Vector3 getPosition() const { return pos; }
		Matrix3 getOrientation() const { return orientation; }

		//Get normalised forward vector - points in the direction the camera is looking at
		Vector3 getForwardVec() const;
		//Get normalised right vector - points to the right of the camera
		Vector3 getRightVec() const;
		//Get normalised up vector - points up from the camera 
		Vector3 getUpVec() const;

		/* OTHER */
		
		//Makes the orientation matrix orthogonal. Indended as a combat tool against accumulated numerical error.
		void orthogonalize();

	private:
		//Position in 3d space 
		Vector3 pos;

		//Columns of the matrix give the three vectors that form the basis of the camera coordinate system.
		//Adopted convention: +X-axis = Right vector; +Y-axis = Up vector; -Z-axis = Forward vector.
		//We assume that the matrix is orthogonal!
		Matrix3 orientation;
	};
}