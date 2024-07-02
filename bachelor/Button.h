#pragma once
#include<iostream>

#include "Utils.h"

class Button
{
public:
	Button(std::pair<int, int> coordinates, std::string assetPath, MainState triggeredEvent);

	void changeDimensions(int width, int height);
	void changeCoordinates(std::pair<int, int> coordinates);
	void changeAssetPath(std::string assetPath);

	std::pair<int, int> getDimensions();
	std::pair<int, int> getCoordinates();
	std::string getAssetPath();

	void setActiveStatus(std::string buttonState);
	MainState triggerEvent();
private:
	int width, height;
	std::pair<int, int> coordinates;
	std::string assetPath;
	std::string buttonState;
	MainState triggeredEvent;
	void check_coordinates(std::pair<int, int> coordinates);
};
