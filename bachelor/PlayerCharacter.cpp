#include "Character.h"
#include "Utils.h"

PlayerCharacter::PlayerCharacter(std::pair<int, int> coordinates, std::string filename) : Character(coordinates, { GAME_CHUNK, GAME_CHUNK }, INITIAL_HEALTH_POINTS, filename)
{
	this->attackCooldown = 0;
}

PlayerCharacter::PlayerCharacter() : Character()
{
	this->attackCooldown = 0;
}

PlayerCharacter::~PlayerCharacter()
{
	
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




