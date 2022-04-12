#pragma once
#include "ColorRGB.h"

namespace ChaosCampAM {

	/* 
	* A class to store and manage scene settings.
	*  - Image resolution
	*  - Background colour
	*/
	class Settings {
	public:
		//Default settings - 1920 x 1080, black screen, red triangles
		Settings() : imgWidth(1920), imgHeight(1080), bgColor(), triColor(255,0,0) {}
		Settings(int width, int height, const ColorRGB& bgColor, const ColorRGB& triColor = ColorRGB(255,0,0)) :
			imgWidth(width), imgHeight(height), bgColor(bgColor) {}

		//Getters

		//Get the background colour
		const ColorRGB& getBgColor() const;
		//A global colour for all triangles
		const ColorRGB& getTriColor() const;
		//Image width in pixels.
		int getWidth() const;
		//Image height in pixels.
		int getHeight() const;
		//Here aspect ratio is defined as width/height
		float getAspectRatio() const;

		//Setters
		void setBgColor(const ColorRGB& newBgColor);
		void setTriColor(const ColorRGB& newTriColor);
		void setWidth(int newWidth);
		void setHeight(int newHeight);

	private:
		ColorRGB bgColor;
		ColorRGB triColor;
		int imgWidth;
		int imgHeight;

	};
}