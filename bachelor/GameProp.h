#pragma once
#include<iostream>
#include "Utils.h"

class GameProp
{
public:
	GameProp(std::pair<int, int> coordinates, std::string assetPath);
	GameProp();

	void resetProp();

	void updateCoordinates(std::pair<int, int> newCoordinates);
	std::pair<int, int> getCoordinates();
	std::pair<int, int> getCoordinatesInPixels();
	std::string getAssetPath();
	std::pair<int, int> getDimensions();

	bool isHidden();
	bool isConsumed();

	void reveal();
	void consume();

private:
	std::pair<int, int> coordinates;
	std::string assetPath;
	std::pair<int, int> dimensions = { GAME_CHUNK, GAME_CHUNK };
	bool hidden;
	bool consumed;
};