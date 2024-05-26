#include "Character.h"

Character::Character(std::pair<int, int> coordinates, std::pair<int, int> dimensions, int healthPoints, std::string assetPath)
{
	this->coordinates = coordinates;
	this->dimensions = dimensions;
	this->healthPoints = healthPoints;
	this->assetPath = assetPath;
	this->direction = MOVE_LEFT;
}

Character::Character()
{
	this->coordinates = { 0, 0 };
	this->healthPoints = INITIAL_HEALTH_POINTS;
	this->direction = MOVE_LEFT;
	this->assetPath = "";
	this->dimensions = { GAME_CHUNK, GAME_CHUNK };
}

Character::~Character()
{
}

void Character::setAssetPath(std::string filename)
{
	this->assetPath = filename;
}

std::pair<int, int> Character::getCoordinatesInPixels()
{
	return { this->coordinates.first * GAME_CHUNK + 40, this->coordinates.second * GAME_CHUNK + 30 };
}

std::pair<int, int> Character::getCoordinatesInGameChunks()
{
	return this->coordinates;
}

int Character::getHealthPoints()
{
	return this->healthPoints;
}


std::string Character::getAssetPath()
{
	return this->assetPath;
}

CharacterState Character::getDirection()
{
	return this->direction;
}

std::pair<int, int> Character::getDimensions()
{
	return this->dimensions;
}


void Character::move(CharacterState playerState, std::pair<int, int> newCoordinates)
{
	this->direction = playerState;
	this->coordinates = newCoordinates;
}

void Character::resetHealthPoints()
{
	this->healthPoints = INITIAL_HEALTH_POINTS;
}

void Character::decrementHealthPoint()
{
	this->healthPoints--;
}

