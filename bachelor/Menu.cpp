#include "Menu.h"

void Menu::check_coordinates(std::pair<int, int> coordinates)
{
	if (coordinates.first < 0 || coordinates.first > VIEWPORT_WIDTH || coordinates.second < 0 || coordinates.second > VIEWPORT_HEIGHT)
	{
		std::cerr << "Coordinates out of bounds!\n";
		exit(ERR_COORDINATES_OUT_OF_BOUND);
	}
}

void Menu::check_dimensions(std::pair<int, int> dimensions)
{
	if (dimensions.first < 1 || dimensions.second < 1)
	{
		std::cerr << "Incorrect dimensions!\n";
		exit(ERR_INCORRECT_DIMENSIONS);
	}
}

Menu::Menu(std::pair<int, int> dimensions, std::pair<int, int> coordinates, std::string backdropPath)
{
	check_coordinates(coordinates);
	check_dimensions(dimensions);
	this->dimensions = dimensions;
	this->backdropPath = backdropPath;
	this->coordinates = coordinates;
	this->currentButton = 0;
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
	this->currentButton = this->currentButton + direction < 0 ? this->numberOfButtons-1 : (this->currentButton + direction) % this->numberOfButtons;
	this->buttons[this->currentButton].setActiveStatus(ACTIVE_BTN); 
}

GameElement* Menu::toGameElements()
{
	GameElement* result = (GameElement*)calloc(1 + this->numberOfButtons, sizeof(GameElement));
	if (result)
	{
		result[0] = GameElement(this->coordinates, this->dimensions, this->backdropPath);
		for (int i = 0; i < this->numberOfButtons; i++)
		{
			result[1 + i] = GameElement(this->buttons[i].getCoordinates(), this->buttons[i].getDimensions(), this->buttons[i].getAssetPath());
		}
	}

	return result;
}
