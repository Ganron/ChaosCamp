#include "Settings.h"

namespace ChaosCampAM {

	const ColorRGB& Settings::getBgColor() const {
		return bgColor;
	}

	const ColorRGB& Settings::getTriColor() const {
		return triColor;
	}

	int Settings::getWidth() const {
		return imgWidth;
	}

	int Settings::getHeight() const	{
		return imgHeight;
	}

	float Settings::getAspectRatio() const {
		return (float)imgWidth/(float)imgHeight;
	}

	void Settings::setBgColor(const ColorRGB& newBgColor)	{
		bgColor = newBgColor;
	}

	void Settings::setTriColor(const ColorRGB& newTriColor)	{
		triColor = newTriColor;
	}

	void Settings::setWidth(int newWidth)	{
		imgWidth = newWidth;
	}

	void Settings::setHeight(int newHeight)	{
		imgHeight = newHeight;
	}

}