#include "Menu.h"

Menu::Menu(std::pair<int, int> dimensions, std::pair<int, int> coordinates, std::string backdropPath)
{
	this->dimensions = dimensions;
	this->backdropPath = backdropPath;
	this->coordinates = coordinates;
	this->currentButton = 0;
}

Menu::~Menu()
{
	free(this->buttons);
}

void Menu::setButtons(Button* buttons, int numberOfButtons)
{
	this->buttons = buttons;
	this->buttons[this->currentButton].setActiveStatus(ACTIVE_BTN);
	this->numberOfButtons = numberOfButtons;
	this->buttons[1].setActiveStatus(INACTIVE_BTN);
}

Button* Menu::getButtons()
{
	return this->buttons;
}

Button Menu::getActiveButton()
{
	return this->buttons[this->currentButton];
}

void Menu::changeActiveButton(int direction)
{
	this->buttons[this->currentButton].setActiveStatus(INACTIVE_BTN);
	this->currentButton = (this->currentButton + direction) % this->numberOfButtons;
	this->buttons[this->currentButton].setActiveStatus(ACTIVE_BTN);
}

GameElement* Menu::toGameElements()
{
	GameElement* result = (GameElement*)calloc(1 + this->numberOfButtons, sizeof(GameElement));
	result[0] = GameElement(this->coordinates, this->dimensions, this->backdropPath);
	for (int i = 0; i < this->numberOfButtons; i++)
	{
		result[1 + i] = GameElement(this->buttons[i].getCoordinates(), this->buttons[i].getDimensions(), this->buttons[i].getAssetPath());
	}

	return result;
}
