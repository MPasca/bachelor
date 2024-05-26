#pragma once
#include<iostream>

#include "./vis_lib/GameElement.h"

/*
* coordinates.first = y axis
* coordinates.second = x axis
*/
class GameChunk
{
public:
	GameChunk();
	~GameChunk();

	GameElement toGameElement(std::pair<int, int>coordinates);

	void setWalls(bool walls[]);
	void setNumberOfNeighbors(int numberOfNeighbors);

	bool* getWalls();
	int getNumberOfNeighbors();

private:
	int numberOfNeighbors;
	bool* walls;
};

