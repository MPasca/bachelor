#pragma once
#include<iostream>

#include "Utils.h"

class Character
{
public:
	Character(std::pair<int, int> coordinates, std::pair<int, int>dimensions, int healthPoints, std::string assetPath);
	Character();
	~Character();

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
};


class PlayerCharacter : public Character
{
public:
	PlayerCharacter(std::pair<int, int> coordinates, std::string filename);
	PlayerCharacter();
	~PlayerCharacter();

	std::pair<int, int> attack();
	void decreaseCooldown();
	bool hasCooldown();
private:
	int attackCooldown;
	std::pair<int, int> getAttackCoordinates();
	void triggerAttackCooldown();
};


class NonplayerCharacter : public Character
{
public:
	NonplayerCharacter(std::pair<int, int> coordinates, std::string filename);
	NonplayerCharacter();
	~NonplayerCharacter();

	void triggerStunCooldownTimer();
	void decrementStunCooldownTimer();
	int getStunCooldownTimer();
private:
	int stunCooldown;
};

