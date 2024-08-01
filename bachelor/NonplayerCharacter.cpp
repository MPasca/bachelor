#include "Character.h"

NonplayerCharacter::NonplayerCharacter(std::pair<int, int> coordinates, std::string filename) : Character(coordinates, { GAME_CHUNK, GAME_CHUNK }, filename)
{
	this->stunCooldown = 0;
}

NonplayerCharacter::NonplayerCharacter() : Character()
{
	this->stunCooldown = 0;
}

void NonplayerCharacter::triggerStunCooldownTimer()
{
	this->stunCooldown = STUN_COOLDOWN;
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

