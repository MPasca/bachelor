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

	int getHealthPoints();
	std::string getAssetPath();
	CharacterState getDirection();

	void move(CharacterState substate, std::pair<int, int> newCoordinates);
	void decrementHealthPoint();
	void resetHealthPoints();
private:

	std::pair<int, int> coordinates;
	std::pair<int, int> dimensions;

	int healthPoints;
	CharacterState direction;
	std::string assetPath;
};


class PlayerCharacter : Character
{
public:
	PlayerCharacter(std::pair<int, int> coordinates, std::string filename);
	PlayerCharacter();
	~PlayerCharacter();

	int getLives();

	void decrementLife();
	void incrementLife();

	void setAssetPath(std::string filename);

	std::pair<int, int> getCoordinatesInPixels();
	std::pair<int, int> getCoordinatesInGameChunks();

	std::pair<int, int> getDimensions();

	int getHealthPoints();
	std::string getAssetPath();

	void move(CharacterState substate, std::pair<int, int> newCoordinates);
	std::pair<int, int> attack();
	void decreaseCooldown();
	bool hasCooldown();
private:
	int lives;
	int attackCooldown;
	std::pair<int, int> getAttackCoordinates();
	void triggerAttackCooldown();
};


class NonplayerCharacter : Character
{
public:
	NonplayerCharacter(std::pair<int, int> coordinates, std::string filename);
	NonplayerCharacter();
	~NonplayerCharacter();

	void setAssetPath(std::string filename);

	std::pair<int, int> getCoordinatesInPixels();
	std::pair<int, int> getCoordinatesInGameChunks();

	std::pair<int, int> getDimensions();

	int getHealthPoints();
	std::string getAssetPath();

	void move(CharacterState substate, std::pair<int, int> newCoordinates);
	void decrementHealthPoint();

	void triggerStunCooldownTimer();
	void decrementStunCooldownTimer();
	int getStunCooldownTimer();
private:
	int stunCooldown;
};

