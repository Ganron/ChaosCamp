#include "Camera.h"
#include "Math/Matrix3x3.h"

namespace ChaosCampAM {
	void Camera::dolly(float dist) {
		//Move along forward vector
		pos = pos + getForwardVec() * dist;
	}

	void Camera::truck(float dist) {
		//Move along right vector
		pos = pos + getRightVec() * dist;
	}

	void Camera::pedestal(float dist) {
		//Move along up vector
		pos = pos + getUpVec() * dist;
	}

	void Camera::pan(float angle) {
		//We apply a camera-centred transform (as opposed to world-centred transform) - we want
		//the rotation to be performed relative to the current camera coordinate frame.
		//We thus apply this rotation BEFORE any other transformations.
		//1. Invert current camera transforms (go back to the default position) - we assume orthogonal matrix;
		//2. Rotate about y axis
		//3. Re-apply previous camera transforms
		orientation = orientation * createRotationY(angle) * orientation.getTranspose();
	}

	void Camera::tilt(float angle) {
		//We apply a camera-centred transform. Rotation is performed BEFORE any other transformations.
		//1. Invert current camera transforms
		//2. Rotate about x axis
		//3. Re-apply previous transforms
		orientation = orientation * createRotationX(angle) * orientation.getTranspose();
	}

	void Camera::roll(float angle) {
		//We apply a camera-centred transform. Rotation is performed BEFORE any other transformations.
		//1. Invert current camera transforms
		//2. Rotate about z axis
		//3. Re-apply previous transforms
		orientation = orientation * createRotationZ(angle) * orientation.getTranspose();
	}

	Vector3 Camera::getForwardVec() const {
		//Get normalized forward vector: -Z camera space axis
		Vector3 forward = -orientation.col(2);
		forward.normalize();
		return forward;
	}

	Vector3 Camera::getRightVec() const {
		//Get normalized right vector: +X camera space axis
		Vector3 right = orientation.col(0);
		right.normalize();
		return right;
	}

	Vector3 Camera::getUpVec() const {
		//Get normalized up vector: +Y camera space axis
		Vector3 up = orientation.col(1);
		up.normalize();
		return up;
	}

	void Camera::orthogonalize() {
		orientation.orthogonalize();
	}
}