#pragma once
#include"Math/Vector3.h"

namespace ChaosCampAM {

	/* 
	* A class to store and manage scene settings.
	*  - Image resolution
	*  - Background colour
	*/
	class Settings {
	public:
		//Default settings - 1920 x 1080, black screen, red triangles
		Settings() : imgWidth(1920), imgHeight(1080), bgColor() {}
		Settings(int width, int height, const Vector3& bgColor) :
			imgWidth(width), imgHeight(height), bgColor(bgColor) {}

		//Getters

		//Get the background colour
		const Vector3& getBgColor() const;
		//Image width in pixels.
		int getWidth() const;
		//Image height in pixels.
		int getHeight() const;
		//Here aspect ratio is defined as width/height
		float getAspectRatio() const;

		//Setters
		void setBgColor(const Vector3& newBgColor);
		void setWidth(int newWidth);
		void setHeight(int newHeight);

	private:
		Vector3 bgColor;
		int imgWidth;
		int imgHeight;

	};
}