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

	GameElement toGameElement();

	void setNeighbors(bool neighbors[]);
	void setCoordinates(std::pair<int, int> coordinates);
	void setNumberOfNeighbors(int numberOfNeighbors);
	void setPathNode(paco::Node* pathNode);

	bool* getNeighbors();
	std::pair<int, int> getCoordinates();
	int getNumberOfNeighbors();
	paco::Node* getPathNode();

private:
	int numberOfNeighbors;
	paco::Node * pathNode;
	bool* neighbors;
	std::pair<int, int>coordinates;
	void check_coordinates(std::pair<int, int> coordinates);
};

