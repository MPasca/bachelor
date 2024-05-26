#pragma once
#include<SDL.h>
#include<SDL_image.h>

#include <iostream>
#include<string>

class GameElement
{
public:
	GameElement(std::pair<int, int> coordinates, std::pair<int, int> dimensions, std::string fileName);
	~GameElement();

	void updateCoordinates(std::pair<int, int> coordinates);

	std::pair<int, int> getCoordinates();
	SDL_Rect getRectangle();
	std::string getPath();

private:
	void clean();

	std::pair<int, int> coordinates;	// {x, y} coordinates for the top-left corner
	SDL_Rect dimRectangle;				// the rectangle associated with the sprite; defined by the width and height
	std::string fileName;						// the path to the asset

};

