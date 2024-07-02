#include "Character.h"
#include "Utils.h"

PlayerCharacter::PlayerCharacter(std::pair<int, int> coordinates, std::string filename) : Character(coordinates, { GAME_CHUNK, GAME_CHUNK }, filename)
{
	this->attackCooldown = 0;
	this->teleportUnlocked = false;
	this->teleportCooldown = 0;
	this->exitKey = false;
}

PlayerCharacter::PlayerCharacter() : Character()
{
	this->attackCooldown = 0;
	this->teleportUnlocked = false;
	this->teleportCooldown = 0;
	this->exitKey = false;
}

void PlayerCharacter::resetCharacter()
{
	this->attackCooldown = 0;
	this->teleportCooldown = 0;

	this->exitKey = true;
	this->teleportUnlocked = false;
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
	this->attackCooldown = ATTACK_COOLDOWN;
}

std::pair<int, int> PlayerCharacter::attack()
{
	if (this->attackCooldown > 0) return { -1, -1 };

	triggerAttackCooldown();
	
	return this->getCoordinatesInGameChunks();
}

bool PlayerCharacter::hasAttackCooldown()
{
	return this->attackCooldown > 0;
}

bool PlayerCharacter::canTeleport()
{
	return teleportUnlocked;
}

bool PlayerCharacter::hasKey()
{
	return exitKey;
}

void PlayerCharacter::unlockTeleport()
{
	this->teleportUnlocked = true;
}

void PlayerCharacter::getKey()
{
	this->exitKey = true;
}

void PlayerCharacter::triggerTeleportCooldown()
{
	this->teleportCooldown = TELEPORT_COOLDOWN;
}

bool PlayerCharacter::hasTeleportCooldown()
{
	return this->teleportCooldown > 0;
}

void PlayerCharacter::decreaseTeleportCooldown()
{
	if (this->teleportCooldown == 0)
	{
		return;
	}
	this->teleportCooldown--;
}

void PlayerCharacter::decreaseAttackCooldown()
{
	this->attackCooldown--;
}




