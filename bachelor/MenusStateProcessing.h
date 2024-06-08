#pragma once
#include<iostream>
#undef main

#include "Menu.h"
#include "Button.h"

MainState process_menu_state(InputState inputState, Menu* crtMenu);
