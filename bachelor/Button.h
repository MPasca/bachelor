#pragma once
#include<iostream>

#include "Utils.h"

class Button
{
public:
	Button(std::pair<int, int> coordinates, std::string assetPath, MainState triggeredEvent);
	Button();
	~Button();

	void changeDimensions(int width, int height);
	void changeCoordinates(std::pair<int, int> coordinates);
	void changeAssetPath(std::string assetPath);

	std::pair<int, int> getDimensions();
	std::pair<int, int> getCoordinates();
	std::string getAssetPath();
	MainState triggerEvent();
private:
	int width, height;
	std::pair<int, int> coordinates;
	std::string assetPath;
	MainState triggeredEvent;
};
