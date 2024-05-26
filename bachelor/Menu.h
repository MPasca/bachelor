#pragma once
#include<iostream>

#include "Button.h"

class Menu
{
public:
	Menu(std::pair<int, int> dimensions, std::string backdropPath, Button* buttons);
	~Menu();

	void changeDimensions(std::pair<int, int> dimensions);
	void changeBackdrop(std::string backdropPath);

	Button* getButtons();
	Button getActiveButton();

	std::pair<int, int> getDimensions();
	std::string getBackdrop();
	void changeActiveButton();


private:
	std::pair<int, int> dimensions;
	Button activeButton, inactiveButton;
	std::string backdropPath;
};

