#pragma once
#include<iostream>

#include "Utils.h"

class Character
{
public:
	Character(std::pair<int, int> coordinates, std::pair<int, int> dimensions, std::string assetPath);
	Character();

	void setAssetPath(std::string filename);

	std::pair<int, int> getCoordinatesInPixels();
	std::pair<int, int> getCoordinatesInGameChunks();
	std::pair<int, int> getDimensions();

	std::string getAssetPath();
	CharacterState getDirection();

	void move(CharacterState substate, std::pair<int, int> newCoordinates);
private:
	std::pair<int, int> coordinates;
	std::pair<int, int> dimensions;

	CharacterState direction;
	std::string assetPath;

	void check_coordinates(std::pair<int, int> coordinates);
	void check_dimensions(std::pair<int, int> dimensions);
};


class PlayerCharacter : public Character
{
public:
	PlayerCharacter(std::pair<int, int> coordinates, std::string filename);
	PlayerCharacter();
	void resetCharacter();

	std::pair<int, int> attack();
	void decreaseAttackCooldown();
	bool hasAttackCooldown();
	bool canTeleport();
	bool hasKey();

	void unlockTeleport();
	void getKey();

	void triggerTeleportCooldown();
	bool hasTeleportCooldown();
	void decreaseTeleportCooldown();
private:
	int attackCooldown;
	int teleportCooldown;
	bool teleportUnlocked;
	bool exitKey;

	std::pair<int, int> getAttackCoordinates();
	void triggerAttackCooldown();
};


class NonplayerCharacter : public Character
{
public:
	NonplayerCharacter(std::pair<int, int> coordinates, std::string filename);
	NonplayerCharacter();

	void triggerStunCooldownTimer();
	void decrementStunCooldownTimer();
	int getStunCooldownTimer();
private:
	int stunCooldown;
};

