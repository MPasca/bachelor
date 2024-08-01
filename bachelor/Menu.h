#pragma once
#include<iostream>
#include "Button.h"

#include "./vis_lib/GameElement.h"

class Menu
{
public:
	Menu(std::pair<int, int> dimensions, std::pair<int, int> coordinates, std::string backdropPath);

	void setButtons(Button* buttons, int numberOfButtons);

	Button* getButtons();
	Button getActiveButton();

	void changeActiveButton(int direction);
	GameElement* toGameElements();

private:
	std::pair<int, int> dimensions, coordinates;
	Button* buttons;
	int currentButton, numberOfButtons;
	std::string backdropPath;
	void check_coordinates(std::pair<int, int> coordinates);
	void check_dimensions(std::pair<int, int> dimensions);
};

