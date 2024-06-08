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

