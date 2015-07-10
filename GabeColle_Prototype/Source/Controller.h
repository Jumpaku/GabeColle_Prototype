#pragma once

# include <Siv3D.hpp>

class Memory;

class Controller
{
	GUI gui_m;
public:
	Controller();
	void update(Memory &memory);
};