#pragma once

#include <Siv3D.hpp>
#include"gc\Memory.h"

template<class DerivedObject>
class Memory;
class CircleObject;

class Controller
{
	GUI gui_m;
public:
	Controller();
	void update(gc::Memory<CircleObject> &memory);
};