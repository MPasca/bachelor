#include "Character.h"

NonplayerCharacter::NonplayerCharacter(std::pair<int, int> coordinates, std::string filename) : Character(coordinates, { GAME_CHUNK, GAME_CHUNK }, INITIAL_HEALTH_POINTS, filename)
{
	this->stunCooldown = 0;
}

NonplayerCharacter::NonplayerCharacter() : Character()
{
	this->stunCooldown = 0;
}

NonplayerCharacter::~NonplayerCharacter()
{
}

void NonplayerCharacter::setAssetPath(std::string filename)
{
	Character::setAssetPath(filename);
}

std::pair<int, int> NonplayerCharacter::getCoordinatesInPixels()
{
	return Character::getCoordinatesInPixels();
}

std::pair<int, int> NonplayerCharacter::getCoordinatesInGameChunks()
{
	return Character::getCoordinatesInGameChunks();
}

std::pair<int, int> NonplayerCharacter::getDimensions()
{
	return Character::getDimensions();
}

int NonplayerCharacter::getHealthPoints()
{
	return Character::getHealthPoints();
}

std::string NonplayerCharacter::getAssetPath()
{
	return Character::getAssetPath();
}

void NonplayerCharacter::move(CharacterState substate, std::pair<int, int> newCoordinates)
{
	Character::move(substate, newCoordinates);
}

void NonplayerCharacter::decrementHealthPoint()
{
	Character::decrementHealthPoint();
}

void NonplayerCharacter::triggerStunCooldownTimer()
{
	this->stunCooldown = COOLDOWN;
}

void NonplayerCharacter::decrementStunCooldownTimer()
{
	if (this->stunCooldown > 0)
	{
		this->stunCooldown--;
	}
}

int NonplayerCharacter::getStunCooldownTimer()
{
	return this->stunCooldown;
}

