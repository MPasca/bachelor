#include "Button.h"

void Button::check_coordinates(std::pair<int, int> coordinates)
{
	if (coordinates.first < 0 || coordinates.first > VIEWPORT_WIDTH || coordinates.second < 0 || coordinates.second > VIEWPORT_HEIGHT)
	{
		std::cerr << "Coordinates out of bounds!\n";
		exit(ERR_COORDINATES_OUT_OF_BOUND);
	}
}

Button::Button(std::pair<int, int> coordinates, std::string assetPath, MainState triggeredEvent)
{
	check_coordinates(coordinates);
	this->coordinates = coordinates;
	this->assetPath = assetPath;
	this->width = BUTTON_WIDTH;
	this->height = BUTTON_HEIGHT;
	this->triggeredEvent = triggeredEvent;
	this->buttonState = INACTIVE_BTN;
}

void Button::changeDimensions(int width, int height)
{
	if(width < 0 || width > VIEWPORT_WIDTH || height < 0 || height > VIEWPORT_HEIGHT)
	{ 
		std::cerr << "Incorrect dimensions!\n";
		exit(ERR_INCORRECT_DIMENSIONS);
	}
	this->width = width;
	this->height = height;
}

void Button::changeCoordinates(std::pair<int, int> coordinates)
{
	check_coordinates(coordinates);
	this->coordinates = coordinates;
}

void Button::changeAssetPath(std::string assetPath)
{
	this->assetPath = assetPath;
}

std::pair<int, int> Button::getDimensions()
{
	return std::pair<int, int>(this->width, this->height);
}

std::pair<int, int> Button::getCoordinates()
{
	return this->coordinates;
}

std::string Button::getAssetPath()
{
	return this->assetPath + this->buttonState;
}

void Button::setActiveStatus(std::string buttonState)
{
	this->buttonState = buttonState;
}

MainState Button::triggerEvent()
{
	return this->triggeredEvent;
}
