#include "Menu.h"

Menu::Menu(std::pair<int, int> dimensions, std::string backdropPath, Button* buttons)
{
	this->dimensions = dimensions;
	this->backdropPath = backdropPath;
	this->activeButton = buttons[0];
	this->inactiveButton = buttons[1];
}

Menu::~Menu()
{
	free(this);
}

void Menu::changeDimensions(std::pair<int, int> dimensions)
{
	this->dimensions = dimensions;
}

void Menu::changeBackdrop(std::string backdropPath)
{
	this->backdropPath = backdropPath;
}

Button* Menu::getButtons()
{
	Button buttons[] = { activeButton, inactiveButton };
	return buttons;
}

Button Menu::getActiveButton()
{
	return this->activeButton;
}

std::pair<int, int> Menu::getDimensions()
{
	return this->dimensions;
}

std::string Menu::getBackdrop()
{
	return this->backdropPath;
}

void Menu::changeActiveButton()
{
	Button auxButton = this->activeButton;
	this->activeButton = this->inactiveButton;
	this->inactiveButton = auxButton;
}
