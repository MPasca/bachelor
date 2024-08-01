#include "GameProp.h"

GameProp::GameProp(std::pair<int, int> coordinates, std::string assetPath)
{
	this->coordinates = coordinates;
	this->assetPath = assetPath;
	this->hidden = true;
	this->consumed = false;
}

GameProp::GameProp()
{
	this->coordinates = { 0, 0 };
	this->assetPath = "";
	this->hidden = true;
	this->consumed = false;
}

void GameProp::resetProp()
{
	this->consumed = false;
	this->hidden = true;
}

std::string GameProp::getAssetPath()
{
	if (hidden || consumed)
	{
		return std::string(PROP_PATH"/blank.png");
	}

	return this->assetPath;
}

std::pair<int, int> GameProp::getDimensions()
{
	return this->dimensions;
}

bool GameProp::isHidden()
{
	return this->hidden;
}

bool GameProp::isConsumed()
{
	return this->consumed;
}

void GameProp::reveal()
{
	this->hidden = false;
}

void GameProp::consume()
{
	this->consumed = true;
	this->hidden = true;
}

void GameProp::updateCoordinates(std::pair<int, int> newCoordinates)
{
	this->coordinates = newCoordinates;
}

std::pair<int, int> GameProp::getCoordinates()
{
	return this->coordinates;
}

std::pair<int, int> GameProp::getCoordinatesInPixels()
{
	return { this->coordinates.first * GAME_CHUNK + 40, this->coordinates.second * GAME_CHUNK + 20 };
}
