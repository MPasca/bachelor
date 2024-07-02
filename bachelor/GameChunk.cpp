#include "GameChunk.h"
#include "./vis_lib/GameElement.h"

#include "Utils.h"

void GameChunk::check_coordinates(std::pair<int, int> coordinates)
{
	if (coordinates.first < 0 || coordinates.first > WIDTH - 1 || coordinates.second < 0 || coordinates.second > HEIGHT - 1)
	{
		std::cerr << "Coordinates out of bounds!\n";
		exit(ERR_COORDINATES_OUT_OF_BOUND);
	}
}

GameChunk::GameChunk()
{
	this->numberOfNeighbors = 0;
	this->neighbors = (bool*)calloc(4, sizeof(bool));
	this->pathNode = (paco::Node*)malloc(sizeof(paco::Node));
	this->coordinates = { 0, 0 };
}


GameElement GameChunk::toGameElement()
{
	std::pair<int, int> coordinatesInPixels = { this->coordinates.first * GAME_CHUNK + 40, this->coordinates.second * GAME_CHUNK + 20 };
	std::string pathname;
	switch (this->numberOfNeighbors)
	{
	case 4:
		pathname = "maze_0.png";
		break;
	case 3:
		pathname = !this->neighbors[0] ? "maze_1_up.png" :
			!this->neighbors[1] ? "maze_1_down.png" :
			!this->neighbors[2] ? "maze_1_left.png" :
			!this->neighbors[3] ? "maze_1_right.png" : "undefined";
		break;
	case 2:
		pathname = this->neighbors[0] ? (this->neighbors[1] ? "maze_2_up_down.png" : this->neighbors[2] ? "maze_2_up_left.png" : this->neighbors[3] ? "maze_2_up_right.png" : "undefined") :
			this->neighbors[1] ? (this->neighbors[2] ? "maze_2_down_left.png" : this->neighbors[3] ? "maze_2_down_right.png" : "undefined") :
			this->neighbors[2] ? (this->neighbors[3] ? "maze_2_left_right.png" : "undefined") : "undefined";
		break;
	case 1:
		pathname = this->neighbors[1] ? "maze_3_down.png" :
			this->neighbors[0] ? "maze_3_up.png" :
			this->neighbors[2] ? "maze_3_left.png" :
			this->neighbors[3] ? "maze_3_right.png" : "undefined";
		break;
	default:
		std::cerr << "Maze didn't generate correctly!\n";
		break;
	}

	return GameElement(coordinatesInPixels, { GAME_CHUNK, GAME_CHUNK }, MAZE_PATH + pathname);
}

// ---------- setters

void GameChunk::setNeighbors(bool neighbors[])
{
	this->neighbors = neighbors;
}

void GameChunk::setCoordinates(std::pair<int, int> coordinates)
{
	check_coordinates(coordinates);
	this->coordinates = coordinates;
}

void GameChunk::setNumberOfNeighbors(int numberOfNeighbors)
{
	if (numberOfNeighbors < 1)
	{
		std::cerr << "Incorrect number of neighbors!\n";
		exit(ERR_INCORRECT_NUMBER_OF_NEIGHBORS);
	}
	this->numberOfNeighbors = numberOfNeighbors;
}

void GameChunk::setPathNode(paco::Node* pathNode)
{
	this->pathNode = pathNode;
}

// ------------- getters

bool* GameChunk::getNeighbors()
{
	return this->neighbors;
}

std::pair<int, int> GameChunk::getCoordinates()
{
	return this->coordinates;
}

int GameChunk::getNumberOfNeighbors()
{
	return this->numberOfNeighbors;
}

paco::Node* GameChunk::getPathNode()
{
	return this->pathNode;
}
