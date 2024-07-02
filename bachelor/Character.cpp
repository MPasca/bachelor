#include "Character.h"

void Character::check_coordinates(std::pair<int, int> coordinates)
{
	if (coordinates.first < 0 || coordinates.first > WIDTH - 1 || coordinates.second < 0 || coordinates.second > HEIGHT - 1)
	{
		std::cerr << "Coordinates out of bounds!\n";
		exit(ERR_COORDINATES_OUT_OF_BOUND);
	}
}

void Character::check_dimensions(std::pair<int, int> dimensions)
{
	if (dimensions.first < 1 || dimensions.second < 1)
	{
		std::cerr << "Incorrect dimensions!\n";
		exit(ERR_INCORRECT_DIMENSIONS);
	}
}

Character::Character(std::pair<int, int> coordinates, std::pair<int, int> dimensions, std::string assetPath)
{
	check_coordinates(coordinates);
	check_dimensions(dimensions);

	this->coordinates = coordinates;
	this->dimensions = dimensions;
	this->assetPath = assetPath;
	this->direction = MOVE_LEFT;
}

Character::Character()
{
	this->coordinates = { 0, 0 };
	this->direction = MOVE_LEFT;
	this->assetPath = "";
	this->dimensions = { GAME_CHUNK, GAME_CHUNK };
}

void Character::setAssetPath(std::string filename)
{
	this->assetPath = filename;
}

std::pair<int, int> Character::getCoordinatesInPixels()
{
	return { this->coordinates.first * GAME_CHUNK + 40, this->coordinates.second * GAME_CHUNK + 20 };
}

std::pair<int, int> Character::getCoordinatesInGameChunks()
{
	return this->coordinates;
}

std::string Character::getAssetPath()
{
	switch (this->direction)
	{
	case MOVE_UP:
		return this->assetPath + "move_up.png";
	case MOVE_DOWN:
		return this->assetPath + "move_down.png";
	case MOVE_RIGHT:
		return this->assetPath + "move_right.png";

	case MOVE_LEFT:
	default:
		return this->assetPath + "move_left.png";
	}
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
	check_coordinates(newCoordinates);
	this->direction = playerState;
	this->coordinates = newCoordinates;
}

