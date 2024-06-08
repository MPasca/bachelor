#include "GameElement.h"

GameElement::GameElement(std::pair<int, int> coordinates, std::pair<int, int> dimensions, std::string fileName)
{
	this->coordinates = coordinates;
	this->dimRectangle = { coordinates.first, coordinates.second, dimensions.first, dimensions.second };
	this->fileName = fileName;
}

GameElement::GameElement()
{
	this->coordinates = { 0, 0 };
	this->dimRectangle = { 0, 0, 0, 0 };
	this->fileName = "";

}

GameElement::~GameElement()
{
	clean();
}

void GameElement::updateCoordinates(std::pair<int, int> coordinates)
{
	this->coordinates = coordinates;
}

void GameElement::clean()
{
	coordinates = { 0, 0 };
}

std::pair<int, int> GameElement::getCoordinates()
{
	return this->coordinates;
}

std::string GameElement::getPath()
{
	return this->fileName;
}

SDL_Rect GameElement::getRectangle()
{
	return this->dimRectangle;
}
