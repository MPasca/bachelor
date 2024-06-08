#include "Button.h"

Button::Button(std::pair<int, int> coordinates, std::string assetPath, MainState triggeredEvent)
{
	this->coordinates = coordinates;
	this->assetPath = assetPath;
	this->width = BUTTON_WIDTH;
	this->height = BUTTON_HEIGHT;
	this->triggeredEvent = triggeredEvent;
	this->buttonState = INACTIVE_BTN;
}

Button::~Button()
{
}

void Button::changeDimensions(int width, int height)
{
	this->width = width;
	this->height = height;
}

void Button::changeCoordinates(std::pair<int, int> coordinates)
{
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
