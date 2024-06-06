#include "GameChunk.h"
#include "./vis_lib/GameElement.h"

#include "Utils.h"

GameChunk::GameChunk()
{
	this->numberOfNeighbors = 0;
	this->walls = (bool*)calloc(4, sizeof(bool));
}

GameChunk::~GameChunk()
{

}

GameElement GameChunk::toGameElement(std::pair<int, int> coordinates)
{
	std::pair<int, int> coordinatesInPixels = { coordinates.first * GAME_CHUNK + 40, coordinates.second * GAME_CHUNK + 30 };
	std::string pathname;
	switch (this->numberOfNeighbors)
	{
	case 4:
		pathname = "maze_0.png";
		break;
	case 3:
		pathname = !this->walls[0] ? "maze_1_up.png" :
			!this->walls[1] ? "maze_1_down.png" :
			!this->walls[2] ? "maze_1_left.png" :
			!this->walls[3] ? "maze_1_right.png" : "undefined";
		break;
	case 2:
		pathname = this->walls[0] ? (this->walls[1] ? "maze_2_up_down.png" : this->walls[2] ? "maze_2_up_left.png" : this->walls[3] ? "maze_2_up_right.png" : "undefined") :
			this->walls[1] ? (this->walls[2] ? "maze_2_down_left.png" : this->walls[3] ? "maze_2_down_right.png" : "undefined") :
			this->walls[2] ? (this->walls[3] ? "maze_2_left_right.png" : "undefined") : "undefined";
		break;
	case 1:
		pathname = this->walls[1] ? "maze_3_down.png" :
			this->walls[0] ? "maze_3_up.png" :
			this->walls[2] ? "maze_3_left.png" :
			this->walls[3] ? "maze_3_right.png" : "undefined";
		break;
	default:
		std::cerr << "Maze didn't generate correctly!\n";
		break;
	}

	return GameElement(coordinatesInPixels, { GAME_CHUNK, GAME_CHUNK }, MAZE_PATH + pathname);
}

// ---------- setters

void GameChunk::setWalls(bool walls[])
{
	this->walls = walls;
}

void GameChunk::setNumberOfNeighbors(int numberOfNeighbors)
{
	this->numberOfNeighbors = numberOfNeighbors;
}

void GameChunk::setPathNode(paco::Node* pathNode)
{
	this->pathNode = pathNode;
}

// ------------- getters

bool* GameChunk::getWalls()
{
	return this->walls;
}

int GameChunk::getNumberOfNeighbors()
{
	return this->numberOfNeighbors;
}

paco::Node* GameChunk::getPathNode()
{
	return this->pathNode;
}
