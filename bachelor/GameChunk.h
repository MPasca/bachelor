#pragma once
#include<iostream>

#include "./vis_lib/GameElement.h"
#include "path_computing/PaCo.h"

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
	void setPathNode(paco::Node* pathNode);

	bool* getWalls();
	int getNumberOfNeighbors();
	paco::Node* getPathNode();

private:
	int numberOfNeighbors;
	paco::Node * pathNode;
	bool* walls;
};

