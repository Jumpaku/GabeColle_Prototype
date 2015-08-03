#pragma once

# include <Siv3D.hpp>

template<class DerivedObject>
class Memory;
class CircleObject;

class Controller
{
	GUI gui_m;
public:
	Controller();
	void update(Memory<CircleObject> &memory);
};