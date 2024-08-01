#include "GameElement.h"

void GameElement::check_coordinates(std::pair<int, int> coordinates)
{
	if (coordinates.first < 0 || coordinates.first > VIEWPORT_WIDTH || coordinates.second < 0 || coordinates.second > VIEWPORT_HEIGHT)
	{
		std::cerr << "Coordinates out of bounds!\n";
		exit(ERR_COORDINATES_OUT_OF_BOUND);
	}
}

void GameElement::check_dimensions(std::pair<int, int> dimensions)
{
	if (dimensions.first < 1 || dimensions.second < 1)
	{
		std::cerr << "Incorrect dimensions!\n";
		exit(ERR_INCORRECT_DIMENSIONS);
	}
}


GameElement::GameElement(std::pair<int, int> coordinates, std::pair<int, int> dimensions, std::string fileName)
{
	check_coordinates(coordinates);
	check_dimensions(dimensions);
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

void GameElement::updateCoordinates(std::pair<int, int> coordinates)
{
	check_coordinates(coordinates);
	this->coordinates = coordinates;
	this->dimRectangle.x = coordinates.first;
	this->dimRectangle.y = coordinates.second;
}

void GameElement::setPath(std::string path)
{
	this->fileName = path;
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
