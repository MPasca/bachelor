#include "Character.h"
#include "Utils.h"

PlayerCharacter::PlayerCharacter(std::pair<int, int> coordinates, std::string filename) : Character(coordinates, { GAME_CHUNK, GAME_CHUNK }, INITIAL_HEALTH_POINTS, filename)
{
	this->lives = INITIAL_LIVES;
	this->attackCooldown = 0;
}

PlayerCharacter::PlayerCharacter() : Character()
{
	this->lives = INITIAL_LIVES;
	this->attackCooldown = 0;
}

PlayerCharacter::~PlayerCharacter()
{
	
}

int PlayerCharacter::getLives()
{
	return this->lives;
}

void PlayerCharacter::decrementLife()
{
	this->lives--;
	resetHealthPoints();
}

void PlayerCharacter::incrementLife()
{
	this->lives++;
}

void PlayerCharacter::setAssetPath(std::string filename)
{
	Character::setAssetPath(filename);
}

std::pair<int, int> PlayerCharacter::getCoordinatesInPixels()
{
	return Character::getCoordinatesInPixels();
}

std::pair<int, int> PlayerCharacter::getCoordinatesInGameChunks()
{
	return Character::getCoordinatesInGameChunks();
}

std::pair<int, int> PlayerCharacter::getDimensions()
{
	return Character::getDimensions();
}

int PlayerCharacter::getHealthPoints()
{
	return Character::getHealthPoints();
}

std::string PlayerCharacter::getAssetPath()
{
	return Character::getAssetPath();
}

void PlayerCharacter::move(CharacterState substate, std::pair<int, int> newCoordinates)
{
	Character::move(substate, newCoordinates);
}

std::pair<int, int> PlayerCharacter::getAttackCoordinates()
{
	std::pair<int, int> coordinatesToAttack = Character::getCoordinatesInGameChunks();
	coordinatesToAttack.second += Character::getDirection() == MOVE_UP ? -1 :
								Character::getDirection() == MOVE_DOWN ? +1 : 0;
	coordinatesToAttack.first += Character::getDirection() == MOVE_LEFT ? -1 :
								Character::getDirection() == MOVE_RIGHT ? +1 : 0;

	return coordinatesToAttack;
}

void PlayerCharacter::triggerAttackCooldown()
{
	this->attackCooldown == COOLDOWN;
}

std::pair<int, int> PlayerCharacter::attack()
{
	if (this->attackCooldown > 0) return this->getCoordinatesInGameChunks();

	std::pair<int, int> coordinatesToAttack = this->getAttackCoordinates();
	
}

bool PlayerCharacter::hasCooldown()
{
	return this->attackCooldown > 0;
}

void PlayerCharacter::decreaseCooldown()
{
	this->attackCooldown--;
}




